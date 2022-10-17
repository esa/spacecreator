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

#include <QString>
#include <asn1library/asn1/types/type.h>
#include <optional>
#include <seds/SedsModel/generics/generictype.h>
#include <seds/SedsModel/generics/generictypemapset.h>
#include <seds/SedsModel/interfaces/interfacedeclaration.h>
#include <seds/SedsModel/types/dimensionsize.h>
#include <vector>

namespace conversion::asn1::translator::seds {

/**
 * @brief   Context for creating types for interface commands
 */
class InterfaceTypeCreatorContext final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   mainContext             Current translation context
     * @param   interfaceContext        Translation context for parent interface
     * @param   interfaceDeclaration    Parent interface declaration
     * @param   parentName              Name of the entity that should be a parent for the created type
     * @param   mappings                Mappings for generic types
     */
    InterfaceTypeCreatorContext(Context &mainContext, Context &interfaceContext,
            const ::seds::model::InterfaceDeclaration *interfaceDeclaration, QString parentName,
            const std::optional<::seds::model::GenericTypeMapSet> &mappings);
    /**
     * @brief   Deleted copy constructor
     */
    InterfaceTypeCreatorContext(const InterfaceTypeCreatorContext &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    InterfaceTypeCreatorContext(InterfaceTypeCreatorContext &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    InterfaceTypeCreatorContext operator=(const InterfaceTypeCreatorContext &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    InterfaceTypeCreatorContext operator=(InterfaceTypeCreatorContext &&) = delete;

public:
    /**
     * @brief   Handle type of the command argument
     *
     * @param   typeRef         Type reference
     * @param   dimensions      Type array dimensions
     */
    auto handleType(const ::seds::model::DataTypeRef &typeRef,
            const std::vector<::seds::model::DimensionSize> &dimensions) -> std::optional<::seds::model::DataTypeRef>;

    /**
     * @brief   Getter for main context
     *
     * @return  Main context
     */
    auto mainContext() const -> Context &;
    /**
     * @brief   Getter for interface context
     *
     * @return  Interface context
     */
    auto interfaceContext() const -> Context &;
    /**
     * @brief   Getter for parent entity name
     *
     * @return  Parent entity name
     */
    auto parentName() const -> const QString &;
    /**
     * @brief   Find determinant argument from given vector
     *
     * @param   arguments   Arguments to check in
     *
     * @return  Name of the determinant argument if found, nullopt otherwise
     */
    auto findDeterminantArgument(const std::vector<::seds::model::CommandArgument> &arguments)
            -> std::optional<QString>;
    /**
     * @brief   Check if given type is a generic type
     *
     * @param   typeRef     Type reference to check
     *
     * @return  True if type is generic, false otherwise
     */
    auto isTypeGeneric(const ::seds::model::DataTypeRef &typeRef) -> bool;
    /**
     * @brief   Check if given command has any argument with generic type
     *
     * @param   command     Commend to check
     *
     * @true    True if command is generic, false otherwise
     */
    auto isCommandGeneric(const ::seds::model::InterfaceCommand &command) -> bool;

private:
    auto collectGenericTypes(const ::seds::model::InterfaceDeclaration *interfaceDeclaration) -> void;
    auto doCollectGenericTypes(
            const ::seds::model::InterfaceDeclaration *interfaceDeclaration, Context &interfaceContext) -> void;

private:
    Context &m_mainContext;
    Context &m_interfaceContext;
    QString m_parentName;
    std::vector<const ::seds::model::GenericType *> m_genericTypes;
    const std::optional<::seds::model::GenericTypeMapSet> &m_mappings;
    GenericTypeMapper m_typeMapper;
    GenericTypeCreator m_genericTypeCreator;
};

} // namespace conversion::asn1::translator::seds
