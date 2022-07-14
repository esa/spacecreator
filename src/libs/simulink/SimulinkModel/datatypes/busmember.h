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

#include "rootdatatype.h"

#include <simulink/SimulinkCommon/basetypesmappings.h>
#include <simulink/SimulinkModel/common/complexity.h>
#include <simulink/SimulinkModel/common/dimensions.h>
#include <simulink/SimulinkModel/common/dimensionsmode.h>
#include <simulink/SimulinkModel/common/namedentity.h>

namespace simulink::model {

class BusMember final : public NamedEntity
{
public:
    BusMember() = default;
    BusMember(BusMember &&) = default;
    BusMember &operator=(BusMember &&) = default;

public:
    auto dataType() const -> const common::String &;
    auto setDataType(common::String dataType) -> void;

    auto complexity() const -> Complexity;
    auto setComplexity(Complexity complexity) -> void;

    auto description() const -> const common::String &;
    auto setDescription(common::String description) -> void;

    auto dimensions() const -> const Dimensions &;
    auto setDimensions(Dimensions dimensions) -> void;

    auto dimensionsMode() const -> DimensionsMode;
    auto setDimensionsMode(DimensionsMode dimensionsMode) -> void;

    auto max() const -> const common::String &;
    auto setMax(common::String max) -> void;

    auto min() const -> const common::String &;
    auto setMin(common::String min) -> void;

    auto sampleTime() const -> const common::String &;
    auto setSampleTime(common::String sampleTime) -> void;

    auto unit() const -> const common::String &;
    auto setUnit(common::String unit) -> void;

private:
    common::String m_dataType;
    Complexity m_complexity;
    common::String m_description;
    Dimensions m_dimensions;
    DimensionsMode m_dimensionsMode;
    common::String m_max;
    common::String m_min;
    common::String m_sampleTime;
    common::String m_unit;
};

} // namespace simulink::model
