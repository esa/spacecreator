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

#include "specialized/interfacesparser.h"

#include "exceptions.h"
#include "specialized/componentsparser.h"
#include "specialized/coreparser.h"
#include "specialized/datatypesparser.h"
#include "specialized/genericsparser.h"

#include <QXmlStreamReader>
#include <seds/SedsModel/interfaces/commandargument.h>
#include <seds/SedsModel/interfaces/commandargumentmode.h>
#include <seds/SedsModel/interfaces/interfacecommand.h>
#include <seds/SedsModel/interfaces/interfacecommandmode.h>
#include <seds/SedsModel/interfaces/interfacedeclaration.h>
#include <seds/SedsModel/interfaces/interfacedeclarationref.h>
#include <seds/SedsModel/interfaces/interfaceparameter.h>
#include <seds/SedsModel/interfaces/interfaceparametermode.h>
#include <seds/SedsModel/interfaces/interfaceref.h>
#include <seds/SedsModel/package/package.h>

namespace seds::parser {

void InterfacesParser::readDeclaredInterfaceSet(
        const InterfacesParser::DeclaredInterfaceAddingFunction &addInterfaceDeclaration, QXmlStreamReader &xmlReader)
{
    for (const auto &attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Interface")) {
            addInterfaceDeclaration(readInterfaceDeclaration(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "DeclaredInterfaceSet");
        }
    }
}

model::InterfaceDeclaration InterfacesParser::readInterfaceDeclaration(QXmlStreamReader &xmlReader)
{
    model::InterfaceDeclaration interfaceDeclaration;

    for (const auto &attribute : xmlReader.attributes()) {
        if (CoreParser::processForNamedEntity(&interfaceDeclaration, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("abstract")) {
            interfaceDeclaration.setAbstract(CoreParser::parseBool(attribute.value()));
        } else if (attribute.name() == QStringLiteral("level")) {
            interfaceDeclaration.setLevel(parseInterfaceLevel(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (CoreParser::processForNamedEntity(&interfaceDeclaration, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("BaseInterfaceSet")) {
            readInterfaceRefSet(interfaceDeclaration, xmlReader);
        } else if (xmlReader.name() == QStringLiteral("GenericTypeSet")) {
            GenericsParser::readGenericTypeSet(interfaceDeclaration, xmlReader);
        } else if (xmlReader.name() == QStringLiteral("ParameterSet")) {
            readParameterSet(interfaceDeclaration, xmlReader);
        } else if (xmlReader.name() == QStringLiteral("CommandSet")) {
            readCommandSet(interfaceDeclaration, xmlReader);
        } else {
            throw UnhandledElement(xmlReader.name(), "InterfaceDeclaration");
        }
    }

    return interfaceDeclaration;
}

void InterfacesParser::readInterfaceRefSet(
        model::InterfaceDeclaration &interfaceDeclaration, QXmlStreamReader &xmlReader)
{
    for (const auto &attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Interface")) {
            interfaceDeclaration.addBaseInterface(readInterfaceRef(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "InterfaceRefSet");
        }
    }
}

model::InterfaceRef InterfacesParser::readInterfaceRef(QXmlStreamReader &xmlReader)
{
    model::InterfaceRef interfaceRef;

    for (const auto &attribute : xmlReader.attributes()) {
        if (attribute.name() == QStringLiteral("type")) {
            interfaceRef.setType(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("GenericTypeMapSet")) {
            interfaceRef.setGenericTypeMapSet(GenericsParser::readGenericTypeMapSet(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "InterfaceRef");
        }
    }

    return interfaceRef;
}

void InterfacesParser::readCommandSet(model::InterfaceDeclaration &interfaceDeclaration, QXmlStreamReader &xmlReader)
{
    for (const auto &attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Command")) {
            interfaceDeclaration.addCommand(readInterfaceCommand(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "CommandSet");
        }
    }
}

model::InterfaceCommand InterfacesParser::readInterfaceCommand(QXmlStreamReader &xmlReader)
{
    model::InterfaceCommand command;

    for (const auto &attribute : xmlReader.attributes()) {
        if (CoreParser::processForNamedEntity(&command, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("mode")) {
            command.setMode(parseInterfaceCommandMode(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (CoreParser::processForNamedEntity(&command, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("Argument")) {
            command.addArgument(readCommandArgument(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "InterfaceCommand");
        }
    }

    return command;
}

model::CommandArgument InterfacesParser::readCommandArgument(QXmlStreamReader &xmlReader)
{
    model::CommandArgument argument;

    for (const auto &attribute : xmlReader.attributes()) {
        if (DataTypesParser::processForExternalField(&argument, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("mode")) {
            argument.setMode(parseCommandArgumentMode(attribute.value()));
        } else if (attribute.name() == QStringLiteral("defaultValue")) {
            argument.setDefaultValue(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("dataUnit")) {
            argument.setDataUnit(CoreParser::parseBool(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (DataTypesParser::processForExternalField(&argument, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "CommandArgument");
        }
    }

    return argument;
}

void InterfacesParser::readParameterSet(model::InterfaceDeclaration &interfaceDeclaration, QXmlStreamReader &xmlReader)
{
    for (const auto &attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Parameter")) {
            interfaceDeclaration.addParameter(readInterfaceParameter(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "ParameterSet");
        }
    }
}

model::InterfaceParameter InterfacesParser::readInterfaceParameter(QXmlStreamReader &xmlReader)
{
    model::InterfaceParameter parameter;

    for (const auto &attribute : xmlReader.attributes()) {
        if (DataTypesParser::processForExternalField(&parameter, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("mode")) {
            parameter.setMode(parseInterfaceParameterMode(attribute.value()));
        } else if (attribute.name() == QStringLiteral("readOnly")) {
            parameter.setReadOnly(CoreParser::parseBool(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (DataTypesParser::processForExternalField(&parameter, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "InterfaceParameter");
        }
    }

    return parameter;
}

model::InterfaceCommandMode InterfacesParser::parseInterfaceCommandMode(const QStringRef &commandModeStr)
{
    auto commandMode = model::enumFromString<model::InterfaceCommandMode>(commandModeStr);

    if (commandMode) {
        return *commandMode;
    } else {
        throw ParserException(QString("Unable to parse InterfaceCommandMode '%1'").arg(commandModeStr));
    }
}

model::CommandArgumentMode InterfacesParser::parseCommandArgumentMode(const QStringRef &commandArgumentModeStr)
{
    auto commandArgumentMode = model::enumFromString<model::CommandArgumentMode>(commandArgumentModeStr);

    if (commandArgumentMode) {
        return *commandArgumentMode;
    } else {
        throw ParserException(QString("Unable to parse CommandArgumentMode '%1'").arg(commandArgumentModeStr));
    }
}

model::InterfaceParameterMode InterfacesParser::parseInterfaceParameterMode(const QStringRef &parameterModeStr)
{
    auto parameterMode = model::enumFromString<model::InterfaceParameterMode>(parameterModeStr);

    if (parameterMode) {
        return *parameterMode;
    } else {
        throw ParserException(QString("Unable to parse InterfaceParameterMode '%1'").arg(parameterModeStr));
    }
}

model::InterfaceLevel InterfacesParser::parseInterfaceLevel(const QStringRef &interfaceLevelStr)
{
    auto interfaceLevel = model::enumFromString<model::InterfaceLevel>(interfaceLevelStr);

    if (interfaceLevel) {
        return *interfaceLevel;
    } else {
        throw ParserException(QString("Unable to parse InterfaceLevel '%1'").arg(interfaceLevelStr));
    }
}

} // namespace seds::parser
