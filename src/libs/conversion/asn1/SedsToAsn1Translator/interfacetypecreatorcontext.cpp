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

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>

using conversion::translator::TranslationException;

namespace conversion::asn1::translator::seds {

InterfaceTypeCreatorContext::InterfaceTypeCreatorContext(Context &mainContext, Context &interfaceContext,
        const ::seds::model::InterfaceDeclaration *interfaceDeclaration, QString parentName,
        const std::optional<::seds::model::GenericTypeMapSet> &mappings)
    : m_mainContext(mainContext)
    , m_interfaceContext(interfaceContext)
    , m_parentName(std::move(parentName))
    , m_mappings(mappings)
    , m_typeMapper(m_mainContext, m_parentName)
    , m_genericTypeCreator(m_mainContext, m_parentName, m_typeMapper)
{
    collectGenericTypes(interfaceDeclaration);

    if (m_mappings.has_value()) {
        m_typeMapper.addMappings(*m_mappings);
    }
}

std::optional<::seds::model::DataTypeRef> InterfaceTypeCreatorContext::handleType(
        const ::seds::model::DataTypeRef &typeRef, const std::vector<::seds::model::DimensionSize> &dimensions)
{
    const auto isGeneric = isTypeGeneric(typeRef);

    if (isGeneric) {
        if (!m_mappings.has_value()) {
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

        auto concreteTypeRef = m_genericTypeCreator.createTypeForGeneric(mapping, genericName);

        return DataTypeTranslationHelper::createArrayType(m_mainContext, concreteTypeRef, dimensions);
    } else {
        return DataTypeTranslationHelper::createArrayType(m_interfaceContext, typeRef, dimensions);
    }
}

Context &InterfaceTypeCreatorContext::mainContext() const
{
    return m_mainContext;
}

Context &InterfaceTypeCreatorContext::interfaceContext() const
{
    return m_interfaceContext;
}

const QString &InterfaceTypeCreatorContext::parentName() const
{
    return m_parentName;
}

std::optional<QString> InterfaceTypeCreatorContext::findDeterminantArgument(
        const std::vector<::seds::model::CommandArgument> &arguments)
{
    const auto &determinantName = m_typeMapper.determinantName();

    if (!determinantName.has_value()) {
        return std::nullopt;
    }

    const auto foundArgument =
            std::find_if(arguments.begin(), arguments.end(), [&](const ::seds::model::CommandArgument &argument) {
                return argument.type().nameStr() == determinantName;
            });

    if (foundArgument == arguments.end()) {
        return std::nullopt;
    } else {
        return Escaper::escapeAsn1FieldName(foundArgument->nameStr());
    }
}

bool InterfaceTypeCreatorContext::isTypeGeneric(const ::seds::model::DataTypeRef &typeRef)
{
    if (typeRef.packageStr()) {
        return false;
    }

    const auto found = std::find_if(m_genericTypes.begin(), m_genericTypes.end(),
            [&](const auto genericType) { return genericType->nameStr() == typeRef.nameStr(); });

    return (found != m_genericTypes.end());
}

bool InterfaceTypeCreatorContext::isCommandGeneric(const ::seds::model::InterfaceCommand &command)
{
    const auto &arguments = command.arguments();
    for (const auto &argument : arguments) {
        const auto &argumentTypeRef = argument.type();
        if (isTypeGeneric(argumentTypeRef)) {
            return true;
        }
    }

    return false;
}

void InterfaceTypeCreatorContext::collectGenericTypes(const ::seds::model::InterfaceDeclaration *interfaceDeclaration)
{
    doCollectGenericTypes(interfaceDeclaration, m_interfaceContext);

    const auto it = std::unique(m_genericTypes.begin(), m_genericTypes.end(),
            [](const auto lhs, const auto rhs) { return lhs->nameStr() == rhs->nameStr(); });
    if (it != m_genericTypes.end()) {
        auto errorMessage = QString("Interface declaration '%1' has a non-unique generic types.")
                                    .arg(interfaceDeclaration->nameStr());
        throw TranslationException(std::move(errorMessage));
    }
}

void InterfaceTypeCreatorContext::doCollectGenericTypes(
        const ::seds::model::InterfaceDeclaration *interfaceDeclaration, Context &interfaceContext)
{
    const auto &interfaceGenericTypes = interfaceDeclaration->genericTypes();

    for (const auto &genericType : interfaceGenericTypes) {
        m_genericTypes.push_back(&genericType);
    }

    for (const auto &baseInterface : interfaceDeclaration->baseInterfaces()) {
        const auto &baseInterfaceTypeRef = baseInterface.type();
        const auto &baseInterfaceDeclaration = interfaceContext.findInterfaceDeclaration(baseInterfaceTypeRef);

        if (baseInterfaceTypeRef.packageStr()) {
            auto baseInterfaceContext = interfaceContext.cloneForPackage(*baseInterfaceTypeRef.packageStr());
            doCollectGenericTypes(baseInterfaceDeclaration, baseInterfaceContext);
        } else {
            doCollectGenericTypes(baseInterfaceDeclaration, interfaceContext);
        }
    }
}

} // namespace conversion::asn1::translator::seds
