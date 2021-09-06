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
#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
#include <ivcore/parameter.h>
#include <seds/SedsModel/package/package.h>

using conversion::translator::MissingGenericTypeMappingException;
using conversion::translator::TranslationException;
using conversion::translator::UndeclaredDataTypeException;
using conversion::translator::UnhandledValueException;

namespace conversion::iv::translator {

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
        auto *ivInterface = createIvInterface(command.nameStr(), interfaceType);
        translateArguments(command.arguments(), seds::model::CommandArgumentMode::In, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::OutOnly: {
        auto *ivInterface = createIvInterface(command.nameStr(), switchInterfaceType(interfaceType));
        translateArguments(command.arguments(), seds::model::CommandArgumentMode::Out, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::InAndNotify: {
        auto *ivInterfaceIn = createIvInterface(command.nameStr() + "_IN", interfaceType);
        translateArguments(command.arguments(), seds::model::CommandArgumentMode::In, ivInterfaceIn);
        m_ivFunction->addChild(ivInterfaceIn);

        auto *ivInterfaceNotify = createIvInterface(command.nameStr() + "_NOTIFY", switchInterfaceType(interfaceType));
        translateArguments(command.arguments(), seds::model::CommandArgumentMode::Notify, ivInterfaceNotify);
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

void AsyncInterfaceCommandTranslator::translateArguments(const std::vector<seds::model::CommandArgument> &arguments,
        seds::model::CommandArgumentMode requestedArgumentMode, ivm::IVInterface *ivInterface)
{
    const auto asn1TypeName = QString("Interface_%1_TypeGroup").arg(m_interface.nameStr());
    auto asn1Type = std::make_unique<Asn1Acn::Types::Sequence>(asn1TypeName);

    for (const auto &argument : arguments) {
        asn1Type->addComponent(std::move(createAsn1SequenceComponent(argument)));
    }

    auto asn1TypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(
            asn1Type->identifier(), asn1Type->identifier(), Asn1Acn::SourceLocation(), std::move(asn1Type));
    m_asn1Definitions->addType(std::move(asn1TypeAssignment));

    const auto ivParameterName = QString("Interface_%1_Parameter").arg(m_interface.nameStr());
    auto ivParameter = ivm::InterfaceParameter(ivParameterName, ivm::BasicParameter::Type::Other, asn1TypeName,
            QStringLiteral("ACN"), ivm::InterfaceParameter::Direction::IN);

    ivInterface->addParam(std::move(ivParameter));
}

std::unique_ptr<Asn1Acn::AsnSequenceComponent> AsyncInterfaceCommandTranslator::createAsn1SequenceComponent(
        const seds::model::CommandArgument &commandArgument) const
{
    const auto &genericArgumentTypeName = commandArgument.type().nameStr();
    const auto &concreteArgumentTypeName = findMappedType(genericArgumentTypeName).nameStr();
    const auto &concreteArgumentType = findDataType(concreteArgumentTypeName);

    std::unique_ptr<Asn1Acn::Types::Type> asn1Type;
    std::visit(conversion::asn1::translator::DataTypeTranslatorVisitor { asn1Type }, concreteArgumentType);

    auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(commandArgument.nameStr(),
            commandArgument.nameStr(), false, "", Asn1Acn::SourceLocation(), std::move(asn1Type));

    return sequenceComponent;
}

ivm::IVInterface *AsyncInterfaceCommandTranslator::createIvInterface(
        const QString &name, ivm::IVInterface::InterfaceType interfaceType) const
{
    ivm::IVInterface::CreationInfo creationInfo;
    creationInfo.function = m_ivFunction;
    creationInfo.type = interfaceType;
    creationInfo.name = name;
    creationInfo.kind = ivm::IVInterface::OperationKind::Sporadic;

    return ivm::IVInterface::createIface(creationInfo);
}

const seds::model::DataTypeRef &AsyncInterfaceCommandTranslator::findMappedType(const QString &genericTypeName) const
{
    const auto &genericTypeMaps = m_interface.genericTypeMapSet().genericTypeMaps();

    const auto found = std::find_if(genericTypeMaps.begin(), genericTypeMaps.end(),
            [&genericTypeName](
                    const seds::model::GenericTypeMap &typeMap) { return typeMap.nameStr() == genericTypeName; });

    if (found != genericTypeMaps.end()) {
        return found->type();
    }

    throw MissingGenericTypeMappingException(genericTypeName, m_interface.nameStr());
}

const seds::model::DataType &AsyncInterfaceCommandTranslator::findDataType(const QString &dataTypeName) const
{
    const auto &componentDataTypes = m_component.dataTypes();
    auto dataType = std::find_if(componentDataTypes.begin(), componentDataTypes.end(),
            [&dataTypeName](
                    const seds::model::DataType &dataType) { return dataTypeNameStr(dataType) == dataTypeName; });
    if (dataType != componentDataTypes.end()) {
        return *dataType;
    }

    const auto &packageDataTypes = m_package.dataTypes();
    dataType = std::find_if(
            packageDataTypes.begin(), packageDataTypes.end(), [&dataTypeName](const seds::model::DataType &dataType) {
                return dataTypeNameStr(dataType) == dataTypeName;
            });
    if (dataType != packageDataTypes.end()) {
        return *dataType;
    }

    throw UndeclaredDataTypeException(dataTypeName);
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
