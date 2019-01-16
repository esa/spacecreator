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
#include "actionitem.h"

namespace msc {
namespace cmd {

CmdActionItemMove::CmdActionItemMove(ActionItem *actionItem, const QPointF &destination)
    : BaseCommand(actionItem)
    , m_posFrom(actionItem ? actionItem->pos() : QPointF())
    , m_posTo(destination)
{
    setText(QObject::tr("Move action"));
}

void CmdActionItemMove::redo()
{
    if (m_graphicsItem)
        m_graphicsItem->setPos(m_posTo);
}

void CmdActionItemMove::undo()
{
    if (m_graphicsItem)
        m_graphicsItem->setPos(m_posFrom);
}

bool CmdActionItemMove::mergeWith(const QUndoCommand *command)
{
    const CmdActionItemMove *other = dynamic_cast<const CmdActionItemMove *>(command);
    if (canMergeWith(other)) {
        m_posTo = other->m_posTo;
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
