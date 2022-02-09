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

#include "base/positivelong.h"
#include "types/datatyperef.h"

#include <optional>

namespace seds::model {

class DimensionSize final
{
public:
    DimensionSize() = default;
    DimensionSize(const DimensionSize &rhs);
    DimensionSize(DimensionSize &&) = default;
    DimensionSize &operator=(DimensionSize &&) = default;

public:
    auto size() const -> const std::optional<PositiveLong> &;
    auto setSize(PositiveLong::Value size) -> void;

    auto indexTypeRef() const -> const std::optional<DataTypeRef> &;
    auto setIndexTypeRef(DataTypeRef indexTypeRef) -> void;

    auto operator==(const DimensionSize &rhs) const -> bool;
    auto operator!=(const DimensionSize &rhs) const -> bool;

private:
    std::optional<PositiveLong> m_size;
    std::optional<DataTypeRef> m_indexTypeRef;
};

} // namespace seds::model
