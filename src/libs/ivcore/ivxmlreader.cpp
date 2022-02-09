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

#include "entityattribute.h"
#include "errorhub.h"
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

static inline QString attrValue(const EntityAttributes &attrs, const QString &name, const QString &defaultValue = {})
{
    const EntityAttribute &attr = attrs.value(name);
    return attr.isValid() ? attr.value<QString>() : defaultValue;
}

static inline QString attrValue(
        const EntityAttributes &attrs, meta::Props::Token token, const QString &defaultValue = {})
{
    return attrValue(attrs, meta::Props::token(token), defaultValue);
}

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

    bool isValid() const { return !m_object.isNull(); }

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
    : shared::XmlReader(parent)
    , d(new IVXMLReaderPrivate)
{
}

IVXMLReader::~IVXMLReader() {}

QVector<IVObject *> IVXMLReader::parsedObjects() const
{
    return d->m_allObjects;
}

IVConnection::EndPointInfo addConnectionPart(const EntityAttributes &otherAttrs)
{
    const bool isRI = otherAttrs.contains(Props::token(Props::Token::ri_name));

    IVConnection::EndPointInfo info;
    info.m_functionName = attrValue(otherAttrs, Props::Token::func_name);
    info.m_interfaceName = attrValue(otherAttrs, isRI ? Props::Token::ri_name : Props::Token::pi_name);
    info.m_ifaceDirection = isRI ? IVInterface::InterfaceType::Required : IVInterface::InterfaceType::Provided;

    Q_ASSERT(info.isReady());
    return info;
}

