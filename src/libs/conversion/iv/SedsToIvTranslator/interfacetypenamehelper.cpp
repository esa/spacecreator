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

#include "interfacetypenamehelper.h"

#include "translator.h"

#include <conversion/asn1/SedsToAsn1Translator/datatypetranslationhelper.h>
#include <conversion/common/translation/exceptions.h>

namespace conversion::iv::translator {

using asn1::translator::DataTypeTranslationHelper;
using conversion::translator::TranslationException;

InterfaceTypeNameHelper::InterfaceTypeNameHelper(Context context, QString parentName,
        const seds::model::InterfaceDeclaration *interfaceDeclaration,
        const std::vector<seds::model::Package> &sedsPackages)
    : m_context(context)
    , m_parentName(std::move(parentName))
    , m_interfaceDeclarationName(interfaceDeclaration->nameStr())
    , m_sedsPackages(sedsPackages)
{
    collectGenericTypes(interfaceDeclaration);
}

QString InterfaceTypeNameHelper::handleTypeName(
        const seds::model::DataTypeRef &typeRef, const std::vector<seds::model::DimensionSize> &dimensions) const
{
    auto typeName = typeRef.nameStr();

    const auto isGeneric = isTypeGeneric(typeRef);

    if (isGeneric) {
        const auto &genericName = typeRef.nameStr();
        typeName = DataTypeTranslationHelper::buildConcreteTypeName(m_parentName, genericName);
    }

    if (!dimensions.empty()) {
        typeName = DataTypeTranslationHelper::buildArrayTypeName(typeName, dimensions);
    }

    return typeName;
}

QString InterfaceTypeNameHelper::handleAsyncCommandTypeName(const seds::model::InterfaceCommand &command,
        const seds::model::CommandArgumentMode requestedArgumentMode) const
{
    const auto commandName = Escaper::escapeAsn1TypeName(command.nameStr());

    const auto isGeneric = isCommandGeneric(command);

    if (isGeneric) {
        return DataTypeTranslationHelper::buildBundledTypeName(m_parentName, commandName, requestedArgumentMode);
    } else {
        return DataTypeTranslationHelper::buildBundledTypeName(
                m_interfaceDeclarationName, commandName, requestedArgumentMode);
    }
}

bool InterfaceTypeNameHelper::isCommandGeneric(const seds::model::InterfaceCommand &command) const
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

bool InterfaceTypeNameHelper::isTypeGeneric(const seds::model::DataTypeRef &typeRef) const
{
    if (typeRef.packageStr()) {
        return false;
    }

    const auto found = std::find_if(m_genericTypes.begin(), m_genericTypes.end(),
            [&](const auto genericType) { return genericType->nameStr() == typeRef.nameStr(); });

    return (found != m_genericTypes.end());
}

void InterfaceTypeNameHelper::collectGenericTypes(const seds::model::InterfaceDeclaration *interfaceDeclaration)
{
    doCollectGenericTypes(interfaceDeclaration, m_context);

    const auto it = std::unique(m_genericTypes.begin(), m_genericTypes.end(),
            [](const auto lhs, const auto rhs) { return lhs->nameStr() == rhs->nameStr(); });
    if (it != m_genericTypes.end()) {
        auto errorMessage = QString("Interface declaration '%1' has a non-unique generic types.")
                                    .arg(interfaceDeclaration->nameStr());
        throw TranslationException(std::move(errorMessage));
    }
}

void InterfaceTypeNameHelper::doCollectGenericTypes(
        const seds::model::InterfaceDeclaration *interfaceDeclaration, Context &context)
{
    const auto &interfaceGenericTypes = interfaceDeclaration->genericTypes();

    for (const auto &genericType : interfaceGenericTypes) {
        m_genericTypes.push_back(&genericType);
    }

    for (const auto &baseInterface : interfaceDeclaration->baseInterfaces()) {
        const auto &baseInterfaceTypeRef = baseInterface.type();

        auto baseInterfaceContext = context;
        if (baseInterfaceTypeRef.packageStr()) {
            const auto baseInterfacePackage =
                    SedsToIvTranslator::getSedsPackage(*baseInterfaceTypeRef.packageStr(), m_sedsPackages);
            baseInterfaceContext = { baseInterfacePackage, nullptr };
        }

        const auto &baseInterfaceDeclaration =
                baseInterfaceContext.findInterfaceDeclaration(baseInterfaceTypeRef.nameStr());

        doCollectGenericTypes(baseInterfaceDeclaration, baseInterfaceContext);
    }
}

} // namespace conversion::iv::translator
