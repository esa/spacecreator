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
    GenericInterfaceTypeCreator(
            Context &context, const seds::model::Interface &interface, const seds::model::Component &component);
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

    auto findInterfaceDeclaration(const seds::model::InterfaceDeclarationRef &interfaceRef)
            -> const seds::model::InterfaceDeclaration *;

    auto buildBundledTypeName(const QString &commandName, QString postfix = "") -> QString;
    auto buildConcreteTypeName(const QString &genericName) -> QString;

private:
    Context &m_context;

    const seds::model::Interface &m_interface;
    const seds::model::InterfaceDeclaration *m_interfaceDeclaration;
    const seds::model::Component &m_component;

    GenericTypeMapper m_typeMapper;

    inline static const QString m_bundledTypeNameTemplate = "%1-%2-Type%3";
    inline static const QString m_concreteTypeNameTemplate = "%1-%2-%3";
};

} // namespace conversion::asn1::translator
