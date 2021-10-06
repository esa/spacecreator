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
#include <conversion/asn1/SedsToAsn1Translator/visitors/datatypetranslatorvisitor.h>
#include <conversion/common/qstringhash.h>
#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
#include <seds/SedsModel/package/package.h>
#include <shared/parameter.h>

using conversion::translator::MissingGenericTypeMappingException;
using conversion::translator::TranslationException;
using conversion::translator::UndeclaredDataTypeException;
using conversion::translator::UnhandledValueException;

namespace conversion::iv::translator {

std::multimap<QString, AsyncInterfaceCommandTranslator::ArgumentsCacheEntry>
        AsyncInterfaceCommandTranslator::m_commandArgumentsCache;

const QString AsyncInterfaceCommandTranslator::m_ivInterfaceParameterName = "InputParam";
const QString AsyncInterfaceCommandTranslator::m_bundledTypeNameTemplate = "%1_Type%2";

AsyncInterfaceCommandTranslator::AsyncInterfaceCommandTranslator(
        const seds::model::Interface &sedsInterface, Asn1Acn::Definitions *asn1Definitions, ivm::IVFunction *ivFunction)
    : InterfaceCommandTranslator(sedsInterface, ivFunction)
    , m_asn1Definitions(asn1Definitions)
{
}

void AsyncInterfaceCommandTranslator::translateCommand(
        const seds::model::InterfaceCommand &sedsCommand, ivm::IVInterface::InterfaceType interfaceType)
{
    switch (sedsCommand.argumentsCombination()) {
    case seds::model::ArgumentsCombination::InOnly: {
        auto *ivInterface = createIvInterface(sedsCommand, interfaceType, ivm::IVInterface::OperationKind::Sporadic);
        translateArguments(sedsCommand, seds::model::CommandArgumentMode::In, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::OutOnly: {
        auto *ivInterface = createIvInterface(
                sedsCommand, switchInterfaceType(interfaceType), ivm::IVInterface::OperationKind::Sporadic);
        translateArguments(sedsCommand, seds::model::CommandArgumentMode::Out, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::InAndNotify: {
        auto *ivInterfaceIn = createIvInterface(sedsCommand, interfaceType, ivm::IVInterface::OperationKind::Sporadic);
        translateArguments(sedsCommand, seds::model::CommandArgumentMode::In, ivInterfaceIn);
        m_ivFunction->addChild(ivInterfaceIn);

        auto *ivInterfaceNotify = createIvInterface(
                sedsCommand, switchInterfaceType(interfaceType), ivm::IVInterface::OperationKind::Sporadic);
        translateArguments(sedsCommand, seds::model::CommandArgumentMode::Notify, ivInterfaceNotify);
        m_ivFunction->addChild(ivInterfaceNotify);
    } break;
    case seds::model::ArgumentsCombination::NoArgs:
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
    const auto bundledTypeName = buildAsn1SequenceType(sedsCommand, requestedArgumentMode);

    auto ivParameter = shared::InterfaceParameter(m_ivInterfaceParameterName, shared::BasicParameter::Type::Other,
            bundledTypeName, m_interfaceParameterEncoding, shared::InterfaceParameter::Direction::IN);
    ivInterface->addParam(ivParameter);
}

QString AsyncInterfaceCommandTranslator::buildAsn1SequenceType(
        const seds::model::InterfaceCommand &sedsCommand, seds::model::CommandArgumentMode requestedArgumentMode)
{
    auto arguments = processArgumentsTypes(sedsCommand.arguments(), requestedArgumentMode);
    const auto bundledTypeHash = calculateArgumentsHash(arguments);

    const auto &sedsCommandName = sedsCommand.nameStr();

    const auto typesForCommand = m_commandArgumentsCache.equal_range(sedsCommandName);
    const auto foundType = std::find_if(
            typesForCommand.first, typesForCommand.second, [&bundledTypeHash, &arguments](const auto &typePair) {
                return typePair.second.typeHash == bundledTypeHash && typePair.second.compareArguments(arguments);
            });

    if (foundType != typesForCommand.second) {
        return foundType->second.asn1TypeName;
    } else {
        const auto cachedTypesCount = m_commandArgumentsCache.count(sedsCommandName);
        auto bundledTypeName = createBundledTypeName(sedsCommandName, cachedTypesCount);
        createAsn1Sequence(bundledTypeName, arguments);

        m_commandArgumentsCache.insert({ sedsCommandName, { bundledTypeName, bundledTypeHash, std::move(arguments) } });

        return bundledTypeName;
    }
}

void AsyncInterfaceCommandTranslator::createAsn1Sequence(
        const QString &name, const std::unordered_map<QString, QString> &arguments)
{
    auto sequence = std::make_unique<Asn1Acn::Types::Sequence>(name);

    for (const auto &[argumentName, argumentTypeName] : arguments) {
        createAsn1SequenceComponent(argumentName, argumentTypeName, sequence.get());
    }

    auto typeAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(name, name, Asn1Acn::SourceLocation(), std::move(sequence));
    m_asn1Definitions->addType(std::move(typeAssignment));
}

void AsyncInterfaceCommandTranslator::createAsn1SequenceComponent(
        const QString &name, const QString &typeName, Asn1Acn::Types::Sequence *sequence) const
{
    const auto *referencedTypeAssignment = m_asn1Definitions->type(typeName);
    const auto *referencedType = referencedTypeAssignment->type();

    auto sequenceComponentType = std::make_unique<Asn1Acn::Types::UserdefinedType>(
            typeName, m_asn1Definitions->name(), referencedTypeAssignment);
    sequenceComponentType->setType(referencedType->clone());

    auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            name, name, false, "", Asn1Acn::SourceLocation(), std::move(sequenceComponentType));
    sequence->addComponent(std::move(sequenceComponent));
}

QString AsyncInterfaceCommandTranslator::createBundledTypeName(
        const QString &sedsCommandName, const std::size_t counter) const
{
    if (counter == 0) {
        return m_bundledTypeNameTemplate.arg(sedsCommandName).arg("");
    } else {
        return m_bundledTypeNameTemplate.arg(sedsCommandName).arg(counter);
    }
}

std::unordered_map<QString, QString> AsyncInterfaceCommandTranslator::processArgumentsTypes(
        const std::vector<seds::model::CommandArgument> &sedsArguments,
        seds::model::CommandArgumentMode requestedArgumentMode) const
{
    std::unordered_map<QString, QString> arguments;

    for (const auto &sedsArgument : sedsArguments) {
        if (sedsArgument.mode() == requestedArgumentMode) {
            const auto &genericTypeName = sedsArgument.type().nameStr();
            const auto &concreteTypeName = findMappedType(genericTypeName);

            arguments.insert({ sedsArgument.nameStr(), concreteTypeName });
        }
    }

    return arguments;
}

std::size_t AsyncInterfaceCommandTranslator::calculateArgumentsHash(
        const std::unordered_map<QString, QString> &arguments) const
{
    std::size_t typeHash = 0;

    for (const auto &[name, typeName] : arguments) {
        const auto typeNameHash = std::hash<QString> {}(typeName);

        if (typeHash == 0) {
            typeHash = typeNameHash;
        } else {
            typeHash ^= (typeNameHash << 1);
        }
    }

    return typeHash;
}

const QString &AsyncInterfaceCommandTranslator::findMappedType(const QString &genericTypeName) const
{
    const auto &genericTypeMaps = m_sedsInterface.genericTypeMapSet().genericTypeMaps();

    const auto found = std::find_if(genericTypeMaps.begin(), genericTypeMaps.end(),
            [&genericTypeName](
                    const seds::model::GenericTypeMap &typeMap) { return typeMap.nameStr() == genericTypeName; });

    if (found != genericTypeMaps.end()) {
        return found->type().nameStr();
    } else {
        return genericTypeName;
    }
}

ivm::IVInterface::InterfaceType AsyncInterfaceCommandTranslator::switchInterfaceType(
        ivm::IVInterface::InterfaceType interfaceType) const
{
    switch (interfaceType) {
    case ivm::IVInterface::InterfaceType::Required:
        return ivm::IVInterface::InterfaceType::Provided;
    case ivm::IVInterface::InterfaceType::Provided:
        return ivm::IVInterface::InterfaceType::Required;
    default:
        throw UnhandledValueException("InterfaceType");
        break;
    }
}

bool AsyncInterfaceCommandTranslator::ArgumentsCacheEntry::compareArguments(
        const std::unordered_map<QString, QString> &arguments) const
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
