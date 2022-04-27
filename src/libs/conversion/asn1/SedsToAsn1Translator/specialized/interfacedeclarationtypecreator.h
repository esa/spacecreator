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

#include <seds/SedsModel/interfaces/interfacedeclaration.h>

namespace conversion::asn1::translator {

class InterfaceDeclarationTypeCreator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context                 Current translation context
     */
    explicit InterfaceDeclarationTypeCreator(Context &context);
    /**
     * @brief   Deleted copy constructor
     */
    InterfaceDeclarationTypeCreator(const InterfaceDeclarationTypeCreator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    InterfaceDeclarationTypeCreator(InterfaceDeclarationTypeCreator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    InterfaceDeclarationTypeCreator &operator=(const InterfaceDeclarationTypeCreator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    InterfaceDeclarationTypeCreator &operator=(InterfaceDeclarationTypeCreator &&) = delete;

public:
    auto createTypes(const seds::model::InterfaceDeclaration &interfaceDeclaration) -> void;

private:
    auto createTypesForAsyncCommand(const seds::model::InterfaceCommand &command) -> void;
    auto createAsyncCommandBundledType(const seds::model::InterfaceCommand &command, const QString &bundledTypeName,
            const seds::model::CommandArgumentMode requestedArgumentMode) -> void;
    auto createAsyncCommandBundledTypeComponent(
            const seds::model::CommandArgument &argument, Asn1Acn::Types::Sequence *bundledType) -> void;

private:
    Context &m_context;

    inline static const QString m_bundledTypeNameTemplate = "%1-Type%2";
};

} // namespace conversion::asn1::translator
