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

#pragma once

#include <QDomElement>
#include <seds/SedsModel/components/interface.h>
#include <seds/SedsModel/interfaces/interfacedeclaration.h>

namespace seds::exporter {

/**
 * @brief   Exporter for SEDS interfaces and interface declarations
 */
class InterfaceExporter final
{
public:
    /**
     * @brief   Export SEDS interface
     *
     * @param   interface       Interface to export
     * @param   setElement      Parent set element
     * @param   sedsDocument    SEDS XML document
     */
    static auto exportInterface(const model::Interface &interface, QDomElement &setElement, QDomDocument &sedsDocument)
            -> void;

    /**
     * @brief   Export SEDS interface declaration
     *
     * @param   interfaceDeclaration    Interface declaration to export
     * @param   setElement              Parent set element
     * @param   sedsDocument            SEDS XML document
     */
    static auto exportInterfaceDeclaration(const model::InterfaceDeclaration &interfaceDeclaration,
            QDomElement &setElement, QDomDocument &sedsDocument) -> void;

private:
    static auto exportInterfaceCommands(const model::InterfaceDeclaration::CommandSet &interfaceCommands,
            QDomElement &interfaceDeclarationElement, QDomDocument &sedsDocument) -> void;
    static auto exportInterfaceCommand(const model::InterfaceCommand &interfaceCommand,
            QDomElement &interfaceCommandSetElement, QDomDocument &sedsDocument) -> void;
};

} // namespace seds::exporter
