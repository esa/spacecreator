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
#include <simulink/SimulinkModel/common/portdimension.h>
#include <simulink/SimulinkModel/common/signaltype.h>

namespace simulink::model {

class Port : public NamedEntity
{
public:
    Port() = default;
    Port(Port &&) = default;

    virtual ~Port() = 0;

    Port &operator=(Port &&) = default;

public:
    auto outDataTypeStr() const -> const common::String &;
    auto setOutDataTypeStr(common::String outDataTypeStr) -> void;

    auto busObject() const -> const common::String &;
    auto setBusObject(common::String busObject) -> void;

    auto busOutputAsStruct() const -> const common::String &;
    auto setBusOutputAsStruct(common::String busOutputAsStruct) -> void;

    auto iconDisplay() const -> const common::String &;
    auto setIconDisplay(common::String iconDisplay) -> void;

    auto outMax() const -> const common::String &;
    auto setOutMax(common::String outMax) -> void;

    auto outMin() const -> const common::String &;
    auto setOutMin(common::String outMin) -> void;

    auto port() const -> const common::String &;
    auto setPort(common::String port) -> void;

    auto portDimensions() const -> const PortDimension &;
    auto setPortDimensions(PortDimension portDimensions) -> void;

    auto sampleTime() const -> const common::String &;
    auto setSampleTime(common::String sampleTime) -> void;

    auto signalType() const -> const SignalType &;
    auto setSignalType(SignalType signalType) -> void;

    auto unit() const -> const common::String &;
    auto setUnit(common::String unit) -> void;

    auto unitNoProp() const -> const common::String &;
    auto setUnitNoProp(common::String unitNoProp) -> void;

    auto lockScale() const -> const common::String &;
    auto setLockScale(common::String lockScale) -> void;

    auto useBusObject() const -> const common::String &;
    auto setUseBusObject(common::String useBusObject) -> void;

protected:
    common::String m_busObject;
    common::String m_busOutputAsStruct;
    common::String m_iconDisplay;
    common::String m_lockScale;
    common::String m_outDataTypeStr;
    common::String m_outMax;
    common::String m_outMin;
    common::String m_port;
    PortDimension m_portDimensions;
    common::String m_sampleTime;
    SignalType m_signalType;
    common::String m_unit;
    common::String m_unitNoProp;
    common::String m_useBusObject;
};

} // namespace seds::model
