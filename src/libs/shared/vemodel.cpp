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

#include "vemodel.h"

#include "veobject.h"

namespace shared {

struct VEModelPrivate {
    QList<shared::Id> m_objectsOrder;
    QHash<shared::Id, VEObject *> m_objects;
};

VEModel::VEModel(QObject *parent)
    : QObject(parent)
    , d(std::make_unique<VEModelPrivate>())
{
}

VEModel::~VEModel() { }

bool VEModel::addObject(VEObject *obj)
{
    if (addObjectImpl(obj)) {
        if (!obj->postInit()) {
            removeObject(obj);
        } else {
            Q_EMIT objectsAdded({ obj->id() });
            return true;
        }
    }
    return false;
}

bool VEModel::removeObject(VEObject *obj)
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

    Q_EMIT objectRemoved(obj->id());
    return true;
}

/*!
   Resets the whole model to the initial state
 */
void VEModel::clear()
{
    const QList<shared::Id> order = objectsOrder();
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        if (auto obj = d->m_objects.value(*it)) {
            removeObject(obj);
            if (!obj->parent() || obj->parent() == this) {
                delete obj;
            }
        }
    }
    d->m_objects.clear();
    d->m_objectsOrder.clear();

    Q_EMIT modelReset();
}

QList<Id> VEModel::objectsOrder() const
{
    return d->m_objectsOrder;
}

QHash<Id, VEObject *> VEModel::objects() const
{
    return d->m_objects;
}

VEObject *VEModel::getObject(const Id &id) const
{
    if (id.isNull()) {
        return nullptr;
    }

    return d->m_objects.value(id, nullptr);
}

bool VEModel::addObjectImpl(VEObject *obj)
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
    return true;
}

} // namespace shared
