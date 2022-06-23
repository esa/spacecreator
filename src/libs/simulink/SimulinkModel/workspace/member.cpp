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

#include "member.h"

namespace simulink::model {

const QString &Member::name() const
{
    return m_name;
}

void Member::setName(QString name)
{
    m_name = std::move(name);
}

const QString &Member::dataType() const
{
    return m_dataType;
}

void Member::setDataType(QString dataType)
{
    m_dataType = std::move(dataType);
}

const QString &Member::complexity() const
{
    return m_complexity;
}

void Member::setComplexity(QString complexity)
{
    m_complexity = std::move(complexity);
}

const QString &Member::description() const
{
    return m_description;
}

void Member::setDescription(QString description)
{
    m_description = std::move(description);
}

const QString &Member::dimensions() const
{
    return m_dimensions;
}

void Member::setDimensions(QString dimensions)
{
    m_dimensions = std::move(dimensions);
}

const QString &Member::dimensionsMode() const
{
    return m_dimensionsMode;
}

void Member::setDimensionsMode(QString dimensionsMode)
{
    m_dimensionsMode = std::move(dimensionsMode);
}

const QString &Member::max() const
{
    return m_max;
}

void Member::setMax(QString max)
{
    m_max = std::move(max);
}

const QString &Member::min() const
{
    return m_min;
}

void Member::setMin(QString min)
{
    m_min = std::move(min);
}

const QString &Member::sampleTime() const
{
    return m_sampleTime;
}

void Member::setSampleTime(QString sampleTime)
{
    m_sampleTime = std::move(sampleTime);
}

const QString &Member::unit() const
{
    return m_unit;
}

void Member::setUnit(QString unit)
{
    m_unit = std::move(unit);
}

} // namespace simulink::model
