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

#include "interfacetypecreator.h"

#include "datatypetranslationhelper.h"
#include "generictypecreator.h"

#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <iostream>

using conversion::translator::TranslationException;
using seds::model::GenericType;
using seds::model::GenericTypeMap;
using seds::model::GenericTypeMapSet;
using seds::model::InterfaceCommandMode;

namespace conversion::asn1::translator {

void InterfaceTypeCreator::createTypes(const seds::model::InterfaceDeclaration &interfaceDeclaration, Context &context)
{
    std::cerr << "Handling interface declaration: " << interfaceDeclaration.nameStr().toStdString() << '\n';

    doCreateTypes(&interfaceDeclaration, "", std::nullopt, context, context);

    std::cerr << '\n';
}

void InterfaceTypeCreator::createTypes(const seds::model::Interface &interface, Context &context)
{
    std::cerr << "Handling interface implementation: " << interface.nameStr().toStdString() << " of type "
              << interface.type().value().pathStr().toStdString() << '\n';

    const auto &interfaceTypeRef = interface.type();
    const auto &interfaceDeclaration = context.findInterfaceDeclaration(interfaceTypeRef);
    const auto parentName = QString("%1-%2").arg(context.componentName()).arg(interface.nameStr());
    const auto &mappings = interface.genericTypeMapSet();

    if (interfaceTypeRef.packageStr()) {
        auto interfaceContext = context.cloneForPackage(*interfaceTypeRef.packageStr());
        doCreateTypes(interfaceDeclaration, parentName, mappings, context, interfaceContext);
    } else {
        doCreateTypes(interfaceDeclaration, parentName, mappings, context, context);
    }
}

void InterfaceTypeCreator::doCreateTypes(const seds::model::InterfaceDeclaration *interfaceDeclaration,
        QString parentName, const std::optional<seds::model::GenericTypeMapSet> &mappings, Context &mainContext,
        Context &interfaceContext)
{
    std::cerr << "\tTranslating interface declaration: " << interfaceDeclaration->nameStr().toStdString() << '\n';

    const auto genericTypes = collectGenericTypes(interfaceDeclaration, interfaceContext);

    InterfaceTypeCreatorContext typeCreatorContext(mainContext, interfaceContext, parentName, genericTypes, mappings);
    typeCreatorContext.debugPrint();

    const auto &parameters = interfaceDeclaration->parameters();
    for (const auto &parameter : parameters) {
        createTypesForParameter(parameter, typeCreatorContext);
    }

    const auto &commands = interfaceDeclaration->commands();
    for (const auto &command : commands) {
    }

    std::cerr << '\n';

    for (const auto &baseInterface : interfaceDeclaration->baseInterfaces()) {
        const auto &baseInterfaceTypeRef = baseInterface.type();
        const auto &baseInterfaceDeclaration = interfaceContext.findInterfaceDeclaration(baseInterfaceTypeRef);

        auto &baseInterfaceParentName =
                baseInterface.genericTypeMapSet() ? interfaceDeclaration->nameStr() : parentName;
        auto &baseInterfaceMainContext = baseInterface.genericTypeMapSet() ? interfaceContext : mainContext;
        auto &baseInterfaceMappings = baseInterface.genericTypeMapSet() ? baseInterface.genericTypeMapSet() : mappings;

        if (baseInterfaceTypeRef.packageStr()) {
            auto baseInterfaceContext = interfaceContext.cloneForPackage(*baseInterfaceTypeRef.packageStr());
            doCreateTypes(baseInterfaceDeclaration, baseInterfaceParentName, baseInterfaceMappings,
                    baseInterfaceMainContext, baseInterfaceContext);
        } else {
            doCreateTypes(baseInterfaceDeclaration, baseInterfaceParentName, baseInterfaceMappings,
                    baseInterfaceMainContext, interfaceContext);
        }
    }
}

void InterfaceTypeCreator::createTypesForParameter(
        const seds::model::InterfaceParameter &parameter, InterfaceTypeCreatorContext &typeCreatorContext)
{
    const auto parameterTypeRef = parameter.type();
    std::cerr << "\tParameter '" << parameter.nameStr().toStdString() << "' type "
              << parameterTypeRef.value().pathStr().toStdString() << '\n';

    typeCreatorContext.handleType(parameterTypeRef, parameter.arrayDimensions());
}

std::vector<const GenericType *> InterfaceTypeCreator::collectGenericTypes(
        const seds::model::InterfaceDeclaration *interfaceDeclaration, Context &interfaceContext) const
{
    std::vector<const GenericType *> genericTypes;

    doCollectGenericTypes(interfaceDeclaration, interfaceContext, genericTypes);

    const auto it = std::unique(genericTypes.begin(), genericTypes.end(),
            [](const auto lhs, const auto rhs) { return lhs->nameStr() == rhs->nameStr(); });
    if (it != genericTypes.end()) {
        auto errorMessage = QString("Interface declaration '%1' has a non-unique generic types.")
                                    .arg(interfaceDeclaration->nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    return genericTypes;
}

void InterfaceTypeCreator::doCollectGenericTypes(const seds::model::InterfaceDeclaration *interfaceDeclaration,
        Context &interfaceContext, std::vector<const GenericType *> &genericTypes) const
{
    const auto &interfaceGenericTypes = interfaceDeclaration->genericTypes();

    for (const auto &genericType : interfaceGenericTypes) {
        genericTypes.push_back(&genericType);
    }

    for (const auto &baseInterface : interfaceDeclaration->baseInterfaces()) {
        const auto &baseInterfaceTypeRef = baseInterface.type();
        const auto &baseInterfaceDeclaration = interfaceContext.findInterfaceDeclaration(baseInterfaceTypeRef);

        if (baseInterfaceTypeRef.packageStr()) {
            auto baseInterfaceContext = interfaceContext.cloneForPackage(*baseInterfaceTypeRef.packageStr());
            doCollectGenericTypes(baseInterfaceDeclaration, baseInterfaceContext, genericTypes);
        } else {
            doCollectGenericTypes(baseInterfaceDeclaration, interfaceContext, genericTypes);
        }
    }
}

bool InterfaceTypeCreator::isInterfaceDeclarationGeneric(const seds::model::InterfaceDeclaration *interfaceDeclaration,
        const std::vector<const GenericType *> &genericTypes)
{
    if (genericTypes.empty()) {
        return false;
    }

    const auto &parameters = interfaceDeclaration->parameters();
    for (const auto &parameter : parameters) {
        if (isParameterGeneric(parameter, genericTypes)) {
            return true;
        }
    }

    const auto &commands = interfaceDeclaration->commands();
    for (const auto &command : commands) {
        if (isCommandGeneric(command, genericTypes)) {
            return true;
        }
    }

    return false;
}

bool InterfaceTypeCreator::isParameterGeneric(
        const seds::model::InterfaceParameter &parameter, const std::vector<const GenericType *> &genericTypes)
{
    const auto &parameterTypeRef = parameter.type();
    return isTypeGeneric(parameterTypeRef, genericTypes);
}

bool InterfaceTypeCreator::isCommandGeneric(
        const seds::model::InterfaceCommand &command, const std::vector<const GenericType *> &genericTypes)
{
    const auto &arguments = command.arguments();
    for (const auto &argument : arguments) {
        const auto &argumentTypeRef = argument.type();
        if (isTypeGeneric(argumentTypeRef, genericTypes)) {
            return true;
        }
    }

    return false;
}

bool InterfaceTypeCreator::isTypeGeneric(
        const seds::model::DataTypeRef &argumentType, const std::vector<const GenericType *> &genericTypes)
{
    if (argumentType.packageStr()) {
        return false;
    }

    const auto found = std::find_if(genericTypes.begin(), genericTypes.end(),
            [&](const auto genericType) { return genericType->nameStr() == argumentType.nameStr(); });

    return (found != genericTypes.end());
}

} // namespace conversion::asn1::translator
