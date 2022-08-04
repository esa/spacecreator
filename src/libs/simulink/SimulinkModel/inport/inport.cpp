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

#include "inport.h"

namespace simulink::model {

const common::String &Inport::outDataTypeStr() const
{
    return m_outDataTypeStr;
}

void Inport::setOutDataTypeStr(common::String outDataTypeStr)
{
    m_outDataTypeStr = std::move(outDataTypeStr);
}

const common::String &Inport::busObject() const
{
    return m_busObject;
}

void Inport::setBusObject(common::String busObject)
{
    m_busObject = std::move(busObject);
}

const common::String &Inport::busOutputAsStruct() const
{
    return m_busOutputAsStruct;
}

void Inport::setBusOutputAsStruct(common::String busOutputAsStruct)
{
    m_busOutputAsStruct = std::move(busOutputAsStruct);
}

const common::String &Inport::iconDisplay() const
{
    return m_iconDisplay;
}

void Inport::setIconDisplay(common::String iconDisplay)
{
    m_iconDisplay = std::move(iconDisplay);
}

const common::String &Inport::interpolate() const
{
    return m_interpolate;
}

void Inport::setInterpolate(common::String interpolate)
{
    m_interpolate = std::move(interpolate);
}

const common::String &Inport::latchByDelayingOutsideSignal() const
{
    return m_latchByDelayingOutsideSignal;
}

void Inport::setLatchByDelayingOutsideSignal(common::String latchByDelayingOutsideSignal)
{
    m_latchByDelayingOutsideSignal = std::move(latchByDelayingOutsideSignal);
}

const common::String &Inport::latchInputForFeedbackSignals() const
{
    return m_latchInputForFeedbackSignals;
}

void Inport::setLatchInputForFeedbackSignals(common::String latchInputForFeedbackSignals)
{
    m_latchInputForFeedbackSignals = std::move(latchInputForFeedbackSignals);
}

const common::String &Inport::lockScale() const
{
    return m_lockScale;
}

void Inport::setLockScale(common::String lockScale)
{
    m_lockScale = std::move(lockScale);
}

const common::String &Inport::outMax() const
{
    return m_outMax;
}

void Inport::setOutMax(common::String outMax)
{
    m_outMax = std::move(outMax);
}

const common::String &Inport::outMin() const
{
    return m_outMin;
}

void Inport::setOutMin(common::String outMin)
{
    m_outMin = std::move(outMin);
}

const common::String &Inport::outputSignalNames() const
{
    return m_outputSignalNames;
}

void Inport::setOutputSignalNames(common::String outputSignalNames)
{
    m_outputSignalNames = std::move(outputSignalNames);
}

const common::String &Inport::port() const
{
    return m_port;
}

void Inport::setPort(common::String port)
{
    m_port = std::move(port);
}

const PortDimension &Inport::portDimensions() const
{
    return m_portDimensions;
}

void Inport::setPortDimensions(PortDimension portDimensions)
{
    m_portDimensions = std::move(portDimensions);
}

const common::String &Inport::sampleTime() const
{
    return m_sampleTime;
}

void Inport::setSampleTime(common::String sampleTime)
{
    m_sampleTime = std::move(sampleTime);
}

SignalType Inport::signalType() const
{
    return m_signalType;
}

void Inport::setSignalType(SignalType signalType)
{
    m_signalType = signalType;
}

const common::String &Inport::unit() const
{
    return m_unit;
}

void Inport::setUnit(common::String unit)
{
    m_unit = std::move(unit);
}

const common::String &Inport::unitNoProp() const
{
    return m_unitNoProp;
}

void Inport::setUnitNoProp(common::String unitNoProp)
{
    m_unitNoProp = std::move(unitNoProp);
}

const common::String &Inport::useBusObject() const
{
    return m_useBusObject;
}

void Inport::setUseBusObject(common::String useBusObject)
{
    m_useBusObject = std::move(useBusObject);
}

} // namespace simulink::model
