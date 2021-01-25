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

#include "aadlxmlreader.h"

#include "aadlcommonprops.h"
#include "aadlcomment.h"
#include "aadlconnection.h"
#include "aadlconnectiongroup.h"
#include "aadlfunction.h"
#include "aadlfunctiontype.h"
#include "aadliface.h"
#include "aadlifacegroup.h"
#include "aadlparameter.h"

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
    void set(AADLObject *object)
    {
        m_object = object;
        m_function = m_object ? m_object->as<AADLFunctionType *>() : nullptr;
        m_iface = m_object ? m_object->as<AADLIface *>() : nullptr;
        m_comment = m_object ? m_object->as<AADLComment *>() : nullptr;
        m_connection = m_object ? m_object->as<AADLConnection *>() : nullptr;
        m_connectionGroup = m_object ? m_object->as<AADLConnectionGroup *>() : nullptr;
    }

    QPointer<AADLObject> get() { return m_object; }
    QPointer<AADLFunctionType> function() { return m_function; }
    QPointer<AADLIface> iface() { return m_iface; }
    QPointer<AADLComment> comment() { return m_comment; }
    QPointer<AADLConnection> connection() { return m_connection; }
    QPointer<AADLConnectionGroup> connectionGroup() { return m_connectionGroup; }

private:
    QPointer<AADLObject> m_object { nullptr };
    QPointer<AADLFunctionType> m_function { nullptr };
    QPointer<AADLIface> m_iface { nullptr };
    QPointer<AADLComment> m_comment { nullptr };
    QPointer<AADLConnection> m_connection { nullptr };
    QPointer<AADLConnectionGroup> m_connectionGroup { nullptr };
};

typedef QHash<QString, QHash<QString, AADLIface *>> IfacesByFunction; // { Function[Type]Id, {IfaceName, Iface} }
struct AADLXMLReaderPrivate {
    QVector<AADLObject *> m_allObjects {};
    QVariantMap m_metaData;
    QHash<QString, AADLFunctionType *> m_functionNames {};
    IfacesByFunction m_ifaceRequiredNames {};
    IfacesByFunction m_ifaceProvidedNames {};
    QHash<QString, AADLConnection *> m_connectionsById {};
    struct GroupInfo {
        QList<shared::Id> m_connectionIds;
        QList<AADLIface *> m_interfaces;
    };

    QHash<QString, GroupInfo> m_connectionGroups;

    CurrentObjectHolder m_currentObject;
    void setCurrentObject(AADLObject *obj)
    {
        m_currentObject.set(obj);
        if (!m_currentObject.get())
            return;

        if (!m_allObjects.contains(m_currentObject.get()))
            m_allObjects.append(m_currentObject.get());

        if (AADLFunctionType *fn = m_currentObject.function()) {
            const QString &fnTitle = fn->title();
            if (!m_functionNames.contains(fnTitle))
                m_functionNames.insert(fnTitle, fn);
        }

        if (AADLIface *iface = m_currentObject.iface()) {
            Q_ASSERT(iface->parentObject() != nullptr);

            const QString &parentId = iface->parentObject()->id().toString();
            const QString &ifaceTitle = iface->title();
            QHash<QString, AADLIface *> &ifacesCollection =
                    iface->isRequired() ? m_ifaceRequiredNames[parentId] : m_ifaceProvidedNames[parentId];
            if (!ifacesCollection.contains(ifaceTitle))
                ifacesCollection[ifaceTitle] = iface;
        }

        if (AADLConnection *conn = m_currentObject.connection()) {
            const QString connId = conn->id().toString();
            if (!m_connectionsById.contains(connId))
                m_connectionsById[connId] = conn;
        }
    }
};

AADLXMLReader::AADLXMLReader(QObject *parent)
    : QObject(parent)
    , d(new AADLXMLReaderPrivate)
{
}

AADLXMLReader::~AADLXMLReader() { }

bool AADLXMLReader::readFile(const QString &file)
{
    QFile in(file);
    if (in.exists(file) && in.open(QFile::ReadOnly | QFile::Text))
        return read(&in);

    const QString &errMsg = QString("Can't open file %1: %2").arg(file, in.errorString());
    qWarning() << errMsg;
    Q_EMIT error(errMsg);

    return false;
}

bool AADLXMLReader::read(QIODevice *openForRead)
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

bool AADLXMLReader::readXml(QIODevice *in)
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

bool AADLXMLReader::readInterfaceView(QXmlStreamReader &xml)
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

