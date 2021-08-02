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

#include "components/activities/operand.h"
#include "components/primitives/parameterrefattributes.h"
#include "components/primitives/primitivesource.h"
#include "components/primitives/parameteroperation.h"

namespace seds::model {

class SendParameterPrimitive final
    : public PrimitiveSource
    , public ParameterRefAttributes
{
  public:
    SendParameterPrimitive() = default;
    SendParameterPrimitive(SendParameterPrimitive&&) = default;
    SendParameterPrimitive& operator=(SendParameterPrimitive&&) = default;

  public:
    auto argumentValue() const -> const std::optional<Operand>&;
    auto setArgumentValue(Operand argumentValue) -> void;

    auto operation() const -> ParameterOperation;
    auto setOperation(ParameterOperation operation) -> void;

  private:
    std::optional<Operand> m_argumentValue;
    ParameterOperation m_operation;
};

} // namespace seds::model
