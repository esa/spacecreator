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

#include "state.h"
#include "transition.h"

#include <QString>
#include <memory>
#include <vector>

namespace sdl {

/**
 * @brief   Represents an SDL state machine
 */
class StateMachine final
{
public:
    /**
     * @brief    Default constructor
     */
    StateMachine() = default;
    /**
     * @brief   Constructor
     *
     * @param   states      a vector of pointers to states
     * @param   transitions a vector of pointers to transitions
     */
    StateMachine(std::vector<std::unique_ptr<State>> states, std::vector<std::unique_ptr<Transition>> transitions);
    /**
     * @brief    Deleted copy constructor
     */
    StateMachine(const StateMachine &) = delete;
    /**
     * @brief   Default move constructor
     */
    StateMachine(StateMachine &&) = default;

    /**
     * @brief   Default copy assignment operator
     */
    StateMachine &operator=(const StateMachine &) = default;
    /**
     * @brief   Default move assignment operator
     */
    StateMachine &operator=(StateMachine &&) = default;

public:
    /**
     * @brief   Getter for the states of the state machine
     *
     * @return  const reference to a vector of pointers to states
     */
    auto states() const -> const std::vector<std::unique_ptr<State>> &;
    /**
     * @brief   Add a state to this state machine
     *
     * @param   state a pointer to state
     */
    auto addState(std::unique_ptr<State> state) -> void;

    /**
     * @brief   Getter for the transitions of the state machine
     *
     * @return  const reference to a vector of pointers to transitions
     */
    auto transitions() const -> const std::vector<std::unique_ptr<Transition>> &;
    /**
     * @brief   Add a transition to this state machine
     *
     * @param   transition a pointer to transition
     */
    auto addTransition(std::unique_ptr<Transition> transition) -> void;

private:
    std::vector<std::unique_ptr<State>> m_states;
    std::vector<std::unique_ptr<Transition>> m_transitions;
};

} // namespace sdl
