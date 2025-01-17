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
#include "ivarchetypelibraryreference.h"
#include "ivarchetypereference.h"
#include "ivcomment.h"
#include "ivcommonprops.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivinterfacegroup.h"
#include "parameter.h"
#include "uireader.h"

#include <QDebug>
#include <QDir>
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
        m_layer = m_object ? m_object->as<IVConnectionLayerType *>() : nullptr;
        m_archetypeReference = m_object ? m_object->as<IVArchetypeReference *>() : nullptr;
        m_archetypeLibraryReference = m_object ? m_object->as<IVArchetypeLibraryReference *>() : nullptr;
    }

    QPointer<IVObject> get() { return m_object; }
    QPointer<IVFunctionType> function() { return m_function; }
    QPointer<IVInterface> iface() { return m_iface; }
    QPointer<IVComment> comment() { return m_comment; }
    QPointer<IVConnection> connection() { return m_connection; }
    QPointer<IVConnectionGroup> connectionGroup() { return m_connectionGroup; }
    QPointer<IVConnectionLayerType> layer() { return m_layer; }
    QPointer<IVArchetypeReference> archetypeReference() { return m_archetypeReference; }
    QPointer<IVArchetypeLibraryReference> archetypeLibraryReference() { return m_archetypeLibraryReference; }

    bool isValid() const { return !m_object.isNull(); }

private:
    QPointer<IVObject> m_object { nullptr };
    QPointer<IVFunctionType> m_function { nullptr };
    QPointer<IVInterface> m_iface { nullptr };
    QPointer<IVComment> m_comment { nullptr };
    QPointer<IVConnection> m_connection { nullptr };
    QPointer<IVConnectionGroup> m_connectionGroup { nullptr };
    QPointer<IVConnectionLayerType> m_layer { nullptr };
    QPointer<IVArchetypeReference> m_archetypeReference { nullptr };
    QPointer<IVArchetypeLibraryReference> m_archetypeLibraryReference { nullptr };
};

typedef QHash<QString, QHash<QString, IVInterface *>> IfacesByFunction; // { Function[Type]Id, {IfaceName, Iface} }
struct IVXMLReaderPrivate {
    IVXMLReaderPrivate()
        : m_uiReader(new shared::UIReader)
    {
    }
    QVector<IVObject *> m_allObjects {};
    QHash<QString, IVObject *> m_layers {};
    QHash<QString, IVFunctionType *> m_functionNames {};
    IfacesByFunction m_ifaceRequiredNames {};
    IfacesByFunction m_ifaceProvidedNames {};
    QHash<QString, IVConnection *> m_connectionsById {};
    struct GroupInfo {
        QList<shared::Id> m_connectionIds;
        QList<IVInterface *> m_interfaces;
    };

    QHash<QString, GroupInfo> m_connectionGroups;
    QScopedPointer<shared::UIReader> m_uiReader { nullptr };
    QString m_creatorGitHash;
    CurrentObjectHolder m_currentObject;
    void setCurrentObject(IVObject *obj)
    {
        m_currentObject.set(obj);
        if (!m_currentObject.get())
            return;

        if (!m_allObjects.contains(m_currentObject.get()) && !m_currentObject.layer())
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

        if (IVConnectionLayerType *layer = m_currentObject.layer()) {
            m_layers.insert(layer->title(), layer);
        }
    }
};

IVXMLReader::IVXMLReader(QObject *parent)
    : shared::XmlReader(parent)
    , d(new IVXMLReaderPrivate)
{
}

IVXMLReader::~IVXMLReader() { }

QVector<IVObject *> IVXMLReader::parsedObjects() const
{
    return d->m_allObjects;
}

QVector<IVObject *> IVXMLReader::parsedLayers() const
{
    return d->m_layers.values().toVector();
}

QString IVXMLReader::uiFileNameTag() const
{
    return d->m_uiReader ? d->m_uiReader->filePathAttributeName() : QString();
}

QHash<shared::Id, EntityAttributes> IVXMLReader::externalAttributes() const
{
    return d->m_uiReader ? d->m_uiReader->data() : QHash<shared::Id, EntityAttributes>();
}

IVConnection::EndPointInfo addConnectionPart(const EntityAttributes &otherAttrs)
{
    const bool isRI = otherAttrs.contains(Props::token(Props::Token::ri_name));

    IVConnection::EndPointInfo info;
    info.m_functionName = attrValue(otherAttrs, Props::Token::func_name);
    info.m_interfaceName = attrValue(otherAttrs, isRI ? Props::Token::ri_name : Props::Token::pi_name);
    info.m_ifaceDirection = isRI ? IVInterface::InterfaceType::Required : IVInterface::InterfaceType::Provided;
    info.m_ifaceId = QUuid::fromString(attrValue(otherAttrs, Props::Token::iface_id));

    // Q_ASSERT(info.isReady());
    return info;
}

