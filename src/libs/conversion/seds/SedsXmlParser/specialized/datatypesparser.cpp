/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "specialized/datatypesparser.h"

#include "exceptions.h"
#include "specialized/componentactivitiesparser.h"
#include "specialized/coreparser.h"

#include <QXmlStreamReader>
#include <seds/SedsModel/package/package.h>
#include <seds/SedsModel/types/binarydatatype.h>
#include <seds/SedsModel/types/booleandatatype.h>
#include <seds/SedsModel/types/constraints/containertypeconstraint.h>
#include <seds/SedsModel/types/constraints/containervalueconstraint.h>
#include <seds/SedsModel/types/datatyperef.h>
#include <seds/SedsModel/types/dimensionsize.h>
#include <seds/SedsModel/types/encodings/booleandataencoding.h>
#include <seds/SedsModel/types/entries/entry.h>
#include <seds/SedsModel/types/entries/fixedvalueentry.h>
#include <seds/SedsModel/types/entries/lengthentry.h>
#include <seds/SedsModel/types/entries/listentry.h>
#include <seds/SedsModel/types/entries/paddingentry.h>
#include <seds/SedsModel/types/enumerateddatatype.h>
#include <seds/SedsModel/types/externalfield.h>
#include <seds/SedsModel/types/field.h>
#include <seds/SedsModel/types/ranges/minmaxrange.h>
#include <seds/SedsModel/types/rootdatatype.h>
#include <seds/SedsModel/types/stringdatatype.h>
#include <seds/SedsModel/types/subrangedatatype.h>
#include <seds/SedsModel/types/valueenumeration.h>
#include <seds/ThirdParty/magicenum.h>

using std::placeholders::_1;

namespace seds::parser {

void DataTypesParser::readDataTypeSet(DataTypesParser::DataTypeAddingFunction addDataType, QXmlStreamReader &xmlReader)
{
    for (const auto &attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("ArrayDataType")) {
            addDataType(readArrayDataType(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("BinaryDataType")) {
            addDataType(readBinaryDataType(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("BooleanDataType")) {
            addDataType(readBooleanDataType(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("ContainerDataType")) {
            addDataType(readContainerDataType(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("EnumeratedDataType")) {
            addDataType(readEnumeratedDataType(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("FloatDataType")) {
            addDataType(readFloatDataType(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("IntegerDataType")) {
            addDataType(readIntegerDataType(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("StringDataType")) {
            addDataType(readStringDataType(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("SubRangeDataType")) {
            addDataType(readSubRangeDataType(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "DataTypeSet");
        }
    }
}

bool DataTypesParser::processForExternalField(model::ExternalField *object, const QXmlStreamAttribute &attribute)
{
    if (processForField(object, attribute)) {
        return true;
    } else {
        return false;
    }

    return true;
}

bool DataTypesParser::processForExternalField(model::ExternalField *object, QXmlStreamReader &xmlReader)
{
    if (processForField(object, xmlReader)) {
        return true;
    } else if (xmlReader.name() == QStringLiteral("SplineCalibrator")) {
        object->setCalibrator(ComponentActivitiesParser::readSplineCalibrator(xmlReader));
    } else if (xmlReader.name() == QStringLiteral("Polynomial")) {
        object->setCalibrator(ComponentActivitiesParser::readPolynomial(xmlReader));
    } else {
        return false;
    }

    return true;
}

bool DataTypesParser::processForField(model::Field *object, const QXmlStreamAttribute &attribute)
{
    if (CoreParser::processForNamedEntity(object, attribute)) {
        return true;
    } else if (attribute.name() == QStringLiteral("type")) {
        object->setType(attribute.value().toString());
    } else {
        return false;
    }

    return true;
}

bool DataTypesParser::processForField(model::Field *object, QXmlStreamReader &xmlReader)
{
    if (CoreParser::processForNamedEntity(object, xmlReader)) {
        return true;
    } else if (xmlReader.name() == QStringLiteral("ValidRange")) {
        object->setValidRange(readDerivedTypeRange(xmlReader));
    } else if (xmlReader.name() == QStringLiteral("ArrayDimensions")) {
        readDimensionList(std::bind(&model::Field::addArrayDimension, object, _1), xmlReader);
    } else {
        return false;
    }

    return true;
}

model::ArrayDataType DataTypesParser::readArrayDataType(QXmlStreamReader &xmlReader)
{
    model::ArrayDataType dataType;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForRootDataType(&dataType, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("dataTypeRef")) {
            dataType.setType(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForRootDataType(&dataType, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("DimensionList")) {
            readDimensionList(std::bind(&model::ArrayDataType::addDimension, &dataType, _1), xmlReader);
        } else {
            throw UnhandledElement(xmlReader.name(), "ArrayDataType");
        }
    }

    return dataType;
}

model::BinaryDataType DataTypesParser::readBinaryDataType(QXmlStreamReader &xmlReader)
{
    model::BinaryDataType dataType;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForRootDataType(&dataType, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("sizeInBits")) {
            dataType.setBits(CoreParser::parsePositiveLong(attribute.value()));
        } else if (attribute.name() == QStringLiteral("fixedSize")) {
            dataType.setFixedSize(CoreParser::parseBool(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForRootDataType(&dataType, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "BinaryDataType");
        }
    }

    return dataType;
}

model::BooleanDataType DataTypesParser::readBooleanDataType(QXmlStreamReader &xmlReader)
{
    model::BooleanDataType dataType;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForRootDataType(&dataType, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForRootDataType(&dataType, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("BooleanDataEncoding")) {
            dataType.setEncoding(readBooleanDataEncoding(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "BooleanDataType");
        }
    }

    return dataType;
}

model::ContainerDataType DataTypesParser::readContainerDataType(QXmlStreamReader &xmlReader)
{
    model::ContainerDataType dataType;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForRootDataType(&dataType, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("abstract")) {
            dataType.setAbstract(CoreParser::parseBool(attribute.value()));
        } else if (attribute.name() == QStringLiteral("baseType")) {
            dataType.setBaseType(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForRootDataType(&dataType, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("ConstraintSet")) {
            readConstraintSet(dataType, xmlReader);
        } else if (xmlReader.name() == QStringLiteral("EntryList")) {
            readEntryList(std::bind(&model::ContainerDataType::addEntry, &dataType, _1), xmlReader);
        } else if (xmlReader.name() == QStringLiteral("TrailerEntryList")) {
            readEntryList(std::bind(&model::ContainerDataType::addTrailerEntry, &dataType, _1), xmlReader);
        } else {
            throw UnhandledElement(xmlReader.name(), "ContainerDataType");
        }
    }

    return dataType;
}

model::EnumeratedDataType DataTypesParser::readEnumeratedDataType(QXmlStreamReader &xmlReader)
{
    model::EnumeratedDataType dataType;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForRootDataType(&dataType, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForRootDataType(&dataType, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("IntegerDataEncoding")) {
            dataType.setEncoding(readIntegerDataEncoding(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("EnumerationList")) {
            readEnumerationList(dataType, xmlReader);
        } else {
            throw UnhandledElement(xmlReader.name(), "EnumeratedDataType");
        }
    }

    return dataType;
}

model::FloatDataType DataTypesParser::readFloatDataType(QXmlStreamReader &xmlReader)
{
    model::FloatDataType dataType;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForRootDataType(&dataType, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForRootDataType(&dataType, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("FloatDataEncoding")) {
            dataType.setEncoding(readFloatDataEncoding(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("Range")) {
            dataType.setRange(readFloatDataRange(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "FloatDataType");
        }
    }

    return dataType;
}

model::IntegerDataType DataTypesParser::readIntegerDataType(QXmlStreamReader &xmlReader)
{
    model::IntegerDataType dataType;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForRootDataType(&dataType, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForRootDataType(&dataType, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("IntegerDataEncoding")) {
            dataType.setEncoding(readIntegerDataEncoding(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("Range")) {
            dataType.setRange(readIntegerDataRange(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "IntegerDataType");
        }
    }

    return dataType;
}

model::StringDataType DataTypesParser::readStringDataType(QXmlStreamReader &xmlReader)
{
    model::StringDataType dataType;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForRootDataType(&dataType, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("length")) {
            dataType.setLength(CoreParser::parsePositiveLong(attribute.value()));
        } else if (attribute.name() == QStringLiteral("fixedLength")) {
            dataType.setFixedLength(CoreParser::parseBool(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForRootDataType(&dataType, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("StringDataEncoding")) {
            dataType.setEncoding(readStringDataEncoding(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "StringDataType");
        }
    }

    return dataType;
}

model::SubRangeDataType DataTypesParser::readSubRangeDataType(QXmlStreamReader &xmlReader)
{
    model::SubRangeDataType dataType;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForRootDataType(&dataType, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("baseType")) {
            dataType.setType(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForRootDataType(&dataType, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("Range")) {
            dataType.setRange(readDerivedTypeRange(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "SubRangeDataType");
        }
    }

    return dataType;
}

model::BooleanDataEncoding DataTypesParser::readBooleanDataEncoding(QXmlStreamReader &xmlReader)
{
    model::BooleanDataEncoding encoding;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForDataEncoding(&encoding, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("sizeInBits")) {
            encoding.setBits(CoreParser::parsePositiveLong(attribute.value()));
        } else if (attribute.name() == QStringLiteral("falseValue")) {
            encoding.setFalseValue(parseFalseValue(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForDataEncoding(&encoding, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "BooleanDataEncoding");
        }
    }

    return encoding;
}

model::FloatDataEncoding DataTypesParser::readFloatDataEncoding(QXmlStreamReader &xmlReader)
{
    model::FloatDataEncoding encoding;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForDataEncoding(&encoding, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("encodingAndPrecision")) {
            encoding.setEncoding(parseFloatEncoding(attribute.value()));
        } else if (attribute.name() == QStringLiteral("sizeInBits")) {
            encoding.setBits(CoreParser::parsePositiveLong(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForDataEncoding(&encoding, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "FloatDataEncoding");
        }
    }

    return encoding;
}

model::IntegerDataEncoding DataTypesParser::readIntegerDataEncoding(QXmlStreamReader &xmlReader)
{
    model::IntegerDataEncoding encoding;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForDataEncoding(&encoding, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("encoding")) {
            encoding.setEncoding(parseIntegerEncoding(attribute.value()));
        } else if (attribute.name() == QStringLiteral("sizeInBits")) {
            encoding.setBits(CoreParser::parsePositiveLong(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForDataEncoding(&encoding, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "IntegerDataEncoding");
        }
    }

    return encoding;
}

model::StringDataEncoding DataTypesParser::readStringDataEncoding(QXmlStreamReader &xmlReader)
{
    model::StringDataEncoding encoding;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForDataEncoding(&encoding, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("encoding")) {
            encoding.setEncoding(parseStringEncoding(attribute.value()));
        } else if (attribute.name() == QStringLiteral("terminationByte")) {
            encoding.setTerminationByte(CoreParser::parseUInt8(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForDataEncoding(&encoding, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "StringDataEncoding");
        }
    }

    return encoding;
}

model::FloatDataType::Range DataTypesParser::readFloatDataRange(QXmlStreamReader &xmlReader)
{
    xmlReader.readNextStartElement();

    if (xmlReader.name() == "MinMaxRange") {
        auto range = readMinMaxRange(xmlReader);
        xmlReader.skipCurrentElement();
        return range;
    } else if (xmlReader.name() == "PrecisionRange") {
        auto range = parseFloatPrecisionRange(xmlReader);
        xmlReader.skipCurrentElement();
        return range;
    } else {
        throw UnhandledElement(xmlReader.name(), "FloatDataRange");
    }
}

model::IntegerDataType::Range DataTypesParser::readIntegerDataRange(QXmlStreamReader &xmlReader)
{
    xmlReader.readNextStartElement();

    if (xmlReader.name() == "MinMaxRange") {
        auto range = readMinMaxRange(xmlReader);
        xmlReader.skipCurrentElement();
        return range;
    } else {
        throw UnhandledElement(xmlReader.name(), "IntegerDataRange");
    }
}

model::DerivedTypeRange DataTypesParser::readDerivedTypeRange(QXmlStreamReader &xmlReader)
{
    xmlReader.readNextStartElement();

    if (xmlReader.name() == "MinMaxRange") {
        auto range = readMinMaxRange(xmlReader);
        xmlReader.skipCurrentElement();
        return range;
    } else {
        throw UnhandledElement(xmlReader.name(), "DerivedTypeRange");
    }
}

model::EnumeratedDataTypeRange DataTypesParser::readEnumeratedDataTypeRange(QXmlStreamReader &xmlReader)
{
    model::EnumeratedDataTypeRange range;

    for (const auto &attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Label")) {
            range.addItem(xmlReader.readElementText());
        } else {
            throw UnhandledElement(xmlReader.name(), "EnumeratedDataTypeRange");
        }
    }

    return range;
}

model::MinMaxRange DataTypesParser::readMinMaxRange(QXmlStreamReader &xmlReader)
{
    model::MinMaxRange range;

    for (const auto &attribute : xmlReader.attributes()) {
        if (attribute.name() == QStringLiteral("rangeType")) {
            range.setType(parseRangeType(attribute.value()));
        } else if (attribute.name() == QStringLiteral("min")) {
            range.setMin(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("max")) {
            range.setMax(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        throw UnhandledElement(xmlReader.name(), "MinMaxRange");
    }

    return range;
}

void DataTypesParser::readEntryList(DataTypesParser::EntryAddingFunction addEntry, QXmlStreamReader &xmlReader)
{
    for (const auto &attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Entry")) {
            addEntry(readEntry(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("ErrorControlEntry")) {
            addEntry(readErrorControlEntry(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("FixedValueEntry")) {
            addEntry(readFixedValueEntry(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("LengthEntry")) {
            addEntry(readLengthEntry(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("ListEntry")) {
            addEntry(readListEntry(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("PaddingEntry")) {
            addEntry(readPaddingEntry(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "EntryList");
        }
    }
}

model::Entry DataTypesParser::readEntry(QXmlStreamReader &xmlReader)
{
    model::Entry entry;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForEntry(&entry, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForEntry(&entry, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "Entry");
        }
    }

    return entry;
}

model::ErrorControlEntry DataTypesParser::readErrorControlEntry(QXmlStreamReader &xmlReader)
{
    model::ErrorControlEntry entry;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForEntry(&entry, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("errorControlType")) {
            entry.setErrorControl(parseErrorControl(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForEntry(&entry, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "ErrorControlEntry");
        }
    }

    return entry;
}

model::FixedValueEntry DataTypesParser::readFixedValueEntry(QXmlStreamReader &xmlReader)
{
    model::FixedValueEntry entry;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForEntry(&entry, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("fixedValue")) {
            entry.setFixedValue(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForEntry(&entry, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "FixedValueEntry");
        }
    }

    return entry;
}

model::LengthEntry DataTypesParser::readLengthEntry(QXmlStreamReader &xmlReader)
{
    model::LengthEntry entry;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForEntry(&entry, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForEntry(&entry, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "LengthEntry");
        }
    }

    return entry;
}

model::ListEntry DataTypesParser::readListEntry(QXmlStreamReader &xmlReader)
{
    model::ListEntry entry;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForEntry(&entry, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("listLengthField")) {
            entry.setListLengthField(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForEntry(&entry, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "ListEntry");
        }
    }

    return entry;
}

model::PaddingEntry DataTypesParser::readPaddingEntry(QXmlStreamReader &xmlReader)
{
    model::PaddingEntry entry;

    for (const auto &attribute : xmlReader.attributes()) {
        if (CoreParser::processForDescription(&entry, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("sizeInBits")) {
            entry.setSizeInBits(CoreParser::parsePositiveLong(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (CoreParser::processForDescription(&entry, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "PaddingEntry");
        }
    }

    return entry;
}

void DataTypesParser::readEnumerationList(model::EnumeratedDataType &dataType, QXmlStreamReader &xmlReader)
{
    for (const auto &attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Enumeration")) {
            dataType.addEnumeration(readEnumeration(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "EnumerationList");
        }
    }
}

model::ValueEnumeration DataTypesParser::readEnumeration(QXmlStreamReader &xmlReader)
{
    model::ValueEnumeration enumeration;

    for (const auto &attribute : xmlReader.attributes()) {
        if (CoreParser::processForDescription(&enumeration, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("value")) {
            enumeration.setValue(CoreParser::parseInt32(attribute.value()));
        } else if (attribute.name() == QStringLiteral("label")) {
            enumeration.setLabel(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (CoreParser::processForDescription(&enumeration, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "Enumeration");
        }
    }

    return enumeration;
}

void DataTypesParser::readDimensionList(
        DataTypesParser::DimensionAddingFunction addDimension, QXmlStreamReader &xmlReader)
{
    for (const auto &attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Dimension")) {
            addDimension(readDimensionSize(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "ArrayDimensions");
        }
    }
}

model::DimensionSize DataTypesParser::readDimensionSize(QXmlStreamReader &xmlReader)
{
    model::DimensionSize dimensionSize;

    for (const auto &attribute : xmlReader.attributes()) {
        if (attribute.name() == QStringLiteral("size")) {
            dimensionSize.setSize(CoreParser::parsePositiveLong(attribute.value()));
        } else if (attribute.name() == QStringLiteral("indexTypeRef")) {
            dimensionSize.setIndexTypeRef(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        throw UnhandledElement(xmlReader.name(), "DimensionSize");
    }

    return dimensionSize;
}

void DataTypesParser::readConstraintSet(model::ContainerDataType &dataType, QXmlStreamReader &xmlReader)
{
    for (const auto &attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("RangeConstraint")) {
            dataType.addConstraint(readRangeConstraint(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("TypeConstraint")) {
            dataType.addConstraint(readTypeConstraint(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("ValueConstraint")) {
            dataType.addConstraint(readValueConstraint(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "ConstraintSet");
        }
    }
}

model::ContainerRangeConstraint DataTypesParser::readRangeConstraint(QXmlStreamReader &xmlReader)
{
    model::ContainerRangeConstraint constraint;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForContainerConstraint(&constraint, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForContainerConstraint(&constraint, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("MinMaxRange")) {
            constraint.setRange(readMinMaxRange(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("EnumeratedRange")) {
            constraint.setRange(readEnumeratedDataTypeRange(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "RangeConstraint");
        }
    }

    return constraint;
}

model::ContainerTypeConstraint DataTypesParser::readTypeConstraint(QXmlStreamReader &xmlReader)
{
    model::ContainerTypeConstraint constraint;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForContainerConstraint(&constraint, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("type")) {
            constraint.setType(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForContainerConstraint(&constraint, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "TypeConstraint");
        }
    }

    return constraint;
}

model::ContainerValueConstraint DataTypesParser::readValueConstraint(QXmlStreamReader &xmlReader)
{
    model::ContainerValueConstraint constraint;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForContainerConstraint(&constraint, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("value")) {
            constraint.setValue(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForContainerConstraint(&constraint, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "ValueConstraint");
        }
    }

    return constraint;
}

model::FloatDataEncoding::Encoding DataTypesParser::parseFloatEncoding(QStringRef encodingStr)
{
    auto encodingStdStr = encodingStr.toString().toStdString();
    std::transform(encodingStdStr.begin(), encodingStdStr.end(), encodingStdStr.begin(), ::toupper);

    auto coreFloatEncoding = magic_enum::enum_cast<model::CoreEncodingAndPrecision>(encodingStdStr);

    if (coreFloatEncoding) {
        return *coreFloatEncoding;
    }

    throw ParserException(QString("Unable to parse float encoding '%1'").arg(encodingStr));
}

model::IntegerDataEncoding::Encoding DataTypesParser::parseIntegerEncoding(QStringRef encodingStr)
{
    auto encodingStdStr = encodingStr.toString().toStdString();
    std::transform(encodingStdStr.begin(), encodingStdStr.end(), encodingStdStr.begin(), ::toupper);

    auto coreIntegerEncoding = magic_enum::enum_cast<model::CoreIntegerEncoding>(encodingStdStr);

    if (coreIntegerEncoding) {
        return *coreIntegerEncoding;
    }

    throw ParserException(QString("Unable to parse integer encoding '%1'").arg(encodingStr));
}

model::StringDataEncoding::Encoding DataTypesParser::parseStringEncoding(QStringRef encodingStr)
{
    auto encodingStdStr = encodingStr.toString().toStdString();
    std::transform(encodingStdStr.begin(), encodingStdStr.end(), encodingStdStr.begin(), ::toupper);
    encodingStdStr.erase(std::remove(encodingStdStr.begin(), encodingStdStr.end(), '-'), encodingStdStr.end());

    auto coreStringEncoding = magic_enum::enum_cast<model::CoreStringEncoding>(encodingStdStr);

    if (coreStringEncoding) {
        return *coreStringEncoding;
    }

    throw ParserException(QString("Unable to parse string encoding '%1'").arg(encodingStr));
}

model::FloatPrecisionRange DataTypesParser::parseFloatPrecisionRange(QXmlStreamReader &xmlReader)
{
    auto rangeStr = xmlReader.readElementText();

    auto rangeStdStr = rangeStr.toStdString();
    std::transform(rangeStdStr.begin(), rangeStdStr.end(), rangeStdStr.begin(), ::toupper);

    auto range = magic_enum::enum_cast<model::FloatPrecisionRange>(rangeStdStr);

    if (range) {
        return *range;
    }

    throw ParserException(QString("Unable to parse float precision range '%1'").arg(rangeStr));
}

model::ByteOrder DataTypesParser::parseByteOrder(QStringRef byteOrderStr)
{
    auto byteOrderStdStr = byteOrderStr.toString().toStdString();
    std::transform(byteOrderStdStr.begin(), byteOrderStdStr.end(), byteOrderStdStr.begin(), ::toupper);

    auto byteOrder = magic_enum::enum_cast<model::ByteOrder>(byteOrderStdStr);

    if (byteOrder) {
        return *byteOrder;
    }

    throw ParserException(QString("Unable to parse byte order '%1'").arg(byteOrderStr));
}

model::ErrorControlEntry::ErrorControl DataTypesParser::parseErrorControl(QStringRef errorControlStr)
{
    auto errorControlStdStr = errorControlStr.toString().toStdString();
    std::transform(errorControlStdStr.begin(), errorControlStdStr.end(), errorControlStdStr.begin(), ::toupper);

    auto coreErrorControl = magic_enum::enum_cast<model::CoreErrorControl>(errorControlStdStr);

    if (coreErrorControl) {
        return *coreErrorControl;
    }

    throw ParserException(QString("Unable to parse error control '%1'").arg(errorControlStr));
}

model::FalseValue DataTypesParser::parseFalseValue(QStringRef falseValueStr)
{
    auto falseValueStdStr = falseValueStr.toString().toStdString();
    std::transform(falseValueStdStr.begin(), falseValueStdStr.end(), falseValueStdStr.begin(), ::toupper);

    auto falseValue = magic_enum::enum_cast<model::FalseValue>(falseValueStdStr);

    if (falseValue) {
        return *falseValue;
    }

    throw ParserException(QString("Unable to parse false value '%1'").arg(falseValueStr));
}

model::RangeType DataTypesParser::parseRangeType(QStringRef rangeTypeStr)
{
    auto rangeTypeStdStr = rangeTypeStr.toString().toStdString();
    std::transform(rangeTypeStdStr.begin(), rangeTypeStdStr.end(), rangeTypeStdStr.begin(), ::toupper);

    auto rangeType = magic_enum::enum_cast<model::RangeType>(rangeTypeStdStr);

    if (rangeType) {
        return *rangeType;
    }

    throw ParserException(QString("Unable to parse range type '%1'").arg(rangeTypeStr));
}

bool DataTypesParser::processForContainerConstraint(
        model::ContainerConstraint *object, const QXmlStreamAttribute &attribute)
{
    if (CoreParser::processForDescription(object, attribute)) {
        return true;
    } else if (attribute.name() == QStringLiteral("entry")) {
        object->setEntry(attribute.value().toString());
    } else {
        return false;
    }

    return true;
}

bool DataTypesParser::processForContainerConstraint(model::ContainerConstraint *object, QXmlStreamReader &xmlReader)
{
    return CoreParser::processForDescription(object, xmlReader);
}

bool DataTypesParser::processForDataEncoding(model::DataEncoding *object, const QXmlStreamAttribute &attribute)
{
    if (attribute.name() == QStringLiteral("byteOrder")) {
        object->setByteOrder(parseByteOrder(attribute.value()));
    } else {
        return false;
    }

    return true;
}

bool DataTypesParser::processForDataEncoding(model::DataEncoding *object, QXmlStreamReader &xmlReader)
{
    (void)object;
    (void)xmlReader;
    return false;
}

bool DataTypesParser::processForEntry(model::Entry *object, const QXmlStreamAttribute &attribute)
{
    return processForExternalField(object, attribute);
}

bool DataTypesParser::processForEntry(model::Entry *object, QXmlStreamReader &xmlReader)
{
    return processForExternalField(object, xmlReader);
}

bool DataTypesParser::processForRootDataType(model::RootDataType *object, const QXmlStreamAttribute &attribute)
{
    return CoreParser::processForNamedEntity(object, attribute);
}

bool DataTypesParser::processForRootDataType(model::RootDataType *object, QXmlStreamReader &xmlReader)
{
    return CoreParser::processForNamedEntity(object, xmlReader);
}

} // namespace seds::parser
