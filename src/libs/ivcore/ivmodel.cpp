/*
  Copyright (C) 2019-2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivmodel.h"

#include "common.h"
#include "errorhub.h"
#include "graphicsviewutils.h"
#include "ivarchetypelibraryreference.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterfacegroup.h"
#include "ivpropertytemplate.h"
#include "propertytemplateconfig.h"

#include <QtDebug>

namespace ivm {

struct IVModelPrivate {
    shared::PropertyTemplateConfig *m_dynPropConfig { nullptr };
    IVModel *m_layersModel { nullptr };
    shared::Id m_rootObjectId { shared::InvalidId };
    QVector<IVObject *> m_components;
    QVector<IVObject *> m_sharedTypes;
    QList<IVObject *> m_visibleObjects;
};

IVModel::IVModel(
        shared::PropertyTemplateConfig *dynPropConfig, IVModel *sharedModel, IVModel *componentModel, QObject *parent)
    : shared::VEModel(parent)
    , d(new IVModelPrivate)
{
    d->m_dynPropConfig = dynPropConfig;
}

IVModel::~IVModel() { }

bool IVModel::addObjectImpl(shared::VEObject *obj)
{
    if (ivm::IVObject *ivObj = obj->as<ivm::IVObject *>()) {
        if (ivObj->hasEntityAttribute(meta::Props::token(meta::Props::Token::origin)) && !ivObj->isReference()) {
            QList<IVObject *> importedEntities;
            clone(getOrigin(ivObj->origin()), importedEntities, IVModel::CloneType::Reference);
            if (!importedEntities.isEmpty())
                addObjects(importedEntities);
            return false; // All nested referenced entities are processed separately,
                          // current one reference should be dropped out
        } else if (shared::VEModel::addObjectImpl(obj)) {
            d->m_visibleObjects.append(ivObj);

            for (const auto attr : d->m_dynPropConfig->propertyTemplatesForObject(ivObj)) {
                if (attr->validate(ivObj) && !attr->isOptional()) {
                    const QVariant &currentValue = obj->entityAttributeValue(attr->name());
                    if (currentValue.isNull()) {
                        const QVariant &defaultValue = attr->defaultValue();
                        if (!defaultValue.isNull()) {
                            if (attr->info() == ivm::IVPropertyTemplate::Info::Attribute) {
                                obj->setEntityAttribute(attr->name(), defaultValue);
                            } else if (attr->info() == ivm::IVPropertyTemplate::Info::Property) {
                                obj->setEntityProperty(attr->name(), defaultValue);
                            } else {
                                static const QMetaEnum metaEnum = QMetaEnum::fromType<shared::PropertyTemplate::Info>();
                                shared::ErrorHub::addError(shared::ErrorItem::Warning,
                                        tr("Unknown dynamic property info: %1")
                                                .arg(metaEnum.valueToKey(int(attr->info()))));
                            }
                        }
                    }
                }
            }
            return true;
        }
    }
    return false;
}

template<typename F, typename Container, typename... Args>
constexpr decltype(auto) apply(Container &&c, F &&f, Args &&...args)
{
    std::for_each(std::begin(c), std::end(c), [&](const auto &t) { f(t, args...); });
}

void IVModel::clone(IVObject *origin, QList<IVObject *> &importedObjects, IVModel::CloneType type,
        IVFunctionType *parent, const QPointF &pos) const
{
    if (!origin || type == IVModel::CloneType::Unknown)
        return;

    auto findIface = [](const QList<IVObject *> &importedObjects, const QString &ifaceTitle, const QString &fnTitle,
                             IVObject::Type type) -> IVInterface * {
        auto it = std::find_if(importedObjects.constBegin(), importedObjects.constEnd(), [&](IVObject *ivObj) {
            if (ivObj->isInterface() || ivObj->isInterfaceGroup())
                return ivObj->title() == ifaceTitle && ivObj->parentObject()->title() == fnTitle
                        && ivObj->type() == type;
            return false;
        });
        return it == importedObjects.constEnd() ? nullptr : (*it)->as<IVInterface *>();
    };

    IVObject *ivClone = nullptr;
    if (type != IVModel::CloneType::Direct) {
        const shared::Id objectId = IVModel::CloneType::Move == type ? origin->id() : shared::createId();
        ivClone = getObject(objectId);
        if (ivClone)
            return;

        switch (origin->type()) {
        case IVObject::Type::Comment:
            ivClone = new IVComment(parent, objectId);
            break;
        case IVObject::Type::Function:
            ivClone = new IVFunction(parent, objectId);
            break;
        case IVObject::Type::FunctionType:
            ivClone = new IVFunctionType(parent, objectId);
            break;
        case IVObject::Type::ProvidedInterface:
        case IVObject::Type::RequiredInterface: {
            IVInterface::CreationInfo ci = IVInterface::CreationInfo::cloneIface(origin->as<IVInterface *>(), parent);
            ci.id = objectId;
            ivClone = IVInterface::createIface(ci);
        } break;
        case IVObject::Type::Connection:
            if (auto connection = origin->as<IVConnection *>()) {
                auto connectionClone = new IVConnection(nullptr, nullptr, parent, objectId);
                if (connection->delayedStart().isReady()) {
                    connectionClone->setDelayedStart(connection->delayedStart());
                } else {
                    connectionClone->setDelayedStart({ connection->sourceInterface()->function()->title(),
                            connection->sourceInterface()->title(), connection->sourceInterface()->direction(),
                            connection->sourceInterface()->id() });
                }

                if (connection->delayedEnd().isReady()) {
                    connectionClone->setDelayedEnd(connection->delayedEnd());
                } else {
                    connectionClone->setDelayedEnd({ connection->targetInterface()->function()->title(),
                            connection->targetInterface()->title(), connection->targetInterface()->direction(),
                            connection->targetInterface()->id() });
                }

                const QString connectionGroupName = connection->groupName();
                if (!connectionGroupName.isEmpty()) {
                    auto it = std::find_if(
                            importedObjects.cbegin(), importedObjects.cend(), [&connectionGroupName](IVObject *ivObj) {
                                return ivObj->title() == connectionGroupName
                                        && ivObj->type() == IVObject::Type::ConnectionGroup;
                            });
                    if (it != importedObjects.cend()) {
                        if (auto group = (*it)->as<IVConnectionGroup *>()) {
                            group->addGroupedConnection(connectionClone);
                        }
                    }
                }
                ivClone = connectionClone;
            }
            break;
        case IVObject::Type::ConnectionGroup:
            if (auto connectionGroup = origin->as<IVConnectionGroup *>()) {
                ivm::IVObject *sourceIfaceGroup =
                        findIface(importedObjects, connectionGroup->sourceInterfaceGroup()->title(),
                                connectionGroup->sourceName(), connectionGroup->sourceInterfaceGroup()->type());
                ivm::IVObject *targetIfaceGroup =
                        findIface(importedObjects, connectionGroup->targetInterfaceGroup()->title(),
                                connectionGroup->targetName(), connectionGroup->targetInterfaceGroup()->type());
                if (sourceIfaceGroup && targetIfaceGroup) {
                    ivClone = new IVConnectionGroup(connectionGroup->groupName(),
                            sourceIfaceGroup->as<IVInterfaceGroup *>(), targetIfaceGroup->as<IVInterfaceGroup *>(), {},
                            parent, objectId);
                }
            }
            break;
        case IVObject::Type::InterfaceGroup:
            if (auto originIfaceGroup = origin->as<IVInterfaceGroup *>()) {
                IVInterface::CreationInfo ci;
                ci.id = objectId;
                auto clonedIfaceGroup = new IVInterfaceGroup(ci);
                clonedIfaceGroup->setGroupName(origin->groupName());
                if (parent->type() == IVObject::Type::Function) {
                    auto fn = qobject_cast<IVFunction *>(parent);
                    fn->addChild(clonedIfaceGroup);
                } else {
                    clonedIfaceGroup->setParentObject(parent);
                }
                for (auto originIface : originIfaceGroup->entities()) {
                    IVInterface *clonedObject = findIface(importedObjects, originIface->title(),
                            originIface->function()->title(), originIface->type());
                    if (clonedObject) {
                        clonedIfaceGroup->addEntity(clonedObject);
                    }
                }
                ivClone = clonedIfaceGroup;
            }
            break;
        default:
            return;
        }

        if (!ivClone)
            return;

        if (parent)
            parent->addChild(ivClone);

        for (const EntityAttribute &attr : origin->entityAttributes()) {
            ivClone->setEntityAttribute(attr);
        }
        for (const EntityAttribute &attr : extEntityAttributes(origin->id())) {
            ivClone->setEntityAttribute(attr);
        }

        if (!pos.isNull()) {
            QRectF originGeometry = shared::graphicsviewutils::rect(origin->coordinates());
            originGeometry.moveTo(pos);
            ivClone->setCoordinates(shared::graphicsviewutils::coordinates(originGeometry));
        }

        if (type == IVModel::CloneType::Reference) {
            ivClone->setEntityAttribute(EntityAttribute { ivm::meta::Props::token(ivm::meta::Props::Token::reference),
                    true, EntityAttribute::Type::Attribute });
            const EntityAttribute attr = EntityAttribute { ivm::meta::Props::token(ivm::meta::Props::Token::origin),
                origin->id(), EntityAttribute::Type::Attribute };
            ivClone->setEntityAttribute(attr);
        }
    } else {
        ivClone = origin;
    }
    importedObjects << ivClone;
    if (auto fn = origin->as<IVFunctionType *>()) {
        auto f = [this](IVObject *obj, QList<IVObject *> &importedObjects, IVModel::CloneType type,
                         IVFunctionType *parent) { clone(obj, importedObjects, type, parent); };

        auto ivCloneFn = ivClone->as<IVFunctionType *>();

        apply(fn->functions(), f, importedObjects, type, ivCloneFn);
        apply(fn->functionTypes(), f, importedObjects, type, ivCloneFn);
        apply(fn->allInterfaces(), f, importedObjects, type, ivCloneFn);
        apply(fn->connectionGroups(), f, importedObjects, type, ivCloneFn);
        auto connections { fn->connections() };
        for (auto group : fn->connectionGroups()) {
            for (auto connection : group->groupedConnections())
                connections.append(connection);
        }
        apply(connections, f, importedObjects, type, ivCloneFn);
        apply(fn->comments(), f, importedObjects, type, ivCloneFn);
    }
}

bool IVModel::removeObject(shared::VEObject *obj)
{
    if (shared::VEModel::removeObject(obj)) {
        if (auto parentObj = qobject_cast<ivm::IVFunctionType *>(obj->parentObject())) {
            parentObj->removeChild(obj->as<ivm::IVObject *>());
        }
        return true;
    }
    return false;
}

void IVModel::setRootObject(shared::Id rootId)
{
    if (d->m_rootObjectId == rootId) {
        return;
    }

    d->m_rootObjectId = rootId;
    d->m_visibleObjects = visibleObjects(rootId);

    if (auto fnType = qobject_cast<IVFunctionType *>(rootObject())) {
        for (auto connection : fnType->connections()) {
            connection->setAttributeExportable(meta::Props::token(meta::Props::Token::coordinates), true);
        }
        for (auto interface : fnType->allInterfaces()) {
            interface->setAttributeExportable(meta::Props::token(meta::Props::Token::RootCoordinates), true);
        }
        for (auto fn : fnType->functionTypes()) {
            fn->setAttributeExportable(meta::Props::token(meta::Props::Token::coordinates), true);
            for (auto fnInterface : fn->allInterfaces()) {
                fnInterface->setAttributeExportable(meta::Props::token(meta::Props::Token::coordinates), true);
            }
        }
        for (auto fnt : fnType->functions()) {
            fnt->setAttributeExportable(meta::Props::token(meta::Props::Token::coordinates), true);
            for (auto fntInterface : fnt->allInterfaces()) {
                fntInterface->setAttributeExportable(meta::Props::token(meta::Props::Token::coordinates), true);
            }
        }
    }

    Q_EMIT rootObjectChanged(d->m_rootObjectId);
}

IVObject *IVModel::rootObject() const
{
    return getObject(d->m_rootObjectId);
}

shared::Id IVModel::rootObjectId() const
{
    return d->m_rootObjectId;
}

QVector<IVObject *> IVModel::externalSharedTypes() const
{
    return d->m_sharedTypes;
}

QVector<IVObject *> IVModel::externalComponents() const
{
    return d->m_components;
}

IVObject *IVModel::getObject(const shared::Id &id) const
{
    return qobject_cast<IVObject *>(shared::VEModel::getObject(id));
}

IVObject *IVModel::getOrigin(const shared::Id &id) const
{
    auto it = std::find_if(d->m_components.cbegin(), d->m_components.cend(),
            [&id](const IVObject *ivObj) { return ivObj->id() == id; });
    return it == d->m_components.cend() ? nullptr : *it;
}

IVObject *IVModel::getObjectByName(const QString &name, IVObject::Type type, Qt::CaseSensitivity caseSensitivity) const
{
    if (name.isEmpty())
        return nullptr;

    for (auto object : objects()) {
        if (auto obj = qobject_cast<ivm::IVObject *>(object)) {
            if ((type == IVObject::Type::Unknown || type == obj->type())
                    && obj->title().compare(name, caseSensitivity) == 0)
                return obj;
        }
    }
    return nullptr;
}

/*!
   Returns the first interface found, that has the given \p name and \p dir
 */
