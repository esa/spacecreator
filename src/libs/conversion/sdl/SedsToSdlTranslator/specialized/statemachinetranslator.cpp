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

#include "statemachinetranslator.h"

namespace conversion::sdl::translator {

auto StateMachineTranslator::translateStateMachine(const seds::model::StateMachine &sedsStateMachine,
        ::sdl::Process *sdlProcess, ::sdl::StateMachine *stateMachine) -> void
{
    // Consider rewriting this to filters when C++20 is supported
    // First pass through states
    for (auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<seds::model::State>(element)) {
            translateState(std::get<seds::model::State>(element), sdlProcess, stateMachine);
        }
    }
    // Second pass through transitions

    // Set entry state
}

auto StateMachineTranslator::translateState(
        const seds::model::State &sedsState, ::sdl::Process *sdlProcess, ::sdl::StateMachine *stateMachine) -> void
{
    Q_UNUSED(sdlProcess);

    auto state = std::make_unique<::sdl::State>();
    state->setName(sedsState.nameStr()); // TODO mangle identifier
    // Entry and exit procedures shall be translated for transitions
    stateMachine->addState(std::move(state));
}

} // namespace conversion::sdl::translator