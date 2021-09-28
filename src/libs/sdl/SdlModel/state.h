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

#include "../SdlExporter/visitors/visitor.h"
#include "input.h"
#include "node.h"

#include <QString>
#include <vector>

namespace sdl {

class ContinuousSignal
{
    // TODO: move to a separate file
    // TODO: this class shall inherit from Transition
    // TODO: this class shall contain an Expression
};

class State : public Node
{
public:
    State();
    State(const State &state);
    State(const QString &name, const std::vector<Input> &inputs);
    ~State();
    auto inputs() const -> const std::vector<Input> &;
    auto continuousSignals() -> const std::vector<ContinuousSignal> &;
    auto accept(Visitor &visitor) const -> void override;

private:
    std::vector<Input> m_inputs;
    std::vector<ContinuousSignal> m_continuousSignals;
};

} // namespace sdl