void IVXMLReader::processTagOpen(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    const EntityAttributes attrs = attributes(xml.attributes());
    //    const XmlAttribute &nameAttr = attrs.take(attrName);

    IVObject *obj { nullptr };
    const Props::Token t = Props::token(tagName);
    switch (t) {
    case Props::Token::Function: {
        const bool isFunctionType =
                attrValue(attrs, Props::Token::is_type, QStringLiteral("no")).toLower() == QStringLiteral("yes");

        obj = addFunction(isFunctionType ? IVObject::Type::FunctionType : IVObject::Type::Function);
        break;
    }
    case Props::Token::Provided_Interface:
    case Props::Token::Required_Interface: {
        Q_ASSERT(d->m_currentObject.function() != nullptr);

        const auto iface = addIface(Props::Token::Required_Interface == t);
        const QString groupName = attrValue(attrs, Props::Token::group_name);
        if (!groupName.isEmpty())
            d->m_connectionGroups[groupName].m_interfaces.append(iface);
        obj = iface;
        break;
    }
    case Props::Token::Output_Parameter:
    case Props::Token::Input_Parameter: {
        Q_ASSERT(d->m_currentObject.iface() != nullptr);

        const shared::InterfaceParameter param = addIfaceParameter(attrs,
                t == Props::Token::Input_Parameter ? shared::InterfaceParameter::Direction::IN
                                                   : shared::InterfaceParameter::Direction::OUT);
        d->m_currentObject.iface()->addParam(param);
        break;
    }
    case Props::Token::ConnectionGroup: {
        obj = addConnectionGroup(attrValue(attrs, Props::Token::name));
        break;
    }
    case Props::Token::Connection: {
        obj = addConnection();
        const QString groupName = attrValue(attrs, Props::Token::group_name);
        if (!groupName.isEmpty())
            d->m_connectionGroups[groupName].m_connectionIds.append(obj->id());
        break;
    }
    case Props::Token::Source:
    case Props::Token::Target: {
        Q_ASSERT(d->m_currentObject.connection() != nullptr);

        if (d->m_currentObject.connection()) {
            IVConnection::EndPointInfo info = addConnectionPart(attrs);
            if (info.isReady()) {
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
        obj = addComment();
        break;
    }
    case Props::Token::Property: {
        if (d->m_currentObject.isValid()) {
            d->m_currentObject.get()->setEntityProperty(
                    attrValue(attrs, Props::Token::name), attrValue(attrs, Props::Token::value));
        }
        break;
    }
    case Props::Token::ContextParameter: {
        auto function = qobject_cast<ivm::IVFunctionType *>(d->m_currentObject.get());
        if (function) {
            const QString typeString = attrValue(attrs, Props::Token::type);
            shared::BasicParameter::Type paramType = typeString == "Timer"
                    ? shared::BasicParameter::Type::Timer
                    : (typeString == "Directive" ? shared::BasicParameter::Type::Directive
                                                 : shared::BasicParameter::Type::Other);
            shared::ContextParameter param(
                    attrValue(attrs, Props::Token::name), paramType, typeString, attrValue(attrs, Props::Token::value));
            function->addContextParam(param);
        }
        break;
    }
    case Props::Token::Implementations: {
        // Container for list of implementations only
        break;
    }
    case Props::Token::Implementation: {
        auto fn = qobject_cast<ivm::IVFunction *>(d->m_currentObject.get().data());
        if (fn) {
            fn->addImplementation(attrValue(attrs, Props::Token::name), attrValue(attrs, Props::Token::language));
        }
        break;
    }
    default:
        static const QString msg("The '%1' is unknown/unexpected here: %2@%3 %4");
        shared::ErrorHub::addError(shared::ErrorItem::Warning,
                msg.arg(tagName, QString::number(xml.lineNumber()), QString::number(xml.columnNumber()),
                        xml.tokenString()));
        break;
    }

    if (obj) {
        obj->setEntityAttributes(attrs);
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

QString IVXMLReader::rootElementName() const
{
    return Props::token(Props::Token::InterfaceView);
}

IVFunctionType *IVXMLReader::addFunction(IVObject::Type fnType)
{
    const bool isFunctionType = fnType == IVObject::Type::FunctionType;

    IVFunctionType *fn =
            isFunctionType ? new IVFunctionType(d->m_currentObject.get()) : new IVFunction(d->m_currentObject.get());

    if (d->m_currentObject.function())
        d->m_currentObject.function()->addChild(fn);

    return fn;
}

IVInterface *IVXMLReader::addIface(bool isRI)
{
    Q_ASSERT(d->m_currentObject.function() != nullptr);

    IVInterface *iface { nullptr };
    if (d->m_currentObject.function()) {
        IVInterface::CreationInfo ci;
        ci.function = d->m_currentObject.function();
        ci.type = isRI ? IVInterface::InterfaceType::Required : IVInterface::InterfaceType::Provided;
        iface = IVInterface::createIface(ci);
        d->m_currentObject.function()->addChild(iface);
    }
    return iface;
}

IVComment *IVXMLReader::addComment()
{
    IVComment *comment = new IVComment(d->m_currentObject.get());
    if (d->m_currentObject.function())
        d->m_currentObject.function()->addChild(comment);

    return comment;
}

IVConnection *IVXMLReader::addConnection()
{
    IVConnection *connection = new IVConnection(nullptr, nullptr, d->m_currentObject.get());
    if (d->m_currentObject.function()) {
        d->m_currentObject.function()->addChild(connection);
    }
    if (d->m_currentObject.connectionGroup()) {
        d->m_currentObject.connectionGroup()->addGroupedConnection(connection);
    }

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
            if (iface->parentObject()->type() == IVObject::Type::Function) {
                auto fn = qobject_cast<IVFunction *>(iface->parentObject());
                fn->addChild(ifaceGroup);
            } else {
                ifaceGroup->setParentObject(iface->parentObject());
            }
            ifaceGroup->setGroupName(groupName);
            ifaceGroup->addEntity(iface);
            mappings.insert(iface->parentObject()->id(), ifaceGroup);
        }
    }
    if (mappings.size() != 2) {
        return nullptr;
    }
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
