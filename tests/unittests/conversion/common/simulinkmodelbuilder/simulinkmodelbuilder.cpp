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

#include "simulinkmodelbuilder.h"

using namespace simulink::model;

namespace tests::conversion::common {

SimulinkModelBuilder::SimulinkModelBuilder(QString name)
{
    m_simulinkModel.setName(name);
}

std::unique_ptr<SimulinkModel> SimulinkModelBuilder::build()
{
    return std::make_unique<SimulinkModel>(std::move(m_simulinkModel));
}

SimulinkModelBuilder &SimulinkModelBuilder::withEnumDataType(
        QString name, DataScope dataScope, QString description, QString headerFile, QStringList elems)
{
    EnumDataType enumDataType;

    enumDataType.setName(std::move(name));

    enumDataType.setHeaderFile(std::move(headerFile));
    enumDataType.setDescription(std::move(description));
    enumDataType.setAddClassNameToEnumNames("false");
    enumDataType.setDataScope(dataScope);
    enumDataType.setDefaultValue("0");

    uint32_t index = 0;
    for (const auto &elem : elems) {
        EnumValue enumValue;

        enumValue.setName(elem);
        enumValue.setValue(index++);
        enumValue.setDescription("");
        enumValue.setDetailedDescription("");

        enumDataType.addEnumValue(std::move(enumValue));
    }

    m_simulinkModel.addDataType(std::move(enumDataType));

    return *this;
}

SimulinkModelBuilder &SimulinkModelBuilder::withAliasDataType(
        QString name, QString baseType, DataScope dataScope, QString description, QString headerFile)
{
    AliasDataType aliasDataType;

    aliasDataType.setName(std::move(name));
    aliasDataType.setBaseType(std::move(baseType));
    aliasDataType.setDataScope(dataScope);
    aliasDataType.setDescription(std::move(description));
    aliasDataType.setHeaderFile(headerFile);

    m_simulinkModel.addDataType(std::move(aliasDataType));

    return *this;
}

SimulinkModelBuilder &SimulinkModelBuilder::withBusDataType(BusDataType busDataType)
{
    m_simulinkModel.addDataType(std::move(busDataType));

    return *this;
}

SimulinkModelBuilder &SimulinkModelBuilder::withInport(
        QString name, QString outputSignalNames, QString outDataTypeStr, QString port, PortDimension portDimension)
{
    Inport inport;

    inport.setName(std::move(name));
    inport.setOutDataTypeStr(std::move(outDataTypeStr));
    inport.setBusObject("BusObject");
    inport.setBusOutputAsStruct("off");
    inport.setIconDisplay("Port number");
    inport.setInterpolate("on");
    inport.setLatchByDelayingOutsideSignal("off");
    inport.setLatchInputForFeedbackSignals("off");
    inport.setLockScale("off");
    inport.setOutMax("[]");
    inport.setOutMin("[]");
    inport.setOutputSignalNames(std::move(outputSignalNames));
    inport.setPort(std::move(port));
    inport.setPortDimensions(std::move(portDimension));
    inport.setSampleTime("-1");
    inport.setSignalType(SignalType::Auto);
    inport.setUnit("inherit");
    inport.setUnitNoProp("");
    inport.setUseBusObject("off");

    m_simulinkModel.addInport(std::move(inport));

    return *this;
}

SimulinkModelBuilder &SimulinkModelBuilder::withOutport(
        QString name, QString inputSignalNames, QString outDataTypeStr, QString port, PortDimension portDimension)
{
    Outport outport;

    outport.setName(std::move(name));
    outport.setOutDataTypeStr(std::move(outDataTypeStr));
    outport.setBusObject("BusObject");
    outport.setBusOutputAsStruct("off");
    outport.setIconDisplay("Port number");
    outport.setInitialOutput("[]");
    outport.setInputSignalNames(std::move(inputSignalNames));
    outport.setLockScale("off");
    outport.setMustResolveToSignalObject("off");
    outport.setOutMax("[]");
    outport.setOutMin("[]");
    outport.setOutputWhenDisabled("held");
    outport.setPort(std::move(port));
    outport.setPortDimensions(std::move(portDimension));
    outport.setSampleTime("-1");
    outport.setSignalName("");
    outport.setSignalObject("");
    outport.setSignalType(SignalType::Auto);
    outport.setStorageClass("auto");
    outport.setUnit("inherit");
    outport.setUnitNoProp("");
    outport.setUseBusObject("off");
    outport.setVarSizeSig("Inherit");

    m_simulinkModel.addOutport(std::move(outport));

    return *this;
}

} // namespace tests::conversion::common
