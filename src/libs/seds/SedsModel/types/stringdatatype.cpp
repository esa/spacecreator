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

#include "types/stringdatatype.h"

namespace seds::model {

StringDataType::StringDataType() noexcept
    : m_fixedLength(false)
{
}

PositiveLong::Value StringDataType::length() const
{
    return m_length.value();
}

void StringDataType::setLength(PositiveLong::Value length)
{
    m_length = length;
}

bool StringDataType::hasFixedLength() const
{
    return m_fixedLength;
}

void StringDataType::setFixedLength(bool fixedLength)
{
    m_fixedLength = fixedLength;
}

const std::optional<StringDataEncoding> &StringDataType::encoding() const
{
    return m_encoding;
}

void StringDataType::setEncoding(StringDataEncoding encoding)
{
    m_encoding = std::move(encoding);
}

} // namespace seds::model
