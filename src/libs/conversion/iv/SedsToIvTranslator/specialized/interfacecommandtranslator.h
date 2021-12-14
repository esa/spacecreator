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

#include <ivcore/ivinterface.h>

namespace Asn1Acn {
class Definitions;
namespace Types {
class Type;
class Sequence;
} // namespace Types
} // namespace Asn1Acn

namespace ivm {
class IVFunction;
} // namespace ivm

namespace seds::model {
class CommandArgument;
class DimensionSize;
class Interface;
class InterfaceCommand;
} // namespace seds::model

#include <ivcore/ivinterface.h>

namespace conversion::iv::translator {

/**
 * @brief   Interface for translators from SEDS interface command to InterfaceView interface
 */
class InterfaceCommandTranslator
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sedsInterface   Parent interface
     * @param   asn1Definitions     ASN.1 type definitions for parent package
     * @param   ivFunction      Output interface view function
     */
    InterfaceCommandTranslator(const seds::model::Interface &sedsInterface, Asn1Acn::Definitions *asn1Definitions,
            ivm::IVFunction *ivFunction);
    /**
     * @brief   Default destructor
     */
    virtual ~InterfaceCommandTranslator() = default;
    /**
     * @brief   Deleted copy constructor
     */
    InterfaceCommandTranslator(const InterfaceCommandTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    InterfaceCommandTranslator(InterfaceCommandTranslator &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    InterfaceCommandTranslator &operator=(const InterfaceCommandTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    InterfaceCommandTranslator &operator=(InterfaceCommandTranslator &&) = delete;

public:
    /**
     * @brief   Translates SEDS interface command to interface view interface
     *
     * This inserts result IV interface into member IV function
     *
     * @param   sedsCommand     SEDS interface command
     * @param   interfaceType   Interface type
     */
    virtual auto translateCommand(
            const seds::model::InterfaceCommand &command, ivm::IVInterface::InterfaceType interfaceType) -> void = 0;

    /**
     * @brief   Assemble name based on the names of a command and its hosting interface
     *
     * @param interfaceName Name of the interface hosting the command
     * @param type Interface type
     * @param commandName Name of the command
     *
     * @return Assembled name
     */
    static auto getCommandName(const QString &interfaceName, const ivm::IVInterface::InterfaceType type,
            const QString &commandName) -> QString;

protected:
    /**
     * @brief   Process argument to get its concrete type name
     *
     * Argument types can
     * 1. Be mapped from 'generic' to 'concrete' types
     * 2. Be handled as an array (ASN.1 sequence)
     *
     * @param   sedsArgument    SEDS command argument
     *
     * @return  Concrete command type name
     */
    auto handleArgumentType(const seds::model::CommandArgument &sedsArgument) const -> QString;

    auto buildArrayType(const seds::model::CommandArgument &sedsArgument, const QString &sedsArgumentTypeName) const
            -> QString;

    /**
     * @brief   Creates new interface view interface
     *
     * @param   sedsCommand     SEDS interface command
     * @param   type            Type of interface to create
     * @param   kind            Kind of interface to create
     *
     * @return  Interface view interface
     */
    auto createIvInterface(const seds::model::InterfaceCommand &sedsCommand, ivm::IVInterface::InterfaceType type,
            ivm::IVInterface::OperationKind kind) const -> ivm::IVInterface *;
    /**
     * @brief   Creates ASN.1 sequence component type
     *
     * Added the created sequence component to the passed ASN.1 sequence
     *
     * @param   name        Name of the component
     * @param   typeName    Name of the component type
     * @param   sequence    Sequence to which component should be added
     */
    auto createAsn1SequenceComponent(
            const QString &name, const QString &typeName, Asn1Acn::Types::Sequence *sequence) const -> void;
    auto createArrayType(const seds::model::CommandArgument &sedsArgument, const QString &sedsArgumentTypeName) const
            -> QString;
    auto createArrayTypeName(const QString &sedsArgumentTypeName) const -> QString;

    auto calculateDimensionsHash(const std::vector<seds::model::DimensionSize> &dimensions) const -> std::size_t;

    /**
     * @brief   Swaps between provided and required interface types
     *
     * @param   interfaceType   Interface type to switch
     *
     * @return  Provided type if required was passed, requried otherwise
     */
    static auto switchInterfaceType(ivm::IVInterface::InterfaceType interfaceType) -> ivm::IVInterface::InterfaceType;
    /**
     * @brief   Converts interface view interface type to string
     *
     * @param   interfaceType   Interface type to convert
     *
     * @return   Interface type name
     */
    static auto interfaceTypeToString(ivm::IVInterface::InterfaceType type) -> const QString &;

private:
    /**
     * @brief   Checks if given type name is mapped in the parent SEDS interface
     *
     * @param   genericTypeName     Generic type name
     *
     * @return  Mapped type name if given type name was mapped, given type name otherwise
     */
    auto findMappedType(const QString &genericTypeName) const -> const QString &;

protected:
    /// @brief  Parent SEDS interface
    const seds::model::Interface &m_sedsInterface;
    /// @brief  Output ASN.1 type definitions
    Asn1Acn::Definitions *m_asn1Definitions;
    /// @brief  Output interface view function
    ivm::IVFunction *m_ivFunction;

    struct ArrayArgumentsCacheEntry final {
        QString asn1TypeName;
        std::size_t dimensionsHash;
        std::vector<seds::model::DimensionSize> arrayDimensions;

        auto compareDimensions(const std::vector<seds::model::DimensionSize> &diumensions) const -> bool;
    };

    static std::multimap<QString, ArrayArgumentsCacheEntry> m_arrayArgumentsCache;

    /// @brief  Interface parameter encoding name
    static const QString m_interfaceParameterEncoding;
    /// @brief  Template for interface view interfaces
    static const QString m_ivInterfaceNameTemplate;
    /// @brief  Template for name of arguments that have dimensions
    static const QString m_arrayArgumentNameTemplate;
};

} // namespace conversion::iv::translator
