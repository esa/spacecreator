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

#include "cmdconditionitemmove.h"

#include "common/commandids.h"
#include "conditionitem.h"
#include "mscchart.h"
#include "msccondition.h"
#include "mscinstance.h"

namespace msc {
namespace cmd {

CmdConditionItemMove::CmdConditionItemMove(
        MscCondition *condition, int newPos, MscInstance *newInsance, ChartLayoutManager *layoutManager)
    : ChartBaseCommand(condition, layoutManager)
    , m_condition(condition)
    , m_oldIndex(m_chart->instanceEvents().indexOf(condition))
    , m_newIndex(newPos)
    , m_oldInstance(condition->instance())
    , m_newInstance(newInsance)
{
    setText(QObject::tr("Move condition"));
}

void CmdConditionItemMove::redo()
{
    if (m_condition && m_chart && m_newInstance) {
        m_chart->updateConditionPos(m_condition, m_newInstance, m_newIndex);
        checkVisualSorting();
    }
}

void CmdConditionItemMove::undo()
{
    if (m_condition && m_chart && m_newInstance) {
        m_chart->updateConditionPos(m_condition, m_oldInstance, m_oldIndex);
        undoVisualSorting();
    }
}

bool CmdConditionItemMove::mergeWith(const QUndoCommand *command)
{
    const CmdConditionItemMove *other = static_cast<const CmdConditionItemMove *>(command);
    if (canMergeWith(other)) {
        m_newIndex = other->m_newIndex;
        m_newInstance = other->m_newInstance;
        return true;
    }

    return false;
}

int CmdConditionItemMove::id() const
{
    return msc::cmd::Id::MoveCondition;
}

} // ns cmd
} // ns msc