IVInterface *IVModel::getIfaceByName(const QString &name, IVInterface::InterfaceType dir, const IVFunctionType *parent,
        Qt::CaseSensitivity caseSensitivity) const
{
    if (name.isEmpty()) {
        return nullptr;
    }

    for (auto obj : objects()) {
        if (obj->title().compare(name, caseSensitivity) == 0) {
            if (IVInterface *iface = obj->as<IVInterface *>()) {
                if (iface->direction() == dir && (!parent || iface->parentObject() == parent)) {
                    return iface;
                }
            }
        }
    }

    return nullptr;
}

/*!
   Returns all interfaces with the given \p name
 */
QList<IVInterface *> IVModel::getIfacesByName(const QString &name, Qt::CaseSensitivity caseSensitivity) const
{
    QList<IVInterface *> result;
    if (name.isEmpty()) {
        return result;
    }

    for (auto obj : objects()) {
        if (obj->title().compare(name, caseSensitivity) == 0) {
            if (IVInterface *iface = obj->as<IVInterface *>()) {
                result << iface;
            }
        }
    }

    return result;
}

IVFunction *IVModel::getFunction(const shared::Id &id) const
{
    return qobject_cast<IVFunction *>(getObject(id));
}

/*!
   Returns the function with the given name.
   If no such function exists nullptr is returned.
 */
