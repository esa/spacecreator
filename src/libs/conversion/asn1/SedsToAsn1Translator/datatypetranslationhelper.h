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

#include <QString>
#include <seds/SedsModel/generics/generictype.h>
#include <seds/SedsModel/interfaces/commandargument.h>
#include <seds/SedsModel/types/datatyperef.h>
#include <seds/SedsModel/types/dimensionsize.h>
#include <vector>

namespace conversion::asn1::translator::seds {

/**
 * @brief   Helper for data types translation
 */
class DataTypeTranslationHelper final
{
public:
    /**
     * @brief   Deleted constructor
     */
    DataTypeTranslationHelper() = delete;

public:
    /**
     * @brief   Build name for the array type
     *
     * @param   baseTypeName    Name of the type that is a base of the array
     * @param   dimensions      Array dimensions
     *
     * @return  Array type name
     */
    static auto buildArrayTypeName(
            const QString &baseTypeName, const std::vector<::seds::model::DimensionSize> &dimensions) -> QString;
    /**
     * @brief   Build name for the bundled type
     *
     * @param   parentName      Name of the entity that is a parent of the bundled type
     * @param   commandName     Name of the command for which the bundled type is created
     * @param   mode            Mode of the parent command
     *
     * @return  Bundled type name
     */
    static auto buildBundledTypeName(const QString &parentName, const QString &commandName,
            const ::seds::model::CommandArgumentMode mode) -> QString;
    /**
     * @brief   Build name for the concrete type name
     *
     * @param   interfaceDeclarationName    Name of the parent interface declaration
     * @param   genericName                 Name of the generic type
     *
     * @return  Concrete type name
     */
    static auto buildConcreteTypeName(const QString &interfaceDeclarationName, const QString &genericName) -> QString;

    /**
     * @brief   Create array type
     *
     * @param   context     Current translation context
     * @param   typeRef     Type that is a base of the array
     * @param   dimensions  Array dimensions
     *
     * @return  Type reference for the created type
     */
    static auto createArrayType(Context &context, const ::seds::model::DataTypeRef &typeRef,
            const std::vector<::seds::model::DimensionSize> &dimensions) -> ::seds::model::DataTypeRef;
    /**
     * @brief   Create array type if it doesn't exists, return reference to it otherwise
     *
     * @param   context             Current translation context
     * @param   argumentTypeRef     Type that is a base of the array
     * @param   dimensions          Array dimensions
     *
     * @return  ASN.1 type
     */
    static auto handleArrayType(Context &context, const ::seds::model::DataTypeRef &argumentTypeRef,
            const std::vector<::seds::model::DimensionSize> &dimensions) -> Asn1Acn::Types::Type *;

    /**
     * @brief   Check if given type is a generic type
     *
     * @param   typeRef         Type to check
     * @param   genericTypes    List of generic types
     *
     * @return  True of type is generic, false otherwise
     */
    static auto isTypeGeneric(const ::seds::model::DataTypeRef &typeRef,
            const std::vector<::seds::model::GenericType> &genericTypes) -> bool;

private:
    inline static const QString m_arrayTypeNameTemplate = "%1Array-%2";
    inline static const QString m_bundledTypeNameTemplate = "%1-%2-Type%3";
    inline static const QString m_genericBundledTypeNameTemplate = "%1-%2-%3-Type";
    inline static const QString m_concreteTypeNameDeclarationTemplate = "%1-%2";
    inline static const QString m_concreteTypeNameImplementationTemplate = "%1-%2-%3";
};

} // namespace conversion::asn1::translator::seds
