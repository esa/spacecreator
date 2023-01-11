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

#include "commandids.h"
#include "mscchart.h"
#include "msccondition.h"

namespace msc {
namespace cmd {

CmdConditionItemMove::CmdConditionItemMove(MscCondition *condition, const ChartIndex &newChartIndex, MscChart *chart)
    : EventMoveBaseCommand(condition, chart)
    , m_condition(condition)
    , m_newIndex(newChartIndex)
{
    ChartIndexList indices = m_chart->indicesOfEvent(m_condition);
    if (!indices.isEmpty()) {
        m_oldIndex = indices.first();
    }
    setText(QObject::tr("Move condition"));
}

void CmdConditionItemMove::redo()
{
    if (m_condition && m_chart && m_newIndex.isValid()) {
        storeGeometries();
        applyNewPos();
        m_chart->updateConditionPos(m_condition, m_newIndex);
    }
}

void CmdConditionItemMove::undo()
{
    if (m_condition && m_chart && m_oldIndex.isValid()) {
        m_chart->updateConditionPos(m_condition, m_oldIndex);
        restoreGeometries();
    }
}

bool CmdConditionItemMove::mergeWith(const QUndoCommand *command)
{
    const CmdConditionItemMove *other = static_cast<const CmdConditionItemMove *>(command);
    if (canMergeWith(other)) {
        m_newIndex = other->m_newIndex;
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
