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

#include "sdlstatemachinebuilder.h"

namespace tests::common {

std::unique_ptr<StateMachine> SdlStateMachineBuilder::build()
{
    return std::make_unique<StateMachine>(std::move(m_states), std::move(m_transitions));
}

SdlStateMachineBuilder &SdlStateMachineBuilder::withStates(std::vector<std::unique_ptr<State>> states)
{
    m_states = std::move(states);

    return *this;
}

SdlStateMachineBuilder &SdlStateMachineBuilder::withState(std::unique_ptr<State> state)
{
    m_states.push_back(std::move(state));

    return *this;
}

SdlStateMachineBuilder &SdlStateMachineBuilder::withTransitions(std::vector<std::unique_ptr<Transition>> transitions)
{
    m_transitions = std::move(transitions);

    return *this;
}

} // namespace tests::common
