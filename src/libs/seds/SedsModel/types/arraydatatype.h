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

#include "types/compositedatatype.h"
#include "types/datatyperef.h"
#include "types/dimensionsize.h"

#include <vector>

namespace seds::model {

class ArrayDataType final : public CompositeDataType
{
public:
    using ArrayDimensions = std::vector<DimensionSize>;

public:
    ArrayDataType() = default;
    ArrayDataType(ArrayDataType &&) = default;
    ArrayDataType &operator=(ArrayDataType &&) = default;

public:
    auto dimensions() const -> const ArrayDimensions &;
    auto addDimension(DimensionSize dimensions) -> void;

    auto typeRef() const -> const DataTypeRef &;
    auto setType(DataTypeRef type) -> void;

private:
    ArrayDimensions m_dimensions;
    DataTypeRef m_type;
};

} // namespace seds::model
