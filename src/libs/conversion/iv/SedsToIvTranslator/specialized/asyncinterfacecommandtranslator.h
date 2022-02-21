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

#include "generictypemapper.h"

#include <asn1library/asn1/definitions.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivinterface.h>
#include <seds/SedsModel/interfaces/interfacecommand.h>
#include <seds/SedsModel/package/package.h>
#include <shared/qstringhash.h>
#include <unordered_map>

namespace conversion::iv::translator {

/**
 * @brief   Translator from SEDS async interface command to InterfaceView interface
 */
class AsyncInterfaceCommandTranslator final
{
private:
    struct ArgumentData {
        QString name;
        QString typeName;
        std::optional<QString> fixedValue;
        std::optional<QString> determinantName;
        bool isDeterminant;

        friend auto operator==(const ArgumentData &lhs, const ArgumentData &rhs) -> bool
        {
            return lhs.name == rhs.name && lhs.typeName == rhs.typeName;
        }

        friend auto operator!=(const ArgumentData &lhs, const ArgumentData &rhs) -> bool { return !(lhs == rhs); }
    };

    struct CommandArgumentEntry final {
        QString bundledTypeName;
        std::size_t typeHash;
        std::vector<ArgumentData> arguments;
    };

    using Arguments = std::vector<ArgumentData>;

public:
    /**
     * @brief   Constructor
     *
     * @param   ivFunction          Output interface view function
     * @param   sedsInterfaceName   Parent interface name
     * @param   asn1Definitions     Parent ASN.1 definitions
     * @param   sedsPackage         Parent SEDS package
     * @param   typeMapper          Generic type mapper
     */
    AsyncInterfaceCommandTranslator(ivm::IVFunction *ivFunction, const QString &sedsInterfaceName,
            Asn1Acn::Definitions *asn1Definitions, const seds::model::Package *sedsPackage,
            const GenericTypeMapper *typeMapper);
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
    AsyncInterfaceCommandTranslator &operator=(AsyncInterfaceCommandTranslator &&) = delete;

public:
    /**
     * @brief   Translates SEDS interface command to interface view interface
     *
     * This inserts result IV interface into member IV function
     *
     * @param   sedsCommand     SEDS interface command
     * @param   interfaceType   Interface type that will be created
     */
    auto translateCommand(
            const seds::model::InterfaceCommand &sedsCommand, ivm::IVInterface::InterfaceType interfaceType) -> void;

private:
    auto translateArguments(const seds::model::InterfaceCommand &sedsCommand,
            seds::model::CommandArgumentMode requestedArgumentMode, ivm::IVInterface *ivInterface) -> void;

    auto buildBundledType(const seds::model::InterfaceCommand &sedsCommand,
            seds::model::CommandArgumentMode requestedArgumentMode) -> QString;
    auto createBundledType(const QString &sedsCommandName, const std::vector<ArgumentData> &arguments) -> QString;
    auto createBundledTypeComponent(const ArgumentData &argumentData, Asn1Acn::Types::Sequence *sequence) const -> void;

    auto processArguments(const std::vector<seds::model::CommandArgument> &sedsArguments,
            seds::model::CommandArgumentMode requestedArgumentMode) -> Arguments;
    auto processArgument(const seds::model::CommandArgument &sedsArgument, const Arguments &arguments)
            -> AsyncInterfaceCommandTranslator::ArgumentData;

    auto handleArgumentSimpleMapping(const seds::model::CommandArgument &sedsArgument,
            const TypeMapping::ConcreteType &concreteType) -> AsyncInterfaceCommandTranslator::ArgumentData;
    auto handleArgumentAlternateMapping(const seds::model::CommandArgument &sedsArgument,
            const std::vector<TypeMapping::ConcreteType> &concreteTypes, const QString &determinantTypeName,
            const Arguments &arguments) -> AsyncInterfaceCommandTranslator::ArgumentData;
    auto handleArrayArgument(const seds::model::CommandArgument &sedsArgument, const QString &typeName) -> QString;

    auto createAlternateType(const QString &genericTypeName,
            const std::vector<TypeMapping::ConcreteType> &concreteTypes, const QString &determinantName) -> QString;

    auto calculateArgumentsHash(const std::vector<AsyncInterfaceCommandTranslator::ArgumentData> &arguments) const
            -> std::size_t;

private:
    /// @brief  Output interface view function
    ivm::IVFunction *m_ivFunction;

    /// @brief  Parent SEDS interface name
    const QString &m_sedsInterfaceName;
    /// @brief  Parent ASN.1 type definitions
    Asn1Acn::Definitions *m_asn1Definitions;
    /// @brief  Parent SEDS package
    const seds::model::Package *m_sedsPackage;

    /// @brief  Generic type mapper
    const GenericTypeMapper *m_typeMapper;

    /// @brief  Name for the argument in the IV interface
    static const QString m_ivInterfaceParameterName;
    /// @brief  Saved arguments for given command
    static std::multimap<QString, CommandArgumentEntry> m_commandArguments;
};

} // namespace conversion::iv::translator
