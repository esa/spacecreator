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

#include <conversion/common/escaper/escaper.h>
#include <seds/SedsModel/interfaces/interfacedeclaration.h>
#include <seds/SedsModel/types/datatyperef.h>
#include <seds/SedsModel/types/dimensionsize.h>

namespace conversion::iv::translator::seds {

/**
 * @brief   Helper for building names for interface command argument types
 */
class InterfaceTypeNameHelper final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context                 Translation context
     * @param   parentName              Parent entity name
     * @param   interfaceDeclaration    Parent interface declaration
     * @param   sedsPackages            List of all SEDS packages
     */
    InterfaceTypeNameHelper(Context context, QString parentName,
            const ::seds::model::InterfaceDeclaration *interfaceDeclaration,
            const std::vector<::seds::model::Package> &sedsPackages);
    /**
     * @brief   Deleted copy constructor
     */
    InterfaceTypeNameHelper(const InterfaceTypeNameHelper &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    InterfaceTypeNameHelper(InterfaceTypeNameHelper &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    InterfaceTypeNameHelper operator=(const InterfaceTypeNameHelper &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    InterfaceTypeNameHelper operator=(InterfaceTypeNameHelper &&) = delete;

public:
    /**
     * @brief   Handle name of given type
     *
     * @param   typeRef     Type reference
     * @param   dimensions  Array dimensions
     *
     * @return  Type name
     */
    auto handleTypeName(const ::seds::model::DataTypeRef &typeRef,
            const std::vector<::seds::model::DimensionSize> &dimensions) const -> QString;
    /**
     * @brief   Handle bundled name of the async command type
     *
     * @param   command                 Command to handle
     * @param   requestedArgumentMode   Mode for command arguments that should be handled
     *
     * @return  Command bundled type name
     */
    auto handleAsyncCommandTypeName(const ::seds::model::InterfaceCommand &command,
            const ::seds::model::CommandArgumentMode requestedArgumentMode) const -> QString;

private:
    auto isCommandGeneric(const ::seds::model::InterfaceCommand &command) const -> bool;
    auto isTypeGeneric(const ::seds::model::DataTypeRef &typeRef) const -> bool;

    auto collectGenericTypes(const ::seds::model::InterfaceDeclaration *interfaceDeclaration) -> void;
    auto doCollectGenericTypes(const ::seds::model::InterfaceDeclaration *interfaceDeclaration, Context &context)
            -> void;

private:
    Context m_context;
    QString m_parentName;
    QString m_interfaceDeclarationName;
    const std::vector<::seds::model::Package> &m_sedsPackages;

    std::vector<const ::seds::model::GenericType *> m_genericTypes;
};

} // namespace conversion::iv::translator::seds
