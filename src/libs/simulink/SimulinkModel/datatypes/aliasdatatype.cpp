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

#include "aliasdatatype.h"

namespace simulink::model {

const common::String &AliasDataType::baseType() const
{
    return m_baseType;
}

void AliasDataType::setBaseType(common::String baseType)
{
    m_baseType = std::move(baseType);
}

const common::String &AliasDataType::dataScope() const
{
    return m_dataScope;
}

void AliasDataType::setDataScope(common::String dataScope)
{
    m_dataScope = std::move(dataScope);
}

const common::String &AliasDataType::description() const
{
    return m_description;
}

void AliasDataType::setDescription(common::String description)
{
    m_description = std::move(description);
}

const common::String &AliasDataType::headerFile() const
{
    return m_headerFile;
}

void AliasDataType::setHeaderFile(common::String headerFile)
{
    m_headerFile = std::move(headerFile);
}

} // namespace simulink::model
