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

#include "sedsstatemachinebuilder.h"

using namespace seds::model;

namespace tests::conversion::common {

seds::model::StateMachine SedsStateMachineBuilder::build()
{
    return std::move(m_machine);
}

auto SedsStateMachineBuilder::withState(seds::model::State state) -> SedsStateMachineBuilder &
{
    m_machine.addElement(std::move(state));

    return *this;
}

auto SedsStateMachineBuilder::withState(seds::common::String name) -> SedsStateMachineBuilder &
{
    seds::model::State state;
    state.setName(name);
    m_machine.addElement(std::move(state));

    return *this;
}

auto SedsStateMachineBuilder::withTransition(seds::model::Transition transition) -> SedsStateMachineBuilder &
{
    m_machine.addElement(std::move(transition));

    return *this;
}

auto SedsStateMachineBuilder::withTransition(seds::common::String from, seds::common::String to,
        seds::model::Transition::Primitive on) -> SedsStateMachineBuilder &
{
    seds::model::Transition transition;
    transition.setFromState(seds::model::StateRef(from));
    transition.setToState(seds::model::StateRef(to));
    transition.setPrimitive(std::move(on));

    m_machine.addElement(std::move(transition));

    return *this;
}

} // namespace tests::conversion::common
