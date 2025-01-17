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

#include "constants.h"
#include "datatypetranslationhelper.h"

#include <asn1library/asn1/acnsequencecomponent.h>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsOptions/options.h>

using conversion::UnhandledValueException;
using conversion::seds::SedsOptions;
using conversion::translator::TranslationException;
using seds::model::CommandArgumentMode;
using seds::model::GenericType;
using seds::model::GenericTypeMap;
using seds::model::GenericTypeMapSet;
using seds::model::InterfaceCommandMode;

namespace conversion::asn1::translator::seds {

InterfaceTypeCreator::InterfaceTypeCreator(const Options &options)
    : m_options(options)
{
}

void InterfaceTypeCreator::createTypes(
        const ::seds::model::InterfaceDeclaration &interfaceDeclaration, Context &context)
{
    doCreateTypes(&interfaceDeclaration, "", std::nullopt, context, context);
}

void InterfaceTypeCreator::createTypes(const ::seds::model::Interface &interface, Context &context)
{
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

void InterfaceTypeCreator::doCreateTypes(const ::seds::model::InterfaceDeclaration *interfaceDeclaration,
        QString parentName, const std::optional<::seds::model::GenericTypeMapSet> &mappings, Context &mainContext,
        Context &interfaceContext)
{
    InterfaceTypeCreatorContext typeCreatorContext(
            mainContext, interfaceContext, interfaceDeclaration, parentName, mappings);

    const auto &parameters = interfaceDeclaration->parameters();
    for (const auto &parameter : parameters) {
        createTypesForParameter(parameter, typeCreatorContext);
    }

    const auto &commands = interfaceDeclaration->commands();
    for (const auto &command : commands) {
        createTypesForCommand(command, interfaceDeclaration->nameStr(), typeCreatorContext);
    }

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
        const ::seds::model::InterfaceParameter &parameter, InterfaceTypeCreatorContext &typeCreatorContext)
{
    const auto parameterTypeRef = parameter.type();
    const auto &parameterDimensions = parameter.arrayDimensions();

    typeCreatorContext.handleType(parameterTypeRef, parameterDimensions);
}

void InterfaceTypeCreator::createTypesForCommand(const ::seds::model::InterfaceCommand &command,
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
        const ::seds::model::InterfaceCommand &command, InterfaceTypeCreatorContext &typeCreatorContext)
{
    const auto &arguments = command.arguments();
    for (const auto &argument : arguments) {
        const auto argumentTypeRef = argument.type();
        const auto &argumentDimensions = argument.arrayDimensions();

        typeCreatorContext.handleType(argumentTypeRef, argumentDimensions);
    }
}

void InterfaceTypeCreator::createTypesForAsyncCommand(const ::seds::model::InterfaceCommand &command,
        const QString &interfaceDeclarationName, InterfaceTypeCreatorContext &typeCreatorContext)
{
    // Process command based on its arguments
    switch (command.argumentsCombination()) {
    case ::seds::model::ArgumentsCombination::InOnly: {
        // In arguments are 'native', so they are handled as-is
        createAsyncCommandBundledType(
                command, interfaceDeclarationName, CommandArgumentMode::In, false, typeCreatorContext);
        if (m_options.isSet(SedsOptions::enableFailureReporting)) {
            createAsyncCommandFailureReportingBundledType(command, interfaceDeclarationName, typeCreatorContext);
        }
    } break;
    case ::seds::model::ArgumentsCombination::OutOnly: {
        // Out arguments aren't supported by TASTE sporadic interface.
        // We cannot change the argument direction, so we switch interface type (provided <-> required)
        const auto createFailureComponent = m_options.isSet(SedsOptions::enableFailureReporting);
        createAsyncCommandBundledType(command, interfaceDeclarationName, CommandArgumentMode::Out,
                createFailureComponent, typeCreatorContext);
    } break;
    case ::seds::model::ArgumentsCombination::InAndNotify: {
        // InAndNotify arguments are separated onto two interfaces
        // In arguments - as-is
        createAsyncCommandBundledType(
                command, interfaceDeclarationName, CommandArgumentMode::In, false, typeCreatorContext);
        // Notify arguments - switched interface type (provided <-> required)
        const auto createFailureComponent = m_options.isSet(SedsOptions::enableFailureReporting);
        createAsyncCommandBundledType(command, interfaceDeclarationName, CommandArgumentMode::Notify,
                createFailureComponent, typeCreatorContext);
    } break;
    case ::seds::model::ArgumentsCombination::NoArgs: {
        if (m_options.isSet(SedsOptions::enableFailureReporting)) {
            createAsyncCommandFailureReportingBundledType(command, interfaceDeclarationName, typeCreatorContext);
        }
        break;
    }
    case ::seds::model::ArgumentsCombination::NotifyOnly:
    case ::seds::model::ArgumentsCombination::InAndOut:
    case ::seds::model::ArgumentsCombination::OutAndNotify:
    case ::seds::model::ArgumentsCombination::All: {
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

void InterfaceTypeCreator::createAsyncCommandBundledType(const ::seds::model::InterfaceCommand &command,
        const QString &interfaceDeclarationName, const CommandArgumentMode requestedArgumentMode,
        const bool createFailureComponent, InterfaceTypeCreatorContext &typeCreatorContext)
{
    const auto commandName = Escaper::escapeAsn1TypeName(command.nameStr());
    const auto isGeneric = typeCreatorContext.isCommandGeneric(command);
    auto parentName = isGeneric ? typeCreatorContext.parentName() : interfaceDeclarationName;
    const auto escapedParentName = parentName.isEmpty() ? parentName : Escaper::escapeAsn1TypeName(parentName);

    const auto bundledTypeName = isGeneric
            ? DataTypeTranslationHelper::buildBundledTypeName(escapedParentName, commandName, requestedArgumentMode)
            : DataTypeTranslationHelper::buildBundledTypeName(escapedParentName, commandName, requestedArgumentMode);

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

    if (createFailureComponent) {
        createAsyncCommandFailureReportingBundledTypeComponent(bundledType.get(), context, typeCreatorContext);
    }

    if (isConcrete) {
        context.addAsn1Type(std::move(bundledType), nullptr);
    }
}

bool InterfaceTypeCreator::createAsyncCommandBundledTypeComponent(const ::seds::model::CommandArgument &argument,
        Asn1Acn::Types::Sequence *bundledType, const std::optional<QString> &determinantArgumentName,
        Context &bundledTypeContext, InterfaceTypeCreatorContext &typeCreatorContext)
{
    const auto argumentTypeRef = argument.type();
    const auto &argumentDimensions = argument.arrayDimensions();

    const auto argumentName = Escaper::escapeAsn1FieldName(argument.nameStr());
    const auto argumentConcreteTypeRef = typeCreatorContext.handleType(argumentTypeRef, argumentDimensions);

    if (!argumentConcreteTypeRef.has_value()) {
        return false;
    }

    const auto isTypeGeneric = typeCreatorContext.isTypeGeneric(argumentTypeRef);

    if (isTypeGeneric) {
        auto &typeContext = typeCreatorContext.mainContext();

        auto argumentType = typeContext.findAsn1Type(*argumentConcreteTypeRef)->clone();
        const auto isArgumentChoice = argumentType->typeEnum() == Asn1Acn::Types::Type::ASN1Type::CHOICE;

        DataTypeTranslationHelper::removeConstraints(argumentType.get());

        auto sequenceComponentType = std::make_unique<Asn1Acn::Types::UserdefinedType>(argumentType->identifier(), "");
        sequenceComponentType->setType(std::move(argumentType));

        if (argumentName == determinantArgumentName) {
            auto sequenceComponent = std::make_unique<Asn1Acn::AcnSequenceComponent>(
                    argumentName, argumentName, std::move(sequenceComponentType));

            bundledType->addComponent(std::move(sequenceComponent));
        } else {
            auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(argumentName, argumentName, false,
                    std::nullopt, "", Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
                    std::move(sequenceComponentType));

            if (determinantArgumentName.has_value() && isArgumentChoice) {
                sequenceComponent->addAcnParameter(*determinantArgumentName);
            }

            bundledType->addComponent(std::move(sequenceComponent));
        }
    } else {
        auto &typeContext = typeCreatorContext.interfaceContext();

        auto argumentType = typeContext.findAsn1Type(*argumentConcreteTypeRef)->clone();
        bundledTypeContext.importType(typeContext.definitionsName(), argumentType->identifier());

        DataTypeTranslationHelper::removeConstraints(argumentType.get());

        auto sequenceComponentType = std::make_unique<Asn1Acn::Types::UserdefinedType>(argumentType->identifier(), "");
        sequenceComponentType->setType(std::move(argumentType));

        auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(argumentName, argumentName, false,
                std::nullopt, "", Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
                std::move(sequenceComponentType));

        bundledType->addComponent(std::move(sequenceComponent));
    }

    return true;
}

void InterfaceTypeCreator::createAsyncCommandFailureReportingBundledType(const ::seds::model::InterfaceCommand &command,
        const QString &interfaceDeclarationName, InterfaceTypeCreatorContext &typeCreatorContext)
{
    const auto commandName = Escaper::escapeAsn1TypeName(command.nameStr());
    const auto &parentName = interfaceDeclarationName;
    const auto escapedParentName = parentName.isEmpty() ? parentName : Escaper::escapeAsn1TypeName(parentName);

    const auto bundledTypeName = DataTypeTranslationHelper::buildBundledTypeName(
            escapedParentName, commandName, CommandArgumentMode::Notify);

    auto &context = typeCreatorContext.interfaceContext();

    if (context.hasAsn1Type(bundledTypeName)) {
        return;
    }

    auto bundledType = std::make_unique<Asn1Acn::Types::Sequence>(bundledTypeName);

    createAsyncCommandFailureReportingBundledTypeComponent(bundledType.get(), context, typeCreatorContext);

    context.addAsn1Type(std::move(bundledType), nullptr);
}

void InterfaceTypeCreator::createAsyncCommandFailureReportingBundledTypeComponent(Asn1Acn::Types::Sequence *bundledType,
        Context &bundledTypeContext, InterfaceTypeCreatorContext &typeCreatorContext)
{
    if (!m_options.isSet(SedsOptions::failureReportingType)) {
        throw TranslationException(
                "SEDS failure reporting feature was used but no ASN.1 type for failure parameter was specified");
    }

    const auto &argumentName = Constants::failureParamName;
    const auto argumentTypeName = m_options.value(SedsOptions::failureReportingType).value();

    auto &typeContext = typeCreatorContext.interfaceContext();

    const auto argumentType = typeContext.findAsn1Type(argumentTypeName);
    bundledTypeContext.importType(typeContext.definitionsName(), argumentType->identifier());

    auto sequenceComponentType = std::make_unique<Asn1Acn::Types::UserdefinedType>(argumentType->identifier(), "");
    sequenceComponentType->setType(argumentType->clone());

    auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(argumentName, argumentName, false,
            std::nullopt, "", Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
            std::move(sequenceComponentType));

    bundledType->addComponent(std::move(sequenceComponent));
}

} // namespace conversion::asn1::translator::seds
