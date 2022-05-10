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

#include "interfacetypecreatorcontext.h"

#include "datatypetranslationhelper.h"

#include <conversion/common/translation/exceptions.h>
#include <iostream>

using conversion::translator::TranslationException;

namespace conversion::asn1::translator {

InterfaceTypeCreatorContext::InterfaceTypeCreatorContext(Context &mainContext, Context &interfaceContext,
        QString parentName, const std::vector<const seds::model::GenericType *> &genericTypes,
        const std::optional<seds::model::GenericTypeMapSet> &mappings)
    : m_mainContext(mainContext)
    , m_interfaceContext(interfaceContext)
    , m_parentName(std::move(parentName))
    , m_genericTypes(genericTypes)
    , m_mappings(mappings)
    , m_typeMapper(m_mainContext, m_parentName)
    , m_genericTypeCreator(m_mainContext, m_parentName, m_typeMapper)
{
    if (m_mappings.has_value()) {
        m_typeMapper.addMappings(*m_mappings);
    }
}

std::optional<seds::model::DataTypeRef> InterfaceTypeCreatorContext::handleType(
        const seds::model::DataTypeRef &typeRef, const std::vector<seds::model::DimensionSize> &dimensions)
{
    const auto isGeneric = isTypeGeneric(typeRef);

    auto concreteTypeRef = typeRef;

    if (isGeneric) {
        std::cerr << "\t\tGeneric type\n";

        if (!m_mappings.has_value()) {
            std::cerr << "\t\tNo mappings provided\n";
            return std::nullopt;
        }

        const auto &genericName = typeRef.nameStr();
        const auto mapping = m_typeMapper.getMapping(genericName);

        if (mapping == nullptr) {
            auto errorMessage =
                    QString("No mapping found in parent '%1' for generic type '%2'").arg(m_parentName).arg(genericName);
            throw TranslationException(std::move(errorMessage));
            return std::nullopt;
        }

        concreteTypeRef = m_genericTypeCreator.createTypeForGeneric(mapping, genericName);
        std::cerr << "\t\tCreating concrete type " << concreteTypeRef.nameStr().toStdString() << '\n';

        if (!dimensions.empty()) {
            concreteTypeRef = DataTypeTranslationHelper::createArrayType(m_mainContext, concreteTypeRef, dimensions);
        }
    } else {
        std::cerr << "\t\tInterface local type\n";
        if (!dimensions.empty()) {
            concreteTypeRef =
                    DataTypeTranslationHelper::createArrayType(m_interfaceContext, concreteTypeRef, dimensions);
        }
    }

    return concreteTypeRef;
}

bool InterfaceTypeCreatorContext::isTypeGeneric(const seds::model::DataTypeRef &typeRef)
{
    if (typeRef.packageStr()) {
        return false;
    }

    const auto found = std::find_if(m_genericTypes.begin(), m_genericTypes.end(),
            [&](const auto genericType) { return genericType->nameStr() == typeRef.nameStr(); });

    return (found != m_genericTypes.end());
}

void InterfaceTypeCreatorContext::debugPrint() const
{
    std::cerr << "\tMain location: " << m_mainContext.packageName().toStdString() << '\n';
    std::cerr << "\tInterface location: " << m_interfaceContext.packageName().toStdString() << '\n';
    std::cerr << "\tGeneric type count: " << m_genericTypes.size() << '\n';
    std::cerr << "\tParent name: " << m_parentName.toStdString() << '\n';
    std::cerr << "\t==========\n";
}

} // namespace conversion::asn1::translator
