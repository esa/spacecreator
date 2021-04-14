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

#include "ivcomment.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivnamevalidator.h"
#include "common.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"

#include <QtDebug>

namespace ivm {

struct IVModelPrivate {
    PropertyTemplateConfig *m_dynPropConfig { nullptr };
    IVModel *m_sharedTypesModel { nullptr };
    shared::Id m_rootObjectId;
    QList<shared::Id> m_objectsOrder;
    QHash<shared::Id, IVObject *> m_objects;
    QList<IVObject *> m_visibleObjects;
    QVector<QString> m_headerTitles;
};

IVModel::IVModel(PropertyTemplateConfig *dynPropConfig, QObject *parent)
    : QObject(parent)
    , d(new IVModelPrivate)
{
    d->m_dynPropConfig = dynPropConfig;
}

IVModel::~IVModel() { }

void IVModel::setSharedTypesModel(IVModel *sharedTypesModel)
{
    d->m_sharedTypesModel = sharedTypesModel;
}

void IVModel::initFromObjects(const QVector<IVObject *> &objects)
{
    clear();
    addObjects(objects);
}

void IVModel::addObjects(const QVector<IVObject *> &objects)
{
    QVector<IVObject *> addedObjects;
    for (auto obj : objects) {
        if (addObjectImpl(obj)) {
            addedObjects.append(obj);
        }
    }

    for (auto it = addedObjects.begin(); it != addedObjects.end(); ++it) {
        if (IVObject *obj = *it) {
            if (!obj->postInit()) {
                if (IVFunctionType *parentFn = qobject_cast<IVFunction *>(obj->parentObject())) {
                    parentFn->removeChild(obj);
                }
                if (removeObject(obj)) {
                    it = addedObjects.erase(it);
                }
            }
        }
    }

    if (!addedObjects.isEmpty()) {
        Q_EMIT objectsAdded(addedObjects);
    }
}

bool IVModel::addObjectImpl(IVObject *obj)
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

bool IVModel::addObject(IVObject *obj)
{
    if (addObjectImpl(obj)) {
        if (!obj->postInit()) {
            removeObject(obj);
            if (auto parentObj = qobject_cast<ivm::IVFunctionType *>(obj->parentObject())) {
                parentObj->removeChild(obj);
            }
        } else {
            Q_EMIT objectsAdded({ obj });
            return true;
        }
    }
    return false;
}

bool IVModel::removeObject(IVObject *obj)
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

    Q_EMIT objectRemoved(obj);
    return true;
}

void IVModel::setRootObject(shared::Id rootId)
{
    if (d->m_rootObjectId == rootId) {
        return;
    }

    d->m_rootObjectId = rootId;
    d->m_visibleObjects = visibleObjects(rootId);

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
    if (id.isNull()) {
        return nullptr;
    }

    return d->m_objects.value(id, nullptr);
}

IVObject *IVModel::getObjectByName(
        const QString &name, IVObject::Type type, Qt::CaseSensitivity caseSensitivity) const
{
    if (name.isEmpty())
        return nullptr;

    for (auto obj : d->m_objects)
        if ((type == IVObject::Type::Unknown || type == obj->type())
                && obj->title().compare(name, caseSensitivity) == 0)
            return obj;
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

    for (auto obj : qAsConst(d->m_objects)) {
        if (obj->isInterface() && obj->title().compare(name, caseSensitivity) == 0) {
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

    for (auto obj : qAsConst(d->m_objects)) {
        if (obj->isInterface() && obj->title().compare(name, caseSensitivity) == 0) {
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
    return qobject_cast<IVFunction *>(getObjectByName(name, IVObject::Type::Unknown, caseSensitivity));
}

IVFunctionType *IVModel::getFunctionType(const shared::Id &id) const
{
    return qobject_cast<IVFunction *>(getObject(id));
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

    for (IVObject *obj : d->m_objects) {
        if (obj->isFunctionType()) {
            if (IVFunctionType *objFnType = qobject_cast<IVFunctionType *>(obj)) {
                if (isValid(objFnType, fnObj)) {
                    result.insert(objFnType->title(), objFnType);
                }
            }
        }
    }

    const auto sharedObjects = d->m_sharedTypesModel->objects();
    for (auto sharedObject : sharedObjects) {
        if (sharedObject->isFunctionType() && sharedObject->parentObject() == nullptr) {
            if (auto fnType = sharedObject->as<IVFunctionType *>()) {
                result[fnType->title()] = fnType;
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
    for (auto it = d->m_objects.constBegin(); it != d->m_objects.constEnd(); ++it) {
        if (auto connection = qobject_cast<IVConnection *>(it.value())) {
            Q_ASSERT(connection->sourceInterface() != nullptr);
            Q_ASSERT(connection->targetInterface() != nullptr);
            if (connection->sourceInterface()->id() == id || connection->targetInterface()->id() == id)
                return connection;
        }
    }
    return nullptr;
}

const QHash<shared::Id, IVObject *> &IVModel::objects() const
{
    return d->m_objects;
}

QVector<IVConnection *> IVModel::getConnectionsForIface(const shared::Id &id) const
{
    QVector<IVConnection *> result;

    for (auto it = d->m_objects.cbegin(); it != d->m_objects.cend(); ++it)
        if (it.value()->type() == IVObject::Type::Connection)
            if (auto connection = qobject_cast<IVConnection *>(it.value()))
                if ((connection->sourceInterface() && connection->sourceInterface()->id() == id)
                        || (connection->targetInterface() && connection->targetInterface()->id() == id))
                    result.append(connection);

    return result;
}

QList<IVObject *> IVModel::visibleObjects() const
{
    return d->m_visibleObjects;
}

//! Get the visible objects with this root id
QList<IVObject *> IVModel::visibleObjects(shared::Id rootId) const
{
    QList<IVObject *> visibleObjects;
    IVObject *rootObj = d->m_objects.value(rootId);
    for (const auto &id : d->m_objectsOrder) {
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

void IVModel::clear()
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
IVConnection *IVModel::getConnection(const QString &interfaceName, const QString &source, const QString &target,
        Qt::CaseSensitivity caseSensitivity) const
{
    for (IVObject *obj : d->m_objects) {
        if (obj->isConnection()) {
            if (IVConnection *connection = qobject_cast<IVConnection *>(obj)) {
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
        for (IVObject *obj : d->m_objects) {
            if (obj->type() == IVObject::Type::Function || obj->type() == IVObject::Type::FunctionType) {
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

QSet<QStringList> IVModel::nestedFunctionPaths(const IVFunctionType *fnt) const
{
    QSet<QStringList> paths;
    if (!fnt) {
        for (IVObject *obj : d->m_objects) {
            if (obj->type() == IVObject::Type::Function || obj->type() == IVObject::Type::FunctionType) {
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
