/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "ivxmlreader.h"

#include "ivcomment.h"
#include "ivcommonprops.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivinterfacegroup.h"
#include "parameter.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QVariant>
#include <QVector>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>

namespace ivm {

using namespace ivm::meta;

struct XmlAttribute {
    XmlAttribute(const QString &name = QString(), const QString &value = QString())
        : m_name(name)
        , m_token(meta::Props::token(m_name))
        , m_value(value)
    {
    }

    QString m_name;
    meta::Props::Token m_token;
    QString m_value;
    static QHash<QString, XmlAttribute> wrapp(const QXmlStreamAttributes &attrs)
    {
        QHash<QString, XmlAttribute> result;

        for (const QXmlStreamAttribute &attr : attrs) {
            const QString &name = attr.name().toString();
            result.insert(name, XmlAttribute(name, attr.value().toString()));
        }

        return result;
    }
};
typedef QHash<QString, XmlAttribute> XmlAttributes;

struct CurrentObjectHolder {
    void set(IVObject *object)
    {
        m_object = object;
        m_function = m_object ? m_object->as<IVFunctionType *>() : nullptr;
        m_iface = m_object ? m_object->as<IVInterface *>() : nullptr;
        m_comment = m_object ? m_object->as<IVComment *>() : nullptr;
        m_connection = m_object ? m_object->as<IVConnection *>() : nullptr;
        m_connectionGroup = m_object ? m_object->as<IVConnectionGroup *>() : nullptr;
    }

    QPointer<IVObject> get() { return m_object; }
    QPointer<IVFunctionType> function() { return m_function; }
    QPointer<IVInterface> iface() { return m_iface; }
    QPointer<IVComment> comment() { return m_comment; }
    QPointer<IVConnection> connection() { return m_connection; }
    QPointer<IVConnectionGroup> connectionGroup() { return m_connectionGroup; }

private:
    QPointer<IVObject> m_object { nullptr };
    QPointer<IVFunctionType> m_function { nullptr };
    QPointer<IVInterface> m_iface { nullptr };
    QPointer<IVComment> m_comment { nullptr };
    QPointer<IVConnection> m_connection { nullptr };
    QPointer<IVConnectionGroup> m_connectionGroup { nullptr };
};

typedef QHash<QString, QHash<QString, IVInterface *>> IfacesByFunction; // { Function[Type]Id, {IfaceName, Iface} }
struct IVXMLReaderPrivate {
    QVector<IVObject *> m_allObjects {};
    QVariantMap m_metaData;
    QHash<QString, IVFunctionType *> m_functionNames {};
    IfacesByFunction m_ifaceRequiredNames {};
    IfacesByFunction m_ifaceProvidedNames {};
    QHash<QString, IVConnection *> m_connectionsById {};
    struct GroupInfo {
        QList<shared::Id> m_connectionIds;
        QList<IVInterface *> m_interfaces;
    };

    QHash<QString, GroupInfo> m_connectionGroups;

