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

#include "cmdrequiredinterfaceitemcreate.h"

#include "commandids.h"

#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

CmdRequiredInterfaceItemCreate::CmdRequiredInterfaceItemCreate(AADLObjectsModel *model,
                                                               AADLObjectFunctionType *function, const QPointF &pos)
    : m_model(model)
    , m_pos(pos)
    , m_entity(new AADLObjectIfaceRequired(function))
    , m_parent(function)
{
}

void CmdRequiredInterfaceItemCreate::redo()
{
    m_entity->setCoordinates({ qRound(m_pos.x()), qRound(m_pos.y()) });
    if (m_parent)
        m_parent->addInterface(m_entity);
    if (m_model)
        m_model->addObject(m_entity);
}

void CmdRequiredInterfaceItemCreate::undo()
{
    if (m_parent)
        m_parent->removeInterface(m_entity);
    if (m_model)
        m_model->removeObject(m_entity);
}

bool CmdRequiredInterfaceItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdRequiredInterfaceItemCreate::id() const
{
    return CreateRequiredInterfaceEntity;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
