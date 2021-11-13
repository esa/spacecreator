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

#include <seds/SedsCommon/basetypesmappings.h>
#include <seds/SedsModel/components/states/entrystate.h>
#include <seds/SedsModel/components/states/exitstate.h>
#include <seds/SedsModel/components/states/statemachine.h>

namespace tests::conversion::common {

class SedsStateMachineBuilder final
{
public:
    seds::model::StateMachine build();

    auto withState(seds::model::State state) -> SedsStateMachineBuilder &;
    auto withState(seds::common::String name) -> SedsStateMachineBuilder &;

    auto withEntryState(seds::model::EntryState state) -> SedsStateMachineBuilder &;
    auto withEntryState(seds::common::String name) -> SedsStateMachineBuilder &;

    auto withExitState(seds::model::ExitState state) -> SedsStateMachineBuilder &;
    auto withExitState(seds::common::String name) -> SedsStateMachineBuilder &;

    auto withTransition(seds::model::Transition transition) -> SedsStateMachineBuilder &;
    auto withTransition(seds::common::String from, seds::common::String to, seds::model::Transition::Primitive on)
            -> SedsStateMachineBuilder &;
    auto withTransition(seds::common::String from, seds::common::String to, seds::model::Transition::Primitive on,
            seds::common::String doing) -> SedsStateMachineBuilder &;

private:
    seds::model::StateMachine m_machine;
};

} // namespace tests::conversion::common
