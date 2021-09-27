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
private:
    using InterfaceCommandArgumentsCache = std::multimap<QString, std::pair<std::size_t, QString>>;

public:
    /**
     * @brief   Constructor
     *
     * @param   package             Parent package
     * @param   component           Parent package
     * @param   interface           Parent interface
     * @param   ivFunction          Output interface view function
     * @param   asn1Definitions     Output ASN.1 type definitions
     */
    AsyncInterfaceCommandTranslator(const seds::model::Package &package, const seds::model::Component &component,
            const seds::model::Interface &interface, ivm::IVFunction *ivFunction,
            Asn1Acn::Definitions *asn1Definitions);
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
     * @param   command         SEDS interface command
     * @param   interfaceType   Interface type
     */
    virtual auto translateCommand(const seds::model::InterfaceCommand &command,
            ivm::IVInterface::InterfaceType interfaceType) -> void override;

private:
    /**
     * @brief   Translates arguments of a SEDS interface command
     *
     * This bundles all arguments into one and creates ASN.1 sequence type for it
     *
     * @param   command                 SEDS interface command
     * @param   requestedArgumentMode   Which arguments should be translated
     * @param   ivInterface             Output interface view interface
     */
    auto translateArguments(const seds::model::InterfaceCommand &command,
            seds::model::CommandArgumentMode requestedArgumentMode, ivm::IVInterface *ivInterface) -> void;

    /**
     * @brief   Builds ASN.1 sequence type for bundled interface argument
     *
     * @param   command                 SEDS interface command
     * @param   requestedArgumentMode   Which arguments should be used
     */
    auto buildAsn1SequenceType(const seds::model::InterfaceCommand &command,
            seds::model::CommandArgumentMode requestedArgumentMode) const -> QString;

    /**
     * @brief   Creates ASN.1 sequence type
     *
     * @param   name        Name of the new type
     * @param   arguments   Command arguments
     *
     * @return  ASN.1 sequence
     */
    auto createAsn1Sequence(const QString &name, const std::unordered_map<QString, QString> &arguments) const -> void;
    /**
     * @brief   Creates ASN.1 sequence component type
     *
     * @param   name        Name of the new type
     * @param   typeName    Name of the SEDS type from which ASN.1 type should be created
     *
     * @return  ASN.1 sequence component
     */
    auto createAsn1SequenceComponent(const QString &name, const QString typeName) const
            -> std::unique_ptr<Asn1Acn::SequenceComponent>;

    /**
     * @brief   Checks if given type name is mapped in the parent SEDS interface
     *
     * @param   genericTypeName     Generic type name
     *
     * @return  Mapped type name if given type name was mapped, given type name otherwise
     */
    auto findMappedType(const QString &genericTypeName) const -> const QString &;
    /**
     * @brief   Searches for SEDS type declaration
     *
     * First it searches in the parent SEDS component. If not found then it searches
     * in the package. Throws an exception if not found
     *
     * @param   dataTypeName    Type name
     *
     * @throws  UndeclaredDataTypeException     If given type wasn't declared
     *
     * @return  Found SEDS data type declaration
     */
    auto findDataType(const QString &dataTypeName) const -> const seds::model::DataType &;

    /**
     * @brief   Swaps between provided and required interface types
     *
     * @param   interfaceType   Interface type to switch
     *
     * @return  Provided type if required was passed, requried otherwise
     */
    auto switchInterfaceType(ivm::IVInterface::InterfaceType interfaceType) const -> ivm::IVInterface::InterfaceType;

private:
    /// @brief  Parent SEDS package
    const seds::model::Package &m_package;
    /// @brief  Parent SEDS component
    const seds::model::Component &m_component;
    /// @brief  Output ASN.1 type definitions
    Asn1Acn::Definitions *m_asn1Definitions;

    /// @brief  Cache of the bundled ASN.1 types that was created for given command
    static InterfaceCommandArgumentsCache m_asn1CommandArgumentsCache;

    /// @brief  Interface parameter name
    static const QString m_interfaceParameterName;
    /// @brief  Template for ASN.1 bundled type name
    static const QString m_asn1BundledTypeTemplate;
};

} // namespace conversion::iv::translator
