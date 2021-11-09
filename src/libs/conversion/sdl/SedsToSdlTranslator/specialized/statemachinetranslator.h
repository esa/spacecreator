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

#include <map>
#include <sdl/SdlModel/sdlmodel.h>
#include <seds/SedsModel/components/states/entrystate.h>
#include <seds/SedsModel/components/states/exitstate.h>
#include <seds/SedsModel/sedsmodel.h>

namespace conversion::sdl::translator {

/**
 *  @brief Translator of SEDS state machines into SDL state machines
 */
class StateMachineTranslator final
{
public:
    using InputHandler = std::pair<std::unique_ptr<::sdl::Input>, std::vector<std::unique_ptr<::sdl::Action>>>;

    /**
     * @brief   Translate the given SEDS state machine into SDL state machine
     *
     * @param sedsStateMachine  Source SEDS state machine
     * @param sdlProcess        Containing SDL process
     * @param stateMachine      Target SDL state machine
     */
    static auto translateStateMachine(const seds::model::StateMachine &sedsStateMachine, ::sdl::Process *sdlProcess,
            ::sdl::StateMachine *stateMachine) -> void;

private:
    static auto translateState(const seds::model::State &sedsState) -> std::unique_ptr<::sdl::State>;

    static auto translateState(const seds::model::ExitState &sedsState) -> std::unique_ptr<::sdl::State>;

    static auto translateState(const seds::model::EntryState &sedsState) -> std::unique_ptr<::sdl::State>;

    static auto translatePrimitive(const seds::model::OnCommandPrimitive &command) -> InputHandler;

    static auto translatePrimitive(const seds::model::Transition::Primitive &primitive) -> InputHandler;

    static auto translateTransition(const seds::model::Transition &sedsTransition, ::sdl::Process *sdlProcess,
            ::sdl::StateMachine *stateMachine, std::map<QString, std::unique_ptr<::sdl::State>> &stateMap) -> void;
};

} // namespace conversion::sdl::translator