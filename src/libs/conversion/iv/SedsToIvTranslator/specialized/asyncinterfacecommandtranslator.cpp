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

const QString AsyncInterfaceCommandTranslator::m_interfaceParameterName = "InputParam";
const QString AsyncInterfaceCommandTranslator::m_asn1BundledTypeTemplate = "%1_Type%2";

AsyncInterfaceCommandTranslator::AsyncInterfaceCommandTranslator(const seds::model::Package &package,
        const seds::model::Component &component, const seds::model::Interface &interface, ivm::IVFunction *ivFunction,
        Asn1Acn::Definitions *asn1Definitions)
    : InterfaceCommandTranslator(interface, ivFunction)
    , m_package(package)
    , m_component(component)
    , m_asn1Definitions(asn1Definitions)
{
}

void AsyncInterfaceCommandTranslator::translateCommand(
        const seds::model::InterfaceCommand &command, ivm::IVInterface::InterfaceType interfaceType)
{
    switch (command.argumentsCombination()) {
    case seds::model::ArgumentsCombination::InOnly: {
        auto *ivInterface = createIvInterface(command, interfaceType, ivm::IVInterface::OperationKind::Sporadic);
        translateArguments(command, seds::model::CommandArgumentMode::In, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::OutOnly: {
        auto *ivInterface = createIvInterface(
                command, switchInterfaceType(interfaceType), ivm::IVInterface::OperationKind::Sporadic);
        translateArguments(command, seds::model::CommandArgumentMode::Out, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::InAndNotify: {
        auto *ivInterfaceIn = createIvInterface(command, interfaceType, ivm::IVInterface::OperationKind::Sporadic);
        translateArguments(command, seds::model::CommandArgumentMode::In, ivInterfaceIn);
        m_ivFunction->addChild(ivInterfaceIn);

        auto *ivInterfaceNotify = createIvInterface(
                command, switchInterfaceType(interfaceType), ivm::IVInterface::OperationKind::Sporadic);
        translateArguments(command, seds::model::CommandArgumentMode::Notify, ivInterfaceNotify);
        m_ivFunction->addChild(ivInterfaceNotify);
    } break;
    case seds::model::ArgumentsCombination::NoArgs:
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

void AsyncInterfaceCommandTranslator::translateArguments(const seds::model::InterfaceCommand &command,
        seds::model::CommandArgumentMode requestedArgumentMode, ivm::IVInterface *ivInterface)
{
    const auto asn1TypeName = buildAsn1SequenceType(command, requestedArgumentMode);

    auto ivParameter = shared::InterfaceParameter(m_interfaceParameterName, shared::BasicParameter::Type::Other,
            asn1TypeName, m_interfaceParameterEncoding, shared::InterfaceParameter::Direction::IN);
    ivInterface->addParam(ivParameter);
}

QString AsyncInterfaceCommandTranslator::buildAsn1SequenceType(
        const seds::model::InterfaceCommand &command, seds::model::CommandArgumentMode requestedArgumentMode) const
{
    std::unordered_map<QString, QString> arguments;
    for (const auto &argument : command.arguments()) {
        if (argument.mode() == requestedArgumentMode) {
            const auto &genericTypeName = argument.type().nameStr();
            const auto &concreteTypeName = findMappedType(genericTypeName);

            arguments.insert({ argument.nameStr(), concreteTypeName });
        }
    }

    std::size_t asn1TypeHash = 0;
    for (const auto &[name, typeName] : arguments) {
        const auto typeNameHash = std::hash<QString> {}(typeName);

        if (asn1TypeHash == 0) {
            asn1TypeHash = typeNameHash;
        } else {
            asn1TypeHash ^= (typeNameHash << 1);
        }
    }

    const auto &commandName = command.nameStr();
    const auto cachedTypesCount = m_asn1CommandArgumentsCache.count(commandName);

    if (cachedTypesCount == 0) {
        const auto asn1TypeName = m_asn1BundledTypeTemplate.arg(commandName, "");
        createAsn1Sequence(asn1TypeName, arguments);

        m_asn1CommandArgumentsCache.insert({ commandName, { asn1TypeHash, asn1TypeName } });

        return asn1TypeName;
    } else {
        const auto hashes = m_asn1CommandArgumentsCache.equal_range(commandName);
        const auto foundHash = std::find_if(hashes.first, hashes.second,
                [&asn1TypeHash](const auto &hashPair) { return hashPair.second.first == asn1TypeHash; });

        if (foundHash != hashes.second) {
            return foundHash->second.second;
        } else {
            const auto asn1TypeName = m_asn1BundledTypeTemplate.arg(commandName).arg(cachedTypesCount);
            createAsn1Sequence(asn1TypeName, arguments);

            m_asn1CommandArgumentsCache.insert({ commandName, { asn1TypeHash, asn1TypeName } });

            return asn1TypeName;
        }
    }
}

void AsyncInterfaceCommandTranslator::createAsn1Sequence(
        const QString &name, const std::unordered_map<QString, QString> &arguments) const
{
    auto asn1Type = std::make_unique<Asn1Acn::Types::Sequence>(name);

    for (const auto &[argumentName, argumentTypeName] : arguments) {
        asn1Type->addComponent(createAsn1SequenceComponent(argumentName, argumentTypeName));
    }

    auto asn1TypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(
            asn1Type->identifier(), asn1Type->identifier(), Asn1Acn::SourceLocation(), std::move(asn1Type));
    m_asn1Definitions->addType(std::move(asn1TypeAssignment));
}

std::unique_ptr<Asn1Acn::SequenceComponent> AsyncInterfaceCommandTranslator::createAsn1SequenceComponent(
        const QString &name, const QString typeName) const
{
    const auto *asn1ReferencedTypeAssignment = m_asn1Definitions->type(typeName);
    const auto *asn1ReferencedType = asn1ReferencedTypeAssignment->type();

    auto componentType = std::make_unique<Asn1Acn::Types::UserdefinedType>(
            asn1ReferencedType->identifier(), m_asn1Definitions->name(), asn1ReferencedTypeAssignment);
    componentType->setType(asn1ReferencedType->clone());

    return std::make_unique<Asn1Acn::AsnSequenceComponent>(
            name, name, false, "", Asn1Acn::SourceLocation(), std::move(componentType));
}

const QString &AsyncInterfaceCommandTranslator::findMappedType(const QString &genericTypeName) const
{
    const auto &genericTypeMaps = m_interface.genericTypeMapSet().genericTypeMaps();

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
