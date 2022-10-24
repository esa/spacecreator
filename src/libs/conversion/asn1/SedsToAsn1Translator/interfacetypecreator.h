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

#include "context.h"
#include "generictypecreator.h"
#include "generictypemapper.h"
#include "interfacetypecreatorcontext.h"

#include <conversion/common/options.h>
#include <optional>
#include <seds/SedsModel/components/interface.h>
#include <seds/SedsModel/generics/generictypemapset.h>
#include <seds/SedsModel/interfaces/interfacedeclaration.h>
#include <seds/SedsModel/interfaces/interfaceparameter.h>

namespace conversion::asn1::translator::seds {

/**
 * @brief   Helper for creating types for interface commands
 */
class InterfaceTypeCreator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   options     Conversion options
     */
    InterfaceTypeCreator(const Options &options);
    /**
     * @brief   Deleted copy constructor
     */
    InterfaceTypeCreator(const InterfaceTypeCreator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    InterfaceTypeCreator(InterfaceTypeCreator &) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    auto operator=(const InterfaceTypeCreator &) -> InterfaceTypeCreator & = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    auto operator=(InterfaceTypeCreator &) -> InterfaceTypeCreator & = delete;

public:
    /**
     * @brief   Create types for interface delcaration
     *
     * @param   interfaceDeclaration    Interface delcaration to handle
     * @param   context                 Current translation context
     */
    auto createTypes(const ::seds::model::InterfaceDeclaration &interfaceDeclaration, Context &context) -> void;
    /**
     * @brief   Create types for interface
     *
     * @param   interface   Interface to handle
     * @param   context     Current translation context
     */
    auto createTypes(const ::seds::model::Interface &interface, Context &context) -> void;

private:
    auto doCreateTypes(const ::seds::model::InterfaceDeclaration *interfaceDeclaration, QString parentName,
            const std::optional<::seds::model::GenericTypeMapSet> &mappings, Context &mainContext,
            Context &interfaceContext) -> void;

    auto createTypesForParameter(const ::seds::model::InterfaceParameter &parameter,
            InterfaceTypeCreatorContext &typeCreatorContext) -> void;
    auto createTypesForCommand(const ::seds::model::InterfaceCommand &command, const QString &interfaceDeclarationName,
            InterfaceTypeCreatorContext &typeCreatorContext) -> void;

    auto createTypesForSyncCommand(
            const ::seds::model::InterfaceCommand &command, InterfaceTypeCreatorContext &typeCreatorContext) -> void;
    auto createTypesForAsyncCommand(const ::seds::model::InterfaceCommand &command,
            const QString &interfaceDeclarationName, InterfaceTypeCreatorContext &typeCreatorContext) -> void;
    auto createAsyncCommandBundledType(const ::seds::model::InterfaceCommand &command,
            const QString &interfaceDeclarationName, const ::seds::model::CommandArgumentMode requestedArgumentMode,
            const bool createFailureComponent, InterfaceTypeCreatorContext &typeCreatorContext) -> void;
    auto createAsyncCommandBundledTypeComponent(const ::seds::model::CommandArgument &argument,
            Asn1Acn::Types::Sequence *bundledType, const std::optional<QString> &determinantArgumentName,
            Context &bundledTypeContext, InterfaceTypeCreatorContext &typeCreatorContext) -> bool;

    auto createAsyncCommandFailureReportingBundledType(const ::seds::model::InterfaceCommand &command,
            const QString &interfaceDeclarationName, InterfaceTypeCreatorContext &typeCreatorContext) -> void;
    auto createAsyncCommandFailureReportingBundledTypeComponent(Asn1Acn::Types::Sequence *bundledType,
            Context &bundledTypeContext, InterfaceTypeCreatorContext &typeCreatorContext) -> void;

private:
    const Options &m_options;
};

} // namespace conversion::asn1::translator::seds
