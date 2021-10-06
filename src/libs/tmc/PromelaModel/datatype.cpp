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

#include "datatype.h"

namespace tmc::promelamodel {
DataType::DataType(Value value)
    : m_value(std::move(value))
{
}

DataType::DataType(UnsignedDataType value)
    : m_value(value)
{
}

DataType::DataType(UtypeRef value)
    : m_value(value)
{
}

DataType::DataType(BasicType value)
    : m_value(value)
{
}

DataType::DataType(ArrayType value)
    : m_value(value)
{
}

const DataType::Value &DataType::getValue() const noexcept
{
    return m_value;
}

bool DataType::isUnsigned() const noexcept
{
    return std::holds_alternative<UnsignedDataType>(m_value);
}

bool DataType::isReference() const noexcept
{
    return std::holds_alternative<UtypeRef>(m_value);
}

bool DataType::isBasicType() const noexcept
{
    return std::holds_alternative<BasicType>(m_value);
}

bool DataType::isArrayType() const noexcept
{
    return std::holds_alternative<ArrayType>(m_value);
}

const UnsignedDataType &DataType::getUnsigned() const
{
    return std::get<UnsignedDataType>(m_value);
}

const UtypeRef &DataType::getReference() const
{
    return std::get<UtypeRef>(m_value);
}

const BasicType &DataType::getBasicType() const
{
    return std::get<BasicType>(m_value);
}

const ArrayType &DataType::getArrayType() const
{
    return std::get<ArrayType>(m_value);
}
}
