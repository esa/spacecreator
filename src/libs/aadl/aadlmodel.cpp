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

#include "aadlmodel.h"

#include "aadlcomment.h"
#include "aadlconnection.h"
#include "aadlfunction.h"
#include "aadlfunctiontype.h"
#include "aadlnamevalidator.h"
#include "common.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"

#include <QtDebug>

namespace ivm {

struct AADLModelPrivate {
    PropertyTemplateConfig *m_dynPropConfig { nullptr };
    AADLModel *m_sharedTypesModel { nullptr };
    shared::Id m_rootObjectId;
    QList<shared::Id> m_objectsOrder;
    QHash<shared::Id, AADLObject *> m_objects;
    QList<AADLObject *> m_visibleObjects;
    QVector<QString> m_headerTitles;
};

AADLModel::AADLModel(PropertyTemplateConfig *dynPropConfig, QObject *parent)
    : QObject(parent)
    , d(new AADLModelPrivate)
{
    d->m_dynPropConfig = dynPropConfig;
}

AADLModel::~AADLModel() { }

void AADLModel::setSharedTypesModel(AADLModel *sharedTypesModel)
{
    d->m_sharedTypesModel = sharedTypesModel;
}

void AADLModel::initFromObjects(const QVector<AADLObject *> &objects)
{
    clear();
    addObjects(objects);
}

void AADLModel::addObjects(const QVector<AADLObject *> &objects)
{
    QVector<AADLObject *> addedObjects;
    for (auto obj : objects) {
        if (addObjectImpl(obj)) {
            addedObjects.append(obj);
        }
    }

    for (auto it = addedObjects.begin(); it != addedObjects.end(); ++it) {
        if (AADLObject *obj = *it) {
            if (!obj->postInit()) {
                if (AADLFunctionType *parentFn = qobject_cast<AADLFunction *>(obj->parentObject())) {
                    parentFn->removeChild(obj);
                }
                if (removeObject(obj)) {
                    it = addedObjects.erase(it);
                }
            }
        }
    }

    if (!addedObjects.isEmpty()) {
        Q_EMIT aadlObjectsAdded(addedObjects);
    }
}

bool AADLModel::addObjectImpl(AADLObject *obj)
{
    if (!obj)
        return false;

    const shared::Id &id = obj->id();
    if (getObject(id)) {
        return false;
    }

    if (!obj->parent()) {
        obj->setParent(this);
    }

    obj->setObjectsModel(this);

    d->m_objects.insert(id, obj);
    d->m_objectsOrder.append(id);
    d->m_visibleObjects.append(obj);

    for (auto attr : d->m_dynPropConfig->propertyTemplatesForObject(obj)) {
        if (attr->validate(obj)) {
            const QVariant &currentValue = obj->attr(attr->name());
            if (currentValue.isNull()) {
                const QVariant &defaultValue = attr->defaultValue();
                if (!defaultValue.isNull()) {
                    if (attr->info() == ivm::PropertyTemplate::Info::Attribute) {
                        obj->setAttr(attr->name(), defaultValue);
                    } else if (attr->info() == ivm::PropertyTemplate::Info::Property) {
                        obj->setProp(attr->name(), defaultValue);
                    } else {
                        qWarning() << "Unknown dynamic property info:" << attr->info();
                    }
                }
            }
        }
    }

    return true;
}

bool AADLModel::addObject(AADLObject *obj)
{
    if (addObjectImpl(obj)) {
        if (!obj->postInit()) {
            removeObject(obj);
            if (auto parentObj = qobject_cast<ivm::AADLFunctionType *>(obj->parentObject())) {
                parentObj->removeChild(obj);
            }
        } else {
            Q_EMIT aadlObjectsAdded({ obj });
            return true;
        }
    }
    return false;
}

bool AADLModel::removeObject(AADLObject *obj)
{
    if (!obj)
        return false;

    const shared::Id &id = obj->id();
    if (!getObject(id))
        return false;

    obj->aboutToBeRemoved();

    d->m_objects.remove(id);
    d->m_objectsOrder.removeAll(id);
    d->m_visibleObjects.removeAll(obj);

    Q_EMIT aadlObjectRemoved(obj);
    return true;
}

void AADLModel::setRootObject(shared::Id rootId)
{
    if (d->m_rootObjectId == rootId) {
        return;
    }

    d->m_rootObjectId = rootId;
    d->m_visibleObjects = visibleObjects(rootId);

    Q_EMIT rootObjectChanged(d->m_rootObjectId);
}

AADLObject *AADLModel::rootObject() const
{
    return getObject(d->m_rootObjectId);
}

shared::Id AADLModel::rootObjectId() const
{
    return d->m_rootObjectId;
}

AADLObject *AADLModel::getObject(const shared::Id &id) const
{
    if (id.isNull()) {
        return nullptr;
    }

    return d->m_objects.value(id, nullptr);
}

AADLObject *AADLModel::getObjectByName(
        const QString &name, AADLObject::Type type, Qt::CaseSensitivity caseSensitivity) const
{
    if (name.isEmpty())
        return nullptr;

    for (auto obj : d->m_objects)
        if ((type == AADLObject::Type::Unknown || type == obj->aadlType())
                && obj->title().compare(name, caseSensitivity) == 0)
            return obj;
    return nullptr;
}

/*!
   Returns the first interface found, that has the given \p name and \p dir
 */
AADLIface *AADLModel::getIfaceByName(const QString &name, AADLIface::IfaceType dir, const AADLFunctionType *parent,
        Qt::CaseSensitivity caseSensitivity) const
{
    if (name.isEmpty()) {
        return nullptr;
    }

    for (auto obj : qAsConst(d->m_objects)) {
        if (obj->isInterface() && obj->title().compare(name, caseSensitivity) == 0) {
            if (AADLIface *iface = obj->as<AADLIface *>()) {
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
QList<AADLIface *> AADLModel::getIfacesByName(const QString &name, Qt::CaseSensitivity caseSensitivity) const
{
    QList<AADLIface *> result;
    if (name.isEmpty()) {
        return result;
    }

    for (auto obj : qAsConst(d->m_objects)) {
        if (obj->isInterface() && obj->title().compare(name, caseSensitivity) == 0) {
            if (AADLIface *iface = obj->as<AADLIface *>()) {
                result << iface;
            }
        }
    }

    return result;
}

AADLFunction *AADLModel::getFunction(const shared::Id &id) const
{
    return qobject_cast<AADLFunction *>(getObject(id));
}

/*!
   Returns the function with the given name.
   If no such function exists nullptr is returned.
 */
AADLFunction *AADLModel::getFunction(const QString &name, Qt::CaseSensitivity caseSensitivity) const
{
    return qobject_cast<AADLFunction *>(getObjectByName(name, AADLObject::Type::Unknown, caseSensitivity));
}

AADLFunctionType *AADLModel::getFunctionType(const shared::Id &id) const
{
    return qobject_cast<AADLFunction *>(getObject(id));
}

QHash<QString, AADLFunctionType *> AADLModel::getAvailableFunctionTypes(const AADLFunction *fnObj) const
{
    QHash<QString, AADLFunctionType *> result;
    if (!fnObj)
        return result;

    auto isValid = [](const AADLFunctionType *objFnType, const AADLFunction *objFn) {
        AADLObject *objFnTypeParent = objFnType->parentObject();

        if (!objFnTypeParent) // it's a global FunctionType
            return true;

        AADLObject *objFnParent = objFn->parentObject();
        while (objFnParent) {
            if (objFnParent == objFnTypeParent)
                return true;
            objFnParent = objFnParent->parentObject();
        }
        return false;
    };

    for (AADLObject *obj : d->m_objects) {
        if (obj->isFunctionType()) {
            if (AADLFunctionType *objFnType = qobject_cast<AADLFunctionType *>(obj)) {
                if (isValid(objFnType, fnObj)) {
                    result.insert(objFnType->title(), objFnType);
                }
            }
        }
    }

    const auto sharedObjects = d->m_sharedTypesModel->objects();
    for (auto sharedObject : sharedObjects) {
        if (sharedObject->isFunctionType() && sharedObject->parentObject() == nullptr) {
            if (auto fnType = sharedObject->as<AADLFunctionType *>()) {
                result[fnType->title()] = fnType;
            }
        }
    }

    return result;
}

AADLIface *AADLModel::getInterface(const shared::Id &id) const
{
    return qobject_cast<AADLIface *>(getObject(id));
}

AADLIfaceRequired *AADLModel::getRequiredInterface(const shared::Id &id) const
{
    return qobject_cast<AADLIfaceRequired *>(getObject(id));
}

AADLIfaceProvided *AADLModel::getProvidedInterface(const shared::Id &id) const
{
    return qobject_cast<AADLIfaceProvided *>(getObject(id));
}

AADLConnection *AADLModel::getConnection(const shared::Id &id) const
{
    return qobject_cast<AADLConnection *>(getObject(id));
}

AADLComment *AADLModel::getCommentById(const shared::Id &id) const
{
    return qobject_cast<AADLComment *>(getObject(id));
}

AADLConnection *AADLModel::getConnectionForIface(const shared::Id &id) const
{
    for (auto it = d->m_objects.constBegin(); it != d->m_objects.constEnd(); ++it) {
        if (auto connection = qobject_cast<AADLConnection *>(it.value())) {
            Q_ASSERT(connection->sourceInterface() != nullptr);
            Q_ASSERT(connection->targetInterface() != nullptr);
            if (connection->sourceInterface()->id() == id || connection->targetInterface()->id() == id)
                return connection;
        }
    }
    return nullptr;
}

const QHash<shared::Id, AADLObject *> &AADLModel::objects() const
{
    return d->m_objects;
}

QVector<AADLConnection *> AADLModel::getConnectionsForIface(const shared::Id &id) const
{
    QVector<AADLConnection *> result;

    for (auto it = d->m_objects.cbegin(); it != d->m_objects.cend(); ++it)
        if (it.value()->aadlType() == AADLObject::Type::Connection)
            if (auto connection = qobject_cast<AADLConnection *>(it.value()))
                if ((connection->sourceInterface() && connection->sourceInterface()->id() == id)
                        || (connection->targetInterface() && connection->targetInterface()->id() == id))
                    result.append(connection);

    return result;
}

QList<AADLObject *> AADLModel::visibleObjects() const
{
    return d->m_visibleObjects;
}

//! Get the visible objects with this root id
QList<AADLObject *> AADLModel::visibleObjects(shared::Id rootId) const
{
    QList<AADLObject *> visibleObjects;
    AADLObject *rootObj = d->m_objects.value(rootId);
    for (const auto &id : d->m_objectsOrder) {
        if (auto obj = getObject(id)) {
            if (rootId.isNull()) {
                visibleObjects.append(obj);
                continue;
            }

            if (!obj->isConnection() && !obj->isConnectionGroup()
                    && (shared::isAncestorOf(rootObj, obj) || rootObj == nullptr)) {
                visibleObjects.append(obj);
            } else if (auto connection = qobject_cast<ivm::AADLConnection *>(obj)) {
                const bool sourceIfaceAncestor =
                        shared::isAncestorOf<ivm::AADLObject>(rootObj, connection->sourceInterface());
                const bool targetIfaceAncestor =
                        shared::isAncestorOf<ivm::AADLObject>(rootObj, connection->targetInterface());
                if ((sourceIfaceAncestor && targetIfaceAncestor) || rootObj == nullptr) {
                    visibleObjects.append(obj);
                }
            }
        }
    }
    return visibleObjects;
}

void AADLModel::clear()
{
    for (auto object : d->m_objects.values())
        object->deleteLater();

    d->m_objects.clear();
    d->m_objectsOrder.clear();
    d->m_visibleObjects.clear();

    d->m_rootObjectId = shared::InvalidId;
    Q_EMIT modelReset();
}

/*!
   Returns the connection with the given \p interfaceName connection from function \p source to function \p target
   If no such connection is found, a nullptr is returned.
 */
AADLConnection *AADLModel::getConnection(const QString &interfaceName, const QString &source, const QString &target,
        Qt::CaseSensitivity caseSensitivity) const
{
    for (AADLObject *obj : d->m_objects) {
        if (obj->isConnection()) {
            if (AADLConnection *connection = qobject_cast<AADLConnection *>(obj)) {
                if (connection->targetInterfaceName().compare(interfaceName, caseSensitivity) == 0
                        && connection->sourceName().compare(source, caseSensitivity) == 0
                        && connection->targetName().compare(target, caseSensitivity) == 0) {
                    return connection;
                }
            }
        }
    }
    return nullptr;
}

static inline QVector<AADLFunctionType *> nestedFunctions(const AADLFunctionType *fnt)
{
    if (!fnt) {
        return {};
    }

    QVector<AADLFunctionType *> children = fnt->functionTypes();
    for (const auto fn : fnt->functions()) {
        children.append(fn);
    }
    for (const auto child : children) {
        children << nestedFunctions(child);
    }
    return children;
}

QSet<QString> AADLModel::nestedFunctionNames(const AADLFunctionType *fnt) const
{
    QSet<QString> names;
    if (!fnt) {
        for (AADLObject *obj : d->m_objects) {
            if (obj->aadlType() == AADLObject::Type::Function || obj->aadlType() == AADLObject::Type::FunctionType) {
                names.insert(obj->title());
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

QSet<QStringList> AADLModel::nestedFunctionPaths(const AADLFunctionType *fnt) const
{
    QSet<QStringList> paths;
    if (!fnt) {
        for (AADLObject *obj : d->m_objects) {
            if (obj->aadlType() == AADLObject::Type::Function || obj->aadlType() == AADLObject::Type::FunctionType) {
                paths.insert(obj->path());
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