IVFunction *IVModel::getFunction(const QString &name, Qt::CaseSensitivity caseSensitivity) const
{
    return qobject_cast<IVFunction *>(getObjectByName(name, IVObject::Type::Function, caseSensitivity));
}

IVFunctionType *IVModel::getFunctionType(const QString &name, Qt::CaseSensitivity caseSensitivity) const
{
    return qobject_cast<IVFunctionType *>(getObjectByName(name, IVObject::Type::FunctionType, caseSensitivity));
}

IVFunctionType *IVModel::getFunctionType(const shared::Id &id) const
{
    return qobject_cast<IVFunction *>(getObject(id));
}

IVFunctionType *IVModel::getSharedFunctionType(const QString &name, Qt::CaseSensitivity caseSensitivity) const
{
    if (auto obj = getObjectByName(d->m_sharedTypes, name, IVObject::Type::FunctionType, caseSensitivity))
        return obj->as<IVFunctionType *>();

    return nullptr;
}

IVFunctionType *IVModel::getSharedFunctionType(const shared::Id &id) const
{
    if (auto obj = VEModel::getObject(d->m_sharedTypes, id))
        return obj->as<IVFunctionType *>();

    return nullptr;
}

QHash<QString, IVFunctionType *> IVModel::getAvailableFunctionTypes(const IVFunction *fnObj) const
{
    QHash<QString, IVFunctionType *> result;
    if (!fnObj)
        return result;

    auto isValid = [](const IVFunctionType *objFnType, const IVFunction *objFn) {
        IVObject *objFnTypeParent = objFnType->parentObject();

        if (!objFnTypeParent) // it's a global FunctionType
            return true;

        IVObject *objFnParent = objFn->parentObject();
        while (objFnParent) {
            if (objFnParent == objFnTypeParent)
                return true;
            objFnParent = objFnParent->parentObject();
        }
        return false;
    };

    for (auto obj : objects()) {
        if (IVFunctionType *objFnType = qobject_cast<IVFunctionType *>(obj)) {
            if (objFnType->isFunctionType() && isValid(objFnType, fnObj)) {
                result.insert(objFnType->title(), objFnType);
            }
        }
    }

    for (auto sharedObject : externalSharedTypes()) {
        if (sharedObject->parentObject() == nullptr) {
            if (auto fnType = sharedObject->as<IVFunctionType *>()) {
                if (fnType->isFunctionType()) {
                    result[fnType->title()] = fnType;
                }
            }
        }
    }

    return result;
}

