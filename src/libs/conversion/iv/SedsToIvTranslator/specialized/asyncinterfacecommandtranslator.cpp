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

std::set<std::size_t> AsyncInterfaceCommandTranslator::m_asn1InterfaceArgumentsHashesCache;

const QString AsyncInterfaceCommandTranslator::m_interfaceParameterName = "InputParam";
const QString AsyncInterfaceCommandTranslator::m_interfaceParameterEncoding = "ACN";
const QString AsyncInterfaceCommandTranslator::m_asn1BundledTypeTemplate = "%1_%2_Type";
const QString AsyncInterfaceCommandTranslator::m_ivInterfaceNameTemplate = "%1_%2_%3";

AsyncInterfaceCommandTranslator::AsyncInterfaceCommandTranslator(const seds::model::Package &package,
        const seds::model::Component &component, const seds::model::Interface &interface, ivm::IVFunction *ivFunction,
        Asn1Acn::Definitions *asn1Definitions)
    : m_package(package)
    , m_component(component)
    , m_interface(interface)
    , m_ivFunction(ivFunction)
    , m_asn1Definitions(asn1Definitions)
{
}

void AsyncInterfaceCommandTranslator::translateCommand(
        const seds::model::InterfaceCommand &command, ivm::IVInterface::InterfaceType interfaceType)
{
    switch (command.argumentsCombination()) {
    case seds::model::ArgumentsCombination::InOnly: {
        auto *ivInterface = createIvInterface(command, interfaceType);
        translateArguments(command, seds::model::CommandArgumentMode::In, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::OutOnly: {
        auto *ivInterface = createIvInterface(command, switchInterfaceType(interfaceType));
        translateArguments(command, seds::model::CommandArgumentMode::Out, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::InAndNotify: {
        auto *ivInterfaceIn = createIvInterface(command, interfaceType);
        translateArguments(command, seds::model::CommandArgumentMode::In, ivInterfaceIn);
        m_ivFunction->addChild(ivInterfaceIn);

        auto *ivInterfaceNotify = createIvInterface(command, switchInterfaceType(interfaceType));
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
    const auto asn1TypeName = m_asn1BundledTypeTemplate.arg(m_interface.nameStr()).arg(command.nameStr());

    buildAsn1SequenceType(command, requestedArgumentMode, asn1TypeName);

    auto ivParameter = shared::InterfaceParameter(m_interfaceParameterName, shared::BasicParameter::Type::Other,
            asn1TypeName, m_interfaceParameterEncoding, shared::InterfaceParameter::Direction::IN);
    ivInterface->addParam(ivParameter);
}

void AsyncInterfaceCommandTranslator::buildAsn1SequenceType(const seds::model::InterfaceCommand &command,
        seds::model::CommandArgumentMode requestedArgumentMode, const QString &asn1TypeName) const
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

    if (m_asn1InterfaceArgumentsHashesCache.count(asn1TypeHash) == 0) {
        auto asn1Type = std::make_unique<Asn1Acn::Types::Sequence>(asn1TypeName);

        for (const auto &[name, typeName] : arguments) {
            asn1Type->addComponent(createAsn1SequenceComponent(name, typeName));
        }

        auto asn1TypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(
                asn1Type->identifier(), asn1Type->identifier(), Asn1Acn::SourceLocation(), std::move(asn1Type));
        m_asn1Definitions->addType(std::move(asn1TypeAssignment));

        m_asn1InterfaceArgumentsHashesCache.insert(asn1TypeHash);
    }
}

std::unique_ptr<Asn1Acn::AsnSequenceComponent> AsyncInterfaceCommandTranslator::createAsn1SequenceComponent(
        const QString &name, const QString &argumentTypeName) const
{
    const auto &argumentType = findDataType(argumentTypeName);

    std::unique_ptr<Asn1Acn::Types::Type> asn1Type;
    std::visit(conversion::asn1::translator::DataTypeTranslatorVisitor { asn1Type }, argumentType);

    auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            name, name, false, "", Asn1Acn::SourceLocation(), std::move(asn1Type));

    return sequenceComponent;
}

ivm::IVInterface *AsyncInterfaceCommandTranslator::createIvInterface(
        const seds::model::InterfaceCommand &command, ivm::IVInterface::InterfaceType type) const
{
    ivm::IVInterface::CreationInfo creationInfo;
    creationInfo.function = m_ivFunction;
    creationInfo.type = type;
    creationInfo.name = m_ivInterfaceNameTemplate.arg(m_interface.nameStr())
                                .arg(command.nameStr())
                                .arg(interfaceTypeToString(type));
    creationInfo.kind = ivm::IVInterface::OperationKind::Sporadic;

    return ivm::IVInterface::createIface(creationInfo);
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

const seds::model::DataType &AsyncInterfaceCommandTranslator::findDataType(const QString &dataTypeName) const
{
    const auto &componentDataTypes = m_component.dataTypes();
    auto found = std::find_if(componentDataTypes.begin(), componentDataTypes.end(),
            [&dataTypeName](
                    const seds::model::DataType &dataType) { return dataTypeNameStr(dataType) == dataTypeName; });
    if (found != componentDataTypes.end()) {
        return *found;
    }

    const auto &packageDataTypes = m_package.dataTypes();
    found = std::find_if(
            packageDataTypes.begin(), packageDataTypes.end(), [&dataTypeName](const seds::model::DataType &dataType) {
                return dataTypeNameStr(dataType) == dataTypeName;
            });
    if (found != packageDataTypes.end()) {
        return *found;
    }

    throw UndeclaredDataTypeException(dataTypeName);
}

const QString &AsyncInterfaceCommandTranslator::interfaceTypeToString(
        ivm::IVInterface::InterfaceType interfaceType) const
{
    switch (interfaceType) {
    case ivm::IVInterface::InterfaceType::Required: {
        static QString name = "Ri";
        return name;
    }
    case ivm::IVInterface::InterfaceType::Provided: {
        static QString name = "Pi";
        return name;
    }
    case ivm::IVInterface::InterfaceType::Grouped: {
        static QString name = "Grp";
        return name;
    }
    default:
        throw UnhandledValueException("ivm::InterfaceType");
        break;
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
