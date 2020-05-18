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

#include "cmdactioninformaltext.h"

#include "common/commandids.h"
#include "mscaction.h"

namespace msc {
namespace cmd {

CmdActionInformalText::CmdActionInformalText(MscAction *action, const QString &newText)
    : BaseCommand(action)
    , m_action(action)
    , m_oldText(action->informalAction())
    , m_newText(newText)
{
}

void CmdActionInformalText::redo()
{
    if (m_action) {
        m_action->setInformalAction(m_newText);
    }
}

void CmdActionInformalText::undo()
{
    if (m_action) {
        m_action->setInformalAction(m_oldText);
    }
}

bool CmdActionInformalText::mergeWith(const QUndoCommand *command)
{
    auto other = dynamic_cast<const CmdActionInformalText *>(command);
    if (canMergeWith(other)) {
        m_newText = other->m_newText;
        return true;
    }

    return false;
}

int CmdActionInformalText::id() const
{
    return msc::cmd::Id::InformatActionText;
}

} // namespace msc
} // namespace cms
