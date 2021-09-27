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

#include "state.h"

namespace sdl {

State::State(const QString &name, const std::vector<Input> &inputs)
    : Node(m_name)
    , m_name(name)
    , m_inputs(inputs)
{
}

State::State()
    : Node("")
{
}

State::State(const State &state)
    : Node(state.name())
{
}

State::~State() {}

auto State::name() const -> const QString &
{
    return m_name;
}

auto State::inputs() const -> const std::vector<Input> &
{
    return m_inputs;
}

auto State::accept(Visitor &visitor) const -> void
{
    visitor.visit(*this);
}

} // namespace sdl
