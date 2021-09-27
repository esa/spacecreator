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

AsyncInterfaceCommandTranslator::InterfaceCommandArgumentsCache
        AsyncInterfaceCommandTranslator::m_asn1CommandArgumentsCache;

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
        const seds::model::InterfaceCommand &sedsCommand, seds::model::CommandArgumentMode requestedArgumentMode) const
{
    const auto arguments = processArgumentsTypes(sedsCommand.arguments(), requestedArgumentMode);
    const auto bundledTypeHash = calculateArgumentsHash(arguments);

    const auto &sedsCommandName = sedsCommand.nameStr();
    const auto cachedTypesCount = m_asn1CommandArgumentsCache.count(sedsCommandName);

    if (cachedTypesCount == 0) {
        const auto bundledTypeName = createBundledTypeName(sedsCommandName);
        createAsn1Sequence(bundledTypeName, arguments);

        m_asn1CommandArgumentsCache.insert({ sedsCommandName, { bundledTypeHash, bundledTypeName } });

        return bundledTypeName;
    } else {
        const auto typesForCommand = m_asn1CommandArgumentsCache.equal_range(sedsCommandName);
        const auto foundType = std::find_if(typesForCommand.first, typesForCommand.second,
                [&bundledTypeHash](const auto &typePair) { return typePair.second.first == bundledTypeHash; });

        if (foundType != typesForCommand.second) {
            return foundType->second.second;
        } else {
            const auto bundledTypeName = createBundledTypeName(sedsCommandName, cachedTypesCount);
            createAsn1Sequence(bundledTypeName, arguments);

            m_asn1CommandArgumentsCache.insert({ sedsCommandName, { bundledTypeHash, bundledTypeName } });

            return bundledTypeName;
        }
    }
}

void AsyncInterfaceCommandTranslator::createAsn1Sequence(
        const QString &name, const std::unordered_map<QString, QString> &arguments) const
{
    auto sequence = std::make_unique<Asn1Acn::Types::Sequence>(name);

    for (const auto &[name, typeName] : arguments) {
        sequence->addComponent(createAsn1SequenceComponent(name, typeName));
    }

    auto typeAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(name, name, Asn1Acn::SourceLocation(), std::move(sequence));
    m_asn1Definitions->addType(std::move(typeAssignment));
}

std::unique_ptr<Asn1Acn::SequenceComponent> AsyncInterfaceCommandTranslator::createAsn1SequenceComponent(
        const QString &name, const QString typeName) const
{
    const auto *referencedTypeAssignment = m_asn1Definitions->type(typeName);
    const auto *referencedType = referencedTypeAssignment->type();

    auto sequenceComponentType = std::make_unique<Asn1Acn::Types::UserdefinedType>(
            typeName, m_asn1Definitions->name(), referencedTypeAssignment);
    sequenceComponentType->setType(referencedType->clone());

    return std::make_unique<Asn1Acn::AsnSequenceComponent>(
            name, name, false, "", Asn1Acn::SourceLocation(), std::move(sequenceComponentType));
}

QString AsyncInterfaceCommandTranslator::createBundledTypeName(const QString sedsCommandName, std::size_t counter) const
{
    if (counter == 0) {
        return m_bundledTypeNameTemplate.arg(sedsCommandName, "");
    } else {
        return m_bundledTypeNameTemplate.arg(sedsCommandName, counter);
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

} // namespace conversion::iv::translator
