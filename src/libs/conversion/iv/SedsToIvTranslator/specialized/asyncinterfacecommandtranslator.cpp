/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include <QDebug>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <iostream>
#include <ivcore/ivfunction.h>
#include <seds/SedsModel/package/package.h>
#include <shared/parameter.h>
#include <shared/qstringhash.h>

using conversion::Escaper;
using conversion::UnhandledValueException;
using conversion::translator::MissingGenericTypeMappingException;
using conversion::translator::TranslationException;
using conversion::translator::UndeclaredDataTypeException;

namespace conversion::iv::translator {

std::multimap<QString, AsyncInterfaceCommandTranslator::ArgumentsCacheEntry>
        AsyncInterfaceCommandTranslator::m_commandArgumentsCache;

const QString AsyncInterfaceCommandTranslator::m_ivInterfaceParameterName = "InputParam";
const QString AsyncInterfaceCommandTranslator::m_bundledTypeNameTemplate = "%1-Type%2";

AsyncInterfaceCommandTranslator::AsyncInterfaceCommandTranslator(ivm::IVFunction *ivFunction,
        const QString &sedsInterfaceName, const std::optional<seds::model::GenericTypeMapSet> &genericTypeMapSet,
        Asn1Acn::Definitions *asn1Definitions, const seds::model::Package *sedsPackage)
    : InterfaceCommandTranslator(ivFunction, sedsInterfaceName, genericTypeMapSet, asn1Definitions, sedsPackage)
{
}

void AsyncInterfaceCommandTranslator::translateCommand(
        const seds::model::InterfaceCommand &sedsCommand, ivm::IVInterface::InterfaceType interfaceType)
{
    // Process command based on its commands
    switch (sedsCommand.argumentsCombination()) {
    case seds::model::ArgumentsCombination::InOnly: {
        // In arguments are 'native', so they are handles as-is
        auto *ivInterface = createIvInterface(sedsCommand, interfaceType, ivm::IVInterface::OperationKind::Sporadic);
        translateArguments(sedsCommand, seds::model::CommandArgumentMode::In, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::OutOnly: {
        // Out arguments aren't supported by TASTE sporadic interface.
        // We cannot change the argument direction, so we switch interface type (provided <-> required)
        auto *ivInterface = createIvInterface(
                sedsCommand, switchInterfaceType(interfaceType), ivm::IVInterface::OperationKind::Sporadic);
        translateArguments(sedsCommand, seds::model::CommandArgumentMode::Out, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::InAndNotify: {
        // InAndNotify arguments are separated onto two interfaces
        // In arguments - as-is
        // Notify arguments - switched interface type (provided <-> required)
        auto *ivInterfaceIn = createIvInterface(sedsCommand, interfaceType, ivm::IVInterface::OperationKind::Sporadic);
        translateArguments(sedsCommand, seds::model::CommandArgumentMode::In, ivInterfaceIn);
        m_ivFunction->addChild(ivInterfaceIn);

        auto *ivInterfaceNotify = createIvInterface(
                sedsCommand, switchInterfaceType(interfaceType), ivm::IVInterface::OperationKind::Sporadic);
        translateArguments(sedsCommand, seds::model::CommandArgumentMode::Notify, ivInterfaceNotify);
        m_ivFunction->addChild(ivInterfaceNotify);
    } break;
    case seds::model::ArgumentsCombination::NoArgs: {
        // No arguments, no problems
        auto *ivInterface = createIvInterface(sedsCommand, interfaceType, ivm::IVInterface::OperationKind::Sporadic);
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
    // As sporadic interface can have max one argument, we need to bundle all command arguments
    // into one by creating an ASN.1 sequence
    const auto bundledTypeName = buildBundledType(sedsCommand, requestedArgumentMode);

    auto ivParameter = shared::InterfaceParameter(m_ivInterfaceParameterName, shared::BasicParameter::Type::Other,
            Escaper::escapeIvName(bundledTypeName), m_interfaceParameterEncoding,
            shared::InterfaceParameter::Direction::IN);
    ivInterface->addParam(ivParameter);
}

QString AsyncInterfaceCommandTranslator::buildBundledType(
        const seds::model::InterfaceCommand &sedsCommand, seds::model::CommandArgumentMode requestedArgumentMode)
{
    const auto &sedsCommandName = sedsCommand.nameStr();

    // Get arguments that have the interesting mode
    auto arguments = filterArguments(sedsCommand.arguments(), requestedArgumentMode);
    const auto bundledTypeHash = calculateArgumentsHash(arguments);

    // Check if the bundled type was already created and cached
    const auto typesForCommand = m_commandArgumentsCache.equal_range(sedsCommandName);
    const auto foundType = std::find_if(
            typesForCommand.first, typesForCommand.second, [&bundledTypeHash, &arguments](const auto &typePair) {
                return typePair.second.typeHash == bundledTypeHash && typePair.second.compareArguments(arguments);
            });

    // Use the existing bundled type
    if (foundType != typesForCommand.second) {
        return foundType->second.asn1TypeName;
    }

    // Create a new bundled type and add it to the ASN.1 model
    auto bundledTypeName = createBundledType(sedsCommandName, arguments);
    m_commandArgumentsCache.insert({ sedsCommandName, { bundledTypeName, bundledTypeHash, std::move(arguments) } });

    return bundledTypeName;
}

QString AsyncInterfaceCommandTranslator::createBundledType(const QString &sedsCommandName,
        const std::vector<AsyncInterfaceCommandTranslator::CommandArgumentData> &arguments)
{
    auto name = createBundledTypeName(sedsCommandName);

    auto sequence = std::make_unique<Asn1Acn::Types::Sequence>(name);

    for (const auto &argumentData : arguments) {
        createAsn1SequenceComponent(argumentData, sequence.get());
    }

    auto typeAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(name, name, Asn1Acn::SourceLocation(), std::move(sequence));
    m_asn1Definitions->addType(std::move(typeAssignment));

    return name;
}

std::vector<AsyncInterfaceCommandTranslator::CommandArgumentData> AsyncInterfaceCommandTranslator::filterArguments(
        const std::vector<seds::model::CommandArgument> &sedsArguments,
        seds::model::CommandArgumentMode requestedArgumentMode) const
{
    std::vector<CommandArgumentData> arguments;

    for (const auto &sedsArgument : sedsArguments) {
        if (sedsArgument.mode() == requestedArgumentMode) {
            const auto sedsArgumentNameEscaped = Escaper::escapeAsn1FieldName(sedsArgument.nameStr());
            const auto sedsArgumentTypeName = handleArgumentType(sedsArgument);

            arguments.push_back({ sedsArgumentNameEscaped, sedsArgumentTypeName });
        }
    }

    return arguments;
}

void AsyncInterfaceCommandTranslator::createAsn1SequenceComponent(
        const AsyncInterfaceCommandTranslator::CommandArgumentData &argumentData,
        Asn1Acn::Types::Sequence *sequence) const
{
    const auto &typeName = argumentData.typeName;

    const auto *referencedTypeAssignment = m_asn1Definitions->type(typeName);

    if (!referencedTypeAssignment) {
        auto errorMessage =
                QString("Type %1 not found while creating ASN.1 sequence %2").arg(typeName).arg(sequence->identifier());
        throw TranslationException(std::move(errorMessage));
    }

    const auto *referencedType = referencedTypeAssignment->type();

    auto sequenceComponentType = std::make_unique<Asn1Acn::Types::UserdefinedType>(typeName, m_asn1Definitions->name());
    sequenceComponentType->setType(referencedType->clone());

    const auto &name = argumentData.name;
    auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            name, name, false, std::nullopt, "", Asn1Acn::SourceLocation(), std::move(sequenceComponentType));
    sequence->addComponent(std::move(sequenceComponent));
}

std::size_t AsyncInterfaceCommandTranslator::calculateArgumentsHash(
        const std::vector<AsyncInterfaceCommandTranslator::CommandArgumentData> &arguments) const
{
    std::size_t typeHash = 0;

    for (const auto &argumentData : arguments) {
        const auto typeNameHash = std::hash<QString> {}(argumentData.typeName);

        if (typeHash == 0) {
            typeHash = typeNameHash;
        } else {
            typeHash ^= (typeNameHash << 1);
        }
    }

    return typeHash;
}

QString AsyncInterfaceCommandTranslator::createBundledTypeName(const QString &sedsCommandName) const
{
    const auto cachedTypesCount = m_commandArgumentsCache.count(sedsCommandName);
    const auto sedsCommandNameEscaped = Escaper::escapeAsn1TypeName(sedsCommandName);

    if (cachedTypesCount == 0) {
        return m_bundledTypeNameTemplate.arg(sedsCommandNameEscaped).arg("");
    } else {
        return m_bundledTypeNameTemplate.arg(sedsCommandNameEscaped).arg(cachedTypesCount);
    }
}

bool AsyncInterfaceCommandTranslator::ArgumentsCacheEntry::compareArguments(
        const std::vector<AsyncInterfaceCommandTranslator::CommandArgumentData> &arguments) const
{
    if (typeArguments.size() != arguments.size()) {
        return false;
    }

    auto lhsIt = typeArguments.begin();
    auto rhsIt = arguments.begin();

    while (lhsIt != typeArguments.end() && rhsIt != arguments.end()) {
        if (*lhsIt != *rhsIt) {
            return false;
        }

        ++lhsIt;
        ++rhsIt;
    }

    return true;
}

} // namespace conversion::iv::translator
