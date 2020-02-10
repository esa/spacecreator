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

#include "aadlobjectfunction.h"
#include "app/common.h"
#include "baseitems/common/utils.h"

#include <QtDebug>

namespace taste3 {
namespace aadl {

struct AADLObjectsModelPrivate {
    common::Id m_rootObjectId;
    QList<common::Id> m_objectsOrder;
    QHash<common::Id, AADLObject *> m_objects;
    QList<AADLObject *> m_visibleObjects;
};

AADLObjectsModel::AADLObjectsModel(QObject *parent)
    : QObject(parent)
    , d(new AADLObjectsModelPrivate)
{
}

AADLObjectsModel::~AADLObjectsModel() {}

bool AADLObjectsModel::initFromObjects(const QVector<AADLObject *> &objects)
{
    emit modelReset();

    for (auto object : d->m_objects.values())
        object->deleteLater();

    d->m_objects.clear();
    d->m_objectsOrder.clear();
    d->m_visibleObjects.clear();

    d->m_rootObjectId = common::Id();

    const bool currentState = blockSignals(true);

    for (auto obj : objects)
        addObject(obj);

    for (auto obj : objects)
        obj->postInit();

    blockSignals(currentState);

    emit rootObjectChanged(d->m_rootObjectId);

    return true;
}

bool AADLObjectsModel::addObject(AADLObject *obj)
{
    if (!obj)
        return false;

    const common::Id &id = obj->id();
    if (getObject(id))
        return false;

    if (!obj->parent())
        obj->setParent(this);

    obj->setObjectsModel(this);

    d->m_objects.insert(id, obj);
    d->m_objectsOrder.append(id);
    d->m_visibleObjects.append(obj);
    emit aadlObjectAdded(obj);
    return true;
}

bool AADLObjectsModel::removeObject(AADLObject *obj)
{
    if (!obj)
        return false;

    const common::Id &id = obj->id();
    if (!getObject(id))
        return false;

    d->m_objects.remove(id);
    d->m_objectsOrder.removeAll(id);
    d->m_visibleObjects.removeAll(obj);

    emit aadlObjectRemoved(obj);
    return true;
}

void AADLObjectsModel::setRootObject(common::Id rootId)
{
    if (d->m_rootObjectId == rootId)
        return;

    d->m_rootObjectId = rootId;
    d->m_visibleObjects.clear();
    emit modelReset();

    auto rootObj = d->m_objects.value(rootId);
    for (const auto &id : d->m_objectsOrder) {
        if (auto obj = getObject(id)) {
            if (rootId.isNull()) {
                d->m_visibleObjects.append(obj);
                continue;
            }

            if (obj->aadlType() != aadl::AADLObject::AADLObjectType::AADLConnection
                && (utils::isAncestorOf(rootObj, obj) || rootObj == nullptr)) {
                d->m_visibleObjects.append(obj);
            } else if (auto connection = qobject_cast<aadl::AADLObjectConnection *>(obj)) {
                const bool sourceIfaceAncestor =
                        utils::isAncestorOf<aadl::AADLObject>(rootObj, connection->sourceInterface());
                const bool targetIfaceAncestor =
                        utils::isAncestorOf<aadl::AADLObject>(rootObj, connection->targetInterface());
                if ((sourceIfaceAncestor && targetIfaceAncestor) || rootObj == nullptr) {
                    d->m_visibleObjects.append(obj);
                }
            }
        }
    }
    emit rootObjectChanged(d->m_rootObjectId);
}

AADLObject *AADLObjectsModel::rootObject() const
{
    return getObject(d->m_rootObjectId);
}

AADLObject *AADLObjectsModel::getObject(const common::Id &id) const
{
    if (id.isNull())
        return nullptr;

    return d->m_objects.value(id, nullptr);
}

AADLObject *AADLObjectsModel::getObjectByName(const QString &name, AADLObject::AADLObjectType type) const
{
    if (name.isEmpty())
        return nullptr;

    for (auto obj : d->m_objects)
        if ((type == AADLObject::AADLObjectType::AADLUnknown || type == obj->aadlType()) && obj->title() == name)
            return obj;
    return nullptr;
}

AADLObjectIface *AADLObjectsModel::getIfaceByName(const QString &name) const
{
    if (name.isEmpty())
        return nullptr;

    for (auto obj : d->m_objects) {
        if (AADLObject::AADLObjectType::AADLIface == obj->aadlType() && obj->title() == name)
            return qobject_cast<AADLObjectIface *>(obj);
    }

    return nullptr;
}

AADLObjectFunction *AADLObjectsModel::getFunction(const common::Id &id) const
{
    return qobject_cast<AADLObjectFunction *>(getObject(id));
}

AADLObjectFunctionType *AADLObjectsModel::getFunctionType(const common::Id &id) const
{
    return qobject_cast<AADLObjectFunction *>(getObject(id));
}

QHash<QString, AADLObjectFunctionType *>
AADLObjectsModel::getAvailableFunctionTypes(const AADLObjectFunction *fnObj) const
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
        if (obj->aadlType() == AADLObject::AADLObjectType::AADLFunctionType)
            if (AADLObjectFunctionType *objFnType = qobject_cast<AADLObjectFunctionType *>(obj))
                if (isValid(objFnType, fnObj))
                    result.insert(objFnType->title(), objFnType);

    return result;
}

AADLObjectIface *AADLObjectsModel::getInterface(const common::Id &id) const
{
    return qobject_cast<AADLObjectIface *>(getObject(id));
}

AADLObjectIfaceRequired *AADLObjectsModel::getRequiredInterface(const common::Id &id) const
{
    return qobject_cast<AADLObjectIfaceRequired *>(getObject(id));
}

AADLObjectIfaceProvided *AADLObjectsModel::getProvidedInterface(const common::Id &id) const
{
    return qobject_cast<AADLObjectIfaceProvided *>(getObject(id));
}

AADLObjectConnection *AADLObjectsModel::getConnection(const common::Id &id) const
{
    return qobject_cast<AADLObjectConnection *>(getObject(id));
}

AADLObjectComment *AADLObjectsModel::getCommentById(const common::Id &id) const
{
    return qobject_cast<AADLObjectComment *>(getObject(id));
}

AADLObjectConnection *AADLObjectsModel::getConnectionForIface(const common::Id &id) const
{
    for (auto it = d->m_objects.constBegin(); it != d->m_objects.constEnd(); ++it) {
        if (it.value()->aadlType() != AADLObject::AADLObjectType::AADLConnection)
            continue;

        if (auto connection = qobject_cast<AADLObjectConnection *>(it.value())) {
            if (connection->sourceInterface()->id() == id || connection->targetInterface()->id() == id)
                return connection;
        }
    }
    return nullptr;
}

const QHash<common::Id, AADLObject *> &AADLObjectsModel::objects() const
{
    return d->m_objects;
}

QList<AADLObject *> AADLObjectsModel::visibleObjects() const
{
    return d->m_visibleObjects;
}

} // ns aadl

} // ns taste3
