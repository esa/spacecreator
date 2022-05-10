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

#include <optional>
#include <seds/SedsModel/components/interface.h>
#include <seds/SedsModel/generics/generictypemapset.h>
#include <seds/SedsModel/interfaces/interfacedeclaration.h>

namespace conversion::asn1::translator {

class InterfaceTypeCreatorContext final
{
public:
    InterfaceTypeCreatorContext(Context &context, Context &interfaceContext, bool isGeneric,
            const QString &interfaceName, const QString &parentInterfaceName,
            const std::vector<const seds::model::GenericType *> &genericTypes,
            const std::optional<seds::model::GenericTypeMapSet> &mappings);

public:
    auto handleType(const seds::model::DataTypeRef &typeRef, const std::vector<seds::model::DimensionSize> &dimensions)
            -> Asn1Acn::Types::Type *;

    auto context() const -> Context &;
    auto interfaceContext() const -> Context &;
    auto interfaceName() const -> const QString &;
    auto isTypeGeneric(const seds::model::DataTypeRef &argumentType) const -> bool;

private:
    Context &m_context;
    Context &m_interfaceContext;
    bool m_isGeneric;
    QString m_interfaceName;
    QString m_parentInterfaceName;
    const std::vector<const seds::model::GenericType *> &m_genericTypes;
    GenericTypeMapper m_typeMapper;
    GenericTypeCreator m_genericTypeCreator;
};

class InterfaceTypeCreator final
{
public:
    InterfaceTypeCreator(Context &context);

public:
    auto createTypes(const seds::model::InterfaceDeclaration &interfaceDeclaration) -> void;

private:
    auto doCreateTypes(const seds::model::InterfaceDeclaration *interfaceDeclaration, Context &context,
            Context &interfaceContext, const QString &parentInterfaceName,
            const std::optional<seds::model::GenericTypeMapSet> &mappings) -> void;

    auto createTypesForParameter(const seds::model::InterfaceParameter &parameter, InterfaceTypeCreatorContext &context)
            -> void;

    auto createTypesForSyncCommand(const seds::model::InterfaceCommand &command, InterfaceTypeCreatorContext &context)
            -> void;

    auto createTypesForAsyncCommand(const seds::model::InterfaceCommand &command, InterfaceTypeCreatorContext &context)
            -> void;
    auto createAsyncCommandBundledType(const seds::model::InterfaceCommand &command, const QString &bundledTypeName,
            const seds::model::CommandArgumentMode requestedArgumentMode, InterfaceTypeCreatorContext &context) -> void;
    auto createAsyncCommandBundledTypeComponent(const seds::model::CommandArgument &argument,
            Asn1Acn::Types::Sequence *bundledType, InterfaceTypeCreatorContext &context) -> void;

    auto collectGenericTypes(
            Context &interfaceContext, const seds::model::InterfaceDeclaration *interfaceDeclaration) const
            -> std::vector<const seds::model::GenericType *>;
    auto doCollectGenericTypes(Context &interfaceContext, const seds::model::InterfaceDeclaration *interfaceDeclaration,
            std::vector<const seds::model::GenericType *> &genericTypes) const -> void;

    auto isInterfaceGeneric(const seds::model::InterfaceDeclaration *interfaceDeclaration,
            const std::vector<const seds::model::GenericType *> &genericTypes) -> bool;
    auto isParameterGeneric(const seds::model::InterfaceParameter &parameter,
            const std::vector<const seds::model::GenericType *> &genericTypes) -> bool;
    auto isCommandGeneric(const seds::model::InterfaceCommand &command,
            const std::vector<const seds::model::GenericType *> &genericTypes) -> bool;
    auto isTypeGeneric(const seds::model::DataTypeRef &argumentType,
            const std::vector<const seds::model::GenericType *> &genericTypes) -> bool;

private:
    Context &m_context;
};

} // namespace conversion::asn1::translator
