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

const QString &Inport::name() const
{
    return m_name;
}

void Inport::setName(QString name)
{
    m_name = std::move(name);
}

const QString &Inport::outDataTypeStr() const
{
    return m_outDataTypeStr;
}

void Inport::setOutDataTypeStr(QString outDataTypeStr)
{
    m_outDataTypeStr = std::move(outDataTypeStr);
}


const QString &Inport::busObject() const
{
    return m_busObject;
}

void Inport::setBusObject(QString busObject)
{
    m_busObject = std::move(busObject);
}

const QString &Inport::busOutputAsStruct() const
{
    return m_busOutputAsStruct;
}

void Inport::setBusOutputAsStruct(QString busOutputAsStruct)
{
    m_busOutputAsStruct = std::move(busOutputAsStruct);
}

const QString &Inport::iconDisplay() const
{
    return m_iconDisplay;
}

void Inport::setIconDisplay(QString iconDisplay)
{
    m_iconDisplay = std::move(iconDisplay);
}

const QString &Inport::interpolate() const
{
    return m_interpolate;
}

void Inport::setInterpolate(QString interpolate)
{
    m_interpolate = std::move(interpolate);
}

const QString &Inport::latchByDelayingOutsideSignal() const
{
    return m_latchByDelayingOutsideSignal;
}

void Inport::setLatchByDelayingOutsideSignal(QString latchByDelayingOutsideSignal)
{
    m_latchByDelayingOutsideSignal = std::move(latchByDelayingOutsideSignal);
}

const QString &Inport::latchInputForFeedbackSignals() const
{
    return m_latchInputForFeedbackSignals;
}

void Inport::setLatchInputForFeedbackSignals(QString latchInputForFeedbackSignals)
{
    m_latchInputForFeedbackSignals = std::move(latchInputForFeedbackSignals);
}

const QString &Inport::lockScale() const
{
    return m_lockScale;
}

void Inport::setLockScale(QString lockScale)
{
    m_lockScale = std::move(lockScale);
}

const QString &Inport::outMax() const
{
    return m_outMax;
}

void Inport::setOutMax(QString outMax)
{
    m_outMax = std::move(outMax);
}

const QString &Inport::outMin() const
{
    return m_outMin;
}

void Inport::setOutMin(QString outMin)
{
    m_outMin = std::move(outMin);
}

const QString &Inport::outputSignalNames() const
{
    return m_outputSignalNames;
}

void Inport::setOutputSignalNames(QString outputSignalNames)
{
    m_outputSignalNames = std::move(outputSignalNames);
}

const QString &Inport::port() const
{
    return m_port;
}

void Inport::setPort(QString port)
{
    m_port = std::move(port);
}

const QString &Inport::portDimensions() const
{
    return m_portDimensions;
}

void Inport::setPortDimensions(QString portDimensions)
{
    m_portDimensions = std::move(portDimensions);
}

const QString &Inport::sampleTime() const
{
    return m_sampleTime;
}

void Inport::setSampleTime(QString sampleTime)
{
    m_sampleTime = std::move(sampleTime);
}

const QString &Inport::signalType() const
{
    return m_signalType;
}

void Inport::setSignalType(QString signalType)
{
    m_signalType = std::move(signalType);
}

const QString &Inport::unit() const
{
    return m_unit;
}

void Inport::setUnit(QString unit)
{
    m_unit = std::move(unit);
}

const QString &Inport::unitNoProp() const
{
    return m_unitNoProp;
}

void Inport::setUnitNoProp(QString unitNoProp)
{
    m_unitNoProp = std::move(unitNoProp);
}

const QString &Inport::useBusObject() const
{
    return m_useBusObject;
}

void Inport::setUseBusObject(QString useBusObject)
{
    m_useBusObject = std::move(useBusObject);
}

} // namespace simulink::model
