/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdfunctionbind.h"

#include "commandids.h"
#include "dvfunction.h"
#include "dvmodel.h"
#include "dvpartition.h"

namespace dve {
namespace cmd {

CmdFunctionBind::CmdFunctionBind(dvm::DVPartition *partition, const QString &functionName)
    : shared::UndoCommand()
    , m_partition(partition)
    , m_function(new dvm::DVFunction)
{
    m_function->setTitle(functionName);
}

void CmdFunctionBind::redo()
{
    m_function->setParent(m_partition);
    if (auto model = m_partition->model()) {
        model->addObject(m_function);
    }
    m_partition->addFunction(m_function);
}

void CmdFunctionBind::undo()
{
    m_partition->removeFunction(m_function);
    if (auto model = m_partition->model()) {
        model->removeObject(m_function);
    }
    m_function->setParent(this);
}

int CmdFunctionBind::id() const
{
    return BindFunctionEntity;
}

} // namespace cmd
} // namespace dve
