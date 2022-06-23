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

const QString &Outport::name() const
{
    return m_name;
}

void Outport::setName(QString name)
{
    m_name = std::move(name);
}

const QString &Outport::outDataTypeStr() const
{
    return m_outDataTypeStr;
}

void Outport::setOutDataTypeStr(QString outDataTypeStr)
{
    m_outDataTypeStr = std::move(outDataTypeStr);
}

const QString &Outport::busObject() const
{
    return m_busObject;
}

void Outport::setBusObject(QString busObject)
{
    m_busObject = std::move(busObject);
}

const QString &Outport::busOutputAsStruct() const
{
    return m_busOutputAsStruct;
}

void Outport::setBusOutputAsStruct(QString busOutputAsStruct)
{
    m_busOutputAsStruct = std::move(busOutputAsStruct);
}

const QString &Outport::iconDisplay() const
{
    return m_iconDisplay;
}

void Outport::setIconDisplay(QString iconDisplay)
{
    m_iconDisplay = std::move(iconDisplay);
}

const QString &Outport::initialOutput() const
{
    return m_initialOutput;
}

void Outport::setInitialOutput(QString initialOutput)
{
    m_initialOutput = std::move(initialOutput);
}

const QString &Outport::inputSignalNames() const
{
    return m_inputSignalNames;
}

void Outport::setInputSignalNames(QString inputSignalNames)
{
    m_inputSignalNames = std::move(inputSignalNames);
}

const QString &Outport::lockScale() const
{
    return m_lockScale;
}

void Outport::setLockScale(QString lockScale)
{
    m_lockScale = std::move(lockScale);
}

const QString &Outport::mustResolveToSignalObject() const
{
    return m_mustResolveToSignalObject;
}

void Outport::setMustResolveToSignalObject(QString mustResolveToSignalObject)
{
    m_mustResolveToSignalObject = std::move(mustResolveToSignalObject);
}

const QString &Outport::outMax() const
{
    return m_outMax;
}

void Outport::setOutMax(QString outMax)
{
    m_outMax = std::move(outMax);
}

const QString &Outport::outMin() const
{
    return m_outMin;
}

void Outport::setOutMin(QString outMin)
{
    m_outMin = std::move(outMin);
}

const QString &Outport::outputWhenDisabled() const
{
    return m_outputWhenDisabled;
}

void Outport::setOutputWhenDisabled(QString outputWhenDisabled)
{
    m_outputWhenDisabled = std::move(outputWhenDisabled);
}

const QString &Outport::port() const
{
    return m_port;
}

void Outport::setPort(QString port)
{
    m_port = std::move(port);
}

const QString &Outport::portDimensions() const
{
    return m_portDimensions;
}

void Outport::setPortDimensions(QString portDimensions)
{
    m_portDimensions = std::move(portDimensions);
}

const QString &Outport::sampleTime() const
{
    return m_sampleTime;
}

void Outport::setSampleTime(QString sampleTime)
{
    m_sampleTime = std::move(sampleTime);
}

const QString &Outport::signalName() const
{
    return m_signalName;
}

void Outport::setSignalName(QString signalName)
{
    m_signalName = std::move(signalName);
}

const QString &Outport::signalObject() const
{
    return m_signalObject;
}

void Outport::setSignalObject(QString signalObject)
{
    m_signalObject = std::move(signalObject);
}

const QString &Outport::signalType() const
{
    return m_signalType;
}

void Outport::setSignalType(QString signalType)
{
    m_signalType = std::move(signalType);
}

const QString &Outport::storageClass() const
{
    return m_storageClass;
}

void Outport::setStorageClass(QString storageClass)
{
    m_storageClass = std::move(storageClass);
}

const QString &Outport::unit() const
{
    return m_unit;
}

void Outport::setUnit(QString unit)
{
    m_unit = std::move(unit);
}

const QString &Outport::unitNoProp() const
{
    return m_unitNoProp;
}

void Outport::setUnitNoProp(QString unitNoProp)
{
    m_unitNoProp = std::move(unitNoProp);
}

const QString &Outport::useBusObject() const
{
    return m_useBusObject;
}

void Outport::setUseBusObject(QString useBusObject)
{
    m_useBusObject = std::move(useBusObject);
}

const QString &Outport::varSizeSig() const
{
    return m_varSizeSig;
}

void Outport::setVarSizeSig(QString varSizeSig)
{
    m_varSizeSig = std::move(varSizeSig);
}

} // namespace simulink::model
