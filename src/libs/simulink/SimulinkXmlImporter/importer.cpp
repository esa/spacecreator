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

#include <QDebug>
#include <QFileInfo>
#include <QVariant>
#include <QXmlStreamReader>
#include <conversion/common/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <datatypes/aliasdatatype.h>
#include <datatypes/busdatatype.h>
#include <datatypes/enumdatatype.h>
#include <datatypes/enumvalue.h>
#include <datatypes/rootdatatype.h>
#include <simulink/SimulinkModel/simulinkmodel.h>
#include <simulink/SimulinkOptions/options.h>

using conversion::FileNotFoundException;
using conversion::Options;
using conversion::importer::ImportException;
using conversion::simulink::SimulinkOptions;
using simulink::model::Dimensions;
using simulink::model::VectorDimensions;

namespace {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
auto SkipEmptyParts = QString::SkipEmptyParts;
#else
auto SkipEmptyParts = Qt::SkipEmptyParts;
#endif
}
namespace simulink::importer {

std::unique_ptr<conversion::Model> SimulinkXmlImporter::importModel(const Options &options) const
{
    const auto inputFilePath = options.value(SimulinkOptions::inputFilepath);
    if (!inputFilePath) {
        throw ImportException("Simulink file to import wasn't specified");
    }

    const QFileInfo inputFileInfo(*inputFilePath);
    if (!inputFileInfo.exists()) {
        throw FileNotFoundException(*inputFilePath, "while importing");
    }

    auto model = parse(*inputFilePath);

    return model;
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
        auto simulinkModel = std::make_unique<model::SimulinkModel>();

        QFileInfo inputSimulinkFileInfo(inputSimulinkFile);
        simulinkModel->setName(inputSimulinkFileInfo.baseName());

        readSimulinkModel(xmlReader, simulinkModel);
        return simulinkModel;
    } else {
        throw UnhandledElement(xmlReader.name(), "Root");
    }
}

void SimulinkXmlImporter::readSimulinkModel(
        QXmlStreamReader &xmlReader, std::unique_ptr<model::SimulinkModel> &simulinkModel)
{
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("DataTypes")) {
            readDataTypes(xmlReader, simulinkModel);
        } else if (xmlReader.name() == QStringLiteral("Inports")) {
            readInports(xmlReader, simulinkModel);
        } else if (xmlReader.name() == QStringLiteral("Outports")) {
            readOutports(xmlReader, simulinkModel);
        } else {
            throw UnhandledElement(xmlReader.name(), "ModelInterface");
        }
    }
}

