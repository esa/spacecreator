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

#include "importer.h"
#include "exceptions.h"

#include <simulink/SimulinkModel/simulinkmodel.h>
#include <simulink/SimulinkOptions/options.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/exceptions.h>

#include <datatypes/rootdatatype.h>
#include <datatypes/enumdatatype.h>
#include <datatypes/enumvalue.h>
#include <datatypes/aliasdatatype.h>
#include <datatypes/busdatatype.h>

#include <QFileInfo>
#include <QXmlStreamReader>

using conversion::Options;
using conversion::FileNotFoundException;
using conversion::importer::ImportException;
using conversion::simulink::SimulinkOptions;

namespace simulink::importer {

std::unique_ptr<conversion::Model> SimulinkXmlImporter::importModel(const Options &options) const
{
    const auto inputFilepath = options.value(SimulinkOptions::inputFilepath);
    if (!inputFilepath) {
        throw ImportException("File to import wasn't specified");
    }

    const QFileInfo inputFileInfo(*inputFilepath);
    if (!inputFileInfo.exists()) {
        throw FileNotFoundException(*inputFilepath, "while importing");
    }

    auto model = parse(*inputFilepath);

    return model;
}

bool SimulinkXmlImporter::processForNamedEntity(model::NamedEntity &namedEntity, const QXmlStreamAttribute &attribute)
{
    if (attribute.name() == QStringLiteral("Name")) {
        namedEntity.setName(attribute.value().toString());
    } else {
        return false;
    }

    return true;
}

std::unique_ptr<conversion::Model> SimulinkXmlImporter::parse(const common::String &inputSimulinkFilename)
{
    QFile inputSimulinkFile(inputSimulinkFilename);
    if (!inputSimulinkFile.open(QIODevice::ReadOnly)) {
        throw ParserException(inputSimulinkFile.errorString());
    }

    auto xmlReader = QXmlStreamReader(&inputSimulinkFile);

    xmlReader.readNextStartElement();

    if (xmlReader.name() == QLatin1String("ModelInterface")) {
        return std::make_unique<model::SimulinkModel>(readModelInterface(xmlReader)); 
    } else {
        throw UnhandledElement(xmlReader.name(), "Root");
    }
}

model::ModelInterface SimulinkXmlImporter::readModelInterface(QXmlStreamReader &xmlReader)
{
    model::ModelInterface modelInterface;

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("DataTypes")) {
            readDataTypes(xmlReader, modelInterface);
        } else if (xmlReader.name() == QStringLiteral("Inports")) {
            readInports(xmlReader, modelInterface);
        } else if (xmlReader.name() == QStringLiteral("Outports")) {
            readOutports(xmlReader, modelInterface);
        } else {
            throw UnhandledElement(xmlReader.name(), "ModelInterface");
        }
    }

    return modelInterface;
}

