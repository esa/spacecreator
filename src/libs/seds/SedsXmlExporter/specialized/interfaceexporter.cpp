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

#include "specialized/interfaceexporter.h"

namespace seds::exporter {

void InterfaceExporter::exportInterface(
        const model::Interface &interface, QDomElement &setElement, QDomDocument &sedsDocument)
{
    auto interfaceElement = sedsDocument.createElement(QStringLiteral("Interface"));

    const auto &interfaceName = interface.nameStr();
    interfaceElement.setAttribute(QStringLiteral("name"), interfaceName);

    const auto &interfaceType = interface.type().nameStr();
    interfaceElement.setAttribute(QStringLiteral("type"), interfaceType);

    setElement.appendChild(std::move(interfaceElement));
}

void InterfaceExporter::exportInterfaceDeclaration(
        const model::InterfaceDeclaration &interfaceDeclaration, QDomElement &setElement, QDomDocument &sedsDocument)
{
    auto interfaceDeclarationElement = sedsDocument.createElement(QStringLiteral("Interface"));

    const auto &interfaceDeclarationName = interfaceDeclaration.nameStr();
    interfaceDeclarationElement.setAttribute(QStringLiteral("name"), interfaceDeclarationName);

    exportCommands(interfaceDeclaration.commands(), interfaceDeclarationElement, sedsDocument);

    setElement.appendChild(std::move(interfaceDeclarationElement));
}

void InterfaceExporter::exportCommands(const model::InterfaceDeclaration::CommandSet &commands,
        QDomElement &interfaceDeclarationElement, QDomDocument &sedsDocument)
{
    auto commandSetElement = sedsDocument.createElement(QStringLiteral("CommandSet"));

    for (const auto &command : commands) {
        exportCommand(command, commandSetElement, sedsDocument);
    }

    interfaceDeclarationElement.appendChild(std::move(commandSetElement));
}

void InterfaceExporter::exportCommand(
        const model::InterfaceCommand &command, QDomElement &commandSetElement, QDomDocument &sedsDocument)
{
    auto commandElement = sedsDocument.createElement(QStringLiteral("Command"));

    const auto &commandName = command.nameStr();
    commandElement.setAttribute(QStringLiteral("name"), commandName);
    for (const auto &commandArgument : command.arguments()) {
        exportCommandArgument(commandArgument, commandElement, sedsDocument);
    }
    commandElement.setAttribute(QStringLiteral("mode"), stringFromEnum(command.mode()));

    commandSetElement.appendChild(std::move(commandElement));
}

void InterfaceExporter::exportCommandArgument(
        const model::CommandArgument &commandArgument, QDomElement &commandElement, QDomDocument &sedsDocument)
{
    auto commandArgumentElement = sedsDocument.createElement(QStringLiteral("Argument"));

    const auto &commandArgumentName = commandArgument.nameStr();
    commandArgumentElement.setAttribute(QStringLiteral("name"), commandArgumentName);
    const auto &commandArgumentType = commandArgument.type().nameStr();
    commandArgumentElement.setAttribute(QStringLiteral("type"), commandArgumentType);
    commandArgumentElement.setAttribute(QStringLiteral("mode"), stringFromEnum(commandArgument.mode()));

    commandElement.appendChild(std::move(commandArgumentElement));
}

} // namespace seds::exporter
