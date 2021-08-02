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

#include <vector>

#include "components/primitives/commandrefattributes.h"
#include "components/primitives/namedargumentvalue.h"
#include "components/primitives/primitivesource.h"

namespace seds::model {

class SendCommandPrimitive final
    : public PrimitiveSource
    , public CommandRefAttributes
{
  public:
    using ArgumentValues = std::vector<NamedArgumentValue>;

  public:
    SendCommandPrimitive() = default;
    SendCommandPrimitive(SendCommandPrimitive&&) = default;
    SendCommandPrimitive& operator=(SendCommandPrimitive&&) = default;

  public:
    auto argumentValues() const -> const ArgumentValues&;
    auto addArgumentValue(NamedArgumentValue argumentValue) -> void;

  private:
    ArgumentValues m_argumentValues;
};

} // namespace seds::model
