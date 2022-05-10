/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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
#include "generictypemapper.h"

#include <seds/SedsModel/components/component.h>
#include <seds/SedsModel/components/interface.h>
#include <seds/SedsModel/interfaces/interfacecommand.h>
#include <seds/SedsModel/interfaces/interfacedeclaration.h>
#include <seds/SedsModel/interfaces/interfacedeclarationref.h>
#include <seds/SedsModel/interfaces/interfaceparameter.h>

namespace conversion::asn1::translator {

class GenericInterfaceTypeCreator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context     Current translation context
     * @param   component   Parent SEDS component
     */
    GenericInterfaceTypeCreator(Context &context, Context &interfaceContext, const seds::model::Interface &interface,
            const seds::model::InterfaceDeclaration *interfaceDeclaration);
    /**
     * @brief   Deleted copy constructor
     */
    GenericInterfaceTypeCreator(const GenericInterfaceTypeCreator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    GenericInterfaceTypeCreator(GenericInterfaceTypeCreator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    GenericInterfaceTypeCreator &operator=(const GenericInterfaceTypeCreator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    GenericInterfaceTypeCreator &operator=(GenericInterfaceTypeCreator &&) = delete;

public:
    auto createTypes() -> void;

private:
    auto createTypeForGeneric(const seds::model::GenericType &genericType) -> void;
    auto createConcreteTypeAlias(
            const seds::model::GenericType &genericType, const TypeMapping::Concrete &concreteMapping) -> void;
    auto createConcreteChoice(const seds::model::GenericType &genericType, const TypeMapping *mapping) -> void;
    auto createConcreteChoiceAlternative(
            const TypeMapping::Concrete &concreteType, Asn1Acn::Types::Choice *choice, const std::size_t count) -> void;
    auto handleFixedValue(Asn1Acn::Types::Type *type, const QString &fixedValue) -> void;

    auto createTypesForParameter(const seds::model::InterfaceParameter &parameter) -> void;

    auto createTypesForSyncCommand(const seds::model::InterfaceCommand &command) -> void;
    auto createSyncArgumentType(const seds::model::CommandArgument &argument) -> void;

    auto createTypesForAsyncCommand(const seds::model::InterfaceCommand &command) -> void;
    auto createAsyncCommandBundledType(const seds::model::InterfaceCommand &command, const QString &bundledTypeName,
            const seds::model::CommandArgumentMode requestedArgumentMode) -> void;
    auto createAsyncCommandBundledTypeComponent(const seds::model::CommandArgument &argument,
            Asn1Acn::Types::Sequence *bundledType, const std::optional<QString> &determinantArgumentName) -> void;

    auto findDeterminantArgument(const std::vector<seds::model::CommandArgument> &arguments) -> std::optional<QString>;

private:
    Context &m_context;
    Context &m_interfaceContext;

    const QString &m_interfaceName;
    const seds::model::InterfaceDeclaration *m_interfaceDeclaration;

    GenericTypeMapper m_typeMapper;
};

} // namespace conversion::asn1::translator
