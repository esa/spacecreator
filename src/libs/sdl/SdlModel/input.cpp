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

#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

Input::Input(const QString &name)
    : Signal(name)
{
    m_transition.reset();
}

Input::Input(const QString &name, const std::shared_ptr<Transition> &transition)
    : Signal(name)
    , m_transition(transition)
{
}

Input::Input(const Input &input)
    : Signal(input.name())
    , m_transition(input.transition())
{
}

const std::shared_ptr<Transition> &Input::transition() const
{
    return m_transition;
}

void Input::setTransition(const std::shared_ptr<Transition> &transition)
{
    m_transition = transition;
}

void Input::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl
