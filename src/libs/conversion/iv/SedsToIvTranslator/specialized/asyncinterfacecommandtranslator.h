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

#include <conversion/common/qstringhash.h>
#include <map>
#include <seds/SedsModel/types/datatype.h>
#include <unordered_map>

namespace seds::model {
class CommandArgument;
class Component;
class DataTypeRef;
class Package;
enum class CommandArgumentMode : uint8_t;
} // namespace seds::model

namespace conversion::iv::translator {

/**
 * @brief   Translator from SEDS async interface command to InterfaceView interface
 */
class AsyncInterfaceCommandTranslator final : public InterfaceCommandTranslator
{

public:
    /**
     * @brief   Constructor
     *
     * @param   sedsInterface       Parent SEDS interface
     * @param   asn1Definitions     ASN.1 type definitions for parent package
     * @param   ivFunction          Output interface view function
     */
    AsyncInterfaceCommandTranslator(const seds::model::Interface &sedsInterface, Asn1Acn::Definitions *asn1Definitions,
            ivm::IVFunction *ivFunction);
    /**
     * @brief   Deleted copy constructor
     */
    AsyncInterfaceCommandTranslator(const AsyncInterfaceCommandTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    AsyncInterfaceCommandTranslator(AsyncInterfaceCommandTranslator &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    AsyncInterfaceCommandTranslator &operator=(const AsyncInterfaceCommandTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    AsyncInterfaceCommandTranslator &operator=(AsyncInterfaceCommandTranslator) = delete;

public:
    /**
     * @brief   Translates SEDS interface command to interface view interface
     *
     * This inserts result IV interface into member IV function
     *
     * @param   sedsCommand     SEDS interface command
     * @param   interfaceType   Interface type that will be created
     */
    virtual auto translateCommand(const seds::model::InterfaceCommand &sedsCommand,
            ivm::IVInterface::InterfaceType interfaceType) -> void override;

private:
    /**
     * @brief   Translates arguments of a SEDS interface command
     *
     * This bundles all arguments into one and creates ASN.1 sequence type for it
     *
     * @param   sedsCommand             SEDS interface command
     * @param   requestedArgumentMode   Which arguments should be translated
     * @param   ivInterface             Output interface view interface
     */
    auto translateArguments(const seds::model::InterfaceCommand &sedsCommand,
            seds::model::CommandArgumentMode requestedArgumentMode, ivm::IVInterface *ivInterface) -> void;

    /**
     * @brief   Builds ASN.1 sequence type for bundled interface argument
     *
     * @param   sedsCommand             SEDS interface command
     * @param   requestedArgumentMode   Which arguments should be used
     *
     * @return  Name of the created type
     */
    auto buildBundledType(const seds::model::InterfaceCommand &sedsCommand,
            seds::model::CommandArgumentMode requestedArgumentMode) -> QString;
    /**
     * @brief   Creates ASN.1 sequence type that bundles all argumenst
     *
     * Adds the created sequence to the ASN.1 definitions member
     *
     * @param   name        Name of the sequence
     * @param   arguments   SEDS command arguments
     *
     * @return  Bundled type name
     */
    auto createBundledType(const QString &sedsCommandName, const std::unordered_map<QString, QString> &arguments)
            -> QString;
    /**
     * @brief   Filters all arguments by their mode
     *
     * @param   sedsArguments           Arguments to process
     * @param   requestedArgumentMode   Which arguments should be used
     *
     * @return  Matching arguments
     */
    auto filterArguments(const std::vector<seds::model::CommandArgument> &sedsArguments,
            seds::model::CommandArgumentMode requestedArgumentMode) const -> std::unordered_map<QString, QString>;

    /**
     * @brief   Calculates hash from arguments types
     *
     * @param   arguments   Arguments to process
     *
     * @return  Calculated hash
     */
    auto calculateArgumentsHash(const std::unordered_map<QString, QString> &arguments) const -> std::size_t;

    /**
     *  Create a name for the bundled argument type
     *
     *  @param  sedsCommandName     SEDS command name
     *
     *  @return Type name
     */
    auto createBundledTypeName(const QString &sedsCommandName) const -> QString;

private:
    struct ArgumentsCacheEntry final {
        QString asn1TypeName;
        std::size_t typeHash;
        std::unordered_map<QString, QString> typeArguments;

        auto compareArguments(const std::unordered_map<QString, QString> &arguments) const -> bool;
    };

    /// @brief  Cache of the bundled ASN.1 types that was created for given command
    static std::multimap<QString, ArgumentsCacheEntry> m_commandArgumentsCache;

    /// @brief  Name for the argument in the IV interface
    static const QString m_ivInterfaceParameterName;
    /// @brief  Template for ASN.1 bundled type name
    static const QString m_bundledTypeNameTemplate;
};

} // namespace conversion::iv::translator
