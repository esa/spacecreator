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

#include "busmember.h"
#include "rootdatatype.h"

#include <simulink/SimulinkCommon/basetypesmappings.h>
#include <simulink/SimulinkModel/common/datascope.h>
#include <vector>

namespace simulink::model {

class BusDataType final : public RootDataType
{
public:
    using BusMembers = std::vector<BusMember>;

public:
    BusDataType() = default;
    BusDataType(BusDataType &&) = default;
    BusDataType &operator=(BusDataType &&) = default;

public:
    auto dataScope() const -> DataScope;
    auto setDataScope(DataScope dataScope) -> void;

    auto description() const -> const common::String &;
    auto setDescription(common::String description) -> void;

    auto headerFile() const -> const common::String &;
    auto setHeaderFile(common::String headerFile) -> void;

    auto alignment() const -> const common::String &;
    auto setAlignment(common::String alignment) -> void;

    auto busMembers() const -> const BusMembers &;
    auto addBusMember(BusMember busMember) -> void;

private:
    DataScope m_dataScope;
    common::String m_description;
    common::String m_headerFile;
    common::String m_alignment;

    BusMembers m_busMembers;
};

} // namespace simulink::model
