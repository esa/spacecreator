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

#include "cmdconditionitemmove.h"
#include "conditionitem.h"

namespace msc {
namespace cmd {

CmdConditionItemMove::CmdConditionItemMove(ConditionItem *conditionItem, const QPointF &destination)
    : BaseCommand(conditionItem)
    , m_posFrom(conditionItem ? conditionItem->pos() : QPointF())
    , m_posTo(destination)
{
    setText(QObject::tr("Move condition"));
}

void CmdConditionItemMove::redo()
{
    if (m_graphicsItem)
        m_graphicsItem->setPos(m_posTo);
}

void CmdConditionItemMove::undo()
{
    if (m_graphicsItem)
        m_graphicsItem->setPos(m_posFrom);
}

bool CmdConditionItemMove::mergeWith(const QUndoCommand *command)
{
    const CmdConditionItemMove *other = static_cast<const CmdConditionItemMove *>(command);
    if (canMergeWith(other)) {
        m_posTo = other->m_posTo;
        return true;
    }

    return false;
}

int CmdConditionItemMove::id() const
{
    return msc::cmd::Id::MoveCondition;
}

} // ns cmd
} // ns msc
