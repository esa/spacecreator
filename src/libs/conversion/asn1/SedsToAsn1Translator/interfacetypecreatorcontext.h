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

namespace conversion::asn1::translator {

class InterfaceTypeCreatorContext final
{
public:
    InterfaceTypeCreatorContext(Context &mainContext, Context &interfaceContext,
            const seds::model::InterfaceDeclaration *interfaceDeclaration, QString parentName,
            const std::optional<seds::model::GenericTypeMapSet> &mappings);
    InterfaceTypeCreatorContext(const InterfaceTypeCreatorContext &) = delete;
    InterfaceTypeCreatorContext(InterfaceTypeCreatorContext &&) = delete;

    InterfaceTypeCreatorContext operator=(const InterfaceTypeCreatorContext &) = delete;
    InterfaceTypeCreatorContext operator=(InterfaceTypeCreatorContext &&) = delete;

public:
    auto handleType(const seds::model::DataTypeRef &typeRef, const std::vector<seds::model::DimensionSize> &dimensions)
            -> std::optional<seds::model::DataTypeRef>;

    auto mainContext() const -> Context &;
    auto interfaceContext() const -> Context &;
    auto parentName() const -> const QString &;
    auto findDeterminantArgument(const std::vector<seds::model::CommandArgument> &arguments) -> std::optional<QString>;
    auto isTypeGeneric(const seds::model::DataTypeRef &typeRef) -> bool;
    auto isCommandGeneric(const seds::model::InterfaceCommand &command) -> bool;

private:
    auto collectGenericTypes(const seds::model::InterfaceDeclaration *interfaceDeclaration) -> void;
    auto doCollectGenericTypes(const seds::model::InterfaceDeclaration *interfaceDeclaration, Context &interfaceContext)
            -> void;

private:
    Context &m_mainContext;
    Context &m_interfaceContext;
    QString m_parentName;
    std::vector<const seds::model::GenericType *> m_genericTypes;
    const std::optional<seds::model::GenericTypeMapSet> &m_mappings;
    GenericTypeMapper m_typeMapper;
    GenericTypeCreator m_genericTypeCreator;
};

} // namespace conversion::asn1::translator
