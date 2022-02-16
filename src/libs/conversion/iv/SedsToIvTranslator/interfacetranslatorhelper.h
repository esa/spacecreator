/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/types/type.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivinterface.h>
#include <seds/SedsModel/interfaces/commandargument.h>
#include <seds/SedsModel/package/package.h>

namespace conversion::iv::translator {

/**
 * @brief   Helper for InterfaceView translator
 */
class InterfaceTranslatorHelper
{
public:
    /// @brief  Types of InterfaceView parameter
    enum class InterfaceParameterType
    {
        Getter,
        Setter
    };

    /**
     * @brief   Create InterfaceView interface
     *
     * @param   name        Interface name
     * @param   type        Interface type
     * @param   kind        Interface kind
     * @param   ivFunction  Parent function
     *
     * @return  InterfaceView interface
     */
    static auto createIvInterface(const QString &name, ivm::IVInterface::InterfaceType type,
            ivm::IVInterface::OperationKind kind, ivm::IVFunction *ivFunction) -> ivm::IVInterface *;
    /**
     * @brief   Create InterfaceView parameter
     *
     * Parameter has an ACN encoding.
     *
     * @param   name        Parameter name
     * @param   typeName    Parameter type name
     * @param   direction   Parameter direction
     *
     * @return  InterfaceView parameter
     */
    static auto createInterfaceParameter(const QString &name, const QString &typeName,
            shared::InterfaceParameter::Direction direction) -> shared::InterfaceParameter;
    /**
     * @brief   Create ASN.1 SEQUENCE OF with given dimensions
     *
     * @param   baseTypeName        Name of type that is the base of the array
     * @param   dimensions          Array dimensions
     * @param   asn1Definitions     Where the created type will be added
     * @param   sedsPackage         Parent SEDS package
     *
     * @return  Create type name
     */
    static auto createArrayType(const QString &baseTypeName, const std::vector<seds::model::DimensionSize> &dimensions,
            Asn1Acn::Definitions *asn1Definitions, const seds::model::Package *sedsPackage) -> QString;

    /**
     * @brief   Assemble name for the parameter
     *
     * @param   sedsInterfaceName       Name of the parent interface
     * @param   sedsparameterName       Name of the source parameter
     * @param   interfaceParameterType  Parameter type
     * @param   interfaceType           Interface type
     *
     * @return  Assembled name
     */
    static auto buildParameterInterfaceName(const QString &sedsInterfaceName, const QString &sedsParameterName,
            const InterfaceParameterType interfaceParameterType, ivm::IVInterface::InterfaceType interfaceType)
            -> QString;
    /**
     * @brief   Assemble name based on the names of a command and its hosting interface
     *
     * @param   sedsInterfaceName   Name of the interface hosting the command
     * @param   type                Interface type
     * @param   commandName         Name of the command
     *
     * @return  Assembled name
     */
    static auto buildCommandInterfaceName(const QString &sedsInterfaceName, const ivm::IVInterface::InterfaceType type,
            const QString &commandName) -> QString;
    /**
     * @brief   Assemble name for the array type
     *
     * @param   baseTypeName    Name of the type that is a base of the array
     * @param   dimensions      Array dimensions
     *
     * @return  Assembled name
     */
    static auto buildArrayTypeName(
            const QString &baseTypeName, const std::vector<seds::model::DimensionSize> &dimensions) -> QString;
    /**
     * @brief   Assemble name for the bundled type
     *
     * @param   sedsCommandName     Parent command name
     * @param   cachedTypesCound    Number of already created types for that command
     *
     * @return Assembled name
     */
    static auto buildBundledTypeName(const QString &sedsCommandName, const std::size_t cachedTypesCount) -> QString;
    /**
     * @brief   Assemble name for the alternate type
     *
     * @param   sedsInterfaceName   Parent interface name
     * @param   genericTypeName     Name of the generic type
     *
     * @return Assembled name
     */
    static auto buildAlternateTypeName(const QString &sedsInterfaceName, const QString &genericTypeName) -> QString;

    /**
     * @brief   Switch interface type
     *
     * @param   interfaceType       Type to switch
     *
     * @return  Switched interface type
     */
    static auto switchInterfaceType(ivm::IVInterface::InterfaceType interfaceType) -> ivm::IVInterface::InterfaceType;

private:
    static auto interfaceTypeToString(ivm::IVInterface::InterfaceType type) -> const QString &;

private:
    /// @brief  Interface parameter encoding name
    static const QString m_interfaceParameterEncoding;
    /// @brief  Template for parameter interface view interfaces
    static const QString m_ivParameterInterfaceNameTemplate;
    /// @brief  Template for command interface view interfaces
    static const QString m_ivCommandInterfaceNameTemplate;
    /// @brief  Template for array ASN.1 type name
    static const QString m_asn1ArrayNameTemplate;
    /// @brief  Template for ASN.1 bundled type name
    static const QString m_bundledTypeNameTemplate;
    /// @brief  Template for ASN.1 alternate type name
    static const QString m_alternateTypeNameTemplate;
    /// @brief  Prefix for getter interfaces
    static const QString m_getterInterfacePrefix;
    /// @brief  Prefix for setter interfaces
    static const QString m_setterInterfacePrefix;
};

} // namespace conversion::iv::translator
