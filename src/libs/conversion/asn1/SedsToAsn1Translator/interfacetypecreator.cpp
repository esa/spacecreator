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

InterfaceTypeCreatorContext::InterfaceTypeCreatorContext(Context &context, Context &interfaceContext, bool isGeneric,
        const QString &interfaceName, const QString &parentInterfaceName,
        const std::vector<const GenericType *> &genericTypes,
        const std::optional<seds::model::GenericTypeMapSet> &mappings)
    : m_context(context)
    , m_interfaceContext(interfaceContext)
    , m_isGeneric(isGeneric)
    , m_interfaceName(interfaceName)
    , m_parentInterfaceName(parentInterfaceName)
    , m_genericTypes(genericTypes)
    , m_typeMapper(m_context, m_parentInterfaceName)
    , m_genericTypeCreator(m_context, m_parentInterfaceName, m_typeMapper)
{
    if (mappings.has_value()) {
        m_typeMapper.addMappings(*mappings);
    }
}

Asn1Acn::Types::Type *InterfaceTypeCreatorContext::handleType(
        const seds::model::DataTypeRef &typeRef, const std::vector<seds::model::DimensionSize> &dimensions)
{
    if (m_isGeneric && isTypeGeneric(typeRef)) {
        const auto &genericName = typeRef.nameStr();
        const auto mapping = m_typeMapper.getMapping(genericName);

        auto genericTypeRef = m_genericTypeCreator.createTypeForGeneric(mapping, genericName);

        if (!dimensions.empty()) {
            if (mapping->concreteMappings.size() > 1) {
                auto errorMessage = QString("Array dimensions for alternate generic type '%1' for interface '%2' are "
                                            "not supported because of the ACN limitations")
                                            .arg(genericName)
                                            .arg(m_parentInterfaceName);
                throw TranslationException(std::move(errorMessage));
            }

            return DataTypeTranslationHelper::handleArrayType(m_context, genericTypeRef, dimensions);
        } else {
            return m_context.findAsn1Type(genericTypeRef);
        }
    } else {
        return DataTypeTranslationHelper::handleArrayType(m_interfaceContext, typeRef, dimensions);
    }
}

Context &InterfaceTypeCreatorContext::context() const
{
    return m_context;
}

Context &InterfaceTypeCreatorContext::interfaceContext() const
{
    return m_interfaceContext;
}

const QString &InterfaceTypeCreatorContext::interfaceName() const
{
    if (m_isGeneric) {
        return m_parentInterfaceName;
    } else {
        return m_interfaceName;
    }
}

bool InterfaceTypeCreatorContext::isTypeGeneric(const seds::model::DataTypeRef &typeRef) const
{
    if (typeRef.packageStr()) {
        return false;
    }

    const auto found = std::find_if(m_genericTypes.begin(), m_genericTypes.end(),
            [&](const auto genericType) { return genericType->nameStr() == typeRef.nameStr(); });

    return (found != m_genericTypes.end());
}

InterfaceTypeCreator::InterfaceTypeCreator(Context &context)
    : m_context(context)
{
}

void InterfaceTypeCreator::createTypes(const seds::model::InterfaceDeclaration &interfaceDeclaration)
{
    doCreateTypes(&interfaceDeclaration, m_context, m_context, "", std::nullopt);
    std::cerr << '\n';
}

