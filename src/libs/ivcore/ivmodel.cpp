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
    IVModel *m_componentModel { nullptr };
    IVModel *m_sharedTypesModel { nullptr };
    shared::Id m_rootObjectId;
    QList<IVObject *> m_visibleObjects;
    IVModel *m_layersModel;
};

IVModel::IVModel(
        shared::PropertyTemplateConfig *dynPropConfig, IVModel *sharedModel, IVModel *componentModel, QObject *parent)
    : shared::VEModel(parent)
    , d(new IVModelPrivate)
{
    d->m_dynPropConfig = dynPropConfig;
    d->m_componentModel = componentModel;
    d->m_sharedTypesModel = sharedModel;
    d->m_layersModel = nullptr;
}

IVModel::~IVModel() { }

bool IVModel::addObjectImpl(shared::VEObject *obj)
{
    if (ivm::IVObject *ivObj = obj->as<ivm::IVObject *>()) {
        if (ivObj->hasEntityAttribute(meta::Props::token(meta::Props::Token::origin)) && !ivObj->isReference()) {
            import(IVModel::ImportType::Reference, getOrigin(ivObj->origin()), nullptr, ivObj->id());
            return false;
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

template<class F, class Container>
constexpr decltype(auto) apply(Container &&c, F &&f)
{
    std::for_each(std::begin(c), std::end(c), [&](const auto &t) { f(t); });
}

void IVModel::import(
        IVModel::ImportType type, IVObject *origin, IVFunctionType *parent, const shared::Id &id, const QPointF &pos)
{
    if (!origin)
        return;

    IVObject *ivClone = getObject(origin->id());
    Q_ASSERT(!ivClone);
    if (ivClone)
        return;

    const shared::Id objectId = IVModel::ImportType::Move == type ? id : shared::createId();

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
            connectionClone->setDelayedStart(
                    { connection->sourceInterface()->function()->title(), connection->sourceInterface()->title(),
                            connection->sourceInterface()->direction(), connection->sourceInterface()->id() });
            connectionClone->setDelayedEnd(
                    { connection->targetInterface()->function()->title(), connection->targetInterface()->title(),
                            connection->targetInterface()->direction(), connection->targetInterface()->id() });

            const QString connectionGroupName = connection->groupName();
            if (!connectionGroupName.isEmpty()) {
                if (IVObject *obj = getObjectByName(connectionGroupName, IVObject::Type::ConnectionGroup)) {
                    if (auto group = obj->as<IVConnectionGroup *>()) {
                        group->addGroupedConnection(connection);
                    }
                }
            }
            ivClone = connectionClone;
        }
        break;
    case IVObject::Type::ConnectionGroup:
        if (auto connectionGroup = origin->as<IVConnectionGroup *>()) {
            static const QString token = meta::Props::token(meta::Props::Token::origin);
            auto sourceIfaceGroup = getObjectByAttributeValue(token, connectionGroup->sourceInterfaceGroup()->id());
            auto targetIfaceGroup = getObjectByAttributeValue(token, connectionGroup->targetInterfaceGroup()->id());
            if (sourceIfaceGroup && targetIfaceGroup) {
                auto ivConnectionGroup =
                        new IVConnectionGroup(connectionGroup->groupName(), sourceIfaceGroup->as<IVInterfaceGroup *>(),
                                targetIfaceGroup->as<IVInterfaceGroup *>(), {}, parent, objectId);

                QList<shared::Id> connectionIds;
                for (auto connection : connectionGroup->groupedConnections()) {
                    if (connection) {
                        connectionIds.append(connection->id());
                    }
                }

                ivConnectionGroup->initConnections(connectionIds);
                ivClone = ivConnectionGroup;
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
            static const QString token = meta::Props::token(meta::Props::Token::origin);

            for (auto originIface : originIfaceGroup->entities()) {
                if (auto clonedObject = getObjectByAttributeValue(token, originIface->id())) {
                    if (auto clonedIface = clonedObject->as<IVInterfaceGroup *>()) {
                        clonedIfaceGroup->addEntity(clonedIface);
                    }
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

    if (type == IVModel::ImportType::Reference) {
        ivClone->setEntityAttribute(EntityAttribute {
                ivm::meta::Props::token(ivm::meta::Props::Token::reference), true, EntityAttribute::Type::Attribute });
        const EntityAttribute attr = EntityAttribute { ivm::meta::Props::token(ivm::meta::Props::Token::origin),
            origin->id(), EntityAttribute::Type::Attribute };
        ivClone->setEntityAttribute(attr);
    }
    addObject(ivClone);

    if (auto fn = origin->as<IVFunctionType *>()) {
        auto f = [&](IVObject *obj) { import(type, obj, ivClone->as<IVFunctionType *>()); };
        apply(fn->functions(), f);
        apply(fn->functionTypes(), f);
        apply(fn->allInterfaces(), f);
        apply(fn->connectionGroups(), f);
        apply(fn->connections(), f);
        apply(fn->comments(), f);
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

IVObject *IVModel::getObject(const shared::Id &id) const
{
    return qobject_cast<IVObject *>(shared::VEModel::getObject(id));
}

IVObject *IVModel::getOrigin(const shared::Id &id) const
{
    return d->m_componentModel ? d->m_componentModel->getObject(id) : nullptr;
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
    return d->m_sharedTypesModel ? qobject_cast<IVFunctionType *>(
                   d->m_sharedTypesModel->getObjectByName(name, IVObject::Type::FunctionType, caseSensitivity))
                                 : nullptr;
}

IVFunctionType *IVModel::getSharedFunctionType(const shared::Id &id) const
{
    return d->m_sharedTypesModel ? qobject_cast<IVFunction *>(d->m_sharedTypesModel->getObject(id)) : nullptr;
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

    if (d->m_sharedTypesModel) {
        const auto sharedObjects = d->m_sharedTypesModel->objects();
        for (auto sharedObject : sharedObjects) {
            if (sharedObject->parentObject() == nullptr) {
                if (auto fnType = sharedObject->as<IVFunctionType *>()) {
                    if (fnType->isFunctionType()) {
                        result[fnType->title()] = fnType;
                    }
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

IVModel *IVModel::getComponentsModel() const
{
    return d->m_componentModel;
}

void IVModel::setComponentsModel(IVModel *model)
{
    d->m_componentModel = model;
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

void IVModel::setNestedObjectsVisible()
{
    QList<IVObject *> objectsToShow;
    IVObject *rootObj = rootObject();
    if (rootObj) {
         objectsToShow = visibleObjects(rootObj->id());
    }
    else {
        objectsToShow = visibleObjects();
    }
    for (shared::VEObject *veObject : objectsToShow) {
        auto ivObject = qobject_cast<ivm::IVObject*>(veObject);
        if (!ivObject) {
            continue;
        }
        ivObject->setVisible(true);
    }
}

void IVModel::setNestedObjectsVisible(shared::Id rootId)
{
    ivm::IVObject *rootObj = getObject(rootId);
    if (!rootObj) {
        return;
    }
    QVector<shared::VEObject *> descendants = rootObj->descendants();
    for (shared::VEObject *veObject : descendants) {
        auto ivObject = qobject_cast<ivm::IVObject*>(veObject);
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
