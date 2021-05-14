/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvmodel.h"

#include "common.h"
#include "dvobject.h"

#include <QDebug>
#include <QHash>

namespace dvm {

struct DVModelPrivate {
    shared::Id m_rootObjectId;
    QList<shared::Id> m_objectsOrder;
    QHash<shared::Id, DVObject *> m_objects;
    QList<DVObject *> m_visibleObjects;
};

DVModel::DVModel(QObject *parent)
    : shared::VEModel(parent)
    , d(std::make_unique<DVModelPrivate>())
{
}

DVModel::~DVModel() { }

bool DVModel::addObject(DVObject *obj)
{
    if (addObjectImpl(obj)) {
        if (!obj->postInit()) {
            removeObject(obj);
        } else {
            Q_EMIT objectsAdded({ obj });
            return true;
        }
    }
    return false;
}

bool DVModel::removeObject(DVObject *obj)
{
    if (!obj) {
        return false;
    }

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

/*!
   Sets the root object
 */
void DVModel::setRootObject(shared::Id rootId)
{
    if (d->m_rootObjectId == rootId) {
        return;
    }

    d->m_rootObjectId = rootId;
    d->m_visibleObjects = visibleObjects(rootId);

    Q_EMIT rootObjectChanged(d->m_rootObjectId);
}

/*!
   Returns the root object
 */
DVObject *DVModel::rootObject() const
{
    return getObject(d->m_rootObjectId);
}

/*!
   Returns the ID of the root object
 */
shared::Id DVModel::rootObjectId() const
{
    return d->m_rootObjectId;
}

/*!
   Returns the object with the given ID if included in this model
 */
DVObject *DVModel::getObject(const shared::Id &id) const
{
    if (id.isNull()) {
        return nullptr;
    }

    return d->m_objects.value(id, nullptr);
}

/*!
   Returns all visible objects
 */
QList<DVObject *> DVModel::visibleObjects() const
{
    return d->m_visibleObjects;
}

/*!
   Get the visible objects with this root id
 */
QList<DVObject *> DVModel::visibleObjects(shared::Id rootId) const
{
    QList<DVObject *> visibleObjects;
    DVObject *rootObj = d->m_objects.value(rootId);
    for (const auto &id : qAsConst(d->m_objectsOrder)) {
        if (auto obj = getObject(id)) {
            if (rootId.isNull()) {
                visibleObjects.append(obj);
                continue;
            }

            if (shared::isAncestorOf(rootObj, obj) || rootObj == nullptr) {
                visibleObjects.append(obj);
            }
        }
    }
    return visibleObjects;
}

/*!
   Resets the whole model to the initial state
 */
void DVModel::clear()
{
    for (auto object : d->m_objects.values()) {
        object->deleteLater();
    }

    d->m_objects.clear();
    d->m_objectsOrder.clear();
    d->m_visibleObjects.clear();

    d->m_rootObjectId = shared::InvalidId;
    Q_EMIT modelReset();
}

void DVModel::initFromObjects(const QVector<DVObject *> &objects)
{
    clear();
    addObjects(objects);
}

void DVModel::addObjects(const QVector<DVObject *> &objects)
{
    QVector<DVObject *> addedObjects;
    for (auto obj : objects) {
        if (addObjectImpl(obj)) {
            addedObjects.append(obj);
        }
    }

    for (auto it = addedObjects.begin(); it != addedObjects.end(); ++it) {
        if (DVObject *obj = *it) {
            if (!obj->postInit()) {
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

bool DVModel::addObjectImpl(DVObject *obj)
{
    if (!obj) {
        return false;
    }

    const shared::Id &id = obj->id();
    if (getObject(id)) {
        return false;
    }

    if (!obj->parent()) {
        obj->setParent(this);
    }

    obj->setModel(this);

    d->m_objects.insert(id, obj);
    d->m_objectsOrder.append(id);
    d->m_visibleObjects.append(obj);
    return true;
}

} // namespace deploy