void InterfaceTypeCreator::doCreateTypes(const seds::model::InterfaceDeclaration *interfaceDeclaration,
        Context &context, Context &interfaceContext, const QString &parentInterfaceName,
        const std::optional<GenericTypeMapSet> &mappings)
{
    const auto genericTypes = collectGenericTypes(interfaceContext, interfaceDeclaration);
    std::cerr << "Number of generic types: " << genericTypes.size() << '\n';

    bool isGeneric = isInterfaceGeneric(interfaceDeclaration, genericTypes);

    if (!isGeneric || mappings.has_value()) {
        InterfaceTypeCreatorContext typeCreatorContext(context, interfaceContext, isGeneric,
                interfaceDeclaration->nameStr(), parentInterfaceName, genericTypes, mappings);

        for (const auto &parameter : interfaceDeclaration->parameters()) {
            createTypesForParameter(parameter, typeCreatorContext);
        }

        for (const auto &command : interfaceDeclaration->commands()) {
            switch (command.mode()) {
            case InterfaceCommandMode::Sync:
                createTypesForSyncCommand(command, typeCreatorContext);
                break;
            case InterfaceCommandMode::Async:
                createTypesForAsyncCommand(command, typeCreatorContext);
                break;
            default:
                throw UnhandledValueException("InterfaceCommandMode");
            }
        }
    }

    for (const auto &baseInterface : interfaceDeclaration->baseInterfaces()) {
        const auto &baseInterfaceType = baseInterface.type();
        const auto baseInterfaceDeclaration = interfaceContext.findInterfaceDeclaration(baseInterfaceType);

        auto &mainContext = baseInterface.genericTypeMapSet() ? interfaceContext : context;
        const auto &baseInterfaceMappings =
                baseInterface.genericTypeMapSet() ? baseInterface.genericTypeMapSet() : mappings;

        if (baseInterfaceType.packageStr()) {
            const auto baseInterfacePackageName = *baseInterfaceType.packageStr();
            auto baseInterfaceContext = interfaceContext.cloneForPackage(baseInterfacePackageName);

            doCreateTypes(baseInterfaceDeclaration, mainContext, baseInterfaceContext, interfaceDeclaration->nameStr(),
                    baseInterfaceMappings);
        } else {
            doCreateTypes(baseInterfaceDeclaration, mainContext, interfaceContext, interfaceDeclaration->nameStr(),
                    baseInterfaceMappings);
        }
    }
}

void InterfaceTypeCreator::createTypesForParameter(
        const seds::model::InterfaceParameter &parameter, InterfaceTypeCreatorContext &typeCreatorContext)
{
    typeCreatorContext.handleType(parameter.type(), parameter.arrayDimensions());
}

void InterfaceTypeCreator::createTypesForSyncCommand(
        const seds::model::InterfaceCommand &command, InterfaceTypeCreatorContext &typeCreatorContext)
{
    for (const auto &argument : command.arguments()) {
        typeCreatorContext.handleType(argument.type(), argument.arrayDimensions());
    }
}