void SimulinkXmlImporter::readDataTypes(QXmlStreamReader &xmlReader, model::ModelInterface &modelInterface)
{
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("EnumDataType")) {
            modelInterface.addDataType(readEnumDataType(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("AliasDataType")) {
            modelInterface.addDataType(readAliasDataType(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("BusDataType")) {
            modelInterface.addDataType(readBusDataType(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Workspace");
        }
    }
}

model::EnumDataType SimulinkXmlImporter::readEnumDataType(QXmlStreamReader &xmlReader)
{
    model::EnumDataType enumDataType;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForNamedEntity(enumDataType, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("AddClassNameToEnumNames")) {
            enumDataType.setAddClassNameToEnumNames(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("DataScope")) {
            enumDataType.setDataScope(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("DefaultValue")) {
            enumDataType.setDefaultValue(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Description")) {
            enumDataType.setDescription(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("HeaderFile")) {
            enumDataType.setHeaderFile(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }
    
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("EnumValues")) {
            readEnumValues(xmlReader, enumDataType);
        } else {
            throw UnhandledElement(xmlReader.name(), "EnumDataType");
        }
    }

    return enumDataType;
}

void SimulinkXmlImporter::readEnumValues(QXmlStreamReader &xmlReader, model::EnumDataType &enumDataType)
{
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("EnumValue")) {
            enumDataType.addEnumValue(readEnumValue(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "EnumValues");
        }
    }
}

model::EnumValue SimulinkXmlImporter::readEnumValue(QXmlStreamReader &xmlReader)
{
    model::EnumValue enumValue;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForNamedEntity(enumValue, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("Value")) {
            enumValue.setValue(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Description")) {
            enumValue.setDescription(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("DetailedDescription")) {
            enumValue.setDetailedDescription(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    xmlReader.skipCurrentElement();

    return enumValue;
}

model::AliasDataType SimulinkXmlImporter::readAliasDataType(QXmlStreamReader &xmlReader)
{
    model::AliasDataType aliasDataType;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForNamedEntity(aliasDataType, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("BaseType")) {
            aliasDataType.setBaseType(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("DataScope")) {
            aliasDataType.setDataScope(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Description")) {
            aliasDataType.setDescription(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("HeaderFile")) {
            aliasDataType.setHeaderFile(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    xmlReader.skipCurrentElement();

    return aliasDataType;
}

model::BusDataType SimulinkXmlImporter::readBusDataType(QXmlStreamReader &xmlReader)
{
    model::BusDataType busDataType;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForNamedEntity(busDataType, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("DataScope")) {
            busDataType.setDataScope(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Description")) {
            busDataType.setDescription(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("HeaderFile")) {
            busDataType.setHeaderFile(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Alignment")) {
            busDataType.setAlignment(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }
    
    while(xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("BusMembers")) {
        readBusMembers(xmlReader, busDataType);
        } else {
            throw UnhandledElement(xmlReader.name(), "BusDataType");
        }
    }

    return busDataType;
}

void SimulinkXmlImporter::readBusMembers(QXmlStreamReader &xmlReader, model::BusDataType &busDataType)
{
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("BusMember")) {
            busDataType.addBusMember(readBusMember(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "BusMembers");
        }
    }
}

model::BusMember SimulinkXmlImporter::readBusMember(QXmlStreamReader &xmlReader)
{
    model::BusMember busMember;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForNamedEntity(busMember, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("DataType")) {
            busMember.setDataType(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Complexity")) {
            busMember.setComplexity(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Description")) {
            busMember.setDescription(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Dimensions")) {
            busMember.setDimensions(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("DimensionsMode")) {
            busMember.setDimensionsMode(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Max")) {
            busMember.setMax(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Min")) {
            busMember.setMin(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SampleTime")) {
            busMember.setSampleTime(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Unit")) {
            busMember.setUnit(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    xmlReader.skipCurrentElement();

    return busMember;
}

void SimulinkXmlImporter::readInports(QXmlStreamReader &xmlReader, model::ModelInterface &modelInterface)
{
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Inport")) {
            modelInterface.addInport(readInport(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Inports");
        }
    }
}

model::Inport SimulinkXmlImporter::readInport(QXmlStreamReader &xmlReader)
{
    model::Inport inport;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForNamedEntity(inport, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("OutDataTypeStr")) {
            inport.setOutDataTypeStr(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("BusObject")) {
            inport.setBusObject(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("BusOutputAsStruct")) {
            inport.setBusOutputAsStruct(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("IconDisplay")) {
            inport.setIconDisplay(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Interpolate")) {
            inport.setInterpolate(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("LatchByDelayingOutsideSignal")) {
            inport.setLatchByDelayingOutsideSignal(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("LatchInputForFeedbackSignals")) {
            inport.setLatchInputForFeedbackSignals(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("LockScale")) {
            inport.setLockScale(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutMax")) {
            inport.setOutMax(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutMin")) {
            inport.setOutMin(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutputSignalNames")) {
            inport.setOutputSignalNames(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Port")) {
            inport.setPort(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("PortDimensions")) {
            inport.setPortDimensions(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SampleTime")) {
            inport.setSampleTime(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SignalType")) {
            inport.setSignalType(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Unit")) {
            inport.setUnit(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("UnitNoProp")) {
            inport.setUnitNoProp(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("UseBusObject")) {
            inport.setUseBusObject(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    xmlReader.skipCurrentElement();

    return inport;
}

void SimulinkXmlImporter::readOutports(QXmlStreamReader &xmlReader, model::ModelInterface &modelInterface)
{
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Outport")) {
            modelInterface.addOutport(readOutport(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Outports");
        }
    }
}

model::Outport SimulinkXmlImporter::readOutport(QXmlStreamReader &xmlReader)
{
    model::Outport outport;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForNamedEntity(outport, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("OutDataTypeStr")) {
            outport.setOutDataTypeStr(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("BusObject")) {
            outport.setBusObject(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("BusOutputAsStruct")) {
            outport.setBusOutputAsStruct(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("IconDisplay")) {
            outport.setIconDisplay(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("InitialOutput")) {
            outport.setInitialOutput(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("InputSignalNames")) {
            outport.setInputSignalNames(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("LockScale")) {
            outport.setLockScale(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("MustResolveToSignalObject")) {
            outport.setMustResolveToSignalObject(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutMax")) {
            outport.setOutMax(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutMin")) {
            outport.setOutMin(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutputWhenDisabled")) {
            outport.setOutputWhenDisabled(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Port")) {
            outport.setPort(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("PortDimensions")) {
            outport.setPortDimensions(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SampleTime")) {
            outport.setSampleTime(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SignalName")) {
            outport.setSignalName(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SignalObject")) {
            outport.setSignalObject(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SignalType")) {
            outport.setSignalType(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("StorageClass")) {
            outport.setStorageClass(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Unit")) {
            outport.setUnit(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("UnitNoProp")) {
            outport.setUnitNoProp(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("UseBusObject")) {
            outport.setUseBusObject(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("VarSizeSig")) {
            outport.setVarSizeSig(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    xmlReader.skipCurrentElement();

    return outport;
}

} // namespace simulink::importer
