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

#include "types/field.h"

namespace seds::model {

Field::~Field() = default;

const DataTypeRef &Field::typeRef() const
{
    return m_type;
}

void Field::setType(DataTypeRef type)
{
    m_type = std::move(type);
}

const Field::FieldEncoding &Field::encoding() const
{
    return m_encoding;
}

void Field::setEncoding(Field::FieldEncoding encoding)
{
    m_encoding = std::move(encoding);
}

const Field::ArrayDimensions &Field::arrayDimensions() const
{
    return m_arrayDimensions;
}

void Field::addArrayDimension(DimensionSize arrayDimension)
{
    m_arrayDimensions.push_back(std::move(arrayDimension));
}

const DerivedTypeRange &Field::validRange() const
{
    return m_validRange;
}

void Field::setValidRange(DerivedTypeRange validRange)
{
    m_validRange = std::move(validRange);
}

} // namespace seds::model
