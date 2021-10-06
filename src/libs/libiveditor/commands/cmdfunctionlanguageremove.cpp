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

#include "cmdfunctionlanguageremove.h"

#include "commandids.h"
#include "ivfunction.h"

namespace ive {
namespace cmd {

CmdFunctionLanguageRemove::CmdFunctionLanguageRemove(ivm::IVFunction *entity, int idx)
    : m_function(entity)
    , m_idx(idx)
{
    Q_ASSERT(m_function);
    Q_ASSERT(idx >= 0 && idx < m_function->languages().size() + 1);
    m_value = m_function->languages().at(idx);
}

void CmdFunctionLanguageRemove::redo()
{
    if (m_function) {
        m_function->removeLanguage(m_idx);
    }
}

void CmdFunctionLanguageRemove::undo()
{
    if (m_function) {
        m_function->insertLanguage(m_idx, m_value);
    }
}

int CmdFunctionLanguageRemove::id() const
{
    return RemoveFunctionLanguage;
}

} // namespace cmd
} // namespace ive
