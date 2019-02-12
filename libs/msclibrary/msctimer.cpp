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

#include "msctimer.h"

#include "mscinstance.h"

namespace msc {

MscTimer::MscTimer(QObject *parent)
    : MscInstanceEvent(parent)
{
}

MscTimer::MscTimer(const QString &name, TimerType type, QObject *parent)
    : MscInstanceEvent(name, parent)
    , m_timerType(type)
{
}

MscEntity::EntityType MscTimer::entityType() const
{
    return MscEntity::EntityType::Timer;
}

void MscTimer::setTimerType(MscTimer::TimerType type)
{
    if (type == m_timerType) {
        return;
    }
    m_timerType = type;

    Q_EMIT timerTypeChanged();
    Q_EMIT dataChanged();
}

MscTimer::TimerType MscTimer::timerType() const
{
    return m_timerType;
}

void MscTimer::setInstance(MscInstance *instance)
{
    if (instance == m_instance) {
        return;
    }

    m_instance = instance;
    Q_EMIT instanceChanged();
    Q_EMIT dataChanged();
}

MscInstance *MscTimer::instance() const
{
    return m_instance;
}

bool MscTimer::relatesTo(MscInstance *instance) const
{
    return m_instance == instance;
}

void MscTimer::setPrecedingTimer(MscTimer *timer)
{
    if (m_precedingTimer == timer) {
        return;
    }

    m_precedingTimer = timer;
    Q_EMIT precedingTimerChanged();
    Q_EMIT dataChanged();
}

MscTimer *MscTimer::precedingTimer() const
{
    return m_precedingTimer;
}

void MscTimer::setFollowingTimer(MscTimer *timer)
{
    if (m_followingTimer == timer) {
        return;
    }

    m_followingTimer = timer;
    Q_EMIT followingTimerChanged();
    Q_EMIT dataChanged();
}

MscTimer *MscTimer::followingTimer() const
{
    return m_followingTimer;
}

}
