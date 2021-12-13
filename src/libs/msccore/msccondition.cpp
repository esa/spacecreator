/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "msccondition.h"

#include "mscinstance.h"

namespace msc {

MscCondition::MscCondition(QObject *parent)
    : MscInstanceEvent(parent)
{
}

MscCondition::MscCondition(const QString &name, QObject *parent)
    : MscInstanceEvent(name, parent)
{
}

bool MscCondition::shared() const
{
    return m_shared;
}

void MscCondition::setShared(bool shared)
{
    if (shared == m_shared) {
        return;
    }
    m_shared = shared;
    Q_EMIT sharedChanged();
    Q_EMIT dataChanged();
}

MscInstance *MscCondition::instance() const
{
    return m_instance;
}

void MscCondition::setInstance(MscInstance *instance)
{
    if (instance == m_instance) {
        return;
    }

    MscInstance *oldInstance = m_instance;
    m_instance = instance;
    Q_EMIT instanceChanged();
    Q_EMIT instanceRelationChanged(m_instance, oldInstance);
    Q_EMIT dataChanged();
}

MscEntity *MscCondition::instanceObj() const
{
    return m_instance;
}

void MscCondition::setInstanceObj(MscEntity *instance)
{
    if (auto inst = qobject_cast<MscInstance *>(instance)) {
        setInstance(inst);
    }
}

MscEntity::EntityType MscCondition::entityType() const
{
    return MscEntity::EntityType::Condition;
}

bool MscCondition::relatesTo(const MscInstance *instance) const
{
    if (m_shared) {
        return true;
    }

    return m_instance == instance;
}

} // namespace msc
