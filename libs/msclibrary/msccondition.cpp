/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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
    m_instance = instance;
    Q_EMIT dataChanged();
}

MscEntity::EntityType MscCondition::entityType() const
{
    return MscEntity::EntityType::Condition;
}

bool MscCondition::relatesTo(MscInstance *instance) const
{
    return m_instance == instance;
}

} // namespace msc
