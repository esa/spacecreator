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

#include "specialized/interfacecommandtranslator.h"

#include <shared/parameter.h>

namespace seds::model {
class CommandArgument;
enum class ArgumentsCombination : uint8_t;
} // namespace seds::model

namespace conversion::iv::translator {

/**
 * @brief   Translator from SEDS sync interface command to InterfaceView interface
 */
class SyncInterfaceCommandTranslator final : public InterfaceCommandTranslator
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sedsInterface   Parent interface
     * @param   asn1Definitions     ASN.1 type definitions for parent package
     * @param   ivFunction      Output interface view function
     */
    SyncInterfaceCommandTranslator(const seds::model::Interface &sedsInterface, Asn1Acn::Definitions *asn1Definitions,
            ivm::IVFunction *ivFunction);
    /**
     * @brief   Deleted copy constructor
     */
    SyncInterfaceCommandTranslator(const SyncInterfaceCommandTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    SyncInterfaceCommandTranslator(SyncInterfaceCommandTranslator &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    SyncInterfaceCommandTranslator &operator=(const SyncInterfaceCommandTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    SyncInterfaceCommandTranslator &operator=(SyncInterfaceCommandTranslator &&) = delete;

public:
    /**
     * @brief   Translates SEDS interface command to interface view interface
     *
     * This inserts result IV interface into member IV function
     *
     * @param   sedsCommand     SEDS interface command
     * @param   interfaceType   Interface type
     */
    virtual auto translateCommand(const seds::model::InterfaceCommand &sedsCommand,
            ivm::IVInterface::InterfaceType interfaceType) -> void override;

private:
    /**
     * @brief   Translates arguments of a SEDS interface command
     *
     * @param   sedsArguments   Arguments to translate
     * @param   ivInterface     Output interface view interface
     */
    auto translateArguments(
            const std::vector<seds::model::CommandArgument> &sedsArguments, ivm::IVInterface *ivInterface) -> void;

    /**
     * @brief   Creates interface view interface parameter
     *
     * @param   name        Name of the parameter
     * @param   typeName    Name of the type
     * @param   direction   Parameter direction
     *
     * @return  Interface view interface parameter
     */
    auto createIvInterfaceParameter(const QString &name, const QString &typeName,
            shared::InterfaceParameter::Direction direction) -> shared::InterfaceParameter;
};

} // namespace conversion::iv::translator