void InterfaceTypeCreator::createTypesForAsyncCommand(
        const seds::model::InterfaceCommand &command, InterfaceTypeCreatorContext &typeCreatorContext)
{
    const auto &interfaceName = typeCreatorContext.interfaceName();
    const auto commandName = Escaper::escapeAsn1TypeName(command.nameStr());

    // Process command based on its commands
    switch (command.argumentsCombination()) {
    case seds::model::ArgumentsCombination::InOnly: {
        // In arguments are 'native', so they are handles as-is
        const auto bundledTypeName = DataTypeTranslationHelper::buildBundledTypeName(interfaceName, commandName);
        createAsyncCommandBundledType(
                command, bundledTypeName, seds::model::CommandArgumentMode::In, typeCreatorContext);
    } break;
    case seds::model::ArgumentsCombination::OutOnly: {
        // Out arguments aren't supported by TASTE sporadic interface.
        // We cannot change the argument direction, so we switch interface type (provided <-> required)
        const auto bundledTypeName = DataTypeTranslationHelper::buildBundledTypeName(interfaceName, commandName);
        createAsyncCommandBundledType(
                command, bundledTypeName, seds::model::CommandArgumentMode::Out, typeCreatorContext);
    } break;
    case seds::model::ArgumentsCombination::InAndNotify: {
        // InAndNotify arguments are separated onto two interfaces
        // In arguments - as-is
        const auto inBundledTypeName =
                DataTypeTranslationHelper::buildBundledTypeName(interfaceName, commandName, "In");
        createAsyncCommandBundledType(
                command, inBundledTypeName, seds::model::CommandArgumentMode::In, typeCreatorContext);
        // Notify arguments - switched interface type (provided <-> required)
        const auto notifyBundledTypeName =
                DataTypeTranslationHelper::buildBundledTypeName(interfaceName, commandName, "Notify");
        createAsyncCommandBundledType(
                command, notifyBundledTypeName, seds::model::CommandArgumentMode::Notify, typeCreatorContext);
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
        const QString &bundledTypeName, const seds::model::CommandArgumentMode requestedArgumentMode,
        InterfaceTypeCreatorContext &typeCreatorContext)
{
    auto &context = typeCreatorContext.interfaceContext();

    if (context.hasAsn1Type(bundledTypeName)) {
        return;
    }

    auto bundledType = std::make_unique<Asn1Acn::Types::Sequence>(bundledTypeName);

    for (const auto &argument : command.arguments()) {
        if (argument.mode() != requestedArgumentMode) {
            continue;
        }

        createAsyncCommandBundledTypeComponent(argument, bundledType.get(), typeCreatorContext);
    }

    context.addAsn1Type(std::move(bundledType), nullptr);
}

void InterfaceTypeCreator::createAsyncCommandBundledTypeComponent(const seds::model::CommandArgument &argument,
        Asn1Acn::Types::Sequence *bundledType, InterfaceTypeCreatorContext &typeCreatorContext)
{
    const auto argumentName = Escaper::escapeAsn1FieldName(argument.nameStr());
    const auto argumentType = typeCreatorContext.handleType(argument.type(), argument.arrayDimensions());

    auto sequenceComponentType =
            std::make_unique<Asn1Acn::Types::UserdefinedType>(argumentType->identifier(), m_context.definitionsName());
    sequenceComponentType->setType(argumentType->clone());

    std::unique_ptr<Asn1Acn::SequenceComponent> sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            argumentName, argumentName, false, std::nullopt, "", Asn1Acn::AsnSequenceComponent::Presence::NotSpecified,
            Asn1Acn::SourceLocation(), std::move(sequenceComponentType));

    bundledType->addComponent(std::move(sequenceComponent));
}

std::vector<const GenericType *> InterfaceTypeCreator::collectGenericTypes(
        Context &interfaceContext, const seds::model::InterfaceDeclaration *interfaceDeclaration) const
{
    std::vector<const GenericType *> genericTypes;

    doCollectGenericTypes(interfaceContext, interfaceDeclaration, genericTypes);

    const auto it = std::unique(genericTypes.begin(), genericTypes.end(),
            [](const auto lhs, const auto rhs) { return lhs->nameStr() == rhs->nameStr(); });
    if (it != genericTypes.end()) {
        auto errorMessage = QString("Interface declaration '%1' has a non-unique generic types.")
                                    .arg(interfaceDeclaration->nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    return genericTypes;
}

void InterfaceTypeCreator::doCollectGenericTypes(Context &interfaceContext,
        const seds::model::InterfaceDeclaration *interfaceDeclaration,
        std::vector<const GenericType *> &genericTypes) const
{
    const auto &interfaceGenericTypes = interfaceDeclaration->genericTypes();

    for (const auto &genericType : interfaceGenericTypes) {
        genericTypes.push_back(&genericType);
    }

    for (const auto &baseInterface : interfaceDeclaration->baseInterfaces()) {
        const auto &baseInterfaceType = baseInterface.type();
        const auto &baseInterfaceDeclaration = interfaceContext.findInterfaceDeclaration(baseInterfaceType);

        if (baseInterfaceType.packageStr()) {
            const auto baseInterfacePackageName = *baseInterfaceType.packageStr();
            auto baseInterfaceContext = interfaceContext.cloneForPackage(baseInterfacePackageName);

            doCollectGenericTypes(baseInterfaceContext, baseInterfaceDeclaration, genericTypes);
        } else {
            doCollectGenericTypes(interfaceContext, baseInterfaceDeclaration, genericTypes);
        }
    }
}

bool InterfaceTypeCreator::isInterfaceGeneric(const seds::model::InterfaceDeclaration *interfaceDeclaration,
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
