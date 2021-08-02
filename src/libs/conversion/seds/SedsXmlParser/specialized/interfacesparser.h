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

#include <QStringRef>
#include <functional>
#include <seds/SedsCommon/basetypesmappings.h>

class QXmlStreamReader;

namespace seds::model {
class CommandArgument;
class InterfaceCommand;
class InterfaceDeclaration;
class InterfaceParameter;
class InterfaceRef;
class Package;

enum class CommandArgumentMode;
enum class InterfaceCommandMode;
enum class InterfaceParameterMode;
} // namespace seds::model

namespace seds::parser {

/**
 * @brief   SEDS parser for interfaces
 */
class InterfacesParser final
{
public:
    using DeclaredInterfaceAddingFunction = std::function<void(model::InterfaceDeclaration)>;

public:
    /**
     * @brief   Adds all interfaces to given package
     * @param   package     Package to which constructed Interfaces will be added
     */
    static auto readDeclaredInterfaceSet(
            DeclaredInterfaceAddingFunction addInterfaceDeclaration, QXmlStreamReader &xmlReader) -> void;

private:
    /**
     * @brief   Constructs InterfaceDeclaration object
     * @return  InterfaceDeclaration
     */
    static auto readInterfaceDeclaration(QXmlStreamReader &xmlReader) -> model::InterfaceDeclaration;

    /**
     * @brief   Adds all base interfaces to given interface declaration
     * @param   interfaceDeclaration    Interface declaration to which constructed InterfaceRefs will be added
     */
    static auto readInterfaceRefSet(model::InterfaceDeclaration &interfaceDeclaration, QXmlStreamReader &xmlReader)
            -> void;
    /**
     * @brief   Constructs InterfaceRef object
     * @return  InterfaceRef
     */
    static auto readInterfaceRef(QXmlStreamReader &xmlReader) -> model::InterfaceRef;

    /**
     * @brief   Adds all commands to given interface declaration
     * @param   interfaceDeclaration    Interface declaration to which constructed Commands will be added
     */
    static auto readCommandSet(model::InterfaceDeclaration &interfaceDeclaration, QXmlStreamReader &xmlReader) -> void;
    /**
     * @brief   Constructs InterfaceCommand object
     * @return  InterfaceCommand
     */
    static auto readInterfaceCommand(QXmlStreamReader &xmlReader) -> model::InterfaceCommand;
    /**
     * @brief   Constructs CommandArgument object
     * @return  CommandArgument
     */
    static auto readCommandArgument(QXmlStreamReader &xmlReader) -> model::CommandArgument;

    /**
     * @brief   Adds all parameters to given interface declaration
     * @param   interfaceDeclaration    Interface declaration to which constructed parameters will be added
     */
    static auto readParameterSet(model::InterfaceDeclaration &interfaceDeclaration, QXmlStreamReader &xmlReader)
            -> void;
    /**
     * @brief   Constructs InterfaceParameter object
     * @return  InterfaceParameter
     */
    static auto readInterfaceParameter(QXmlStreamReader &xmlReader) -> model::InterfaceParameter;

    /**
     * @brief   Parses given string as an interface command mode
     * @param   commandModeStr      String to parse
     * @return  Command mode
     */
    static auto parseInterfaceCommandMode(QStringRef commandModeStr) -> model::InterfaceCommandMode;
    /**
     * @brief   Parses given string as a command argument mode
     * @param   commandArgumentModeStr      String to parse
     * @return  Command argument mode
     */
    static auto parseCommandArgumentMode(QStringRef commandArgumentModeStr) -> model::CommandArgumentMode;
    /**
     * @brief   Parses given string as an interface parameter mode
     * @param   parameterModeStr      String to parse
     * @return  parameter mode
     */
    static auto parseInterfaceParameterMode(QStringRef parameterModeStr) -> model::InterfaceParameterMode;
};

} // namespace seds::parser
