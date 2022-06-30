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

#pragma once

#include <vector>

#include <simulink/SimulinkCommon/basetypesmappings.h>
#include "enumvalue.h"
#include "rootdatatype.h"

namespace simulink::model {

class EnumDataType final : public RootDataType
{
public:
    using EnumValues = std::vector<EnumValue>;

public:
    EnumDataType() = default;
    EnumDataType(EnumDataType &&) = default;
    EnumDataType &operator=(EnumDataType &&) = default;

public:
    auto headerFile() const -> const common::String &;
    auto setHeaderFile(common::String headerFile) -> void;

    auto description() const -> const common::String &;
    auto setDescription(common::String description) -> void;

    auto addClassNameToEnumNames() const -> const common::String &;
    auto setAddClassNameToEnumNames(common::String addClassNameToEnumNames) -> void;

    auto dataScope() const -> const common::String &;
    auto setDataScope(common::String dataScope) -> void;

    auto defaultValue() const -> const common::String &;
    auto setDefaultValue(common::String defaultValue) -> void;

    auto enumValues() const -> const EnumValues &;
    auto addEnumValue(EnumValue enumValue) -> void;
    
private:
    common::String m_headerFile;
    common::String m_description;
    common::String m_addClassNameToEnumNames;
    common::String m_dataScope;
    common::String m_defaultValue;

    EnumValues m_enumValues;
};

} // namespace simulink::model
