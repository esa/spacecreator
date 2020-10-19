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

#include "cmdconditionitemcreate.h"

#include "commandids.h"
#include "mscchart.h"
#include "msccondition.h"
#include "mscinstance.h"

namespace msc {
namespace cmd {

CmdConditionItemCreate::CmdConditionItemCreate(
        MscCondition *condition, msc::MscInstance *instance, int eventIndex, ChartLayoutManager *layoutManager)
    : ChartBaseCommand(condition, layoutManager)
    , m_condition(condition)
    , m_instance(instance)
    , m_eventIndex(eventIndex)
{
    Q_ASSERT(m_chart.data());

    setText(QObject::tr("Add condition"));
}

void CmdConditionItemCreate::redo()
{
    if (!m_condition) {
        m_condition = new MscCondition();
        m_condition->setName(QObject::tr("Condition_%1").arg(m_chart->instanceEvents().size()));
        m_modelItem = m_condition;
    }
    if (m_instance) {
        m_condition->setInstance(m_instance.data());
    } else if (!m_condition->instance() && !m_chart->instances().empty()) {
        m_condition->setInstance(m_chart->instances().at(0));
    }

    // The chart takes over parent-/owner-ship
    m_chart->addInstanceEvent(m_condition, m_eventIndex);

    checkVisualSorting();
}

void CmdConditionItemCreate::undo()
{
    Q_ASSERT(m_chart.data());
    m_chart->removeInstanceEvent(m_condition);

    // this command takes over ownership
    m_condition->setParent(this);

    undoVisualSorting();
}

bool CmdConditionItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdConditionItemCreate::id() const
{
    return msc::cmd::Id::CreateCondition;
}

} // namespace cmd
} // namespace msc
