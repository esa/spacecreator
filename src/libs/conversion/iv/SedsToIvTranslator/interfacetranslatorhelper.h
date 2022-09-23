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

#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/types/type.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivinterface.h>
#include <seds/SedsModel/interfaces/commandargument.h>
#include <seds/SedsModel/package/package.h>

namespace conversion::iv::translator::seds {

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
            ivm::IVInterface::OperationKind kind, const ::seds::model::Description &sedsDescription,
            ivm::IVFunction *ivFunction) -> ivm::IVInterface *;
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
    static auto buildCommandInterfaceName(const QString &sedsInterfaceName, const QString &commandName,
            const ivm::IVInterface::InterfaceType type) -> QString;

    /**
     * @brief   Searches for interface declaration
     *
     * It first searches in the component interface declarations. If no declaration was found
     * then it searches in the package interface declarations.
     *
     * @param   interfaceDeclarationRef     Interface declaration to find
     * @param   sedsComponent               Component to search in
     * @param   sedsPackage                 Package to search in, if the search in the component fails
     * @param   sedsPackages                List of SEDS packages
     *
     * @throw UndeclaredInterfaceException  If interface declaration was not found
     *
     * @return  Found interface declarartion
     */
    static auto findInterfaceDeclaration(const ::seds::model::InterfaceDeclarationRef &interfaceDeclarationRef,
            const ::seds::model::Component &sedsComponent, const ::seds::model::Package *sedsPackage,
            const std::vector<::seds::model::Package> &sedsPackages) -> const ::seds::model::InterfaceDeclaration &;

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
    inline static const QString m_interfaceParameterEncoding = "ACN";
    /// @brief  Template for parameter interface view interfaces
    inline static const QString m_ivParameterInterfaceNameTemplate = "%1_%2_%3_%4";
    /// @brief  Template for command interface view interfaces
    inline static const QString m_ivCommandInterfaceNameTemplate = "%1_%2_%3";
    /// @brief  Prefix for getter interfaces
    inline static const QString m_getterInterfacePrefix = "Get";
    /// @brief  Prefix for setter interfaces
    inline static const QString m_setterInterfacePrefix = "Set";
};

} // namespace conversion::iv::translator::seds
