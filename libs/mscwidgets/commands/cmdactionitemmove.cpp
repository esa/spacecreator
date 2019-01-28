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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "cmdactionitemmove.h"

#include <mscaction.h>
#include <mscchart.h>
#include <mscinstance.h>

namespace msc {
namespace cmd {

CmdActionItemMove::CmdActionItemMove(MscAction *action, int newPos, MscInstance *newInsance, MscChart *chart)
    : BaseCommand(action)
    , m_action(action)
    , m_oldIndex(chart->instanceEvents().indexOf(action))
    , m_newIndex(newPos)
    , m_oldInstance(action->instance())
    , m_newInstance(newInsance)
    , m_chart(chart)
{
    setText(QObject::tr("Move action"));
}

void CmdActionItemMove::redo()
{
    if (m_action && m_chart && m_newInstance) {
        m_chart->updateActionPos(m_action, m_newInstance, m_newIndex);
    }
}

void CmdActionItemMove::undo()
{
    if (m_action && m_chart && m_oldInstance) {
        m_chart->updateActionPos(m_action, m_oldInstance, m_oldIndex);
    }
}

bool CmdActionItemMove::mergeWith(const QUndoCommand *command)
{
    const CmdActionItemMove *other = dynamic_cast<const CmdActionItemMove *>(command);
    if (canMergeWith(other)) {
        return true;
    }

    return false;
}

int CmdActionItemMove::id() const
{
    return msc::cmd::Id::MoveAction;
}

} // namespace cmd
} // namespace msc
