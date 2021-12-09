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

#include "types/dimensionsize.h"

namespace seds::model {

DimensionSize::DimensionSize(const DimensionSize &rhs)
    : m_size(std::nullopt)
    , m_indexTypeRef(std::nullopt)
{
    if (rhs.size()) {
        m_size = rhs.size()->value();
    }

    if (rhs.indexTypeRef()) {
        m_indexTypeRef = rhs.indexTypeRef()->nameStr();
    }
}

const std::optional<PositiveLong> &DimensionSize::size() const
{
    return m_size;
}

void DimensionSize::setSize(PositiveLong::Value size)
{
    m_size = size;
}

const std::optional<DataTypeRef> &DimensionSize::indexTypeRef() const
{
    return m_indexTypeRef;
}

void DimensionSize::setIndexTypeRef(DataTypeRef indexTypeRef)
{
    m_indexTypeRef = std::move(indexTypeRef);
}

} // namespace seds::model
