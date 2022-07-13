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

#include "outport.h"

namespace simulink::model {

const common::String &Outport::outDataTypeStr() const
{
    return m_outDataTypeStr;
}

void Outport::setOutDataTypeStr(common::String outDataTypeStr)
{
    m_outDataTypeStr = std::move(outDataTypeStr);
}

const common::String &Outport::busObject() const
{
    return m_busObject;
}

void Outport::setBusObject(common::String busObject)
{
    m_busObject = std::move(busObject);
}

const common::String &Outport::busOutputAsStruct() const
{
    return m_busOutputAsStruct;
}

void Outport::setBusOutputAsStruct(common::String busOutputAsStruct)
{
    m_busOutputAsStruct = std::move(busOutputAsStruct);
}

const common::String &Outport::iconDisplay() const
{
    return m_iconDisplay;
}

void Outport::setIconDisplay(common::String iconDisplay)
{
    m_iconDisplay = std::move(iconDisplay);
}

const common::String &Outport::initialOutput() const
{
    return m_initialOutput;
}

void Outport::setInitialOutput(common::String initialOutput)
{
    m_initialOutput = std::move(initialOutput);
}

const common::String &Outport::inputSignalNames() const
{
    return m_inputSignalNames;
}

void Outport::setInputSignalNames(common::String inputSignalNames)
{
    m_inputSignalNames = std::move(inputSignalNames);
}

const common::String &Outport::lockScale() const
{
    return m_lockScale;
}

void Outport::setLockScale(common::String lockScale)
{
    m_lockScale = std::move(lockScale);
}

const common::String &Outport::mustResolveToSignalObject() const
{
    return m_mustResolveToSignalObject;
}

void Outport::setMustResolveToSignalObject(common::String mustResolveToSignalObject)
{
    m_mustResolveToSignalObject = std::move(mustResolveToSignalObject);
}

const common::String &Outport::outMax() const
{
    return m_outMax;
}

void Outport::setOutMax(common::String outMax)
{
    m_outMax = std::move(outMax);
}

const common::String &Outport::outMin() const
{
    return m_outMin;
}

void Outport::setOutMin(common::String outMin)
{
    m_outMin = std::move(outMin);
}

const common::String &Outport::outputWhenDisabled() const
{
    return m_outputWhenDisabled;
}

void Outport::setOutputWhenDisabled(common::String outputWhenDisabled)
{
    m_outputWhenDisabled = std::move(outputWhenDisabled);
}

const common::String &Outport::port() const
{
    return m_port;
}

void Outport::setPort(common::String port)
{
    m_port = std::move(port);
}

const PortDimension &Outport::portDimensions() const
{
    return m_portDimensions;
}

void Outport::setPortDimensions(PortDimension portDimensions)
{
    m_portDimensions = std::move(portDimensions);
}

const common::String &Outport::sampleTime() const
{
    return m_sampleTime;
}

void Outport::setSampleTime(common::String sampleTime)
{
    m_sampleTime = std::move(sampleTime);
}

const common::String &Outport::signalName() const
{
    return m_signalName;
}

void Outport::setSignalName(common::String signalName)
{
    m_signalName = std::move(signalName);
}

const common::String &Outport::signalObject() const
{
    return m_signalObject;
}

void Outport::setSignalObject(common::String signalObject)
{
    m_signalObject = std::move(signalObject);
}

const SignalType &Outport::signalType() const
{
    return m_signalType;
}

void Outport::setSignalType(SignalType signalType)
{
    m_signalType = std::move(signalType);
}

const common::String &Outport::storageClass() const
{
    return m_storageClass;
}

void Outport::setStorageClass(common::String storageClass)
{
    m_storageClass = std::move(storageClass);
}

const common::String &Outport::unit() const
{
    return m_unit;
}

void Outport::setUnit(common::String unit)
{
    m_unit = std::move(unit);
}

const common::String &Outport::unitNoProp() const
{
    return m_unitNoProp;
}

void Outport::setUnitNoProp(common::String unitNoProp)
{
    m_unitNoProp = std::move(unitNoProp);
}

const common::String &Outport::useBusObject() const
{
    return m_useBusObject;
}

void Outport::setUseBusObject(common::String useBusObject)
{
    m_useBusObject = std::move(useBusObject);
}

const common::String &Outport::varSizeSig() const
{
    return m_varSizeSig;
}

void Outport::setVarSizeSig(common::String varSizeSig)
{
    m_varSizeSig = std::move(varSizeSig);
}

} // namespace simulink::model