bool IVXMLReader::processTagOpen(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    const Props::Token t = Props::token(tagName);

    EntityAttributes attrs = attributes(xml.attributes());
    const EntityAttribute &idAttr = attrs.take(QLatin1String("id"));

    QUuid id = idAttr.value<QUuid>();
    if (!isIdUnique(id)) {
        const QUuid oldId = id;
        id = QUuid::createUuid();

        EntityAttribute nameAttr = attrs["name"];
        QString oldName = nameAttr.value().toString();
        QString name = oldName;
        if (t == Props::Token::Function) {
            name = uniqueName(oldName);
            nameAttr.setValue(name);
            attrs["name"] = nameAttr;
        }
        QString message = QString("Duplicate ID found. Replacing %1/%2 with %3/%4. Line %5")
                              .arg(oldId.toString(), oldName, id.toString(), name)
                              .arg(xml.lineNumber());
        shared::ErrorHub::addError(shared::ErrorItem::Warning, message);
    }

    if (d->m_uiReader && d->m_uiReader->rootElementName() == tagName) {
        auto fileIt = attrs.constFind(d->m_uiReader->filePathAttributeName());
        if (fileIt != attrs.constEnd()) {
            processUIFile(fileIt->value<QString>());
        } else if (d->m_uiReader->readSection(xml)) {
            processUIData();
        }
        return true;
    }

    IVObject *obj { nullptr };
    switch (t) {
    case Props::Token::Function: {
        const bool isFunctionType =
                attrValue(attrs, Props::Token::is_type, QStringLiteral("no")).toLower() == QStringLiteral("yes");

        obj = addFunction(id,
                          isFunctionType ? IVObject::Type::FunctionType : IVObject::Type::Function);
        break;
    }
    case Props::Token::Provided_Interface:
    case Props::Token::Required_Interface: {
        Q_ASSERT(d->m_currentObject.function() != nullptr);

        const auto iface = addIface(id, Props::Token::Required_Interface == t);
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
        const EntityAttribute sourceAttr = attrs.take(Props::token(Props::Token::Source));
        const EntityAttribute targetAttr = attrs.take(Props::token(Props::Token::Target));
        obj = addConnectionGroup(id,
                                 attrValue(attrs, Props::Token::name),
                                 sourceAttr.value<QString>(),
                                 targetAttr.value<QString>());
        break;
    }
    case Props::Token::Connection: {
        obj = addConnection(id);
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
            } else {
                shared::ErrorHub::addError(shared::ErrorItem::Warning,
                                           QString("Invalid connection at line %1")
                                               .arg(xml.lineNumber()));
            }
        }
        break;
    }
    case Props::Token::Comment: {
        obj = addComment(id);
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
    case Props::Token::Layer: {
        ivm::IVConnectionLayerType *layer = new ivm::IVConnectionLayerType();
        layer->rename(attrValue(attrs, Props::Token::name));
        obj = layer;
        break;
    }
    case Props::Token::ArchetypeReference: {
        obj = addArchetypeReference(
                attrValue(attrs, Props::Token::archetype_library), attrValue(attrs, Props::Token::archetype_function));
        break;
    }
    case Props::Token::ArchetypeLibraryReference: {
        obj = new IVArchetypeLibraryReference(
                attrValue(attrs, Props::Token::archetype_library), attrValue(attrs, Props::Token::path));
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
        for (const EntityAttribute &attr : qAsConst(attrs)) {
            obj->setEntityAttribute(attr);
        }
        d->setCurrentObject(obj);
    }
    return true;
}

bool IVXMLReader::processTagClose(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    switch (Props::token(tagName)) {
    case Props::Token::InterfaceView: {
        const QVariantMap &metaInfo = metaData();
        auto it = metaInfo.constFind(d->m_uiReader->filePathAttributeName());
        if (it != metaInfo.constEnd()) {
            processUIFile(it->toString());
        }
    } break;
    case Props::Token::Function:
    case Props::Token::Required_Interface:
    case Props::Token::Provided_Interface:
    case Props::Token::ConnectionGroup:
    case Props::Token::Connection:
    case Props::Token::Comment:
    case Props::Token::ArchetypeReference: {
        d->setCurrentObject(d->m_currentObject.get() ? d->m_currentObject.get()->parentObject() : nullptr);
        break;
    }
    default:
        break;
    }
    return true;
}

QString IVXMLReader::rootElementName() const
{
    return Props::token(Props::Token::InterfaceView);
}

void IVXMLReader::processUIFile(const QString &relFileName)
{
    const QFileInfo fi(file());
    const QString uiFile = fi.absoluteDir().filePath(relFileName);
    if (d->m_uiReader->readFile(uiFile)) {
        processUIData();
    }
}

void IVXMLReader::processUIData()
{
    const QHash<shared::Id, EntityAttributes> attrs = d->m_uiReader->data();
    for (auto attrIt = attrs.constBegin(); attrIt != attrs.constEnd(); ++attrIt) {
        auto objIt = std::find_if(d->m_allObjects.begin(), d->m_allObjects.end(),
                [id = attrIt.key()](IVObject *obj) { return obj->id() == id; });
        if (objIt != d->m_allObjects.end()) {
            for (const EntityAttribute &entityAttr : attrIt.value()) {
                (*objIt)->setEntityAttribute(entityAttr);
            }
        }
    }
}

