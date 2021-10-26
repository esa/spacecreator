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

#pragma once

#include <memory>
#include <sdl/SdlModel/procedure.h>
#include <sdl/SdlModel/process.h>
#include <sdl/SdlModel/statemachine.h>
#include <sdl/SdlModel/transition.h>
#include <sdl/SdlModel/variabledeclaration.h>

using sdl::Procedure;
using sdl::Process;
using sdl::StateMachine;
using sdl::Transition;
using sdl::VariableDeclaration;

namespace tests::common {

class SdlProcessBuilder final
{
public:
    SdlProcessBuilder(QString processName);

    auto build() -> std::unique_ptr<Process>;

    auto withStartTransition(std::unique_ptr<Transition> transition) -> SdlProcessBuilder &;
    auto withStateMachine(std::unique_ptr<StateMachine> stateMachine) -> SdlProcessBuilder &;
    auto withProcedure(std::unique_ptr<Procedure> procedure) -> SdlProcessBuilder &;
    auto withVariable(std::unique_ptr<VariableDeclaration> variable) -> SdlProcessBuilder &;

private:
    std::unique_ptr<Process> m_process;
};

} // namespace tests::common
