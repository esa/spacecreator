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

#include "components/parameteractivitymap.h"
#include "components/parametermap.h"
#include "components/variable.h"
#include "components/activities/activity.h"
#include "components/states/statemachine.h"

namespace seds::model {

class ComponentImplementation final
{
  public:
    using ActivitySet = std::vector<Activity>;
    using ParameterActivityMapSet = std::vector<ParameterActivityMap>;
    using ParameterMapSet = std::vector<ParameterMap>;
    using StateMachineSet = std::vector<StateMachine>;
    using VariableSet = std::vector<Variable>;

  public:
    auto variables() const -> const VariableSet&;
    auto addVariable(Variable variable) -> void;

    auto parameterMaps() const -> const ParameterMapSet&;
    auto addParameterMap(ParameterMap parameterMap) -> void;

    auto parameterActivityMaps() const -> const ParameterActivityMapSet&;
    auto addParameterActivityMap(ParameterActivityMap parameterActivityMap) -> void;

    auto activities() const -> const ActivitySet&;
    auto addActivity(Activity activity) -> void;

    auto stateMachines() const -> const StateMachineSet&;
    auto addStateMachine(StateMachine stateMachine) -> void;

  private:
    VariableSet m_variables;
    ParameterMapSet m_parameterMaps;
    ParameterActivityMapSet m_parameterActivityMaps;
    ActivitySet m_activities;
    StateMachineSet m_stateMachines;
};

} // namespace seds::model
