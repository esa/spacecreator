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

#include "components/activities/valueoperand.h"
#include "components/activities/variablerefoperand.h"

#include <variant>

namespace seds::model {

class Operand
{
public:
    using Value = std::variant<ValueOperand, VariableRefOperand>;

public:
    Operand() = default;
    Operand(Operand &&) = default;
    Operand &operator=(Operand &&) = default;

public:
    auto value() const -> const Value &;
    auto setValue(Value value) -> void;

private:
    Value m_value;
};

} // namespace seds::model
