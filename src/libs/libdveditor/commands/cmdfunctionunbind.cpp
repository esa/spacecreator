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

#include "cmdfunctionunbind.h"

#include "abstractsystemchecks.h"
#include "commandids.h"
#include "dveditorcore.h"
#include "dvfunction.h"
#include "dvmodel.h"
#include "dvnode.h"
#include "dvpartition.h"
#include "errorhub.h"

namespace dve {
namespace cmd {

CmdFunctionUnbind::CmdFunctionUnbind(dvm::DVPartition *partition, dvm::DVFunction *function, DVEditorCore *dvCore)
    : shared::UndoCommand()
    , m_partition(partition)
{
    m_functions.append(function);

    // find/add connected protedted functions
    if (dvCore && dvCore->systemChecker()) {
        QStringList functionNames = dvCore->systemChecker()->connectedProtectedFunctionNames(function->title());
        for (dvm::DVFunction *func : m_partition->functions()) {
            if (func->title() != function->title() && functionNames.contains(func->title(), Qt::CaseInsensitive)) {
                m_functions.append(func);
            }
        }
    }
}

void CmdFunctionUnbind::redo()
{
    dvm::DVModel *model = m_partition->model();
    for (dvm::DVFunction *function : m_functions) {
        if (function) {
            m_partition->removeFunction(function);
            if (model) {
                model->removeObject(function);
            }
            function->setParent(this);
        }
    }
}

void CmdFunctionUnbind::undo()
{
    dvm::DVModel *model = m_partition->model();
    for (dvm::DVFunction *function : m_functions) {
        if (function) {
            function->setParent(m_partition);
            if (model) {
                model->addObject(function);
            }
            m_partition->addFunction(function);
        }
    }
}

int CmdFunctionUnbind::id() const
{
    return UnbindFunctionEntity;
}

} // namespace cmd
} // namespace dve
