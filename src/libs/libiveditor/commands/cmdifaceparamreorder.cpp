/*
  Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdifaceparamreorder.h"

#include "commandids.h"

#include <ivinterface.h>
#include <ivmodel.h>

namespace ive {
namespace cmd {

CmdIfaceParamReorder::CmdIfaceParamReorder(ivm::IVInterface *entity, int from, int to)
    : QUndoCommand()
    , m_entity(entity)
    , m_fromPos(from)
    , m_toPos(to)
{
    setText(QObject::tr("Change Iface Parameter order"));
}

void CmdIfaceParamReorder::swapParam(int from, int to)
{
    if (!m_entity)
        return;

    QVector<shared::InterfaceParameter> params = m_entity->params();
    std::swap(params[from], params[to]);
    m_entity->setParams(params);
}

void CmdIfaceParamReorder::redo()
{
    swapParam(m_fromPos, m_toPos);
}

void CmdIfaceParamReorder::undo()
{
    swapParam(m_toPos, m_fromPos);
}

bool CmdIfaceParamReorder::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)

    return false;
}

int CmdIfaceParamReorder::id() const
{
    return ReorderIfaceParam;
}

} // namespace cmd
} // namespace ive
