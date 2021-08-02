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

#include <variant>

#include "base/positivelong.h"
#include "types/encodings/dataencoding.h"

namespace seds::model {

enum class CoreIntegerEncoding
{
    UNSIGNED,
    SIGNMAGNITUDE,
    TWOSCOMPLEMENT,
    ONESCOMPLEMENT,
    BCD,
    PACKEDBCD
};

class IntegerDataEncoding final : public DataEncoding
{
  public:
    /** DoTIntegerEncoding, ExtIntegerEncoding */
    using Encoding = std::variant<CoreIntegerEncoding>;

  public:
    IntegerDataEncoding() = default;
    IntegerDataEncoding(IntegerDataEncoding&&) = default;
    IntegerDataEncoding& operator=(IntegerDataEncoding&&) = default;

  public:
    auto encoding() const -> const Encoding&;
    auto setEncoding(Encoding encoding) -> void;

    auto bits() const -> PositiveLong::Value;
    auto setBits(PositiveLong::Value bits) -> void;

  private:
    Encoding m_encoding;
    PositiveLong m_bits;
};

} // namespace seds::model
