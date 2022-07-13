/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "busmember.h"

namespace simulink::model {

const common::String &BusMember::dataType() const
{
    return m_dataType;
}

void BusMember::setDataType(common::String dataType)
{
    m_dataType = std::move(dataType);
}

Complexity BusMember::complexity() const
{
    return m_complexity;
}

void BusMember::setComplexity(Complexity complexity)
{
    m_complexity = complexity;
}

const common::String &BusMember::description() const
{
    return m_description;
}

void BusMember::setDescription(common::String description)
{
    m_description = std::move(description);
}

const Dimensions &BusMember::dimensions() const
{
    return m_dimensions;
}

void BusMember::setDimensions(Dimensions dimensions)
{
    m_dimensions = std::move(dimensions);
}

DimensionsMode BusMember::dimensionsMode() const
{
    return m_dimensionsMode;
}

void BusMember::setDimensionsMode(DimensionsMode dimensionsMode)
{
    m_dimensionsMode = dimensionsMode;
}

const common::String &BusMember::max() const
{
    return m_max;
}

void BusMember::setMax(common::String max)
{
    m_max = std::move(max);
}

const common::String &BusMember::min() const
{
    return m_min;
}

void BusMember::setMin(common::String min)
{
    m_min = std::move(min);
}

const common::String &BusMember::sampleTime() const
{
    return m_sampleTime;
}

void BusMember::setSampleTime(common::String sampleTime)
{
    m_sampleTime = std::move(sampleTime);
}

const common::String &BusMember::unit() const
{
    return m_unit;
}

void BusMember::setUnit(common::String unit)
{
    m_unit = std::move(unit);
}

} // namespace simulink::model