IVFunctionType *IVXMLReader::addFunction(const shared::Id &id, IVObject::Type fnType)
{
    const bool isFunctionType = fnType == IVObject::Type::FunctionType;

    IVFunctionType *fn = isFunctionType ? new IVFunctionType(d->m_currentObject.get(), id)
                                        : new IVFunction(d->m_currentObject.get(), id);

    if (d->m_currentObject.function())
        d->m_currentObject.function()->addChild(fn);

    return fn;
}

IVInterface *IVXMLReader::addIface(const shared::Id &id, bool isRI)
{
    Q_ASSERT(d->m_currentObject.function() != nullptr);

    IVInterface *iface { nullptr };
    if (d->m_currentObject.function()) {
        IVInterface::CreationInfo ci;
        ci.id = id;
        ci.function = d->m_currentObject.function();
        ci.type = isRI ? IVInterface::InterfaceType::Required : IVInterface::InterfaceType::Provided;
        iface = IVInterface::createIface(ci);
        d->m_currentObject.function()->addChild(iface);
    }
    return iface;
}

IVComment *IVXMLReader::addComment(const shared::Id &id)
{
    IVComment *comment = new IVComment(d->m_currentObject.get(), id);
    if (d->m_currentObject.function())
        d->m_currentObject.function()->addChild(comment);

    return comment;
}

IVConnection *IVXMLReader::addConnection(const shared::Id &id)
{
    IVConnection *connection = new IVConnection(nullptr, nullptr, d->m_currentObject.get(), id);
    if (d->m_currentObject.function()) {
        d->m_currentObject.function()->addChild(connection);
    }
    if (d->m_currentObject.connectionGroup()) {
        d->m_currentObject.connectionGroup()->addGroupedConnection(connection);
    }

    return connection;
}

IVConnectionGroup *IVXMLReader::addConnectionGroup(
        const shared::Id &id, const QString &groupName, const QString &source, const QString &target)
{
    QHash<QString, IVInterfaceGroup *> mappings;
    for (const auto iface : d->m_connectionGroups.value(groupName).m_interfaces) {
        Q_ASSERT(iface->parentObject());
        auto it = mappings.constFind(iface->parentObject()->title());
        if (it != mappings.end()) {
            it.value()->addEntity(iface);
        } else {
            IVInterface::CreationInfo ci;
            ci.id = shared::createId();
            auto ifaceGroup = new IVInterfaceGroup(ci);
            if (iface->function()) {
                iface->function()->addChild(ifaceGroup);
            } else {
                ifaceGroup->setParentObject(iface->parentObject());
            }
            ifaceGroup->setGroupName(groupName);
            ifaceGroup->addEntity(iface);
            mappings.insert(iface->parentObject()->title(), ifaceGroup);
        }
    }
    if (mappings.size() != 2) {
        return nullptr;
    }
    auto sourceIfaceGroup = source.isEmpty() ? mappings.begin().value() : mappings[source];
    auto targetIfaceGroup = target.isEmpty() ? std::next(mappings.begin()).value() : mappings[target];

    d->m_allObjects.append(sourceIfaceGroup);
    d->m_allObjects.append(targetIfaceGroup);

    IVConnectionGroup *connection =
            new IVConnectionGroup(groupName, sourceIfaceGroup, targetIfaceGroup, {}, d->m_currentObject.get(), id);
    connection->initConnections(d->m_connectionGroups.value(groupName).m_connectionIds);
    if (d->m_currentObject.function())
        d->m_currentObject.function()->addChild(connection);

    return connection;
}

IVArchetypeReference *IVXMLReader::addArchetypeReference(
        const QString &archetypeLibrary, const QString &archetypeFunction)
{
    IVArchetypeReference *archetypeReference { nullptr };
    if (d->m_currentObject.function()) {
        archetypeReference = new IVArchetypeReference(d->m_currentObject.get(), archetypeLibrary, archetypeFunction);
        d->m_currentObject.function()->addChild(archetypeReference);
    }
    return archetypeReference;
}

bool IVXMLReader::isIdUnique(const QUuid &id) const
{
    return !std::any_of(d->m_allObjects.begin(), d->m_allObjects.end(), [&id](IVObject *obj) {
        return obj->id() == id;
    });
}

QString IVXMLReader::uniqueName(const QString &name) const
{
    auto nameExists = [this](const QString &testName) -> bool {
        return std::any_of(d->m_allObjects.begin(),
                           d->m_allObjects.end(),
                           [&testName](IVObject *obj) { return obj->title() == testName; });
    };

    QString newName = name;
    int count = 0;
    while (nameExists(newName)) {
        ++count;
        newName = QString("%1_renamed_%2").arg(name).arg(count);
    }

    return newName;
}

} // namespace ivm
