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

#include "sdlprocessbuilder.h"

#include <memory>

namespace tests::common {

SdlProcessBuilder::SdlProcessBuilder(QString processName)
    : m_process(std::make_unique<Process>())
{
    m_process->setName(std::move(processName));
}

std::unique_ptr<Process> SdlProcessBuilder::build()
{
    return std::move(m_process);
}

SdlProcessBuilder &SdlProcessBuilder::withStartTransition(std::unique_ptr<Transition> transition)
{
    m_process->setStartTransition(std::move(transition));

    return *this;
}

SdlProcessBuilder &SdlProcessBuilder::withStateMachine(std::unique_ptr<StateMachine> stateMachine)
{
    m_process->setStateMachine(std::move(stateMachine));

    return *this;
}

SdlProcessBuilder &SdlProcessBuilder::withProcedure(std::unique_ptr<Procedure> procedure)
{
    m_process->addProcedure(std::move(procedure));

    return *this;
}

SdlProcessBuilder &SdlProcessBuilder::withVariable(std::unique_ptr<VariableDeclaration> variable)
{
    m_process->addVariable(std::move(variable));

    return *this;
}

} // namespace tests::common
