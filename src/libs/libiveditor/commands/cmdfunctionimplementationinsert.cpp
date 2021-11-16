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
 along with this program. If not, see
 <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "cmdfunctionimplementationinsert.h"

#include "commandids.h"
#include "ivfunction.h"

namespace ive {
namespace cmd {

CmdFunctionImplementationInsert::CmdFunctionImplementationInsert(ivm::IVFunction *entity, int idx, const EntityAttribute &values)
    : shared::UndoCommand()
    , m_function(entity)
    , m_idx(idx)
    , m_newValues(values)
{
    Q_ASSERT(m_function);
    Q_ASSERT(idx >= 0 && idx < m_function->implementations().size() + 1);
}

void CmdFunctionImplementationInsert::redo()
{
    if (m_function) {
        m_function->insertImplementation(m_idx, m_newValues);
        Q_EMIT implementationListChanged(m_function.data());
    }
}

void CmdFunctionImplementationInsert::undo()
{
    if (m_function) {
        m_function->removeImplementation(m_idx);
        Q_EMIT implementationListChanged(m_function.data());
    }
}

int CmdFunctionImplementationInsert::id() const
{
    return InsertFunctionImplementation;
}

} // namespace cmd
} // namespace ive
