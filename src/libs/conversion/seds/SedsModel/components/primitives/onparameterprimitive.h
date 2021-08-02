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

#include "components/variableref.h"
#include "components/primitives/parameteroperation.h"
#include "components/primitives/parameterrefattributes.h"
#include "components/primitives/primitivesink.h"

namespace seds::model {

class OnParameterPrimitive final
    : public PrimitiveSink
    , public ParameterRefAttributes
{
  public:
    OnParameterPrimitive() = default;
    OnParameterPrimitive(OnParameterPrimitive&&) = default;
    OnParameterPrimitive& operator=(OnParameterPrimitive&&) = default;

  public:
    auto variableRef() const -> const std::optional<VariableRef>&;
    auto setVariableRef(VariableRef variableRef) -> void;

    auto operation() const -> ParameterOperation;
    auto setOperation(ParameterOperation operation) -> void;

  private:
    std::optional<VariableRef> m_variableRef;
    ParameterOperation m_operation;
};

} // namespace seds::model
