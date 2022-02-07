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

#include "specialized/syncinterfacecommandtranslator.h"

#include "interfacetranslatorhelper.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <shared/parameter.h>

using conversion::translator::TranslationException;

namespace conversion::iv::translator {

SyncInterfaceCommandTranslator::SyncInterfaceCommandTranslator(ivm::IVFunction *ivFunction,
        const QString &sedsInterfaceName, Asn1Acn::Definitions *asn1Definitions,
        const seds::model::Package *sedsPackage, const GenericTypeMapper *typeMapper)
    : m_ivFunction(ivFunction)
    , m_sedsInterfaceName(sedsInterfaceName)
    , m_asn1Definitions(asn1Definitions)
    , m_sedsPackage(sedsPackage)
    , m_typeMapper(typeMapper)
{
}

void SyncInterfaceCommandTranslator::translateCommand(
        const seds::model::InterfaceCommand &sedsCommand, ivm::IVInterface::InterfaceType interfaceType)
{
    const auto interfaceName = InterfaceTranslatorHelper::buildCommandInterfaceName(
            m_sedsInterfaceName, interfaceType, sedsCommand.nameStr());

    switch (sedsCommand.argumentsCombination()) {
    case seds::model::ArgumentsCombination::InOnly:
    case seds::model::ArgumentsCombination::OutOnly:
    case seds::model::ArgumentsCombination::InAndOut: {
        auto *ivInterface = InterfaceTranslatorHelper::createIvInterface(
                interfaceName, interfaceType, ivm::IVInterface::OperationKind::Protected, m_ivFunction);
        translateArguments(sedsCommand.arguments(), ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::NoArgs:
    case seds::model::ArgumentsCombination::NotifyOnly:
    case seds::model::ArgumentsCombination::InAndNotify:
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

void SyncInterfaceCommandTranslator::translateArguments(
        const std::vector<seds::model::CommandArgument> &sedsArguments, ivm::IVInterface *ivInterface)
{
    for (const auto &sedsArgument : sedsArguments) {
        const auto sedsArgumentTypeName = handleArgumentType(sedsArgument, ivInterface->title());

        switch (sedsArgument.mode()) {
        case seds::model::CommandArgumentMode::In: {
            const auto ivParameter = InterfaceTranslatorHelper::createInterfaceParameter(
                    sedsArgument.nameStr(), sedsArgumentTypeName, shared::InterfaceParameter::Direction::IN);
            ivInterface->addParam(ivParameter);
        } break;
        case seds::model::CommandArgumentMode::Out: {
            const auto ivParameter = InterfaceTranslatorHelper::createInterfaceParameter(
                    sedsArgument.nameStr(), sedsArgumentTypeName, shared::InterfaceParameter::Direction::OUT);
            ivInterface->addParam(ivParameter);
        } break;
        case seds::model::CommandArgumentMode::InOut: {
            const auto ivParameterIn =
                    InterfaceTranslatorHelper::createInterfaceParameter(QString("%1_In").arg(sedsArgument.nameStr()),
                            sedsArgumentTypeName, shared::InterfaceParameter::Direction::IN);
            ivInterface->addParam(ivParameterIn);
            const auto ivParameterOut =
                    InterfaceTranslatorHelper::createInterfaceParameter(QString("%2_Out").arg(sedsArgument.nameStr()),
                            sedsArgumentTypeName, shared::InterfaceParameter::Direction::OUT);
            ivInterface->addParam(ivParameterOut);
        } break;
        case seds::model::CommandArgumentMode::Notify:
            throw UnsupportedValueException("CommandArgumentMode", "Notify");
            break;
        }
    }
}

QString SyncInterfaceCommandTranslator::handleArgumentType(
        const seds::model::CommandArgument &sedsArgument, const QString interfaceName) const
{
    const auto argumentTypeName = sedsArgument.type().nameStr();

    const auto typeMapping = m_typeMapper->getMapping(argumentTypeName);

    if (typeMapping == nullptr) {
        return argumentTypeName;
    }

    const auto &concreteTypes = typeMapping->concreteTypes;

    if (concreteTypes.empty()) {
        auto errorMessage = QString("Type \"%1\" of the argument \"%2\" in the sync interface \"%3\" is handled as "
                                    "generic, but no mappings was provided")
                                    .arg(argumentTypeName)
                                    .arg(sedsArgument.nameStr())
                                    .arg(interfaceName);
        throw TranslationException(std::move(errorMessage));
    } else if (concreteTypes.size() != 1) {
        auto errorMessage = QString("Generic type \"%1\" of the argument \"%2\" in the sync interface \"%3\" can only "
                                    "be simply mapped (AlternateSet not supported)")
                                    .arg(argumentTypeName)
                                    .arg(sedsArgument.nameStr())
                                    .arg(interfaceName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto argumentConcreteTypeName = concreteTypes.front().typeName;

    if (sedsArgument.arrayDimensions().empty()) {
        return Escaper::escapeAsn1TypeName(argumentConcreteTypeName);
    } else {
        return InterfaceTranslatorHelper::createArrayType(
                argumentConcreteTypeName, sedsArgument.arrayDimensions(), m_asn1Definitions, m_sedsPackage);
    }
}

} // namespace conversion::iv::translator
