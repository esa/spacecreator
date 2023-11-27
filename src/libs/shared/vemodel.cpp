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
    QHash<shared::Id, EntityAttributes> m_extAttrs;
};

VEModel::VEModel(QObject *parent)
    : shared::DataModel(parent)
    , d(std::make_unique<VEModelPrivate>())
{
}

VEModel::~VEModel() { }

/**
 * Returns true, if the is no object in this model
 */
bool VEModel::isEmpty() const
{
    return d->m_objects.isEmpty();
}

bool VEModel::addObject(VEObject *obj)
{
    try {
        if (addObjectImpl(obj)) {
            if (!obj->postInit()) {
                removeObject(obj);
            } else {
                Q_EMIT objectsAdded({ obj->id() });
                return true;
            }
        }
    } catch (InconsistentModelException &e) {
        qDebug() << e.what();
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
    d->m_extAttrs.remove(id);

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
    d->m_extAttrs.clear();

    Q_EMIT modelReset();
}

VEObject *VEModel::getObjectByAttributeValue(const QString &attrName, const QVariant &value) const
{
    auto it = std::find_if(d->m_objects.constBegin(), d->m_objects.constEnd(),
            [&](VEObject *object) { return object->entityAttributeValue(attrName) == value; });
    return it == d->m_objects.constEnd() ? nullptr : *it;
}

const QList<Id> &VEModel::objectsOrder() const
{
    return d->m_objectsOrder;
}

const QHash<Id, VEObject *> &VEModel::objects() const
{
    return d->m_objects;
}

void VEModel::setExtAttributes(const QHash<Id, EntityAttributes> &attrs)
{
    d->m_extAttrs = attrs;
}

QHash<Id, EntityAttributes> VEModel::extAttributes() const
{
    return d->m_extAttrs;
}

EntityAttributes VEModel::extEntityAttributes(const Id &id) const
{
    return d->m_extAttrs.value(id);
}

VEObject *VEModel::getObject(const Id &id) const
{
    if (id.isNull()) {
        return nullptr;
    }

    return getObject(d->m_objects, id);
}

bool VEModel::addObjectImpl(VEObject *obj)
{
    if (!obj) {
        return false;
    }

    const shared::Id &id = obj->id();
    if (getObject(id)) {
        throw InconsistentModelException();
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
