/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "process.h"

#include <memory>

namespace conversion::Sdl {

Process::Process(const QString &name)
    : m_name(name)
{
}

Process::Process(const QString &name, std::unique_ptr<StateMachine> &stateMachine,
        const std::vector<VariableDeclaration> &variables, const std::vector<Procedure> &procedures)
    : m_name(name)
    , m_variables(variables)
    , m_procedures(procedures)
{
    m_stateMachine = std::move(stateMachine);
}

auto Process::name() const -> QString
{
    return m_name;
}

auto Process::stateMachine() const -> const std::unique_ptr<StateMachine> &
{
    return m_stateMachine;
}

auto Process::variables() const -> const std::vector<VariableDeclaration> &
{
    return m_variables;
}

auto Process::procedure() const -> const std::vector<Procedure> &
{
    return m_procedures;
}

} // namespace conversion::Sdl
