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

#include "specialized/coreparser.h"

#include "exceptions.h"
#include "specialized/datatypesparser.h"

#include <QVariant>
#include <QXmlStreamReader>
#include <seds/SedsModel/base/description.h>
#include <seds/SedsModel/base/namedentity.h>

namespace seds::parser {

common::String CoreParser::readLongDescription(QXmlStreamReader &xmlReader)
{
    return xmlReader.readElementText();
}

bool CoreParser::parseBool(QStringRef valueStr)
{
    return QVariant(valueStr.toString()).toBool();
}

uint8_t CoreParser::parseUInt8(QStringRef valueStr)
{
    bool ok = false;
    const auto value = valueStr.toShort(&ok);

    if (ok) {
        return value;
    }

    throw ParserException(QString("Unable to parse uint8 '%1'").arg(valueStr));
}

int32_t CoreParser::parseInt32(QStringRef valueStr)
{
    bool ok = false;
    const auto value = valueStr.toInt(&ok);

    if (ok) {
        return value;
    }

    throw ParserException(QString("Unable to parse int32 '%1'").arg(valueStr));
}

int64_t CoreParser::parseInt64(QStringRef valueStr)
{
    bool ok = false;
    const auto value = valueStr.toLong(&ok);

    if (ok) {
        return value;
    }

    throw ParserException(QString("Unable to parse int64 '%1'").arg(valueStr));
}

uint64_t CoreParser::parseUInt64(QStringRef valueStr)
{
    bool ok = false;
    const auto value = valueStr.toULong(&ok);

    if (ok) {
        return value;
    }

    throw ParserException(QString("Unable to parse uint64 '%1'").arg(valueStr));
}

double CoreParser::parseDouble(QStringRef valueStr)
{
    bool ok = false;
    const auto value = valueStr.toDouble(&ok);

    if (ok) {
        return value;
    }

    throw ParserException(QString("Unable to parse double '%1'").arg(valueStr));
}

model::PositiveLong::Value CoreParser::parsePositiveLong(QStringRef valueStr)
{
    bool ok = false;
    const auto value = valueStr.toULong(&ok);

    if (ok) {
        return value;
    }

    throw ParserException(QString("Unable to parse positive long '%1'").arg(valueStr));
}

bool CoreParser::processForDescription(model::Description *object, const QXmlStreamAttribute &attribute)
{
    if (attribute.name() == QStringLiteral("shortDescription")) {
        object->setShortDescription(attribute.value().toString());
    } else {
        return false;
    }

    return true;
}

bool CoreParser::processForDescription(model::Description *object, QXmlStreamReader &xmlReader)
{
    if (xmlReader.name() == QStringLiteral("LongDescription")) {
        object->setLongDescription(readLongDescription(xmlReader));
    } else {
        return false;
    }

    return true;
}

bool CoreParser::processForNamedEntity(model::NamedEntity *object, const QXmlStreamAttribute &attribute)
{
    if (processForDescription(object, attribute)) {
        return true;
    } else if (attribute.name() == QStringLiteral("name")) {
        object->setName(attribute.value().toString());
    } else {
        return false;
    }

    return true;
}

bool CoreParser::processForNamedEntity(model::NamedEntity *object, QXmlStreamReader &xmlReader)
{
    return processForDescription(object, xmlReader);
}

} // namespace seds::parser
