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

#include "cmdconditionitemresize.h"
#include "conditionitem.h"

namespace msc {
namespace cmd {

CmdConditionItemResize::CmdConditionItemResize(ConditionItem *conditionItem, const QRectF &destination)
    : BaseCommand(conditionItem)
    , m_conditionItem(conditionItem)
    , m_newGeometry(destination)
    , m_oldGeometry(conditionItem ? conditionItem->boundingRect() : QRectF())
{
    setText(QObject::tr("Resize condition"));
}

void CmdConditionItemResize::redo()
{
    if (m_conditionItem)
        m_conditionItem->setBoundingRect(m_newGeometry);
}

void CmdConditionItemResize::undo()
{
    if (m_conditionItem)
        m_conditionItem->setBoundingRect(m_oldGeometry);
}

bool CmdConditionItemResize::mergeWith(const QUndoCommand *command)
{
    const CmdConditionItemResize *other = static_cast<const CmdConditionItemResize *>(command);
    if (canMergeWith(other)) {
        m_newGeometry = other->m_newGeometry;
        return true;
    }

    return false;
}

int CmdConditionItemResize::id() const
{
    return msc::cmd::Id::ResizeCondition;
}

} // ns cmd
} // ns msc
