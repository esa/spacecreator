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

#include "cmdcoregionitemcreate.h"

#include "commandids.h"
#include "mscchart.h"
#include "msccoregion.h"
#include "mscinstance.h"

namespace msc {
namespace cmd {

CmdCoregionItemCreate::CmdCoregionItemCreate(msc::MscCoregion *begin, msc::MscCoregion *end, msc::MscInstance *instance,
        int eventIndex, ChartLayoutManager *layoutManager)
    : ChartBaseCommand(nullptr, layoutManager)
    , m_begin(begin)
    , m_end(end)
    , m_instance(instance)
    , m_eventIndex(eventIndex)
{
    Q_ASSERT(m_begin != nullptr);
    Q_ASSERT(m_end != nullptr);
}

void CmdCoregionItemCreate::redo()
{
    m_begin->setInstance(m_instance.data());
    m_end->setInstance(m_instance.data());

    // The chart takes over parent-/owner-ship
    ChartIndexList instanceIndexes { { m_instance, m_eventIndex } };
    m_chart->addInstanceEvent(m_begin, instanceIndexes);
    instanceIndexes = { { m_instance, m_eventIndex + 1 } };
    m_chart->addInstanceEvent(m_end, instanceIndexes);

    checkVisualSorting();
}

void CmdCoregionItemCreate::undo()
{
    Q_ASSERT(m_chart.data());
    m_chart->removeInstanceEvent(m_begin);
    m_chart->removeInstanceEvent(m_end);

    // this command takes over ownership
    m_begin->setParent(this);
    m_end->setParent(this);

    undoVisualSorting();
}

bool CmdCoregionItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdCoregionItemCreate::id() const
{
    return msc::cmd::Id::CreateCoregion;
}

} // namespace cmd
} // namespace msc
