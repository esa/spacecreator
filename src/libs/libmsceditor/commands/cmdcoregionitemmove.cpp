/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdcoregionitemmove.h"

#include "common/commandids.h"
#include "mscchart.h"
#include "msccoregion.h"
#include "mscinstance.h"

namespace msc {
namespace cmd {

CmdCoRegionItemMove::CmdCoRegionItemMove(MscCoregion *coregionBegin, MscCoregion *coregionEnd, int newBeginPos,
        int newEndPos, MscInstance *newInsance, MscChart *chart, ChartLayoutManager *layoutManager)
    : ChartBaseCommand(coregionBegin, chart, layoutManager)
    , m_coregionBegin(coregionBegin)
    , m_coregionEnd(coregionEnd)
    , m_oldBeginIndex(chart->instanceEvents().indexOf(coregionBegin))
    , m_oldEndIndex(chart->instanceEvents().indexOf(coregionEnd))
    , m_newBeginIndex(newBeginPos)
    , m_newEndIndex(newEndPos)
    , m_oldInstance(coregionEnd->instance())
    , m_newInstance(newInsance)
{
    setText(QObject::tr("Move coregion"));
}

void CmdCoRegionItemMove::redo()
{
    if (m_coregionBegin && m_coregionEnd && m_chart && m_newInstance) {
        m_chart->updateCoregionPos(
                m_coregionBegin.data(), m_coregionEnd.data(), m_newInstance, m_newBeginIndex, m_newEndIndex);
        checkVisualSorting();
    }
}

void CmdCoRegionItemMove::undo()
{
    if (m_coregionBegin && m_coregionEnd && m_chart && m_oldInstance) {
        m_chart->updateCoregionPos(
                m_coregionBegin.data(), m_coregionEnd.data(), m_oldInstance, m_oldBeginIndex, m_oldEndIndex);
        undoVisualSorting();
    }
}

bool CmdCoRegionItemMove::mergeWith(const QUndoCommand *command)
{
    auto other = dynamic_cast<const CmdCoRegionItemMove *>(command);
    if (canMergeWith(other)) {
        m_newBeginIndex = other->m_newBeginIndex;
        m_newEndIndex = other->m_newEndIndex;
        m_newInstance = other->m_newInstance;
        return true;
    }

    return false;
}

int CmdCoRegionItemMove::id() const
{
    return msc::cmd::Id::MoveCoRegion;
}

}
}
