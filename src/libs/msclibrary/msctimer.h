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

#pragma once

#include "mscinstanceevent.h"

#include <QPointer>

namespace msc {

class MscInstance;

class MscTimer : public MscInstanceEvent
{
    Q_OBJECT
    Q_PROPERTY(msc::MscTimer::TimerType timerType READ timerType WRITE setTimerType NOTIFY timerTypeChanged)
    Q_PROPERTY(msc::MscInstance *instance READ instance WRITE setInstance NOTIFY instanceChanged)
    Q_PROPERTY(
            QString timerInstanceName READ timerInstanceName WRITE setTimerInstanceName NOTIFY timerInstanceNameChanged)

public:
    enum class TimerType
    {
        Start,
        Stop,
        Timeout,
        Unknown
    };
    Q_ENUM(TimerType)

    explicit MscTimer(QObject *parent = nullptr);
    MscTimer(const QString &name, TimerType type, QObject *parent = nullptr);

    MscEntity::EntityType entityType() const override;

    void setTimerType(TimerType type);
    TimerType timerType() const;

    void setInstance(msc::MscInstance *instance);
    msc::MscInstance *instance() const;

    bool relatesTo(const MscInstance *instance) const override;

    void setPrecedingTimer(MscTimer *timer);
    MscTimer *precedingTimer() const;
    bool allowPrecedingTimer(MscTimer *timer);

    void setFollowingTimer(MscTimer *timer);
    MscTimer *followingTimer() const;
    bool allowFollowingTimer(MscTimer *timer);

    const QString &timerInstanceName() const;
    void setTimerInstanceName(const QString &name);

    QString fullName() const;

Q_SIGNALS:
    void timerTypeChanged();
    void instanceChanged();
    void precedingTimerChanged();
    void followingTimerChanged();
    void timerInstanceNameChanged();

private:
    TimerType m_timerType = TimerType::Unknown;
    MscInstance *m_instance = nullptr;
    QPointer<MscTimer> m_precedingTimer;
    QPointer<MscTimer> m_followingTimer;
    QString m_timerInstanceName;
};
}
