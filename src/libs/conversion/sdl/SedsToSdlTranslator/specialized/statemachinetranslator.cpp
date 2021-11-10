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

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <sdl/SdlModel/nextstate.h>

using conversion::Escaper;
using conversion::translator::TranslationException;

namespace conversion::sdl::translator {

auto StateMachineTranslator::translateStateMachine(const seds::model::StateMachine &sedsStateMachine,
        ::sdl::Process *sdlProcess, ::sdl::StateMachine *stateMachine) -> void
{
    // Consider rewriting this to filters when C++20 is supported
    std::map<QString, std::unique_ptr<::sdl::State>> stateMap;
    // First pass through states
    for (auto &element : sedsStateMachine.elements()) {
        // Tried to rewrite it with std::visit and overload - this is the simpler way
        if (std::holds_alternative<seds::model::State>(element)) {
            const auto &state = std::get<seds::model::State>(element);
            stateMap[state.nameStr()] = translateState(state);
        } else if (std::holds_alternative<seds::model::EntryState>(element)) {
            const auto &state = std::get<seds::model::EntryState>(element);
            stateMap[state.name().value()] = translateState(state);
        } else if (std::holds_alternative<seds::model::ExitState>(element)) {
            const auto &state = std::get<seds::model::ExitState>(element);
            stateMap[state.name().value()] = translateState(state);
        }
    }

    // Setup the start transition
    createStartTransition(sedsStateMachine, sdlProcess, stateMap);

    // Second pass through transitions
    for (auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<seds::model::Transition>(element)) {
            translateTransition(std::get<seds::model::Transition>(element), sdlProcess, stateMachine, stateMap);
        }
    }
    for (auto &entry : stateMap) {
        stateMachine->addState(std::move(entry.second));
    }
}

auto StateMachineTranslator::createStartTransition(const seds::model::StateMachine &sedsStateMachine,
        ::sdl::Process *sdlProcess, std::map<QString, std::unique_ptr<::sdl::State>> &stateMap) -> void
{
    for (auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<seds::model::EntryState>(element)) {
            if (sdlProcess->startTransition() != nullptr) {
                throw TranslationException("Multiple entry states are not supported");
            }
            auto transition = std::make_unique<::sdl::Transition>();
            const auto &state = std::get<seds::model::EntryState>(element);
            transition->addAction(std::make_unique<::sdl::NextState>("", stateMap[state.nameStr()].get()));
            sdlProcess->setStartTransition(std::move(transition));
        }
    }
}

auto StateMachineTranslator::translateState(const seds::model::State &sedsState) -> std::unique_ptr<::sdl::State>
{
    auto state = std::make_unique<::sdl::State>();
    state->setName(Escaper::escapeSdlName(sedsState.nameStr()));
    // Entry and exit procedures shall be translated for transitions
    return state;
}

auto StateMachineTranslator::translateState(const seds::model::ExitState &sedsState) -> std::unique_ptr<::sdl::State>
{
    auto state = std::make_unique<::sdl::State>();
    state->setName(Escaper::escapeSdlName(sedsState.nameStr()));
    // No Entry and exit procedures
    return state;
}

auto StateMachineTranslator::translateState(const seds::model::EntryState &sedsState) -> std::unique_ptr<::sdl::State>
{
    auto state = std::make_unique<::sdl::State>();
    state->setName(Escaper::escapeSdlName(sedsState.nameStr()));
    // No Entry and exit procedures
    return state;
}

auto StateMachineTranslator::translatePrimitive(const seds::model::OnCommandPrimitive &command) -> InputHandler
{
    Q_UNUSED(command);

    auto input = std::make_unique<::sdl::Input>();
    std::vector<std::unique_ptr<::sdl::Action>> unpackingActions;

    input->setName(command.interface().value() + "_" + command.command().value()); // TODO mangle identifier
    // TODO Create actions for argument unpacking

    return std::make_pair(std::move(input), std::move(unpackingActions));
}

auto StateMachineTranslator::translatePrimitive(const seds::model::Transition::Primitive &primitive) -> InputHandler
{
    if (std::holds_alternative<seds::model::OnCommandPrimitive>(primitive)) {
        return translatePrimitive(std::get<seds::model::OnCommandPrimitive>(primitive));
    }
    throw TranslationException("Encountered unsupported primitive");
    return InputHandler();
}

auto StateMachineTranslator::translateTransition(const seds::model::Transition &sedsTransition,
        ::sdl::Process *sdlProcess, ::sdl::StateMachine *stateMachine,
        std::map<QString, std::unique_ptr<::sdl::State>> &stateMap) -> void
{
    Q_UNUSED(sdlProcess);

    const auto fromStateName = sedsTransition.fromState().nameStr();
    const auto toStateName = sedsTransition.toState().nameStr();
    const auto &fromStateIterator = stateMap.find(fromStateName);
    const auto &toStateIterator = stateMap.find(toStateName);
    if (fromStateIterator == stateMap.end()) {
        throw TranslationException(QString("Unknown state %1 when translating transition").arg(fromStateName));
    }
    if (toStateIterator == stateMap.end()) {
        throw TranslationException(QString("Unknown state %1 when translating transition").arg(toStateName));
    }
    auto fromState = (*fromStateIterator).second.get();
    auto toState = (*toStateIterator).second.get();
    auto inputHandler = translatePrimitive(sedsTransition.primitive());

    auto transition = std::make_unique<::sdl::Transition>();
    inputHandler.first->setTransition(transition.get());
    fromState->addInput(std::move(inputHandler.first));
    // Argument unpacking
    for (auto &action : inputHandler.second) {
        transition->addAction(std::move(action));
    }
    // TODO Guard
    // TODO From Exit
    // TODO Activity
    // TODO To Entry
    // State switch
    transition->addAction(std::make_unique<::sdl::NextState>("", toState));

    stateMachine->addTransition(std::move(transition));
}

} // namespace conversion::sdl::translator