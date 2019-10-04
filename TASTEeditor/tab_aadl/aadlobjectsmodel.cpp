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

#include "app/common.h"

namespace taste3 {
namespace aadl {

struct AADLObjectsModelPrivate {
    QHash<common::Id, AADLObject *> m_objects;
};

AADLObjectsModel::AADLObjectsModel(QObject *parent)
    : QObject(parent)
    , d(new AADLObjectsModelPrivate)
{
}

AADLObjectsModel::~AADLObjectsModel() {}

bool AADLObjectsModel::initFromObjects(const QVector<AADLObject *> &objects)
{
    qDeleteAll(d->m_objects);
    d->m_objects.clear();

    for (auto obj : objects)
        addObject(obj);

    return d->m_objects.size() == objects.size();
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

    d->m_objects.insert(id, obj);
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
    emit aadlObjectRemoved(obj);
    return true;
}

AADLObject *AADLObjectsModel::getObject(const common::Id &id) const
{
    return d->m_objects.value(id, nullptr);
}

AADLObjectFunction *AADLObjectsModel::getFunction(const common::Id &id) const
{
    return qobject_cast<AADLObjectFunction *>(getObject(id));
}

AADLObjectContainer *AADLObjectsModel::getContainer(const common::Id &id) const
{
    return qobject_cast<AADLObjectFunction *>(getObject(id));
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
            if (connection->requiredInterface()->id() == id || connection->providedInterface()->id() == id)
                return connection;
        }
    }
    return nullptr;
}

} // ns aadl

} // ns taste3
