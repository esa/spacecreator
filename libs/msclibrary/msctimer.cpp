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

#include "msctimer.h"

#include "mscinstance.h"

namespace msc {

MscTimer::MscTimer(QObject *parent)
    : MscInstanceEvent(parent)
{
    connect(this, &MscInstanceEvent::nameChanged, this, [this](const QString &name) {
        if (auto precedingTimer = this->precedingTimer()) {
            const QSignalBlocker blocker(precedingTimer);
            precedingTimer->setName(name);
        }

        if (auto followingTimer = this->followingTimer()) {
            const QSignalBlocker blocker(followingTimer);
            followingTimer->setName(name);
        }
    });
}

MscTimer::MscTimer(const QString &name, TimerType type, QObject *parent)
    : MscTimer(parent)
{
    setName(name);
    setTimerType(type);
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

bool MscTimer::relatesTo(const MscInstance *instance) const
{
    return m_instance == instance;
}

void MscTimer::setPrecedingTimer(MscTimer *timer)
{
    if (m_precedingTimer == timer)
        return;

    m_precedingTimer = allowPrecedingTimer(timer) ? timer : nullptr;

    Q_EMIT precedingTimerChanged();
    Q_EMIT dataChanged();
}

MscTimer *MscTimer::precedingTimer() const
{
    return m_precedingTimer;
}

bool MscTimer::allowPrecedingTimer(MscTimer *timer)
{
    if (timer == nullptr)
        return true;

    if (timer->m_instance != m_instance || m_instance == nullptr || timer->name() != name())
        return false;

    if (timer->m_timerType == TimerType::Start || m_timerType == TimerType::Start)
        return true;

    if (timer->m_timerType == TimerType::Timeout || timer->m_timerType == TimerType::Stop)
        return false;

    return true;
}

void MscTimer::setFollowingTimer(MscTimer *timer)
{
    if (m_followingTimer == timer)
        return;

    m_followingTimer = allowFollowingTimer(timer) ? timer : nullptr;

    Q_EMIT followingTimerChanged();
    Q_EMIT dataChanged();
}

MscTimer *MscTimer::followingTimer() const
{
    return m_followingTimer;
}

bool MscTimer::allowFollowingTimer(MscTimer *timer)
{
    if (timer == nullptr)
        return true;

    if (timer->m_instance != m_instance || m_instance == nullptr || timer->name() != name())
        return false;

    if (m_timerType == TimerType::Start || timer->m_timerType == TimerType::Start)
        return true;

    if (m_timerType == TimerType::Timeout || m_timerType == TimerType::Stop)
        return false;

    return true;
}

/*!
   \fn MscTimer::timerInstanceName
   Where the <timer name> is not sufficient for a unique mapping the <timer instance name> must be
   employed.
 */
const QString &MscTimer::timerInstanceName() const
{
    return m_timerInstanceName;
}

void MscTimer::setTimerInstanceName(const QString &name)
{
    if (name == m_timerInstanceName) {
        return;
    }

    m_timerInstanceName = name;
    Q_EMIT timerInstanceNameChanged();
    Q_EMIT dataChanged();
}

QString MscTimer::fullName() const
{
    return m_timerInstanceName.isEmpty() ? name() : QString("%1,%2").arg(name(), m_timerInstanceName);
}

}