void SimulinkXmlImporter::readDataTypes(
        QXmlStreamReader &xmlReader, std::unique_ptr<model::SimulinkModel> &simulinkModel)
{
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("EnumDataType")) {
            simulinkModel->addDataType(readEnumDataType(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("AliasDataType")) {
            simulinkModel->addDataType(readAliasDataType(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("BusDataType")) {
            simulinkModel->addDataType(readBusDataType(xmlReader));
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
            enumDataType.setAddClassNameToEnumNames(parseBool(attribute.value()));
        } else if (attribute.name() == QStringLiteral("DataScope")) {
            enumDataType.setDataScope(parseDataScope(attribute.value()));
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
            enumValue.setValue(parseInt64(attribute.value()));
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
            aliasDataType.setDataScope(parseDataScope(attribute.value()));
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
            busDataType.setDataScope(parseDataScope(attribute.value()));
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

    while (xmlReader.readNextStartElement()) {
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
            busMember.setComplexity(parseComplexity(attribute.value()));
        } else if (attribute.name() == QStringLiteral("Description")) {
            busMember.setDescription(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Dimensions")) {
            busMember.setDimensions(parseDimensions(attribute.value()));
        } else if (attribute.name() == QStringLiteral("DimensionsMode")) {
            busMember.setDimensionsMode(parseDimensionsMode(attribute.value()));
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

void SimulinkXmlImporter::readInports(QXmlStreamReader &xmlReader, std::unique_ptr<model::SimulinkModel> &simulinkModel)
{
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Inport")) {
            simulinkModel->addInport(readInport(xmlReader));
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
            inport.setPortDimensions(parsePortDimension(attribute.value()));
        } else if (attribute.name() == QStringLiteral("SampleTime")) {
            inport.setSampleTime(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SignalType")) {
            inport.setSignalType(parseSignalType(attribute.value()));
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

void SimulinkXmlImporter::readOutports(
        QXmlStreamReader &xmlReader, std::unique_ptr<model::SimulinkModel> &simulinkModel)
{
    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Outport")) {
            simulinkModel->addOutport(readOutport(xmlReader));
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
            outport.setPortDimensions(parsePortDimension(attribute.value()));
        } else if (attribute.name() == QStringLiteral("SampleTime")) {
            outport.setSampleTime(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SignalName")) {
            outport.setSignalName(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SignalObject")) {
            outport.setSignalObject(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SignalType")) {
            outport.setSignalType(parseSignalType(attribute.value()));
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

bool SimulinkXmlImporter::parseBool(const StringRef &valueStr)
{
    return QVariant(valueStr.toString()).toBool();
}

int64_t SimulinkXmlImporter::parseInt64(const StringRef &valueStr)
{
    bool ok = false;
    const auto value = valueStr.toLong(&ok);

    if (ok) {
        return value;
    }

    throw ParserException(QString("Unable to parse int64 value '%1'").arg(valueStr));
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

model::DataScope SimulinkXmlImporter::parseDataScope(const StringRef &dataScopeStr)
{
    auto dataScope = model::enumFromString<model::DataScope>(dataScopeStr);

    if (dataScope) {
        return *dataScope;
    } else {
        throw ParserException(QString("Unable to parse data scope '%1'").arg(dataScopeStr));
    }
}

model::Complexity SimulinkXmlImporter::parseComplexity(const StringRef &complexityStr)
{
    auto complexity = model::enumFromString<model::Complexity>(complexityStr);

    if (complexity) {
        return *complexity;
    } else {
        throw ParserException(QString("Unable to parse complexity '%1'").arg(complexityStr));
    }
}

model::Dimensions SimulinkXmlImporter::parseDimensions(const StringRef &dimensionsStr)
{
    if (dimensionsStr.isEmpty())
        throw ParserException(QString("Dimensions is empty"));

    const auto trimmedStrRef = dimensionsStr.trimmed();

    const auto conversionNumberBase = 10;
    auto conversionSuccess = false;
    const auto scalarValue = trimmedStrRef.toLong(&conversionSuccess, conversionNumberBase);

    if (conversionSuccess) {
        if (scalarValue == -1 || scalarValue > 0)
            return Dimensions(scalarValue);
        else {
            throw ParserException(
                    QString("Value of dimension should be -1 or be greater than 0 '%1'").arg(scalarValue));
        }
    } else if ((trimmedStrRef.front() == '[') && (trimmedStrRef.back() == ']')) {
        const auto preparedStr = trimmedStrRef.toString()
                                         .remove(trimmedStrRef.size() - 1, 1) // removing ']'
                                         .remove(0, 1) // removing '['
                                         .replace(QChar(';'), QChar(' '), Qt::CaseSensitive); // replacing ';' to ' '

        const auto splitBySpacesStrList = preparedStr.split(' ', SkipEmptyParts, Qt::CaseSensitive);
        const auto splitBySpacesStrListSize = splitBySpacesStrList.size();

        if (splitBySpacesStrListSize == 0) {
            throw ParserException(QString("Dimensions Array is empty"));
        } else if (splitBySpacesStrListSize > 2) {
            throw ParserException(QString("Dimensions Array has '%1' dimension, but it should be max 2-dimensional")
                                          .arg(splitBySpacesStrListSize));
        } else {
            VectorDimensions vectorDimensions;

            for (const auto &dimensionValueStr : splitBySpacesStrList) {
                const auto convertedDimensionValue = dimensionValueStr.toLong(&conversionSuccess, conversionNumberBase);

                if (conversionSuccess) {
                    if (convertedDimensionValue > 0)
                        vectorDimensions.addValue(convertedDimensionValue);
                    else {
                        throw ParserException(QString("Value of dimension array should be greater than 0 '%1'")
                                                      .arg(convertedDimensionValue));
                    }
                } else if (dimensionValueStr.front().isLetter()) {
                    throw ParserException(QString("Importer doesn't handle variables '%1'").arg(dimensionValueStr));
                } else {
                    throw ParserException(QString("Unable to parse one of array value '%1'").arg(dimensionValueStr));
                }
            }

            return Dimensions(std::move(vectorDimensions));
        }
    } else if ((trimmedStrRef.front() == '{') && (trimmedStrRef.back() == '}')) {
        throw ParserException(QString("Importer doesn't handle cell array '%1'").arg(dimensionsStr));
    } else if (trimmedStrRef.front().isLetter()) {
        throw ParserException(QString("Importer doesn't handle variables '%1'").arg(dimensionsStr));
    } else if (trimmedStrRef.front().isDigit()) {
        throw ParserException(QString("Unable to parse dimension number '%1'").arg(dimensionsStr));
    } else {
        throw ParserException(QString("Unable to parse dimensions '%1'").arg(dimensionsStr));
    }
    return model::Dimensions();
}

model::DimensionsMode SimulinkXmlImporter::parseDimensionsMode(const StringRef &dimensionsModeStr)
{
    auto dimensionsMode = model::enumFromString<model::DimensionsMode>(dimensionsModeStr);

    if (dimensionsMode) {
        return *dimensionsMode;
    } else {
        throw ParserException(QString("Unable to parse dimensions mode '%1'").arg(dimensionsModeStr));
    }
}

model::PortDimension SimulinkXmlImporter::parsePortDimension(const StringRef &portDimensionStr)
{
    return parseDimensions(portDimensionStr);
}

model::SignalType SimulinkXmlImporter::parseSignalType(const StringRef &signalTypeStr)
{
    auto signalType = model::enumFromString<model::SignalType>(signalTypeStr);

    if (signalType) {
        return *signalType;
    } else {
        throw ParserException(QString("Unable to parse signal type '%1'").arg(signalTypeStr));
    }
}

} // namespace simulink::importer
