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

#include <conversion/asn1/SedsToAsn1Translator/constants.h>
#include <conversion/asn1/SedsToAsn1Translator/datatypetranslationhelper.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsOptions/options.h>
#include <shared/parameter.h>

using conversion::asn1::translator::seds::DataTypeTranslationHelper;
using conversion::seds::SedsOptions;
using conversion::translator::TranslationException;

namespace conversion::iv::translator::seds {

SyncInterfaceCommandTranslator::SyncInterfaceCommandTranslator(ivm::IVFunction *ivFunction,
        const QString &sedsInterfaceName, const InterfaceTypeNameHelper &typeNameHelper, const Options &options)
    : m_ivFunction(ivFunction)
    , m_sedsInterfaceName(sedsInterfaceName)
    , m_typeNameHelper(typeNameHelper)
    , m_options(options)
{
}

void SyncInterfaceCommandTranslator::translateCommand(
        const ::seds::model::InterfaceCommand &sedsCommand, ivm::IVInterface::InterfaceType interfaceType)
{
    const auto interfaceName = InterfaceTranslatorHelper::buildCommandInterfaceName(
            m_sedsInterfaceName, sedsCommand.nameStr(), interfaceType, m_options);

    switch (sedsCommand.argumentsCombination()) {
    case ::seds::model::ArgumentsCombination::NoArgs:
    case ::seds::model::ArgumentsCombination::InOnly:
    case ::seds::model::ArgumentsCombination::OutOnly:
    case ::seds::model::ArgumentsCombination::InAndOut: {
        auto *ivInterface = InterfaceTranslatorHelper::createIvInterface(
                interfaceName, interfaceType, ivm::IVInterface::OperationKind::Protected, sedsCommand, m_ivFunction);
        translateArguments(sedsCommand.arguments(), ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case ::seds::model::ArgumentsCombination::NotifyOnly:
    case ::seds::model::ArgumentsCombination::InAndNotify:
    case ::seds::model::ArgumentsCombination::OutAndNotify:
    case ::seds::model::ArgumentsCombination::All: {
        const auto message = QString(
                "Interface command arguments combination '%1' is not supported for TASTE InterfaceView sync interface")
                                     .arg(argumentsCombinationToString(sedsCommand.argumentsCombination()));
        throw TranslationException(message);
    } break;
    default:
        throw UnhandledValueException("ArgumentsCombination");
        break;
    }
}

void SyncInterfaceCommandTranslator::translateArguments(
        const std::vector<::seds::model::CommandArgument> &sedsArguments, ivm::IVInterface *ivInterface)
{
    for (const auto &sedsArgument : sedsArguments) {
        const auto sedsArgumentTypeName = handleArgumentTypeName(sedsArgument);

        switch (sedsArgument.mode()) {
        case ::seds::model::CommandArgumentMode::In: {
            const auto ivParameter = InterfaceTranslatorHelper::createInterfaceParameter(
                    sedsArgument.nameStr(), sedsArgumentTypeName, shared::InterfaceParameter::Direction::IN);
            ivInterface->addParam(ivParameter);
        } break;
        case ::seds::model::CommandArgumentMode::Out: {
            const auto ivParameter = InterfaceTranslatorHelper::createInterfaceParameter(
                    sedsArgument.nameStr(), sedsArgumentTypeName, shared::InterfaceParameter::Direction::OUT);
            ivInterface->addParam(ivParameter);
        } break;
        case ::seds::model::CommandArgumentMode::InOut: {
            const auto ivParameterIn =
                    InterfaceTranslatorHelper::createInterfaceParameter(QString("%1_In").arg(sedsArgument.nameStr()),
                            sedsArgumentTypeName, shared::InterfaceParameter::Direction::IN);
            ivInterface->addParam(ivParameterIn);
            const auto ivParameterOut =
                    InterfaceTranslatorHelper::createInterfaceParameter(QString("%2_Out").arg(sedsArgument.nameStr()),
                            sedsArgumentTypeName, shared::InterfaceParameter::Direction::OUT);
            ivInterface->addParam(ivParameterOut);
        } break;
        case ::seds::model::CommandArgumentMode::Notify:
            throw UnsupportedValueException("CommandArgumentMode", "Notify");
            break;
        }
    }

    if (m_options.isSet(SedsOptions::enableFailureReporting)) {
        if (!m_options.isSet(SedsOptions::failureReportingType)) {
            throw TranslationException(
                    "SEDS failure reporting feature was used but no ASN.1 type for failure parameter was specified");
        }

        const auto &failureParamName = asn1::translator::seds::Constants::failureParamName;
        const auto failureParamTypeName = m_options.value(SedsOptions::failureReportingType).value();

        const auto failureParam = InterfaceTranslatorHelper::createInterfaceParameter(
                failureParamName, failureParamTypeName, shared::InterfaceParameter::Direction::OUT);
        ivInterface->addParam(failureParam);
    }
}

QString SyncInterfaceCommandTranslator::handleArgumentTypeName(const ::seds::model::CommandArgument &sedsArgument) const
{
    const auto &argumentTypeRef = sedsArgument.type();
    const auto &argumentDimensions = sedsArgument.arrayDimensions();

    return m_typeNameHelper.handleTypeName(argumentTypeRef, argumentDimensions);
}

} // namespace conversion::iv::translator::seds
