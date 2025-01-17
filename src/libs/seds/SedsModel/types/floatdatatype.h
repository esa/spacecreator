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

#include "seds/SedsCommon/basetypesmappings.h"
#include "types/encodings/floatdataencoding.h"
#include "types/numericdatatype.h"
#include "types/ranges/floatprecisionrange.h"
#include "types/ranges/minmaxrange.h"

#include <optional>
#include <variant>

namespace seds::model {

class FloatDataType final : public NumericDataType
{
public:
    using Range = std::variant<FloatPrecisionRange, MinMaxRange>;

public:
    FloatDataType() = default;
    FloatDataType(FloatDataType &&) = default;
    FloatDataType &operator=(FloatDataType &&) = default;

public:
    auto range() const -> const Range &;
    auto setRange(Range minMaxRange) -> void;

    auto encoding() const -> const std::optional<FloatDataEncoding> &;
    auto setEncoding(FloatDataEncoding encoding) -> void;

public:
    std::optional<FloatDataEncoding> m_encoding;
    Range m_range;
};

} // namespace seds::model
