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

#include "arraytype.h"

namespace promela::model {
ArrayType::ArrayType(size_t size, Type type)
    : m_size(size)
    , m_type(std::move(type))
{
}

ArrayType::ArrayType(QString constantName, Type type)
    : m_size(std::move(constantName))
    , m_type(std::move(type))
{
}

const QString &ArrayType::getSizeConstantName() const
{
    return std::get<QString>(m_size);
}

bool ArrayType::hasExternallyDefinedSize() const
{
    return std::holds_alternative<QString>(m_size);
}

size_t ArrayType::getSize() const noexcept
{
    return std::get<size_t>(m_size);
}

const ArrayType::Type &ArrayType::getType() const noexcept
{
    return m_type;
}
}