IVInterface *IVModel::getInterface(const shared::Id &id) const
{
    return qobject_cast<IVInterface *>(getObject(id));
}

IVInterfaceRequired *IVModel::getRequiredInterface(const shared::Id &id) const
{
    return qobject_cast<IVInterfaceRequired *>(getObject(id));
}

IVInterfaceProvided *IVModel::getProvidedInterface(const shared::Id &id) const
{
    return qobject_cast<IVInterfaceProvided *>(getObject(id));
}

IVConnection *IVModel::getConnection(const shared::Id &id) const
{
    return qobject_cast<IVConnection *>(getObject(id));
}

IVComment *IVModel::getCommentById(const shared::Id &id) const
{
    return qobject_cast<IVComment *>(getObject(id));
}

IVConnection *IVModel::getConnectionForIface(const shared::Id &id) const
{
    for (auto obj : objects()) {
        if (auto connection = qobject_cast<IVConnection *>(obj)) {
            Q_ASSERT(connection->sourceInterface() != nullptr);
            Q_ASSERT(connection->targetInterface() != nullptr);
            if (connection->sourceInterface()->id() == id || connection->targetInterface()->id() == id)
                return connection;
        }
    }
    return nullptr;
}

QVector<IVConnection *> IVModel::getConnectionsForIface(const shared::Id &id) const
{
    QVector<IVConnection *> result;

    for (auto obj : objects()) {
        if (auto connection = qobject_cast<IVConnection *>(obj)) {
            if ((connection->sourceInterface() && connection->sourceInterface()->id() == id)
                    || (connection->targetInterface() && connection->targetInterface()->id() == id)) {
                result.append(connection);
            }
        }
    }
    return result;
}

