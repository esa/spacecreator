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

#include <optional>
#include <variant>

#include "components/activities/activityinvocation.h"
#include "components/activities/booleanexpression.h"
#include "components/primitives/oncommandprimitive.h"
#include "components/primitives/onparameterprimitive.h"
#include "components/primitives/timersink.h"
#include "components/states/statemachineelement.h"
#include "components/states/stateref.h"

namespace seds::model {

class Transition final : public StateMachineElement
{
  public:
    using Primitive = std::variant<OnCommandPrimitive, OnParameterPrimitive, TimerSink>;

  public:
    Transition() = default;
    Transition(Transition&&) = default;
    Transition& operator=(Transition&&) = default;

  public:
    auto primitive() const -> const Primitive&;
    auto setPrimitive(Primitive primitive) -> void;

    auto guard() const -> const std::optional<BooleanExpression>&;
    auto setGuard(BooleanExpression guard) -> void;

    auto doActivity() const -> const std::optional<ActivityInvocation>&;
    auto setDoActivity(ActivityInvocation doActivity) -> void;

    auto fromState() const -> const StateRef&;
    auto setFromState(StateRef fromState) -> void;

    auto toState() const -> const StateRef&;
    auto setToState(StateRef toState) -> void;

  private:
    Primitive m_primitive;
    std::optional<BooleanExpression> m_guard;
    std::optional<ActivityInvocation> m_do;
    StateRef m_fromState;
    StateRef m_toState;
};

} // namespace seds::model