IfaceParameter addIfaceParameter(
        const QString &name, const XmlAttributes &otherAttrs, IfaceParameter::Direction direction)
{
    IfaceParameter param;

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

AADLConnection::EndPointInfo *addConnectionPart(const XmlAttributes &otherAttrs)
{
    const QString attrRiName = Props::token(Props::Token::ri_name);
    const bool isRI = otherAttrs.contains(attrRiName);

    AADLConnection::EndPointInfo *info = new AADLConnection::EndPointInfo();
    info->m_functionName = otherAttrs.value(Props::token(Props::Token::func_name)).m_value;
    info->m_interfaceName =
            isRI ? otherAttrs.value(attrRiName).m_value : otherAttrs.value(Props::token(Props::Token::pi_name)).m_value;
    info->m_ifaceDirection = isRI ? AADLIface::IfaceType::Required : AADLIface::IfaceType::Provided;

    Q_ASSERT(info->isReady());
    return info;
}

void AADLXMLReader::processTagOpen(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    const QString &attrName = Props::token(Props::Token::name);
    XmlAttributes attrs = XmlAttribute::wrapp(xml.attributes());
    const XmlAttribute &nameAttr = attrs.take(attrName);

    AADLObject *obj { nullptr };
    const Props::Token t = Props::token(tagName);
    switch (t) {
    case Props::Token::Function: {
        const bool isFunctionType =
                attrs.value(Props::token(Props::Token::is_type), QStringLiteral("no")).m_value.toLower()
                == QStringLiteral("yes");

        obj = addFunction(
                nameAttr.m_value, isFunctionType ? AADLObject::Type::FunctionType : AADLObject::Type::Function);
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

        const IfaceParameter param = addIfaceParameter(nameAttr.m_value, attrs,
                t == Props::Token::Input_Parameter ? IfaceParameter::Direction::In : IfaceParameter::Direction::Out);
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
            if (AADLConnection::EndPointInfo *info = addConnectionPart(attrs)) {
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
        d->m_currentObject.get()->setProp(nameAttr.m_value, attrs.value(Props::token(Props::Token::value)).m_value);
        break;
    }
    case Props::Token::ContextParameter: {
        auto function = qobject_cast<ivm::AADLFunctionType *>(d->m_currentObject.get());
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
        for (const XmlAttribute &xmlAttr : attrs)
            obj->setAttr(xmlAttr.m_name, xmlAttr.m_value);

        d->setCurrentObject(obj);
    }
}

void AADLXMLReader::processTagClose(QXmlStreamReader &xml)
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

AADLFunctionType *AADLXMLReader::addFunction(const QString &name, AADLObject::Type fnType)
{
    const bool isFunctionType = fnType == AADLObject::Type::FunctionType;

    AADLFunctionType *fn = isFunctionType ? new AADLFunctionType(name, d->m_currentObject.get())
                                                : new AADLFunction(name, d->m_currentObject.get());

    if (d->m_currentObject.function())
        d->m_currentObject.function()->addChild(fn);

    return fn;
}

AADLIface *AADLXMLReader::addIface(const QString &name, bool isRI)
{
    Q_ASSERT(d->m_currentObject.function() != nullptr);

    AADLIface *iface { nullptr };
    if (d->m_currentObject.function()) {

        AADLIface::CreationInfo ci;
        ci.function = d->m_currentObject.function();
        ci.name = name;

        if (isRI)
            iface = new AADLIfaceRequired(ci);
        else
            iface = new AADLIfaceProvided(ci);

        d->m_currentObject.function()->addChild(iface);
    }
    return iface;
}

AADLComment *AADLXMLReader::addComment(const QString &text)
{
    AADLComment *comment = new AADLComment(text, d->m_currentObject.get());
    if (d->m_currentObject.function())
        d->m_currentObject.function()->addChild(comment);

    return comment;
}

AADLConnection *AADLXMLReader::addConnection()
{
    AADLConnection *connection = new AADLConnection(nullptr, nullptr, d->m_currentObject.get());
    if (d->m_currentObject.function())
        d->m_currentObject.function()->addChild(connection);

    return connection;
}

AADLConnectionGroup *AADLXMLReader::addConnectionGroup(const QString &groupName)
{
    QHash<shared::Id, AADLIfaceGroup *> mappings;
    for (const auto iface : d->m_connectionGroups.value(groupName).m_interfaces) {
        Q_ASSERT(iface->parentObject());
        auto it = mappings.find(iface->parentObject()->id());
        if (it != mappings.end()) {
            it.value()->addEntity(iface);
        } else {
            auto ifaceGroup = new AADLIfaceGroup({});
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

    AADLConnectionGroup *connection =
            new AADLConnectionGroup(groupName, sourceIfaceGroup, targetIfaceGroup, {}, d->m_currentObject.get());
    connection->initConnections(d->m_connectionGroups.value(groupName).m_connectionIds);
    if (d->m_currentObject.function())
        d->m_currentObject.function()->addChild(connection);

    return connection;
}
}
