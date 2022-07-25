/*
  Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "archetypemodel.h"

#include "common.h"
#include "errorhub.h"

#include <QtDebug>

namespace ivm {

struct ArchetypeModelPrivate {
    shared::Id m_rootObjectId;
};

ArchetypeModel::ArchetypeModel(QObject *parent)
    : shared::VEModel(parent)
    , m_privateModel(new ArchetypeModelPrivate)
{
}

ArchetypeModel::~ArchetypeModel() {}

void ArchetypeModel::setRootObject(shared::Id rootId)
{
    if (m_privateModel->m_rootObjectId == rootId) {
        return;
    }

    m_privateModel->m_rootObjectId = rootId;
}

ArchetypeObject *ArchetypeModel::rootObject() const
{
    return getObject(m_privateModel->m_rootObjectId);
}

shared::Id ArchetypeModel::rootObjectId() const
{
    return m_privateModel->m_rootObjectId;
}

ArchetypeObject *ArchetypeModel::getObject(const shared::Id &id) const
{
    return qobject_cast<ArchetypeObject *>(shared::VEModel::getObject(id));
}

ArchetypeObject *ArchetypeModel::getObjectByName(const QString &name, ArchetypeObject::Type type, Qt::CaseSensitivity caseSensitivity) const
{
    if (name.isEmpty())
        return nullptr;

    for (auto object : objects()) {
        if (auto obj = qobject_cast<ivm::ArchetypeObject *>(object)) {
            if ((type == ArchetypeObject::Type::Unknown || type == obj->type())
                    && obj->title().compare(name, caseSensitivity) == 0)
                return obj;
        }
    }
    return nullptr;
}

void ArchetypeModel::clear()
{
    m_privateModel->m_rootObjectId = shared::InvalidId;
    shared::VEModel::clear();
}
}
