/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdcommenttextchange.h"

#include "cmdcommentitemcreate.h"
#include "commandids.h"

#include <tab_aadl/aadlobjectcomment.h>
#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

CmdCommentTextChange::CmdCommentTextChange(AADLObjectComment *comment, const QString &text)
    : QUndoCommand(QObject::tr("Edit Comment"))
    , m_entity(comment)
    , m_prevText(comment->title())
    , m_newText(text)
{
}

void CmdCommentTextChange::redo()
{
    if (m_entity)
        m_entity->setTitle(m_newText);
}

void CmdCommentTextChange::undo()
{
    if (m_entity)
        m_entity->setTitle(m_prevText);
}

bool CmdCommentTextChange::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);

    return false;
}

int CmdCommentTextChange::id() const
{
    return ChangeCommentText;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
