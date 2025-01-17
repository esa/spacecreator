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

#include <memory>
#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

State::State(QString name, std::vector<std::unique_ptr<Input>> inputs,
        std::vector<std::unique_ptr<ContinuousSignal>> continuousSignals)
    : Node(std::move(name))
    , m_inputs(std::move(inputs))
    , m_continuousSignals(std::move(continuousSignals))
{
}

const std::vector<std::unique_ptr<Input>> &State::inputs() const
{
    return m_inputs;
}

void State::addInput(std::unique_ptr<Input> input)
{
    m_inputs.push_back(std::move(input));
}

const std::vector<std::unique_ptr<ContinuousSignal>> &State::continuousSignals() const
{
    return m_continuousSignals;
}

void State::addContinuousSignal(std::unique_ptr<ContinuousSignal> continuousSignal)
{
    m_continuousSignals.push_back(std::move(continuousSignal));
}

void State::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl
