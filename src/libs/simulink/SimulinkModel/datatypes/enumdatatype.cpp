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

#include "enumdatatype.h"

namespace simulink::model {

const common::String &EnumDataType::headerFile() const
{
    return m_headerFile;
}

void EnumDataType::setHeaderFile(common::String headerFile)
{
    m_headerFile = std::move(headerFile);
}

const common::String &EnumDataType::description() const
{
    return m_description;
}

void EnumDataType::setDescription(common::String description)
{
    m_description = std::move(description);
}

const common::String &EnumDataType::addClassNameToEnumNames() const
{
    return m_addClassNameToEnumNames;
}

void EnumDataType::setAddClassNameToEnumNames(common::String addClassNameToEnumNames)
{
    m_addClassNameToEnumNames = std::move(addClassNameToEnumNames);
}

const common::String &EnumDataType::dataScope() const
{
    return m_dataScope;
}

void EnumDataType::setDataScope(common::String dataScope)
{
    m_dataScope = std::move(dataScope);
}

const common::String &EnumDataType::defaultValue() const
{
    return m_defaultValue;
}

void EnumDataType::setDefaultValue(common::String defaultValue)
{
    m_defaultValue = std::move(defaultValue);
}

const EnumDataType::EnumValues &EnumDataType::enumValues() const
{
    return m_enumValues;
}

void EnumDataType::addEnumValue(EnumValue enumValue)
{
    m_enumValues.push_back(std::move(enumValue));
}

} // namespace simulink::model
