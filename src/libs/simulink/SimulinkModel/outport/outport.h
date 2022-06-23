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

#include <QString>

namespace simulink::model {

class Outport final
{
public:
    Outport() = default;
    Outport(Outport &&) = default;
    Outport &operator=(Outport &&) = default;

public:
	auto name() const -> const QString &;
	auto setName(QString name) -> void;

	auto outDataTypeStr() const -> const QString &;
	auto setOutDataTypeStr(QString outDataTypeStr) -> void;

	auto busObject() const -> const QString &;
	auto setBusObject(QString busObject) -> void;

	auto busOutputAsStruct() const -> const QString &;
	auto setBusOutputAsStruct(QString busOutputAsStruct) -> void;

	auto iconDisplay() const -> const QString &;
	auto setIconDisplay(QString iconDisplay) -> void;

	auto initialOutput() const -> const QString &;
	auto setInitialOutput(QString initialOutput) -> void;

	auto inputSignalNames() const -> const QString &;
	auto setInputSignalNames(QString inputSignalNames) -> void;

	auto lockScale() const -> const QString &;
	auto setLockScale(QString lockScale) -> void;

	auto mustResolveToSignalObject() const -> const QString &;
	auto setMustResolveToSignalObject(QString mustResolveToSignalObject) -> void;

	auto outMax() const -> const QString &;
	auto setOutMax(QString outMax) -> void;

	auto outMin() const -> const QString &;
	auto setOutMin(QString outMin) -> void;

	auto outputWhenDisabled() const -> const QString &;
	auto setOutputWhenDisabled(QString outputWhenDisabled) -> void;

	auto port() const -> const QString &;
	auto setPort(QString port) -> void;

	auto portDimensions() const -> const QString &;
	auto setPortDimensions(QString portDimensions) -> void;

	auto sampleTime() const -> const QString &;
	auto setSampleTime(QString sampleTime) -> void;

	auto signalName() const -> const QString &;
	auto setSignalName(QString signalName) -> void;

	auto signalObject() const -> const QString &;
	auto setSignalObject(QString signalObject) -> void;

	auto signalType() const -> const QString &;
	auto setSignalType(QString signalType) -> void;

	auto storageClass() const -> const QString &;
	auto setStorageClass(QString storageClass) -> void;

	auto unit() const -> const QString &;
	auto setUnit(QString unit) -> void;

	auto unitNoProp() const -> const QString &;
	auto setUnitNoProp(QString unitNoProp) -> void;

	auto useBusObject() const -> const QString &;
	auto setUseBusObject(QString useBusObject) -> void;

	auto varSizeSig() const -> const QString &;
	auto setVarSizeSig(QString varSizeSig) -> void;
private:
	QString m_name;
	QString m_outDataTypeStr;
	QString m_busObject;
	QString m_busOutputAsStruct;
	QString m_iconDisplay;
	QString m_initialOutput;
	QString m_inputSignalNames;
	QString m_lockScale;
	QString m_mustResolveToSignalObject;
	QString m_outMax;
	QString m_outMin;
	QString m_outputWhenDisabled;
	QString m_port;
	QString m_portDimensions;
	QString m_sampleTime;
	QString m_signalName;
	QString m_signalObject;
	QString m_signalType;
	QString m_storageClass;
	QString m_unit;
	QString m_unitNoProp;
	QString m_useBusObject;
	QString m_varSizeSig;
};

} // namespace simulink::model
