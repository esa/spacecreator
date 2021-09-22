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

CmdFunctionBind::CmdFunctionBind(dvm::DVPartition *partition, const QString &functionName, dve::DVEditorCore *dvCore)
    : shared::UndoCommand()
    , m_partition(partition)
{
    auto function = new dvm::DVFunction;
    function->setTitle(functionName);
    m_functions.append(function);

    // find/add connected protedted functions
    if (dvCore && dvCore->systemChecker()) {
        dvm::DVNode *node = partition->node();
        dvm::DVModel *model = m_partition->model();
        QStringList functionNames = dvCore->systemChecker()->connectedProtectedFunctionNames(functionName);
        for (const QString &name : functionNames) {
            if (node && !node->hasSystemFunction(name)) {
                if (model->getObjectByName(name, dvm::DVObject::Type::Function) != nullptr) {
                    shared::ErrorHub::addError(shared::ErrorItem::Error,
                            tr("Protected function '%1' already used without '%2'").arg(name, functionName),
                            dvCore->filePath());

                } else {
                    function = new dvm::DVFunction;
                    function->setTitle(name);
                    m_functions.append(function);
                }
            }
        }
    }
}

void CmdFunctionBind::redo()
{
    if (!m_partition) {
        return;
    }

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

void CmdFunctionBind::undo()
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

int CmdFunctionBind::id() const
{
    return BindFunctionEntity;
}

} // namespace cmd
} // namespace dve
