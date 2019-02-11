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

#include "cmdtimeritemcreate.h"

#include "mscchart.h"
#include "mscinstance.h"

namespace msc {
namespace cmd {

CmdTimerItemCreate::CmdTimerItemCreate(MscTimer *timer, MscTimer::TimerType timerType, MscChart *chart,
                                       msc::MscInstance *instance, int eventIndex)
    : BaseCommand(timer)
    , m_timer(timer)
    , m_chart(chart)
    , m_instance(instance)
    , m_eventIndex(eventIndex)
    , m_timerType(m_timer ? m_timer->timerType() : timerType)
{
    Q_ASSERT(m_chart.data());

    setText(QObject::tr("Add timer"));
}

void CmdTimerItemCreate::redo()
{
    if (!m_timer) {
        m_timer = new MscTimer();
        m_timer->setTimerType(MscTimer::TimerType::Start);
        m_timer->setName(tr("New timer"));
        m_modelItem = m_timer;
    }
    if (m_instance) {
        m_timer->setInstance(m_instance.data());
    } else if (!m_timer->instance() && !m_chart->instances().empty()) {
        m_timer->setInstance(m_chart->instances().at(0));
    }

    // The chart takes over parent-/owner-ship
    m_chart->addInstanceEvent(m_timer, m_eventIndex);
}

void CmdTimerItemCreate::undo()
{
    Q_ASSERT(m_chart.data());
    m_chart->removeInstanceEvent(m_timer);

    // this command takes over ownership
    m_timer->setParent(this);
}

bool CmdTimerItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdTimerItemCreate::id() const
{
    return msc::cmd::Id::CreateTimer;
}

} // namespace cmd
} // namespace msc
