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

#include <optional>
#include <seds/SedsModel/components/interface.h>
#include <seds/SedsModel/generics/generictypemapset.h>
#include <seds/SedsModel/interfaces/interfacedeclaration.h>
#include <seds/SedsModel/interfaces/interfaceparameter.h>

namespace conversion::asn1::translator {

class InterfaceTypeCreator final
{
public:
    InterfaceTypeCreator() = default;

public:
    auto createTypes(const seds::model::InterfaceDeclaration &interfaceDeclaration, Context &context) -> void;
    auto createTypes(const seds::model::Interface &interface, Context &context) -> void;

private:
    auto doCreateTypes(const seds::model::InterfaceDeclaration *interfaceDeclaration, QString parentName,
            const std::optional<seds::model::GenericTypeMapSet> &mappings, Context &mainContext,
            Context &interfaceContext) -> void;

    auto createTypesForParameter(
            const seds::model::InterfaceParameter &parameter, InterfaceTypeCreatorContext &typeCreatorContext) -> void;
    auto createTypesForCommand(const seds::model::InterfaceCommand &command, const QString &interfaceDeclarationName,
            InterfaceTypeCreatorContext &typeCreatorContext) -> void;

    auto createTypesForSyncCommand(
            const seds::model::InterfaceCommand &command, InterfaceTypeCreatorContext &typeCreatorContext) -> void;
    auto createTypesForAsyncCommand(const seds::model::InterfaceCommand &command,
            const QString &interfaceDeclarationName, InterfaceTypeCreatorContext &typeCreatorContext) -> void;
    auto createAsyncCommandBundledType(const seds::model::InterfaceCommand &command,
            const QString &interfaceDeclarationName, const seds::model::CommandArgumentMode requestedArgumentMode,
            InterfaceTypeCreatorContext &typeCreatorContext) -> void;
    auto createAsyncCommandBundledTypeComponent(const seds::model::CommandArgument &argument,
            Asn1Acn::Types::Sequence *bundledType, const std::optional<QString> &determinantArgumentName,
            Context &bundledTypeContext, InterfaceTypeCreatorContext &typeCreatorContext) -> bool;
};

} // namespace conversion::asn1::translator
