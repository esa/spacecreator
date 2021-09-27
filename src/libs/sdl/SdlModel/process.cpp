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

namespace sdl {

Process::Process(const Process &process)
    : Node(m_name)
    , m_name(process.name())
    , m_stateMachine(process.stateMachine())
    , m_variables(process.variables())
    , m_procedures(process.procedures())
{
}

Process::Process(const QString &name, const StateMachine &stateMachine,
        const std::vector<VariableDeclaration> &variables, const std::vector<Procedure> &procedures)
    : Node(m_name)
    , m_name(name)
    , m_stateMachine(stateMachine)
    , m_variables(variables)
    , m_procedures(procedures)
{
}

Process::~Process() {}

auto Process::name() const -> QString
{
    return m_name;
}

auto Process::stateMachine() const -> const StateMachine &
{
    return m_stateMachine;
}

auto Process::variables() const -> const std::vector<VariableDeclaration> &
{
    return m_variables;
}

auto Process::procedures() const -> const std::vector<Procedure> &
{
    return m_procedures;
}

auto Process::accept(Visitor &visitor) const -> void
{
    visitor.visit(*this);
}

} // namespace sdl
