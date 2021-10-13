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

#include "input.h"

#include <memory>
#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

Input::Input(QString name, Transition *transition)
    : Signal(std::move(name))
    , m_transition(transition)
{
}

Transition *Input::transition() const
{
    return m_transition;
}

void Input::setTransition(Transition *transition)
{
    m_transition = transition;
}

const std::vector<std::unique_ptr<Input::Parameter>> &Input::parameters() const
{
    return m_parameters;
}

void Input::addParameter(std::unique_ptr<Parameter> parameter)
{
    m_parameters.push_back(std::move(parameter));
}

void Input::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl
