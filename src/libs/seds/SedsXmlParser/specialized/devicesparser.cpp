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

#include "specialized/devicesparser.h"

#include "device/device.h"
#include "device/metadata/datemetadatavalue.h"
#include "device/metadata/floatmetadatavalue.h"
#include "device/metadata/integermetadatavalue.h"
#include "device/metadata/metadata.h"
#include "device/metadata/metadatacategory.h"
#include "device/metadata/metadatavalue.h"
#include "device/metadata/stringmetadatavalue.h"
#include "exceptions.h"
#include "specialized/coreparser.h"

#include <QXmlStreamReader>

using std::placeholders::_1;

namespace seds::parser {

model::Device DevicesParser::readDevice(QXmlStreamReader &xmlReader)
{
    model::Device device;

    for (const auto &attribute : xmlReader.attributes()) {
        if (CoreParser::processForNamedEntity(&device, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (CoreParser::processForNamedEntity(&device, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("Metadata")) {
            device.setMetadata(readMetadata(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Device");
        }
    }

    return device;
}

model::Metadata DevicesParser::readMetadata(QXmlStreamReader &xmlReader)
{
    model::Metadata metadata;

    for (const auto &attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("MetadataValueSet")) {
            readMetadataValueSet(std::bind(&model::Metadata::addValue, &metadata, _1), xmlReader);
        } else if (xmlReader.name() == QStringLiteral("Category")) {
            metadata.addCategory(readMetadataCategory(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Metadata");
        }
    }

    return metadata;
}

void DevicesParser::readMetadataValueSet(
        const DevicesParser::MetadataValueAddingFunction &addValue, QXmlStreamReader &xmlReader)
{
    for (const auto &attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("DateValue")) {
            addValue(readDateMetadataValue(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("FloatValue")) {
            addValue(readFloatMetadataValue(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("IntegerValue")) {
            addValue(readIntegerMetadataValue(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("StringValue")) {
            addValue(readStringMetadataValue(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Metadata");
        }
    }
}

model::DateMetadataValue DevicesParser::readDateMetadataValue(QXmlStreamReader &xmlReader)
{
    model::DateMetadataValue dateMetadata;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForMetadataValue(&dateMetadata, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("value")) {
            dateMetadata.setValue(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForMetadataValue(&dateMetadata, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "DateMetadataValue");
        }
    }

    return dateMetadata;
}

model::FloatMetadataValue DevicesParser::readFloatMetadataValue(QXmlStreamReader &xmlReader)
{
    model::FloatMetadataValue dateMetadata;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForMetadataValue(&dateMetadata, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("value")) {
            dateMetadata.setValue(CoreParser::parseDouble(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForMetadataValue(&dateMetadata, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "FloatMetadataValue");
        }
    }

    return dateMetadata;
}

model::IntegerMetadataValue DevicesParser::readIntegerMetadataValue(QXmlStreamReader &xmlReader)
{
    model::IntegerMetadataValue dateMetadata;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForMetadataValue(&dateMetadata, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("value")) {
            dateMetadata.setValue(CoreParser::parseInt64(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForMetadataValue(&dateMetadata, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "IntegerMetadataValue");
        }
    }

    return dateMetadata;
}

model::StringMetadataValue DevicesParser::readStringMetadataValue(QXmlStreamReader &xmlReader)
{
    model::StringMetadataValue dateMetadata;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForMetadataValue(&dateMetadata, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("value")) {
            dateMetadata.setValue(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForMetadataValue(&dateMetadata, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "StringMetadataValue");
        }
    }

    return dateMetadata;
}

model::MetadataCategory DevicesParser::readMetadataCategory(QXmlStreamReader &xmlReader)
{
    model::MetadataCategory category;

    for (const auto &attribute : xmlReader.attributes()) {
        if (CoreParser::processForNamedEntity(&category, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (CoreParser::processForNamedEntity(&category, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("MetadataValueSet")) {
            readMetadataValueSet(std::bind(&model::MetadataCategory::addValue, &category, _1), xmlReader);
        } else if (xmlReader.name() == QStringLiteral("Category")) {
            category.addSubcategory(readMetadataCategory(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "MetadataCategory");
        }
    }

    return category;
}

bool DevicesParser::processForMetadataValue(model::MetadataValue *object, const QXmlStreamAttribute &attribute)
{
    return CoreParser::processForNamedEntity(object, attribute);
}

bool DevicesParser::processForMetadataValue(model::MetadataValue *object, QXmlStreamReader &xmlReader)
{
    return CoreParser::processForNamedEntity(object, xmlReader);
}

} // namespace seds::parser
