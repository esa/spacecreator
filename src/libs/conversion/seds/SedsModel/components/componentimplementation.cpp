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

#include "components/componentimplementation.h"

namespace seds::model {

const ComponentImplementation::VariableSet&
ComponentImplementation::variables() const
{
    return m_variables;
}

void
ComponentImplementation::addVariable(Variable variable)
{
    m_variables.push_back(std::move(variable));
}

const ComponentImplementation::ParameterMapSet&
ComponentImplementation::parameterMaps() const
{
    return m_parameterMaps;
}

void
ComponentImplementation::addParameterMap(ParameterMap parameterMap)
{
    m_parameterMaps.push_back(std::move(parameterMap));
}

const ComponentImplementation::ParameterActivityMapSet&
ComponentImplementation::parameterActivityMaps() const
{
    return m_parameterActivityMaps;
}

void
ComponentImplementation::addParameterActivityMap(ParameterActivityMap parameterActivityMap)
{
    m_parameterActivityMaps.push_back(std::move(parameterActivityMap));
}

const ComponentImplementation::ActivitySet&
ComponentImplementation::activities() const
{
    return m_activities;
}

void
ComponentImplementation::addActivity(Activity activity)
{
    m_activities.push_back(std::move(activity));
}

const ComponentImplementation::StateMachineSet&
ComponentImplementation::stateMachines() const
{
    return m_stateMachines;
}

void
ComponentImplementation::addStateMachine(StateMachine stateMachine)
{
    m_stateMachines.push_back(std::move(stateMachine));
}

} // namespace seds::model
