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

#include <asn1library/asn1/acnsequencecomponent.h>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/translation/exceptions.h>
#include <iostream>

using conversion::UnhandledValueException;
using conversion::translator::TranslationException;
using seds::model::GenericType;
using seds::model::GenericTypeMap;
using seds::model::GenericTypeMapSet;
using seds::model::InterfaceCommandMode;

namespace conversion::asn1::translator {

void InterfaceTypeCreator::createTypes(const seds::model::InterfaceDeclaration &interfaceDeclaration, Context &context)
{
    /* std::cerr << "Handling interface declaration: " << interfaceDeclaration.nameStr().toStdString() << '\n'; */

    doCreateTypes(&interfaceDeclaration, "", std::nullopt, context, context);

    /* std::cerr << '\n'; */
}

void InterfaceTypeCreator::createTypes(const seds::model::Interface &interface, Context &context)
{
    /* std::cerr << "Handling interface implementation: " << interface.nameStr().toStdString() << " of type " */
    /* << interface.type().value().pathStr().toStdString() << '\n'; */

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
    /* std::cerr << "\tTranslating interface declaration: " << interfaceDeclaration->nameStr().toStdString() << '\n'; */

    const auto genericTypes = collectGenericTypes(interfaceDeclaration, interfaceContext);

    InterfaceTypeCreatorContext typeCreatorContext(mainContext, interfaceContext, parentName, genericTypes, mappings);
    /* typeCreatorContext.debugPrint(); */

    const auto &parameters = interfaceDeclaration->parameters();
    for (const auto &parameter : parameters) {
        createTypesForParameter(parameter, typeCreatorContext);
    }

    const auto &commands = interfaceDeclaration->commands();
    for (const auto &command : commands) {
        createTypesForCommand(command, interfaceDeclaration->nameStr(), typeCreatorContext);
    }

    /* std::cerr << '\n'; */

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
    const auto &parameterDimensions = parameter.arrayDimensions();

    /* std::cerr << "\tParameter '" << parameter.nameStr().toStdString() << "' type " */
    /*           << parameterTypeRef.value().pathStr().toStdString() << '\n'; */

    typeCreatorContext.handleType(parameterTypeRef, parameterDimensions);
}

void InterfaceTypeCreator::createTypesForCommand(const seds::model::InterfaceCommand &command,
        const QString &interfaceDeclarationName, InterfaceTypeCreatorContext &typeCreatorContext)
{
    switch (command.mode()) {
    case InterfaceCommandMode::Sync:
        createTypesForSyncCommand(command, typeCreatorContext);
        break;
    case InterfaceCommandMode::Async:
        createTypesForAsyncCommand(command, interfaceDeclarationName, typeCreatorContext);
        break;
    default:
        throw UnhandledValueException("InterfaceCommandMode");
    }
}

void InterfaceTypeCreator::createTypesForSyncCommand(
        const seds::model::InterfaceCommand &command, InterfaceTypeCreatorContext &typeCreatorContext)
{
    /* std::cerr << "\tSync command '" << command.nameStr().toStdString() << "'\n"; */

    const auto &arguments = command.arguments();
    for (const auto &argument : arguments) {
        const auto argumentTypeRef = argument.type();
        const auto &argumentDimensions = argument.arrayDimensions();

        /* std::cerr << "\t\tArgument '" << argument.nameStr().toStdString() << "' type " */
        /*           << argumentTypeRef.value().pathStr().toStdString() << '\n'; */

        typeCreatorContext.handleType(argumentTypeRef, argumentDimensions);
    }
}

void InterfaceTypeCreator::createTypesForAsyncCommand(const seds::model::InterfaceCommand &command,
        const QString &interfaceDeclarationName, InterfaceTypeCreatorContext &typeCreatorContext)
{
    /* std::cerr << "\tAsync command '" << command.nameStr().toStdString() << "'\n"; */

    // Process command based on its commands
    switch (command.argumentsCombination()) {
    case seds::model::ArgumentsCombination::InOnly: {
        // In arguments are 'native', so they are handles as-is
        createAsyncCommandBundledType(
                command, interfaceDeclarationName, seds::model::CommandArgumentMode::In, typeCreatorContext);
    } break;
    case seds::model::ArgumentsCombination::OutOnly: {
        // Out arguments aren't supported by TASTE sporadic interface.
        // We cannot change the argument direction, so we switch interface type (provided <-> required)
        createAsyncCommandBundledType(
                command, interfaceDeclarationName, seds::model::CommandArgumentMode::Out, typeCreatorContext);
    } break;
    case seds::model::ArgumentsCombination::InAndNotify: {
        // InAndNotify arguments are separated onto two interfaces
        // In arguments - as-is
        createAsyncCommandBundledType(
                command, interfaceDeclarationName, seds::model::CommandArgumentMode::In, typeCreatorContext);
        // Notify arguments - switched interface type (provided <-> required)
        createAsyncCommandBundledType(
                command, interfaceDeclarationName, seds::model::CommandArgumentMode::Notify, typeCreatorContext);
    } break;
    case seds::model::ArgumentsCombination::NoArgs: {
        // No arguments, no problems
        break;
    }
    case seds::model::ArgumentsCombination::NotifyOnly:
    case seds::model::ArgumentsCombination::InAndOut:
    case seds::model::ArgumentsCombination::OutAndNotify:
    case seds::model::ArgumentsCombination::All: {
        const auto message = QString(
                "Interface command arguments combination '%1' is not supported for TASTE InterfaceView async interface")
                                     .arg(argumentsCombinationToString(command.argumentsCombination()));
        throw TranslationException(message);
    } break;
    default:
        throw UnhandledValueException("ArgumentsCombination");
        break;
    }
}

void InterfaceTypeCreator::createAsyncCommandBundledType(const seds::model::InterfaceCommand &command,
        const QString &interfaceDeclarationName, const seds::model::CommandArgumentMode requestedArgumentMode,
        InterfaceTypeCreatorContext &typeCreatorContext)
{
    const auto commandName = Escaper::escapeAsn1TypeName(command.nameStr());
    const auto isGeneric = typeCreatorContext.isCommandGeneric(command);

    const auto bundledTypeName = isGeneric
            ? DataTypeTranslationHelper::buildBundledTypeName(
                      typeCreatorContext.parentName(), commandName, requestedArgumentMode)
            : DataTypeTranslationHelper::buildBundledTypeName(
                      interfaceDeclarationName, commandName, requestedArgumentMode);

    /* std::cerr << "\t\tCreating bundled type " << bundledTypeName.toStdString() << '\n'; */

    auto &context = isGeneric ? typeCreatorContext.mainContext() : typeCreatorContext.interfaceContext();

    if (context.hasAsn1Type(bundledTypeName)) {
        return;
    }

    auto bundledType = std::make_unique<Asn1Acn::Types::Sequence>(bundledTypeName);
    const auto determinantArgumentName = typeCreatorContext.findDeterminantArgument(command.arguments());

    auto isConcrete = true;

    for (const auto &argument : command.arguments()) {
        if (argument.mode() != requestedArgumentMode) {
            continue;
        }

        isConcrete = createAsyncCommandBundledTypeComponent(
                argument, bundledType.get(), determinantArgumentName, context, typeCreatorContext);

        if (!isConcrete) {
            break;
        }
    }

    if (isConcrete) {
        context.addAsn1Type(std::move(bundledType), nullptr);
    }
}

bool InterfaceTypeCreator::createAsyncCommandBundledTypeComponent(const seds::model::CommandArgument &argument,
        Asn1Acn::Types::Sequence *bundledType, const std::optional<QString> &determinantArgumentName,
        Context &bundledTypeContext, InterfaceTypeCreatorContext &typeCreatorContext)
{
    const auto argumentTypeRef = argument.type();
    const auto &argumentDimensions = argument.arrayDimensions();

    /* std::cerr << "\t\tArgument '" << argument.nameStr().toStdString() << "' type " */
    /*           << argumentTypeRef.value().pathStr().toStdString() << '\n'; */

    const auto argumentName = Escaper::escapeAsn1FieldName(argument.nameStr());
    const auto argumentConcreteTypeRef = typeCreatorContext.handleType(argumentTypeRef, argumentDimensions);

    if (!argumentConcreteTypeRef.has_value()) {
        return false;
    }

    const auto isTypeGeneric = typeCreatorContext.isTypeGeneric(argumentTypeRef);

    if (isTypeGeneric) {
        auto &typeContext = typeCreatorContext.mainContext();

        const auto argumentType = typeContext.findAsn1Type(*argumentConcreteTypeRef);

        auto sequenceComponentType = std::make_unique<Asn1Acn::Types::UserdefinedType>(argumentType->identifier(), "");
        sequenceComponentType->setType(argumentType->clone());

        if (argumentName == determinantArgumentName) {
            auto sequenceComponent = std::make_unique<Asn1Acn::AcnSequenceComponent>(
                    argumentName, argumentName, std::move(sequenceComponentType));

            bundledType->addComponent(std::move(sequenceComponent));
        } else {
            auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(argumentName, argumentName, false,
                    std::nullopt, "", Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
                    std::move(sequenceComponentType));

            if (determinantArgumentName.has_value()
                    && argumentType->typeEnum() == Asn1Acn::Types::Type::ASN1Type::CHOICE) {
                sequenceComponent->addAcnParameter(*determinantArgumentName);
            }

            bundledType->addComponent(std::move(sequenceComponent));
        }
    } else {
        auto &typeContext = typeCreatorContext.interfaceContext();

        const auto argumentType = typeContext.findAsn1Type(*argumentConcreteTypeRef);
        bundledTypeContext.importType(typeContext.definitionsName(), argumentType->identifier());

        auto sequenceComponentType = std::make_unique<Asn1Acn::Types::UserdefinedType>(argumentType->identifier(), "");
        sequenceComponentType->setType(argumentType->clone());

        std::unique_ptr<Asn1Acn::SequenceComponent> sequenceComponent =
                std::make_unique<Asn1Acn::AsnSequenceComponent>(argumentName, argumentName, false, std::nullopt, "",
                        Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
                        std::move(sequenceComponentType));

        bundledType->addComponent(std::move(sequenceComponent));
    }

    return true;
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
