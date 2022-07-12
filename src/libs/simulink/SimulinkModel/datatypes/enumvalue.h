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

#include <simulink/SimulinkCommon/basetypesmappings.h>
#include <simulink/SimulinkModel/common/namedentity.h>

namespace simulink::model {

class EnumValue final : public NamedEntity
{
public:
    using Value = int64_t;

public:
    EnumValue() = default;
    EnumValue(EnumValue &&) = default;
    EnumValue &operator=(EnumValue &&) = default;

public:
    auto value() const -> Value;
    auto setValue(Value value) -> void;

    auto description() const -> const common::String &;
    auto setDescription(common::String description) -> void;

    auto detailedDescription() const -> const common::String &;
    auto setDetailedDescription(common::String detailedDescription) -> void;

private:
    Value m_value;
    common::String m_description;
    common::String m_detailedDescription;
};

} // namespace simulink::model