/*!
 * Returns all connections that start/end at this or any clone of this interface
 */
QVector<IVConnection *> IVModel::getRelatedConnections(IVInterface *interface) const
{
    QVector<ivm::IVConnection *> affected;

    if (interface && interface->model()) {
        IVModel *model = interface->model();
        affected += model->getConnectionsForIface(interface->id());
        for (const auto &i : interface->clones()) {
            affected += model->getConnectionsForIface(i->id());
        }
    }
    return affected;
}

void IVModel::setConnectionLayersModel(IVModel *layersModel)
{
    d->m_layersModel = layersModel;
}

IVModel *IVModel::getConnectionLayersModel() const
{
    return d->m_layersModel;
}

IVConnectionLayerType *IVModel::getConnectionLayerByName(const QString &name) const
{
    if (d->m_layersModel != nullptr) {
        for (auto *const layer : d->m_layersModel->allObjectsByType<IVConnectionLayerType>()) {
            if (layer->title().compare(name) == 0) {
                return layer;
            }
        }
    }
    return nullptr;
}

QVector<IVArchetypeLibraryReference *> IVModel::getArchetypeLibraryReferences()
{
    QVector<IVArchetypeLibraryReference *> result;

    for (auto obj : objects()) {
        if (auto reference = qobject_cast<IVArchetypeLibraryReference *>(obj)) {
            result.append(reference);
        }
    }
    return result;
}