    CurrentObjectHolder m_currentObject;
    void setCurrentObject(IVObject *obj)
    {
        m_currentObject.set(obj);
        if (!m_currentObject.get())
            return;

        if (!m_allObjects.contains(m_currentObject.get()))
            m_allObjects.append(m_currentObject.get());

        if (IVFunctionType *fn = m_currentObject.function()) {
            const QString &fnTitle = fn->title();
            if (!m_functionNames.contains(fnTitle))
                m_functionNames.insert(fnTitle, fn);
        }

        if (IVInterface *iface = m_currentObject.iface()) {
            Q_ASSERT(iface->parentObject() != nullptr);

            const QString &parentId = iface->parentObject()->id().toString();
            const QString &ifaceTitle = iface->title();
            QHash<QString, IVInterface *> &ifacesCollection =
                    iface->isRequired() ? m_ifaceRequiredNames[parentId] : m_ifaceProvidedNames[parentId];
            if (!ifacesCollection.contains(ifaceTitle))
                ifacesCollection[ifaceTitle] = iface;
        }

        if (IVConnection *conn = m_currentObject.connection()) {
            const QString connId = conn->id().toString();
            if (!m_connectionsById.contains(connId))
                m_connectionsById[connId] = conn;
        }
    }
};

IVXMLReader::IVXMLReader(QObject *parent)
    : QObject(parent)
    , d(new IVXMLReaderPrivate)
{
}

IVXMLReader::~IVXMLReader() { }

bool IVXMLReader::readFile(const QString &file)
{
    QFile in(file);
    if (in.exists(file) && in.open(QFile::ReadOnly | QFile::Text))
        return read(&in);

    const QString &errMsg = QString("Can't open file %1: %2").arg(file, in.errorString());
    qWarning() << errMsg;
    Q_EMIT error(errMsg);

    return false;
}

bool IVXMLReader::read(QIODevice *openForRead)
{
    if (openForRead && openForRead->isOpen() && openForRead->isReadable()) {
        if (readXml(openForRead)) {
            Q_EMIT objectsParsed(d->m_allObjects);
            Q_EMIT metaDataParsed(d->m_metaData);
            return true;
        }
    }

    return false;
}

bool IVXMLReader::read(const QByteArray &data)
{
    if (data.isEmpty()) {
        return false;
    }

    QXmlStreamReader xml(data);
    if (xml.readNext() == QXmlStreamReader::StartDocument) {
        if (xml.readNext() == QXmlStreamReader::StartElement) {
            if (Props::token(xml.name().toString()) == Props::Token::InterfaceView) {
                if (readInterfaceView(xml)) {
                    Q_EMIT objectsParsed(d->m_allObjects);
                    return true;
                }
            }
        }
    }
    return false;
}

bool IVXMLReader::readXml(QIODevice *in)
{
    if (!in)
        return false;

    QXmlStreamReader xml(in);
    if (xml.readNext() == QXmlStreamReader::StartDocument)
        if (xml.readNext() == QXmlStreamReader::StartElement)
            if (Props::token(xml.name().toString()) == Props::Token::InterfaceView)
                return readInterfaceView(xml);

    return false;
}

bool IVXMLReader::readInterfaceView(QXmlStreamReader &xml)
{
    for (const QXmlStreamAttribute &attribute : xml.attributes()) {
        d->m_metaData[attribute.name().toString()] = QVariant::fromValue(attribute.value().toString());
    }

    while (!xml.atEnd()) {
        if (xml.hasError()) {
            qWarning() << xml.errorString();
            Q_EMIT error(xml.errorString());
            return false;
        }

        switch (xml.readNext()) {
        case QXmlStreamReader::TokenType::StartElement:
            processTagOpen(xml);
            break;
        case QXmlStreamReader::TokenType::EndElement:
            processTagClose(xml);
            break;
        default:
            break;
        }
    }

    if (xml.hasError()) {
        qWarning() << xml.errorString();
        Q_EMIT error(xml.errorString());
        return false;
    }

    return true;
}

InterfaceParameter addIfaceParameter(
        const QString &name, const XmlAttributes &otherAttrs, InterfaceParameter::Direction direction)
{
    InterfaceParameter param;

    for (const XmlAttribute &attr : otherAttrs) {
        switch (attr.m_token) {
        case Props::Token::type: {
            param.setParamTypeName(attr.m_value);
            break;
        }
        case Props::Token::encoding: {
            param.setEncoding(attr.m_value);
            break;
        }
        default: {
            qWarning() << QStringLiteral("Interface Parameter - unknown attribute: %1").arg(attr.m_name);
            break;
        }
        }
    }

    param.setName(name);
    param.setDirection(direction);

    return param;
}

IVConnection::EndPointInfo *addConnectionPart(const XmlAttributes &otherAttrs)
{
    const QString attrRiName = Props::token(Props::Token::ri_name);
    const bool isRI = otherAttrs.contains(attrRiName);

    IVConnection::EndPointInfo *info = new IVConnection::EndPointInfo();
    info->m_functionName = otherAttrs.value(Props::token(Props::Token::func_name)).m_value;
    info->m_interfaceName =
            isRI ? otherAttrs.value(attrRiName).m_value : otherAttrs.value(Props::token(Props::Token::pi_name)).m_value;
    info->m_ifaceDirection = isRI ? IVInterface::InterfaceType::Required : IVInterface::InterfaceType::Provided;

    Q_ASSERT(info->isReady());
    return info;
}

void IVXMLReader::processTagOpen(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    const QString &attrName = Props::token(Props::Token::name);
    XmlAttributes attrs = XmlAttribute::wrapp(xml.attributes());
    const XmlAttribute &nameAttr = attrs.take(attrName);

    IVObject *obj { nullptr };
    const Props::Token t = Props::token(tagName);
    switch (t) {
    case Props::Token::Function: {
        const bool isFunctionType =
                attrs.value(Props::token(Props::Token::is_type), QStringLiteral("no")).m_value.toLower()
                == QStringLiteral("yes");

        obj = addFunction(nameAttr.m_value, isFunctionType ? IVObject::Type::FunctionType : IVObject::Type::Function);
        break;
    }
    case Props::Token::Provided_Interface:
    case Props::Token::Required_Interface: {
        Q_ASSERT(d->m_currentObject.function() != nullptr);

        const auto iface = addIface(nameAttr.m_value, Props::Token::Required_Interface == t);
        const QString groupName = attrs.value(Props::token(Props::Token::group_name)).m_value;
        if (!groupName.isEmpty())
            d->m_connectionGroups[groupName].m_interfaces.append(iface);
        obj = iface;
        break;
    }
    case Props::Token::Output_Parameter:
    case Props::Token::Input_Parameter: {
        Q_ASSERT(d->m_currentObject.iface() != nullptr);

        const InterfaceParameter param = addIfaceParameter(nameAttr.m_value, attrs,
                t == Props::Token::Input_Parameter ? InterfaceParameter::Direction::IN
                                                   : InterfaceParameter::Direction::OUT);
        d->m_currentObject.iface()->addParam(param);
        break;
    }
    case Props::Token::ConnectionGroup: {
        obj = addConnectionGroup(nameAttr.m_value);
        break;
    }
    case Props::Token::Connection: {
        obj = addConnection();
        const QString groupName = attrs.value(Props::token(Props::Token::group_name)).m_value;
        if (!groupName.isEmpty())
            d->m_connectionGroups[groupName].m_connectionIds.append(obj->id());
        break;
    }
    case Props::Token::Source:
    case Props::Token::Target: {
        Q_ASSERT(d->m_currentObject.connection() != nullptr);

        if (d->m_currentObject.connection()) {
            if (IVConnection::EndPointInfo *info = addConnectionPart(attrs)) {
                if (t == Props::Token::Source) {
                    d->m_currentObject.connection()->setDelayedStart(info);
                } else {
                    d->m_currentObject.connection()->setDelayedEnd(info);
                }
            }
        }
        break;
    }
    case Props::Token::Comment: {
        obj = addComment(nameAttr.m_value);
        break;
    }
    case Props::Token::Property: {
        d->m_currentObject.get()->setEntityProperty(nameAttr.m_value, attrs.value(Props::token(Props::Token::value)).m_value);
        break;
    }
    case Props::Token::ContextParameter: {
        auto function = qobject_cast<ivm::IVFunctionType *>(d->m_currentObject.get());
        if (function) {
            const QString typeString = attrs.value(Props::token(Props::Token::type)).m_value;
            ivm::BasicParameter::Type paramType = typeString == "Timer"
                    ? ivm::BasicParameter::Type::Timer
                    : (typeString == "Directive" ? ivm::BasicParameter::Type::Directive
                                                 : ivm::BasicParameter::Type::Other);
            ContextParameter param(
                    nameAttr.m_value, paramType, typeString, attrs.value(Props::token(Props::Token::value)).m_value);
            function->addContextParam(param);
        }
        break;
    }
    default:
        static const QString msg("The '%1' is unknown/unexpected here: %2@%3 %4");
        qWarning() << msg.arg(
                tagName, QString::number(xml.lineNumber()), QString::number(xml.columnNumber()), xml.tokenString());
        break;
    }

    if (obj) {
        for (const XmlAttribute &xmlAttr : attrs) {
            obj->setEntityAttribute(xmlAttr.m_name, xmlAttr.m_value);
        }
        d->setCurrentObject(obj);
    }
}

void IVXMLReader::processTagClose(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    switch (Props::token(tagName)) {
    case Props::Token::Function:
    case Props::Token::Required_Interface:
    case Props::Token::Provided_Interface:
    case Props::Token::ConnectionGroup:
    case Props::Token::Connection:
    case Props::Token::Comment: {
        d->setCurrentObject(d->m_currentObject.get() ? d->m_currentObject.get()->parentObject() : nullptr);
        break;
    }
    default:
        break;
    }
}

IVFunctionType *IVXMLReader::addFunction(const QString &name, IVObject::Type fnType)
{
    const bool isFunctionType = fnType == IVObject::Type::FunctionType;

    IVFunctionType *fn = isFunctionType ? new IVFunctionType(name, d->m_currentObject.get())
                                        : new IVFunction(name, d->m_currentObject.get());

    if (d->m_currentObject.function())
        d->m_currentObject.function()->addChild(fn);

    return fn;
}

IVInterface *IVXMLReader::addIface(const QString &name, bool isRI)
{
    Q_ASSERT(d->m_currentObject.function() != nullptr);

    IVInterface *iface { nullptr };
    if (d->m_currentObject.function()) {

        IVInterface::CreationInfo ci;
        ci.function = d->m_currentObject.function();
        ci.name = name;

        if (isRI)
            iface = new IVInterfaceRequired(ci);
        else
            iface = new IVInterfaceProvided(ci);

        d->m_currentObject.function()->addChild(iface);
    }
    return iface;
}

IVComment *IVXMLReader::addComment(const QString &text)
{
    IVComment *comment = new IVComment(text, d->m_currentObject.get());
    if (d->m_currentObject.function())
        d->m_currentObject.function()->addChild(comment);

    return comment;
}

IVConnection *IVXMLReader::addConnection()
{
    IVConnection *connection = new IVConnection(nullptr, nullptr, d->m_currentObject.get());
    if (d->m_currentObject.function())
        d->m_currentObject.function()->addChild(connection);

    return connection;
}

IVConnectionGroup *IVXMLReader::addConnectionGroup(const QString &groupName)
{
    QHash<shared::Id, IVInterfaceGroup *> mappings;
    for (const auto iface : d->m_connectionGroups.value(groupName).m_interfaces) {
        Q_ASSERT(iface->parentObject());
        auto it = mappings.find(iface->parentObject()->id());
        if (it != mappings.end()) {
            it.value()->addEntity(iface);
        } else {
            auto ifaceGroup = new IVInterfaceGroup({});
            ifaceGroup->setParentObject(iface->parentObject());
            ifaceGroup->setGroupName(groupName);
            ifaceGroup->addEntity(iface);
            mappings.insert(iface->parentObject()->id(), ifaceGroup);
        }
    }
    Q_ASSERT(mappings.size() == 2);
    auto sourceIfaceGroup = *mappings.begin();
    auto targetIfaceGroup = *std::next(mappings.begin());

    d->m_allObjects.append(sourceIfaceGroup);
    d->m_allObjects.append(targetIfaceGroup);

    IVConnectionGroup *connection =
            new IVConnectionGroup(groupName, sourceIfaceGroup, targetIfaceGroup, {}, d->m_currentObject.get());
    connection->initConnections(d->m_connectionGroups.value(groupName).m_connectionIds);
    if (d->m_currentObject.function())
        d->m_currentObject.function()->addChild(connection);

    return connection;
}
}
