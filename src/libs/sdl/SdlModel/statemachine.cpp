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

#include <iostream>
#include <memory>

namespace sdl {

StateMachine::StateMachine(
        std::vector<std::unique_ptr<State>> &states, std::vector<std::unique_ptr<Transition>> &transitions)
{
    for (unsigned long int i = 0; i < states.size(); i++) {
        m_states.push_back(std::move(states[i]));
    }
    for (unsigned long int i = 0; i < transitions.size(); i++) {
        m_transitions.push_back(std::move(transitions[i]));
    }
}

auto StateMachine::states() const -> const std::vector<std::unique_ptr<State>> &
{
    return m_states;
}

auto StateMachine::transitions() const -> const std::vector<std::unique_ptr<Transition>> &
{
    return m_transitions;
}

} // namespace sdl
