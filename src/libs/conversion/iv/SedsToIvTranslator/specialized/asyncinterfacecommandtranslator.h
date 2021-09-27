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

#include <map>
#include <seds/SedsModel/types/datatype.h>

namespace Asn1Acn {
class AsnSequenceComponent;
class Definitions;
class SequenceComponent;
} // namespace Asn1Acn

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
     * @param   sedsPackage         Parent SEDS package
     * @param   sedsComponent       Parent SEDS component
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
    auto buildAsn1SequenceType(const seds::model::InterfaceCommand &sedsCommand,
            seds::model::CommandArgumentMode requestedArgumentMode) -> QString;

    /**
     * @brief   Creates ASN.1 sequence type
     *
     * @param   name        Name of the sequence
     * @param   arguments   SEDS command arguments
     *
     * @return  ASN.1 sequence
     */
    auto createAsn1Sequence(const QString &name, const std::unordered_map<QString, QString> &arguments) -> void;
    /**
     * @brief   Creates ASN.1 sequence component type
     *
     * @param   name        Name of the component
     * @param   typeName    Name of the component type
     *
     * @return  ASN.1 sequence component
     */
    auto createAsn1SequenceComponent(const QString &name, const QString typeName) const
            -> std::unique_ptr<Asn1Acn::SequenceComponent>;
    /**
     *  Create a name for the bundled argument type
     *
     *  @param  sedsCommandName     SEDS command name
     *  @param  counter             Additional counter
     *
     *  @return Type name
     */
    auto createBundledTypeName(const QString sedsCommandName, std::size_t counter = 0) const -> QString;

    /**
     * @brief   Process SEDS command arguments types
     *
     * Maps generic types of the arguments to a concrete type
     *
     * @param   sedsArguments   Arguments to process
     * @param   requestedArgumentMode   Which arguments should be used
     *
     * @return  Processed arguments
     */
    auto processArgumentsTypes(const std::vector<seds::model::CommandArgument> &sedsArguments,
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
     * @brief   Checks if given type name is mapped in the parent SEDS interface
     *
     * @param   genericTypeName     Generic type name
     *
     * @return  Mapped type name if given type name was mapped, given type name otherwise
     */
    auto findMappedType(const QString &genericTypeName) const -> const QString &;

    /**
     * @brief   Swaps between provided and required interface types
     *
     * @param   interfaceType   Interface type to switch
     *
     * @return  Provided type if required was passed, requried otherwise
     */
    auto switchInterfaceType(ivm::IVInterface::InterfaceType interfaceType) const -> ivm::IVInterface::InterfaceType;

private:
    struct ArgumentsCacheEntry final {
        QString asn1TypeName;
        std::size_t typeHash;
        std::unordered_map<QString, QString> typeArguments;

        auto compareArguments(const std::unordered_map<QString, QString> &arguments) const -> bool;
    };

    /// @brief  Output ASN.1 type definitions
    Asn1Acn::Definitions *m_asn1Definitions;

    /// @brief  Cache of the bundled ASN.1 types that was created for given command
    static std::multimap<QString, ArgumentsCacheEntry> m_commandArgumentsCache;

    /// @brief  Name for the argument in the IV interface
    static const QString m_ivInterfaceParameterName;
    /// @brief  Template for ASN.1 bundled type name
    static const QString m_bundledTypeNameTemplate;
};

} // namespace conversion::iv::translator
