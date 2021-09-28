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

#include <QString>
#include <memory>
#include <vector>

namespace sdl {

class Transition
{
    // TODO
};

class StateMachine
{
public:
    StateMachine(std::vector<std::unique_ptr<State>> &states,
            const std::vector<Transition> &transitions = std::vector<Transition>());
    auto states() const -> const std::vector<std::unique_ptr<State>> &;
    auto transitions() const -> const std::vector<Transition> &;

private:
    std::vector<std::unique_ptr<State>> m_states;
    std::vector<Transition> m_transitions;
};

} // namespace sdl
