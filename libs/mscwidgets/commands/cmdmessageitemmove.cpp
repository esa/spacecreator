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
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/
#include "cmdmessageitemmove.h"

#include <messageitem.h>

namespace msc {
namespace cmd {

CmdMessageItemMove::CmdMessageItemMove(MessageItem *messageItem, const QPointF &destination)
    : BaseCommand(messageItem)
    , m_posFrom(messageItem ? messageItem->pos() : QPointF())
    , m_posTo(destination)
{
    setText(QObject::tr("Move message"));
}

void CmdMessageItemMove::redo()
{
    if (m_graphicsItem)
        m_graphicsItem->setPos(m_posTo);
}

void CmdMessageItemMove::undo()
{
    if (m_graphicsItem)
        m_graphicsItem->setPos(m_posFrom);
}

bool CmdMessageItemMove::mergeWith(const QUndoCommand *command)
{
    const CmdMessageItemMove *other = static_cast<const CmdMessageItemMove *>(command);
    if (canMergeWith(other)) {
        m_posTo = other->m_posTo;
        return true;
    }

    return false;
}

int CmdMessageItemMove::id() const
{
    return msc::cmd::Id::MoveMessage;
}

} // ns cmd
} // ns msc
