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

#include "specialized/componentprimitivesparser.h"

#include "exceptions.h"
#include "specialized/componentactivitiesparser.h"
#include "specialized/componentsparser.h"
#include "specialized/coreparser.h"

#include <QXmlStreamReader>
#include <seds/SedsModel/components/primitives/commandrefattributes.h>
#include <seds/SedsModel/components/primitives/namedargumentvalue.h>
#include <seds/SedsModel/components/primitives/oncommandprimitive.h>
#include <seds/SedsModel/components/primitives/onparameterprimitive.h>
#include <seds/SedsModel/components/primitives/parameteroperation.h>
#include <seds/SedsModel/components/primitives/parameterrefattributes.h>
#include <seds/SedsModel/components/primitives/primitivesink.h>
#include <seds/SedsModel/components/primitives/primitivesource.h>
#include <seds/SedsModel/components/primitives/sendcommandprimitive.h>
#include <seds/SedsModel/components/primitives/sendparameterprimitive.h>
#include <seds/SedsModel/components/primitives/sinkargumentvalue.h>
#include <seds/SedsModel/components/primitives/timersink.h>
#include <seds/ThirdParty/magicenum.h>

namespace seds::parser {

model::NamedArgumentValue ComponentPrimitivesParser::readNamedArgumentValue(QXmlStreamReader &xmlReader)
{
    model::NamedArgumentValue argumentValue;

    for (const auto &attribute : xmlReader.attributes()) {
        if (ComponentActivitiesParser::processForOperand(&argumentValue, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("name")) {
            argumentValue.setName(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (ComponentActivitiesParser::processForOperand(&argumentValue, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "NamedArgumentValue");
        }
    }

    return argumentValue;
}

model::SendCommandPrimitive ComponentPrimitivesParser::readSendCommandPrimitive(QXmlStreamReader &xmlReader)
{
    model::SendCommandPrimitive primitive;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForPrimitiveSource(&primitive, attribute)) {
            continue;
        } else if (processForCommandRefAttributes(&primitive, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForPrimitiveSource(&primitive, xmlReader)) {
            continue;
        } else if (processForCommandRefAttributes(&primitive, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("ArgumentValue")) {
            primitive.addArgumentValue(readNamedArgumentValue(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "SendCommandPrimitive");
        }
    }

    return primitive;
}

model::SendParameterPrimitive ComponentPrimitivesParser::readSendParameterPrimitive(QXmlStreamReader &xmlReader)
{
    model::SendParameterPrimitive primitive;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForPrimitiveSource(&primitive, attribute)) {
            continue;
        } else if (processForParameterRefAttributes(&primitive, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("operation")) {
            primitive.setOperation(parseParameterOperation(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForPrimitiveSource(&primitive, xmlReader)) {
            continue;
        } else if (processForParameterRefAttributes(&primitive, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("ArgumentValue")) {
            primitive.setArgumentValue(ComponentActivitiesParser::readOperand(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "SendParameterPrimitive");
        }
    }

    return primitive;
}

model::OnCommandPrimitive ComponentPrimitivesParser::readOnCommandPrimitive(QXmlStreamReader &xmlReader)
{
    model::OnCommandPrimitive primitive;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForPrimitiveSink(&primitive, attribute)) {
            continue;
        } else if (processForCommandRefAttributes(&primitive, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForPrimitiveSink(&primitive, xmlReader)) {
            continue;
        } else if (processForCommandRefAttributes(&primitive, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("ArgumentValue")) {
            primitive.addArgumentValue(readSinkArgumentValue(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "OnCommandPrimitive");
        }
    }

    return primitive;
}

model::OnParameterPrimitive ComponentPrimitivesParser::readOnParameterPrimitive(QXmlStreamReader &xmlReader)
{
    model::OnParameterPrimitive primitive;

    for (const auto &attribute : xmlReader.attributes()) {
        if (processForPrimitiveSink(&primitive, attribute)) {
            continue;
        } else if (processForParameterRefAttributes(&primitive, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("operation")) {
            primitive.setOperation(parseParameterOperation(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (processForPrimitiveSink(&primitive, xmlReader)) {
            continue;
        } else if (processForParameterRefAttributes(&primitive, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("VariableRef")) {
            primitive.setVariableRef(ComponentsParser::readVariableRef(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "OnParameterPrimitive");
        }
    }

    return primitive;
}

model::TimerSink ComponentPrimitivesParser::readTimerSink(QXmlStreamReader &xmlReader)
{
    model::TimerSink timerSink;

    for (const auto &attribute : xmlReader.attributes()) {
        if (attribute.name() == QStringLiteral("nanosecondsAfterEntry")) {
            timerSink.setNanosecondsAfterEntry(CoreParser::parseUInt64(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        throw UnhandledElement(xmlReader.name(), "TimerSink");
    }

    return timerSink;
}

bool ComponentPrimitivesParser::processForParameterRefAttributes(
        model::ParameterRefAttributes *object, const QXmlStreamAttribute &attribute)
{
    if (attribute.name() == QStringLiteral("interface")) {
        object->setInterface(attribute.value().toString());
    } else if (attribute.name() == QStringLiteral("parameter")) {
        object->setParameter(attribute.value().toString());
    } else {
        return false;
    }

    return true;
}

bool ComponentPrimitivesParser::processForParameterRefAttributes(
        model::ParameterRefAttributes *object, QXmlStreamReader &xmlReader)
{
    (void)object;
    (void)xmlReader;

    return false;
}

model::SinkArgumentValue ComponentPrimitivesParser::readSinkArgumentValue(QXmlStreamReader &xmlReader)
{
    model::SinkArgumentValue value;

    for (const auto &attribute : xmlReader.attributes()) {
        if (attribute.name() == QStringLiteral("name")) {
            value.setName(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("outputVariableRef")) {
            value.setOutputVariableRef(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        throw UnhandledElement(xmlReader.name(), "SinkArgumentValue");
    }

    return value;
}

model::ParameterOperation ComponentPrimitivesParser::parseParameterOperation(QStringRef valueStr)
{
    auto parameterOperationStdStr = valueStr.toString().toStdString();
    std::transform(parameterOperationStdStr.begin(), parameterOperationStdStr.end(), parameterOperationStdStr.begin(),
            ::toupper);

    auto parameterOperation = magic_enum::enum_cast<model::ParameterOperation>(parameterOperationStdStr);

    if (parameterOperation) {
        return *parameterOperation;
    }

    throw ParserException(QString("Unable to parse parameter operation '%1'").arg(valueStr));
}

bool ComponentPrimitivesParser::processForCommandRefAttributes(
        model::CommandRefAttributes *object, const QXmlStreamAttribute &attribute)
{
    if (attribute.name() == QStringLiteral("interface")) {
        object->setInterface(attribute.value().toString());
    } else if (attribute.name() == QStringLiteral("command")) {
        object->setCommand(attribute.value().toString());
    } else {
        return false;
    }

    return true;
}

bool ComponentPrimitivesParser::processForCommandRefAttributes(
        model::CommandRefAttributes *object, QXmlStreamReader &xmlReader)
{
    (void)object;
    (void)xmlReader;

    return false;
}

bool ComponentPrimitivesParser::processForPrimitiveSink(
        model::PrimitiveSink *object, const QXmlStreamAttribute &attribute)
{
    if (attribute.name() == QStringLiteral("transaction")) {
        object->setTransaction(attribute.value().toString());
    } else if (attribute.name() == QStringLiteral("failed")) {
        object->setFailed(CoreParser::parseBool(attribute.value()));
    } else {
        return false;
    }

    return true;
}

bool ComponentPrimitivesParser::processForPrimitiveSink(model::PrimitiveSink *object, QXmlStreamReader &xmlReader)
{
    (void)object;
    (void)xmlReader;

    return false;
}

bool ComponentPrimitivesParser::processForPrimitiveSource(
        model::PrimitiveSource *object, const QXmlStreamAttribute &attribute)
{
    if (ComponentActivitiesParser::processForStatement(object, attribute)) {
        return true;
    } else if (attribute.name() == QStringLiteral("transaction")) {
        object->setTransaction(attribute.value().toString());
    } else if (attribute.name() == QStringLiteral("failed")) {
        object->setFailed(CoreParser::parseBool(attribute.value()));
    } else {
        return false;
    }

    return true;
}

bool ComponentPrimitivesParser::processForPrimitiveSource(model::PrimitiveSource *object, QXmlStreamReader &xmlReader)
{
    if (ComponentActivitiesParser::processForStatement(object, xmlReader)) {
        return true;
    } else {
        return false;
    }
}

} // namespace seds::parser
