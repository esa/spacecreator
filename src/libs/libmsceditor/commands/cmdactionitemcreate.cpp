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

#include "cmdactionitemcreate.h"

#include "commandids.h"
#include "mscaction.h"
#include "mscchart.h"
#include "mscinstance.h"

namespace msc {
namespace cmd {

CmdActionItemCreate::CmdActionItemCreate(
        MscAction *action, msc::MscInstance *instance, int eventIndex, ChartLayoutManager *layoutManager)
    : ChartBaseCommand(action, layoutManager)
    , m_action(action)
    , m_instance(instance)
    , m_eventIndex(eventIndex)
{
    Q_ASSERT(m_chart.data());

    setText(QObject::tr("Add action"));
}

void CmdActionItemCreate::redo()
{
    if (!m_action) {
        m_action = new MscAction();
        m_action->setInformalAction(QObject::tr("Action_%1").arg(m_chart->instanceEvents().size()));
        m_modelItem = m_action;
    }
    if (m_instance) {
        m_action->setInstance(m_instance.data());
    } else if (!m_action->instance() && !m_chart->instances().empty()) {
        m_action->setInstance(m_chart->instances().at(0));
    }

    // The chart takes over parent-/owner-ship
    m_chart->addInstanceEvent(m_action, m_eventIndex);

    checkVisualSorting();
}

void CmdActionItemCreate::undo()
{
    Q_ASSERT(m_chart.data());
    m_chart->removeInstanceEvent(m_action);

    // this command takes over ownership
    m_action->setParent(this);

    undoVisualSorting();
}

bool CmdActionItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdActionItemCreate::id() const
{
    return msc::cmd::Id::CreateAction;
}

} // namespace cmd
} // namespace msc
