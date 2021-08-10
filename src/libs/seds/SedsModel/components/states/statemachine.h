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

#include "base/namedentity.h"
#include "components/states/entrystate.h"
#include "components/states/exitstate.h"
#include "components/states/state.h"
#include "components/states/stateref.h"
#include "components/states/transition.h"

#include <optional>
#include <variant>
#include <vector>

namespace seds::model {

class StateMachine final : public NamedEntity
{
public:
    using Element = std::variant<EntryState, ExitState, State, Transition>;
    using Elements = std::vector<Element>;

public:
    StateMachine() = default;
    StateMachine(StateMachine &&) = default;
    StateMachine &operator=(StateMachine &&) = default;

public:
    auto elements() const -> const Elements &;
    auto addElement(Element element) -> void;

    auto defaultEntryState() const -> const std::optional<StateRef> &;
    auto setDefaultEntryState(StateRef defaultEntryState) -> void;

private:
    Elements m_elements;
    std::optional<StateRef> m_defaultEntryState;
};

} // namespace seds::model
