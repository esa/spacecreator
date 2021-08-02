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

#include "seds/SedsCommon/basetypesmappings.h"
#include "types/scalardatatype.h"
#include "types/encodings/booleandataencoding.h"

namespace seds::model {

class BooleanDataType final : public ScalarDataType
{
  public:
    BooleanDataType() = default;
    BooleanDataType(BooleanDataType&&) = default;
    BooleanDataType& operator=(BooleanDataType&&) = default;

  public:
    auto encoding() const -> const std::optional<BooleanDataEncoding>&;
    auto setEncoding(BooleanDataEncoding encoding) -> void;

  private:
    std::optional<BooleanDataEncoding> m_encoding;
};

} // namespace seds::model
