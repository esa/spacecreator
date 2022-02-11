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

#include "cmdcontextparameterreorder.h"

#include "commandids.h"

#include <ivfunctiontype.h>
#include <ivmodel.h>

namespace ive {
namespace cmd {

CmdContextParameterReorder::CmdContextParameterReorder(ivm::IVFunctionType *entity, int from, int to)
    : QUndoCommand()
    , m_entity(entity)
    , m_fromPos(from)
    , m_toPos(to)
{
    setText(QObject::tr("Change Context Parameter order"));
}

void CmdContextParameterReorder::swapParam(int from, int to)
{
    if (!m_entity)
        return;

    QVector<shared::ContextParameter> params = m_entity->contextParams();
    std::swap(params[from], params[to]);
    m_entity->setContextParams(params);
}

void CmdContextParameterReorder::redo()
{
    swapParam(m_fromPos, m_toPos);
}

void CmdContextParameterReorder::undo()
{
    swapParam(m_toPos, m_fromPos);
}

bool CmdContextParameterReorder::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)

    return false;
}

int CmdContextParameterReorder::id() const
{
    return ReorderContextParameter;
}

} // namespace cmd
} // namespace ive