void IVModel::setArchetypeLibraryReferences(QVector<IVArchetypeLibraryReference *> references)
{
    QVector<IVArchetypeLibraryReference *> oldReferences = getArchetypeLibraryReferences();
    for (auto reference : oldReferences) {
        removeObject(reference);
    }

    for (auto reference : references) {
        addObjectImpl(reference);
    }
}

QVector<IVConnection *> IVModel::getConnectionsForFunction(const shared::Id &id) const
{
    QVector<IVConnection *> result;

    for (auto obj : objects()) {
        if (auto connection = qobject_cast<IVConnection *>(obj)) {
            if ((connection->source() && connection->source()->id() == id)
                    || (connection->target() && connection->target()->id() == id)) {
                result.append(connection);
            }
        }
    }
    return result;
}

/*!
 * Returns all connections between the two given functions. Both directions
 */
QVector<IVConnection *> IVModel::getConnectionsBetweenFunctions(const shared::Id &id1, const shared::Id &id2) const
{
    QVector<IVConnection *> result;

    for (auto obj : objects()) {
        if (auto connection = qobject_cast<IVConnection *>(obj)) {
            const bool sourceOk = (connection->source() && connection->source()->id() == id1)
                    || (connection->source() && connection->source()->id() == id2);
            const bool targetOk = (connection->target() && connection->target()->id() == id1)
                    || (connection->target() && connection->target()->id() == id2);
            if (sourceOk && targetOk) {
                result.append(connection);
            }
        }
    }
    return result;
}

/**
 * visibleObjects are the object that is visible in the current level, regardless if the user has hidden it.
 * @return
 */
QList<IVObject *> IVModel::visibleObjects() const
{
    return d->m_visibleObjects;
}

//! Get the visible objects with this root id
QList<IVObject *> IVModel::visibleObjects(shared::Id rootId) const
{
    QList<IVObject *> visibleObjects;
    IVObject *rootObj = getObject(rootId);
    for (const auto &id : objectsOrder()) {
        if (auto obj = getObject(id)) {
            if (rootId.isNull()) {
                visibleObjects.append(obj);
                continue;
            }

            if (!obj->isConnection() && !obj->isConnectionGroup()
                    && (shared::isAncestorOf(rootObj, obj) || rootObj == nullptr)) {
                visibleObjects.append(obj);
            } else if (auto connection = qobject_cast<ivm::IVConnection *>(obj)) {
                const bool sourceIfaceAncestor =
                        shared::isAncestorOf<ivm::IVObject>(rootObj, connection->sourceInterface());
                const bool targetIfaceAncestor =
                        shared::isAncestorOf<ivm::IVObject>(rootObj, connection->targetInterface());
                if ((sourceIfaceAncestor && targetIfaceAncestor) || rootObj == nullptr) {
                    visibleObjects.append(obj);
                }
            }
        }
    }
    return visibleObjects;
}

/**
 * All children of the current root is set visible
 */
void IVModel::setNestedObjectsVisible()
{
    QList<IVObject *> objectsToShow;
    IVObject *rootObj = rootObject();
    if (rootObj) {
        objectsToShow = visibleObjects(rootObj->id());
    } else {
        objectsToShow = visibleObjects();
    }
    for (shared::VEObject *veObject : objectsToShow) {
        auto ivObject = qobject_cast<ivm::IVObject *>(veObject);
        if (!ivObject) {
            continue;
        }
        ivObject->setVisible(true);
    }
}

