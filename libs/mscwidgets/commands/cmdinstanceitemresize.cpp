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

#include "cmdinstanceitemresize.h"

#include <instanceitem.h>

namespace msc {
namespace cmd {

CmdInstanceItemResize::CmdInstanceItemResize(InstanceItem *instanceItem, const QRectF &destination)
    : BaseCommand(msc::cmd::Id::ResizeInstance, instanceItem)
    , m_instanceItem(instanceItem)
    , m_newGeometry(destination)
    , m_oldGeometry(instanceItem ? instanceItem->boundingRect() : QRectF())
{
    setText(QObject::tr("Resize instance"));
}

void CmdInstanceItemResize::redo()
{
    if (m_instanceItem)
        m_instanceItem->setBoundingRect(m_newGeometry);
}

void CmdInstanceItemResize::undo()
{
    if (m_instanceItem)
        m_instanceItem->setBoundingRect(m_oldGeometry);
}

bool CmdInstanceItemResize::mergeWith(const QUndoCommand *command)
{
    const CmdInstanceItemResize *other = static_cast<const CmdInstanceItemResize *>(command);
    if (canMergeWith(other)) {
        m_newGeometry = other->m_newGeometry;
        return true;
    }

    return false;
}

} // ns cmd
} // ns msc
