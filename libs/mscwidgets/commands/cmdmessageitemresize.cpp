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

#include "cmdmessageitemresize.h"

#include <messageitem.h>

namespace msc {
namespace cmd {

CmdMessageItemResize::CmdMessageItemResize(MessageItem *messageItem,
                                           const QPointF &head, const QPointF &tail)
    : BaseCommand(messageItem)
    , m_messageItem(messageItem)
    , m_newHead(head)
    , m_newTail(tail)
    , m_oldHead(messageItem ? messageItem->head() : QPointF())
    , m_oldTail(messageItem ? messageItem->tail() : QPointF())

{
    setText(QObject::tr("Resize message"));
}

void CmdMessageItemResize::redo()
{
    if (m_messageItem) {
        m_messageItem->setHead(m_newHead);
        m_messageItem->setTail(m_newTail);
    }
}

void CmdMessageItemResize::undo()
{
    if (m_messageItem) {
        m_messageItem->setHead(m_oldHead);
        m_messageItem->setTail(m_oldTail);
    }
}

bool CmdMessageItemResize::mergeWith(const QUndoCommand *command)
{
    const CmdMessageItemResize *other = static_cast<const CmdMessageItemResize *>(command);
    if (canMergeWith(other)) {
        m_newHead = other->m_newHead;
        m_newTail = other->m_newTail;
        return true;
    }

    return false;
}

int CmdMessageItemResize::id() const
{
    return msc::cmd::Id::RetargetMessage;
}

} // ns cmd
} // ns msc
