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

#include "specialized/asyncinterfacecommandtranslator.h"

#include "interfacetranslatorhelper.h"

#include <asn1library/asn1/acnparameter.h>
#include <asn1library/asn1/acnsequencecomponent.h>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/asn1/SedsToAsn1Translator/translator.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <iostream>

using conversion::asn1::translator::SedsToAsn1Translator;
using conversion::translator::TranslationException;

namespace conversion::iv::translator {

const QString AsyncInterfaceCommandTranslator::m_ivInterfaceParameterName = "InputParam";

std::multimap<QString, AsyncInterfaceCommandTranslator::CommandArgumentEntry>
        AsyncInterfaceCommandTranslator::m_commandArguments;

AsyncInterfaceCommandTranslator::AsyncInterfaceCommandTranslator(ivm::IVFunction *ivFunction,
        const QString &sedsInterfaceName, Asn1Acn::Definitions *asn1Definitions,
        const seds::model::Package *sedsPackage, const Asn1Acn::Asn1Model::Data &asn1Files,
        const std::vector<seds::model::Package> &sedsPackages, const GenericTypeMapper *typeMapper)
    : m_ivFunction(ivFunction)
    , m_sedsInterfaceName(sedsInterfaceName)
    , m_asn1Definitions(asn1Definitions)
    , m_sedsPackage(sedsPackage)
    , m_asn1Files(asn1Files)
    , m_sedsPackages(sedsPackages)
    , m_typeMapper(typeMapper)
{
}

void AsyncInterfaceCommandTranslator::translateCommand(
        const seds::model::InterfaceCommand &sedsCommand, ivm::IVInterface::InterfaceType interfaceType)
{
    // Process command based on its commands
    switch (sedsCommand.argumentsCombination()) {
    case seds::model::ArgumentsCombination::InOnly: {
        // In arguments are 'native', so they are handles as-is
        const auto interfaceName = InterfaceTranslatorHelper::buildCommandInterfaceName(
                m_sedsInterfaceName, sedsCommand.nameStr(), interfaceType);
        auto *ivInterface = InterfaceTranslatorHelper::createIvInterface(
                interfaceName, interfaceType, ivm::IVInterface::OperationKind::Sporadic, m_ivFunction);
        translateArguments(sedsCommand, seds::model::CommandArgumentMode::In, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::OutOnly: {
        // Out arguments aren't supported by TASTE sporadic interface.
        // We cannot change the argument direction, so we switch interface type (provided <-> required)
        const auto interfaceName = InterfaceTranslatorHelper::buildCommandInterfaceName(m_sedsInterfaceName,
                sedsCommand.nameStr(), InterfaceTranslatorHelper::switchInterfaceType(interfaceType));
        auto *ivInterface = InterfaceTranslatorHelper::createIvInterface(interfaceName,
                InterfaceTranslatorHelper::switchInterfaceType(interfaceType),
                ivm::IVInterface::OperationKind::Sporadic, m_ivFunction);
        translateArguments(sedsCommand, seds::model::CommandArgumentMode::Out, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::InAndNotify: {
        // InAndNotify arguments are separated onto two interfaces
        // In arguments - as-is
        // Notify arguments - switched interface type (provided <-> required)
        const auto inInterfaceName = InterfaceTranslatorHelper::buildCommandInterfaceName(
                m_sedsInterfaceName, sedsCommand.nameStr(), interfaceType);
        auto *ivInterfaceIn = InterfaceTranslatorHelper::createIvInterface(
                inInterfaceName, interfaceType, ivm::IVInterface::OperationKind::Sporadic, m_ivFunction);
        translateArguments(sedsCommand, seds::model::CommandArgumentMode::In, ivInterfaceIn);
        m_ivFunction->addChild(ivInterfaceIn);

        const auto notifyInterfaceName = InterfaceTranslatorHelper::buildCommandInterfaceName(m_sedsInterfaceName,
                sedsCommand.nameStr(), InterfaceTranslatorHelper::switchInterfaceType(interfaceType));
        auto *ivInterfaceNotify = InterfaceTranslatorHelper::createIvInterface(notifyInterfaceName,
                InterfaceTranslatorHelper::switchInterfaceType(interfaceType),
                ivm::IVInterface::OperationKind::Sporadic, m_ivFunction);
        translateArguments(sedsCommand, seds::model::CommandArgumentMode::Notify, ivInterfaceNotify);
        m_ivFunction->addChild(ivInterfaceNotify);
    } break;
    case seds::model::ArgumentsCombination::NoArgs: {
        // No arguments, no problems
        const auto interfaceName = InterfaceTranslatorHelper::buildCommandInterfaceName(
                m_sedsInterfaceName, sedsCommand.nameStr(), interfaceType);
        auto *ivInterface = InterfaceTranslatorHelper::createIvInterface(
                interfaceName, interfaceType, ivm::IVInterface::OperationKind::Sporadic, m_ivFunction);
        m_ivFunction->addChild(ivInterface);
        break;
    }
    case seds::model::ArgumentsCombination::NotifyOnly:
    case seds::model::ArgumentsCombination::InAndOut:
    case seds::model::ArgumentsCombination::OutAndNotify:
    case seds::model::ArgumentsCombination::All: {
        const auto message = QString(
                "Interface command arguments combination '%1' is not supported for TASTE InterfaceView async interface")
                                     .arg(argumentsCombinationToString(sedsCommand.argumentsCombination()));
        throw TranslationException(message);
    } break;
    default:
        throw UnhandledValueException("ArgumentsCombination");
        break;
    }
}

void AsyncInterfaceCommandTranslator::translateArguments(const seds::model::InterfaceCommand &sedsCommand,
        seds::model::CommandArgumentMode requestedArgumentMode, ivm::IVInterface *ivInterface)
{
    // Async commands are translated to sporadic interfaces, which can accept only one argument
    // To satisfy this we need to pack all command arguments into one
    const auto bundledTypeName = buildBundledType(sedsCommand, requestedArgumentMode);

    const auto ivParameter = InterfaceTranslatorHelper::createInterfaceParameter(
            m_ivInterfaceParameterName, bundledTypeName, shared::InterfaceParameter::Direction::IN);
    ivInterface->addParam(ivParameter);
}

QString AsyncInterfaceCommandTranslator::buildBundledType(
        const seds::model::InterfaceCommand &sedsCommand, seds::model::CommandArgumentMode requestedArgumentMode)
{
    const auto &sedsCommandName = sedsCommand.nameStr();

    // Get arguments that have the requested mode
    auto arguments = processArguments(sedsCommand.arguments(), requestedArgumentMode);
    const auto bundledTypeHash = calculateArgumentsHash(arguments);

    // Check if the bundled type was already created and saved
    const auto typesForCommand = m_commandArguments.equal_range(sedsCommandName);
    const auto foundType = std::find_if(typesForCommand.first, typesForCommand.second, [&](const auto &typePair) {
        return typePair.second.typeHash == bundledTypeHash && typePair.second.arguments == arguments;
    });

    // Use the existing bundled type
    if (foundType != typesForCommand.second) {
        return foundType->second.bundledTypeName;
    }

    // Create a new bundled type, add it to the ASN.1 model
    auto bundledTypeName = createBundledType(sedsCommandName, arguments);

    // Save command with its arguments for later
    m_commandArguments.insert({ sedsCommandName, { bundledTypeName, bundledTypeHash, std::move(arguments) } });

    return bundledTypeName;
}

QString AsyncInterfaceCommandTranslator::createBundledType(
        const QString &sedsCommandName, const std::vector<AsyncInterfaceCommandTranslator::ArgumentData> &arguments)
{
    const auto cachedTypesCount = m_commandArguments.count(sedsCommandName);
    auto name = InterfaceTranslatorHelper::buildBundledTypeName(sedsCommandName, cachedTypesCount);

    auto sequence = std::make_unique<Asn1Acn::Types::Sequence>(name);

    for (const auto &argumentData : arguments) {
        createBundledTypeComponent(argumentData, sequence.get());
    }

    auto typeAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(name, name, Asn1Acn::SourceLocation(), std::move(sequence));
    m_asn1Definitions->addType(std::move(typeAssignment));

    return name;
}

void AsyncInterfaceCommandTranslator::createBundledTypeComponent(
        const AsyncInterfaceCommandTranslator::ArgumentData &argumentData, Asn1Acn::Types::Sequence *sequence) const
{
    const auto &name = argumentData.name;
    const auto &typeRef = argumentData.typeRef;

    const auto asn1Definitions = typeRef.packageStr()
            ? SedsToAsn1Translator::getAsn1Definitions(*typeRef.packageStr(), m_asn1Files)
            : m_asn1Definitions;

    const auto &typeName = typeRef.nameStr();
    const auto *referencedTypeAssignment = asn1Definitions->type(typeName);

    if (!referencedTypeAssignment) {
        auto errorMessage =
                QString("Type %1 not found while creating ASN.1 sequence %2").arg(typeName).arg(sequence->identifier());
        throw TranslationException(std::move(errorMessage));
    }

    const auto *referencedType = referencedTypeAssignment->type();

    auto sequenceComponentType = std::make_unique<Asn1Acn::Types::UserdefinedType>(typeName, m_asn1Definitions->name());
    sequenceComponentType->setType(referencedType->clone());

    std::unique_ptr<Asn1Acn::SequenceComponent> sequenceComponent;

    if (argumentData.isDeterminant) {
        sequenceComponent =
                std::make_unique<Asn1Acn::AcnSequenceComponent>(name, name, std::move(sequenceComponentType));
    } else {
        sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(
                name, name, false, std::nullopt, "", Asn1Acn::SourceLocation(), std::move(sequenceComponentType));

        if (argumentData.determinantRef.has_value()) {
            sequenceComponent->addAcnParameter(argumentData.determinantRef->nameStr());
        }
    }

    sequence->addComponent(std::move(sequenceComponent));
}

AsyncInterfaceCommandTranslator::Arguments AsyncInterfaceCommandTranslator::processArguments(
        const std::vector<seds::model::CommandArgument> &sedsArguments,
        seds::model::CommandArgumentMode requestedArgumentMode)
{
    Arguments arguments;

    for (const auto &sedsArgument : sedsArguments) {
        if (sedsArgument.mode() == requestedArgumentMode) {
            auto argumentData = processArgument(sedsArgument, arguments);
            arguments.push_back(std::move(argumentData));
        }
    }

    return arguments;
}

AsyncInterfaceCommandTranslator::ArgumentData AsyncInterfaceCommandTranslator::processArgument(
        const seds::model::CommandArgument &sedsArgument, const AsyncInterfaceCommandTranslator::Arguments &arguments)
{
    const auto argumentName = Escaper::escapeAsn1FieldName(sedsArgument.nameStr());
    const auto argumentTypeRef = sedsArgument.type();
    const auto argumentTypeName = argumentTypeRef.nameStr();

    const auto typeMapping = m_typeMapper->getMapping(argumentTypeName);

    // Type is not mapped
    if (typeMapping == nullptr) {
        // Handle optional array dimensions
        const auto typeName = handleArrayArgument(sedsArgument, argumentTypeRef);
        return { argumentName, typeName, std::nullopt, std::nullopt, false };
    }

    // Get all concrete types that this generic type can be mapped to
    const auto &concreteTypes = typeMapping->concreteTypes;

    if (concreteTypes.empty()) {
        auto errorMessage = QString("Type \"%1\" of argument \"%2\" of interface \"%3\" is handled as generic, but no "
                                    "mappings was provided")
                                    .arg(argumentTypeName)
                                    .arg(argumentName)
                                    .arg(m_sedsInterfaceName);

        throw TranslationException(std::move(errorMessage));
    }

    if (concreteTypes.size() == 1) { // 'Simple' mapping
        return handleArgumentSimpleMapping(sedsArgument, concreteTypes.front());
    } else { // 'Alternate' mapping
        if (!typeMapping->determinantTypeRef.has_value()) {
            auto errorMessage = QString("Type \"%1\" of argument \"%2\" of interface \"%3\" has alternate mapping, but "
                                        "no determinant was set")
                                        .arg(argumentTypeName)
                                        .arg(argumentName)
                                        .arg(m_sedsInterfaceName);
            throw TranslationException(std::move(errorMessage));
        }

        return handleArgumentAlternateMapping(sedsArgument, concreteTypes, *typeMapping->determinantTypeRef, arguments);
    }
}

AsyncInterfaceCommandTranslator::ArgumentData AsyncInterfaceCommandTranslator::handleArgumentSimpleMapping(
        const seds::model::CommandArgument &sedsArgument, const TypeMapping::ConcreteType &concreteType)
{
    const auto argumentName = Escaper::escapeAsn1FieldName(sedsArgument.nameStr());
    const auto argumentTypeName = handleArrayArgument(sedsArgument, concreteType.typeRef);

    return { argumentName, argumentTypeName, concreteType.fixedValue, std::nullopt, false };
}

AsyncInterfaceCommandTranslator::ArgumentData AsyncInterfaceCommandTranslator::handleArgumentAlternateMapping(
        const seds::model::CommandArgument &sedsArgument, const std::vector<TypeMapping::ConcreteType> &concreteTypes,
        const seds::model::DataTypeRef &determinantTypeRef, const AsyncInterfaceCommandTranslator::Arguments &arguments)
{
    const auto argumentName = Escaper::escapeAsn1FieldName(sedsArgument.nameStr());

    // If all of the concrete types are the same, then we can handle this a simple mapping
    const auto firstConcreteTypeRef = concreteTypes.front().typeRef;
    const auto allTheSame = std::all_of(concreteTypes.begin(), concreteTypes.end(),
            [&](const auto &concreteType) { return concreteType.typeRef.nameStr() == firstConcreteTypeRef; });

    if (allTheSame) {
        if (firstConcreteTypeRef == determinantTypeRef) {
            return { argumentName, firstConcreteTypeRef, std::nullopt, std::nullopt, true };
        } else {
            const auto argumentTypeRef = handleArrayArgument(sedsArgument, firstConcreteTypeRef);
            return { argumentName, argumentTypeRef, std::nullopt, std::nullopt, false };
        }
    } else {
        // Try to find an argument that is a determinant
        // Limitation is that the determinant has to be present before any of the field that will be using it
        const auto foundDeterminant = std::find_if(arguments.begin(), arguments.end(),
                [&](const auto &argument) { return argument.typeRef == determinantTypeRef; });

        if (foundDeterminant == arguments.end()) {
            auto errorMessage =
                    QString("Determinant type \"%1\" of argument \"%2\" of interface \"%3\" couldn't be find")
                            .arg(determinantTypeRef.nameStr())
                            .arg(argumentName)
                            .arg(m_sedsInterfaceName);
            throw TranslationException(std::move(errorMessage));
        }

        // Create an ASN.1 choice that will have a generic parameters to choose an alternative based on determinant
        const auto alternateTypeName =
                createAlternateType(sedsArgument.type().nameStr(), concreteTypes, determinantTypeRef);
        const auto argumentTypeRef = handleArrayArgument(sedsArgument, alternateTypeName);

        return { argumentName, argumentTypeRef, std::nullopt, foundDeterminant->name, false };
    }
}

seds::model::DataTypeRef AsyncInterfaceCommandTranslator::handleArrayArgument(
        const seds::model::CommandArgument &sedsArgument, const seds::model::DataTypeRef &typeRef)
{
    if (sedsArgument.arrayDimensions().empty()) {
        return typeRef;
    } else {
        return InterfaceTranslatorHelper::createArrayType(
                typeRef, sedsArgument.arrayDimensions(), m_asn1Definitions, m_sedsPackage, m_asn1Files, m_sedsPackages);
    }
}

QString AsyncInterfaceCommandTranslator::createAlternateType(const QString &genericTypeName,
        const std::vector<TypeMapping::ConcreteType> &concreteTypes, const seds::model::DataTypeRef &determinantRef)
{
    auto name = InterfaceTranslatorHelper::buildAlternateTypeName(m_sedsInterfaceName, genericTypeName);

    auto choice = std::make_unique<Asn1Acn::Types::Choice>(name);

    // Each of the alternative is present-when determinant has given value
    for (const auto &concreteType : concreteTypes) {
        const auto asn1ConcreteType = m_asn1Definitions->type(concreteType.typeRef.nameStr())->type();
        const auto presentWhen = QString("determinant==%1").arg(*concreteType.determinantValue);

        const auto choiceAlternativeName = QString("concrete-%1").arg(concreteType.typeRef.nameStr());
        auto choiceAlternative = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(choiceAlternativeName,
                choiceAlternativeName, choiceAlternativeName, choiceAlternativeName, presentWhen,
                Asn1Acn::SourceLocation(), asn1ConcreteType->clone());

        choice->addComponent(std::move(choiceAlternative));
    }

    // Add an ACN parameter for determinant
    auto acnParameter = std::make_unique<Asn1Acn::AcnParameter>("determinant", "determinant", determinantRef.nameStr());
    choice->addParameter(std::move(acnParameter));

    auto typeAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(name, name, Asn1Acn::SourceLocation(), std::move(choice));
    m_asn1Definitions->addType(std::move(typeAssignment));

    return name;
}

std::size_t AsyncInterfaceCommandTranslator::calculateArgumentsHash(
        const std::vector<AsyncInterfaceCommandTranslator::ArgumentData> &arguments) const
{
    std::size_t typeHash = 0;

    for (const auto &argumentData : arguments) {
        const auto typeNameHash = std::hash<QString> {}(argumentData.typeRef.value().pathStr());

        if (typeHash == 0) {
            typeHash = typeNameHash;
        } else {
            typeHash ^= (typeNameHash << 1);
        }
    }

    return typeHash;
}

} // namespace conversion::iv::translator
