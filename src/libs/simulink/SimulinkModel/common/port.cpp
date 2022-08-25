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

#include "port.h"

namespace simulink::model {

Port::~Port() = default;

const common::String &Port::outDataTypeStr() const
{
    return m_outDataTypeStr;
}

void Port::setOutDataTypeStr(common::String outDataTypeStr)
{
    m_outDataTypeStr = std::move(outDataTypeStr);
}

const common::String &Port::busObject() const
{
    return m_busObject;
}

void Port::setBusObject(common::String busObject)
{
    m_busObject = std::move(busObject);
}

const common::String &Port::busOutputAsStruct() const
{
    return m_busOutputAsStruct;
}

void Port::setBusOutputAsStruct(common::String busOutputAsStruct)
{
    m_busOutputAsStruct = std::move(busOutputAsStruct);
}

const common::String &Port::iconDisplay() const
{
    return m_iconDisplay;
}

void Port::setIconDisplay(common::String iconDisplay)
{
    m_iconDisplay = std::move(iconDisplay);
}

const common::String &Port::lockScale() const
{
    return m_lockScale;
}

void Port::setLockScale(common::String lockScale)
{
    m_lockScale = std::move(lockScale);
}

const common::String &Port::outMax() const
{
    return m_outMax;
}

void Port::setOutMax(common::String outMax)
{
    m_outMax = std::move(outMax);
}

const common::String &Port::outMin() const
{
    return m_outMin;
}

void Port::setOutMin(common::String outMin)
{
    m_outMin = std::move(outMin);
}

const common::String &Port::port() const
{
    return m_port;
}

void Port::setPort(common::String port)
{
    m_port = std::move(port);
}

const PortDimension &Port::portDimensions() const
{
    return m_portDimensions;
}

void Port::setPortDimensions(PortDimension portDimensions)
{
    m_portDimensions = std::move(portDimensions);
}

const common::String &Port::sampleTime() const
{
    return m_sampleTime;
}

void Port::setSampleTime(common::String sampleTime)
{
    m_sampleTime = std::move(sampleTime);
}

const SignalType &Port::signalType() const
{
    return m_signalType;
}

void Port::setSignalType(SignalType signalType)
{
    m_signalType = signalType;
}

const common::String &Port::unit() const
{
    return m_unit;
}

void Port::setUnit(common::String unit)
{
    m_unit = std::move(unit);
}

const common::String &Port::unitNoProp() const
{
    return m_unitNoProp;
}

void Port::setUnitNoProp(common::String unitNoProp)
{
    m_unitNoProp = std::move(unitNoProp);
}

const common::String &Port::useBusObject() const
{
    return m_useBusObject;
}

void Port::setUseBusObject(common::String useBusObject)
{
    m_useBusObject = std::move(useBusObject);
}

} // namespace simulink::model
