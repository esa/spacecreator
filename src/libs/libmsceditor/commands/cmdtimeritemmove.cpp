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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "cmdtimeritemmove.h"

#include "commandids.h"
#include "mscchart.h"
#include "mscinstance.h"
#include "msctimer.h"

namespace msc {
namespace cmd {

CmdTimerItemMove::CmdTimerItemMove(
        msc::MscTimer *timer, int newPos, MscInstance *newInsance, ChartLayoutManager *layoutManager)
    : ChartBaseCommand(timer, layoutManager)
    , m_timer(timer)
    , m_oldIndex(m_chart->instanceEvents().indexOf(timer))
    , m_newIndex(newPos)
    , m_oldInstance(timer->instance())
    , m_newInstance(newInsance)
{
    setText(QObject::tr("Move timer"));
}

void CmdTimerItemMove::redo()
{
    if (m_timer && m_chart && m_newInstance) {
        m_chart->updateTimerPos(m_timer, m_newInstance, m_newIndex);
        checkVisualSorting();
    }
}

void CmdTimerItemMove::undo()
{
    if (m_timer && m_chart && m_oldInstance) {
        m_chart->updateTimerPos(m_timer, m_oldInstance, m_oldIndex);
        undoVisualSorting();
    }
}

bool CmdTimerItemMove::mergeWith(const QUndoCommand *command)
{
    const CmdTimerItemMove *other = dynamic_cast<const CmdTimerItemMove *>(command);
    if (canMergeWith(other)) {
        m_newIndex = other->m_newIndex;
        m_newInstance = other->m_newInstance;
        return true;
    }

    return false;
}

int CmdTimerItemMove::id() const
{
    return msc::cmd::Id::MoveTimer;
}

} // namespace cmd
} // namespace msc
