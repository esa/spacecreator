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

#ifndef MSCTIMER_H
#define MSCTIMER_H

#include "mscinstanceevent.h"

namespace msc {

class MscInstance;

class MscTimer : public MscInstanceEvent
{
    Q_OBJECT

public:
    enum class TimerType
    {
        Start,
        Stop,
        Timeout,
        Unknown
    };

    explicit MscTimer(QObject *parent = nullptr);
    MscTimer(const QString &name, TimerType type, QObject *parent = nullptr);

    MscEntity::EntityType entityType() const override;

    void setTimerType(TimerType type);
    TimerType timerType() const;

    void setInstance(msc::MscInstance *instance);
    msc::MscInstance *instance() const;

    bool relatesTo(MscInstance *instance) const override;

Q_SIGNALS:
    void timerTypeChanged();
    void instanceChanged();

private:
    TimerType m_timerType = TimerType::Unknown;
    MscInstance *m_instance = nullptr;
};

}

#endif // MSCTIMER_H
