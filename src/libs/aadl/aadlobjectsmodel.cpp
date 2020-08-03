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

#include "aadlobjectsmodel.h"

#include "aadlobjectcomment.h"
#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectfunctiontype.h"
#include "common.h"

#include <QtDebug>

namespace aadl {

struct AADLObjectsModelPrivate {
    shared::Id m_rootObjectId;
    QList<shared::Id> m_objectsOrder;
    QHash<shared::Id, AADLObject *> m_objects;
    QList<AADLObject *> m_visibleObjects;
};

AADLObjectsModel::AADLObjectsModel(QObject *parent)
    : QObject(parent)
    , d(new AADLObjectsModelPrivate)
{
}

AADLObjectsModel::~AADLObjectsModel() { }

bool AADLObjectsModel::initFromObjects(const QVector<AADLObject *> &objects)
{
    clear();

    const bool currentState = blockSignals(true);

    for (auto obj : objects)
        addObject(obj);

    for (auto obj : objects)
        if (!obj->postInit()) {
            switch (obj->aadlType()) {
            case AADLObject::Type::Connection: {
                if (AADLObjectFunction *parentFn = qobject_cast<AADLObjectFunction *>(obj->parentObject()))
                    parentFn->removeChild(obj);
                break;
            }
            default:
                break;
            }

            removeObject(obj);
        }

    blockSignals(currentState);

    Q_EMIT rootObjectChanged(d->m_rootObjectId);

    return true;
}

bool AADLObjectsModel::addObject(AADLObject *obj)
{
    if (!obj)
        return false;

    const shared::Id &id = obj->id();
    if (getObject(id))
        return false;

    if (!obj->parent())
        obj->setParent(this);

    obj->setObjectsModel(this);

    d->m_objects.insert(id, obj);
    d->m_objectsOrder.append(id);
    d->m_visibleObjects.append(obj);
    Q_EMIT aadlObjectAdded(obj);
    return true;
}

bool AADLObjectsModel::removeObject(AADLObject *obj)
{
    if (!obj)
        return false;

    const shared::Id &id = obj->id();
    if (!getObject(id))
        return false;

    d->m_objects.remove(id);
    d->m_objectsOrder.removeAll(id);
    d->m_visibleObjects.removeAll(obj);

    Q_EMIT aadlObjectRemoved(obj);
    return true;
}

void AADLObjectsModel::setRootObject(shared::Id rootId)
{
    if (d->m_rootObjectId == rootId) {
        return;
    }

    d->m_rootObjectId = rootId;
    d->m_visibleObjects.clear();
    Q_EMIT modelReset();

    d->m_visibleObjects = visibleObjects(rootId);
    Q_EMIT rootObjectChanged(d->m_rootObjectId);
}

AADLObject *AADLObjectsModel::rootObject() const
{
    return getObject(d->m_rootObjectId);
}

shared::Id AADLObjectsModel::rootObjectId() const
{
    return d->m_rootObjectId;
}

AADLObject *AADLObjectsModel::getObject(const shared::Id &id) const
{
    if (id.isNull()) {
        return nullptr;
    }

    return d->m_objects.value(id, nullptr);
}

AADLObject *AADLObjectsModel::getObjectByName(const QString &name, AADLObject::Type type) const
{
    if (name.isEmpty())
        return nullptr;

    for (auto obj : d->m_objects)
        if ((type == AADLObject::Type::Unknown || type == obj->aadlType()) && obj->title() == name)
            return obj;
    return nullptr;
}

AADLObjectIface *AADLObjectsModel::getIfaceByName(
        const QString &name, AADLObjectIface::IfaceType dir, AADLObjectFunctionType *parent) const
{
    if (name.isEmpty())
        return nullptr;

    for (auto obj : d->m_objects) {
        if (obj->isInterface() && obj->title() == name)
            if (AADLObjectIface *iface = obj->as<AADLObjectIface *>())
                if (iface->direction() == dir && (!parent || iface->parentObject() == parent))
                    return iface;
    }

    return nullptr;
}

AADLObjectFunction *AADLObjectsModel::getFunction(const shared::Id &id) const
{
    return qobject_cast<AADLObjectFunction *>(getObject(id));
}

AADLObjectFunctionType *AADLObjectsModel::getFunctionType(const shared::Id &id) const
{
    return qobject_cast<AADLObjectFunction *>(getObject(id));
}

QHash<QString, AADLObjectFunctionType *> AADLObjectsModel::getAvailableFunctionTypes(
        const AADLObjectFunction *fnObj) const
{
    QHash<QString, AADLObjectFunctionType *> result;
    if (!fnObj)
        return result;

    auto isValid = [](const AADLObjectFunctionType *objFnType, const AADLObjectFunction *objFn) {
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

    for (AADLObject *obj : d->m_objects)
        if (obj->isFunctionType())
            if (AADLObjectFunctionType *objFnType = qobject_cast<AADLObjectFunctionType *>(obj))
                if (isValid(objFnType, fnObj))
                    result.insert(objFnType->title(), objFnType);

    return result;
}

AADLObjectIface *AADLObjectsModel::getInterface(const shared::Id &id) const
{
    return qobject_cast<AADLObjectIface *>(getObject(id));
}

AADLObjectIfaceRequired *AADLObjectsModel::getRequiredInterface(const shared::Id &id) const
{
    return qobject_cast<AADLObjectIfaceRequired *>(getObject(id));
}

AADLObjectIfaceProvided *AADLObjectsModel::getProvidedInterface(const shared::Id &id) const
{
    return qobject_cast<AADLObjectIfaceProvided *>(getObject(id));
}

AADLObjectConnection *AADLObjectsModel::getConnection(const shared::Id &id) const
{
    return qobject_cast<AADLObjectConnection *>(getObject(id));
}

AADLObjectComment *AADLObjectsModel::getCommentById(const shared::Id &id) const
{
    return qobject_cast<AADLObjectComment *>(getObject(id));
}

AADLObjectConnection *AADLObjectsModel::getConnectionForIface(const shared::Id &id) const
{
    for (auto it = d->m_objects.constBegin(); it != d->m_objects.constEnd(); ++it) {
        if (it.value()->aadlType() != AADLObject::Type::Connection)
            continue;

        if (auto connection = qobject_cast<AADLObjectConnection *>(it.value())) {
            Q_ASSERT(connection->sourceInterface() != nullptr);
            Q_ASSERT(connection->targetInterface() != nullptr);
            if (connection->sourceInterface()->id() == id || connection->targetInterface()->id() == id)
                return connection;
        }
    }
    return nullptr;
}

const QHash<shared::Id, AADLObject *> &AADLObjectsModel::objects() const
{
    return d->m_objects;
}

QVector<AADLObjectConnection *> AADLObjectsModel::getConnectionsForIface(const shared::Id &id) const
{
    QVector<AADLObjectConnection *> result;

    for (auto it = d->m_objects.cbegin(); it != d->m_objects.cend(); ++it)
        if (it.value()->aadlType() == AADLObject::Type::Connection)
            if (auto connection = qobject_cast<AADLObjectConnection *>(it.value()))
                if ((connection->sourceInterface() && connection->sourceInterface()->id() == id)
                        || (connection->targetInterface() && connection->targetInterface()->id() == id))
                    result.append(connection);

    return result;
}

QList<AADLObject *> AADLObjectsModel::visibleObjects() const
{
    return d->m_visibleObjects;
}

//! Get the visible objects with this root id
QList<AADLObject *> AADLObjectsModel::visibleObjects(shared::Id rootId) const
{
    QList<AADLObject *> visibleObjects;
    AADLObject *rootObj = d->m_objects.value(rootId);
    for (const auto &id : d->m_objectsOrder) {
        if (auto obj = getObject(id)) {
            if (rootId.isNull()) {
                visibleObjects.append(obj);
                continue;
            }

            if (!obj->isConnection() && (shared::isAncestorOf(rootObj, obj) || rootObj == nullptr)) {
                visibleObjects.append(obj);
            } else if (auto connection = qobject_cast<aadl::AADLObjectConnection *>(obj)) {
                const bool sourceIfaceAncestor =
                        shared::isAncestorOf<aadl::AADLObject>(rootObj, connection->sourceInterface());
                const bool targetIfaceAncestor =
                        shared::isAncestorOf<aadl::AADLObject>(rootObj, connection->targetInterface());
                if ((sourceIfaceAncestor && targetIfaceAncestor) || rootObj == nullptr) {
                    visibleObjects.append(obj);
                }
            }
        }
    }
    return visibleObjects;
}

void AADLObjectsModel::clear()
{
    Q_EMIT modelReset();

    for (auto object : d->m_objects.values())
        object->deleteLater();

    d->m_objects.clear();
    d->m_objectsOrder.clear();
    d->m_visibleObjects.clear();

    d->m_rootObjectId = shared::InvalidId;
}

}
