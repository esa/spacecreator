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

class InterfaceTranslatorHelper
{
public:
    enum class InterfaceParameterType
    {
        Getter,
        Setter
    };

    static auto createIvInterface(const QString &name, ivm::IVInterface::InterfaceType type,
            ivm::IVInterface::OperationKind kind, ivm::IVFunction *m_ivFunction) -> ivm::IVInterface *;
    static auto createInterfaceParameter(const QString &parameterName, const QString &sedsParameterTypeName,
            shared::InterfaceParameter::Direction direction) -> shared::InterfaceParameter;
    static auto createArrayType(const QString &baseTypeName, const std::vector<seds::model::DimensionSize> &dimensions,
            Asn1Acn::Definitions *asn1Definitions, const seds::model::Package *sedsPackage) -> QString;

    static auto buildParameterInterfaceName(const QString &sedsInterfaceName, const QString &sedsParameterName,
            const InterfaceParameterType interfaceParameterType, ivm::IVInterface::InterfaceType interfaceType)
            -> QString;
    /**
     * @brief   Assemble name based on the names of a command and its hosting interface
     *
     * @param interfaceName Name of the interface hosting the command
     * @param type Interface type
     * @param commandName Name of the command
     *
     * @return Assembled name
     */
    static auto buildCommandInterfaceName(const QString &sedsInterfaceName, const ivm::IVInterface::InterfaceType type,
            const QString &commandName) -> QString;
    static auto buildArrayTypeName(
            const QString &baseTypeName, const std::vector<seds::model::DimensionSize> &dimensions) -> QString;
    static auto buildBundledTypeName(const QString &sedsCommandName, const std::size_t cachedTypesCount) -> QString;

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
    /// @brief  Prefix for getter interfaces
    static const QString m_getterInterfacePrefix;
    /// @brief  Prefix for setter interfaces
    static const QString m_setterInterfacePrefix;
};

} // namespace conversion::iv::translator