/**
 * All children of the specified root visible
 */
void IVModel::setNestedObjectsVisible(shared::Id rootId)
{
    ivm::IVObject *rootObj = getObject(rootId);
    if (!rootObj) {
        return;
    }
    QVector<shared::VEObject *> descendants = rootObj->descendants();
    for (shared::VEObject *veObject : descendants) {
        auto ivObject = qobject_cast<ivm::IVObject *>(veObject);
        if (!ivObject) {
            continue;
        }
        ivObject->setVisible(true);
    }
}

void IVModel::clear()
{
    d->m_visibleObjects.clear();

    d->m_rootObjectId = shared::InvalidId;
    shared::VEModel::clear();
}

conversion::ModelType IVModel::modelType() const
{
    return conversion::ModelProperties<IVModel>::type;
}

shared::PropertyTemplateConfig *IVModel::dynPropConfig() const
{
    return d->m_dynPropConfig;
}

QString IVModel::defaultFunctionLanguage() const
{
    IVFunction f;
    shared::PropertyTemplate *t =
            d->m_dynPropConfig->propertyTemplateForObject(&f, meta::Props::token(meta::Props::Token::language));
    if (!t) {
        return {};
    }
    return t->defaultValue().toString();
}

QStringList IVModel::availableFunctionLanguages() const
{
    IVFunction f;
    shared::PropertyTemplate *t =
            d->m_dynPropConfig->propertyTemplateForObject(&f, meta::Props::token(meta::Props::Token::language));
    if (t) {
        return t->value().toStringList();
    }

    return {};
}

/*!
   Returns the connection with the given \p interfaceName connection from function \p source to function \p target
   If no such connection is found, a nullptr is returned.
 */
IVConnection *IVModel::getConnection(const QString &interfaceName, const QString &source, const QString &target,
        Qt::CaseSensitivity caseSensitivity) const
{
    for (shared::VEObject *obj : objects()) {
        if (IVConnection *connection = qobject_cast<IVConnection *>(obj)) {
            if (connection->targetInterfaceName().compare(interfaceName, caseSensitivity) == 0
                    && connection->sourceName().compare(source, caseSensitivity) == 0
                    && connection->targetName().compare(target, caseSensitivity) == 0) {
                return connection;
            }
        }
    }
    return nullptr;
}

static inline QVector<IVFunctionType *> nestedFunctions(const IVFunctionType *fnt)
{
    if (!fnt) {
        return {};
    }

    QVector<IVFunctionType *> children = fnt->functionTypes();
    for (const auto fn : fnt->functions()) {
        children.append(fn);
    }
    for (const auto child : children) {
        children << nestedFunctions(child);
    }
    return children;
}

QSet<QString> IVModel::nestedFunctionNames(const IVFunctionType *fnt) const
{
    QSet<QString> names;
    if (!fnt) {
        for (shared::VEObject *object : objects()) {
            if (auto obj = qobject_cast<ivm::IVObject *>(object)) {
                if (obj->type() == IVObject::Type::Function || obj->type() == IVObject::Type::FunctionType) {
                    names.insert(obj->title());
                }
            }
        }
    } else {
        names.insert(fnt->title());
        for (const auto obj : nestedFunctions(fnt)) {
            names.insert(obj->title());
        }
    }

    return names;
}

QSet<QStringList> IVModel::nestedFunctionPaths(const IVFunctionType *fnt) const
{
    QSet<QStringList> paths;
    if (!fnt) {
        for (shared::VEObject *object : objects()) {
            if (auto obj = qobject_cast<ivm::IVObject *>(object)) {
                if (obj->type() == IVObject::Type::Function || obj->type() == IVObject::Type::FunctionType) {
                    paths.insert(obj->path());
                }
            }
        }
    } else {
        paths.insert(fnt->path());
        for (const auto obj : nestedFunctions(fnt)) {
            paths.insert(obj->path());
        }
    }

    return paths;
}
}
