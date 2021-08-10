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

#include "types/encodings/integerdataencoding.h"
#include "types/scalardatatype.h"
#include "types/valueenumeration.h"

#include <seds/SedsCommon/basetypesmappings.h>
#include <vector>

namespace seds::model {

class EnumeratedDataType final : public ScalarDataType
{
public:
    using EnumerationList = std::vector<ValueEnumeration>;

public:
    EnumeratedDataType() = default;
    EnumeratedDataType(EnumeratedDataType &&) = default;
    EnumeratedDataType &operator=(EnumeratedDataType &&) = default;

public:
    auto encoding() const -> const std::optional<IntegerDataEncoding> &;
    auto setEncoding(IntegerDataEncoding encoding) -> void;

    auto enumerationList() const -> const EnumerationList &;
    auto addEnumeration(ValueEnumeration value) -> void;

private:
    std::optional<IntegerDataEncoding> m_encoding;
    EnumerationList m_enumerationList;
};

} // namespace seds::model
