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

#include "statemachine.h"

namespace sdl {

StateMachine::StateMachine(
        std::vector<std::unique_ptr<State>> states, std::vector<std::unique_ptr<Transition>> transitions)
    : m_states(std::move(states))
    , m_transitions(std::move(transitions))
{
}

const std::vector<std::unique_ptr<State>> &StateMachine::states() const
{
    return m_states;
}

void StateMachine::addState(std::unique_ptr<State> state)
{
    m_states.push_back(std::move(state));
}

const std::vector<std::unique_ptr<Transition>> &StateMachine::transitions() const
{
    return m_transitions;
}

void StateMachine::addTransition(std::unique_ptr<Transition> transition)
{
    m_transitions.push_back(std::move(transition));
}

} // namespace sdl
