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

#include "types/ranges/minmaxrange.h"

namespace seds::model {

RangeType MinMaxRange::type() const
{
    return m_type;
}

void MinMaxRange::setType(RangeType type)
{
    m_type = type;
}

const std::optional<LiteralValue> &MinMaxRange::min() const
{
    return m_min;
}

void MinMaxRange::setMin(LiteralValue value)
{
    m_min = std::move(value);
}

const std::optional<LiteralValue> &MinMaxRange::max() const
{
    return m_max;
}

void MinMaxRange::setMax(LiteralValue value)
{
    m_max = std::move(value);
}

} // namespace seds::model
