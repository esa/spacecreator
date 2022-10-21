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

#include "specialized/interfaceparametertranslator.h"

#include <conversion/asn1/SedsToAsn1Translator/constants.h>
#include <conversion/asn1/SedsToAsn1Translator/datatypetranslationhelper.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsOptions/options.h>

using conversion::UnhandledValueException;
using conversion::asn1::translator::seds::DataTypeTranslationHelper;
using conversion::seds::SedsOptions;
using conversion::translator::TranslationException;

namespace conversion::iv::translator::seds {

InterfaceParameterTranslator::InterfaceParameterTranslator(ivm::IVFunction *ivFunction,
        const QString &sedsInterfaceName, const InterfaceTypeNameHelper &typeNameHelper, const Options &options)
    : m_ivFunction(ivFunction)
    , m_sedsInterfaceName(sedsInterfaceName)
    , m_typeNameHelper(typeNameHelper)
    , m_options(options)
{
}

void InterfaceParameterTranslator::translateParameter(
        const ::seds::model::InterfaceParameter &sedsParameter, const ivm::IVInterface::InterfaceType interfaceType)
{
    translateGetterParameter(sedsParameter, interfaceType);

    if (!sedsParameter.isReadOnly()) {
        translateSetterParameter(sedsParameter, interfaceType);
    }
}

void InterfaceParameterTranslator::translateGetterParameter(
        const ::seds::model::InterfaceParameter &sedsParameter, const ivm::IVInterface::InterfaceType interfaceType)
{
    switch (sedsParameter.mode()) {
    case ::seds::model::InterfaceParameterMode::Sync: {
        buildParameter(sedsParameter, InterfaceTranslatorHelper::InterfaceParameterType::Getter, interfaceType,
                ivm::IVInterface::OperationKind::Protected, shared::InterfaceParameter::Direction::OUT);
    } break;
    case ::seds::model::InterfaceParameterMode::Async: {
        buildParameter(sedsParameter, InterfaceTranslatorHelper::InterfaceParameterType::Getter,
                InterfaceTranslatorHelper::switchInterfaceType(interfaceType),
                ivm::IVInterface::OperationKind::Sporadic, shared::InterfaceParameter::Direction::IN);
    } break;
    default:
        throw UnhandledValueException("InterfaceParameterMode");
        break;
    }
}

void InterfaceParameterTranslator::translateSetterParameter(
        const ::seds::model::InterfaceParameter &sedsParameter, const ivm::IVInterface::InterfaceType interfaceType)
{
    switch (sedsParameter.mode()) {
    case ::seds::model::InterfaceParameterMode::Sync: {
        buildParameter(sedsParameter, InterfaceTranslatorHelper::InterfaceParameterType::Setter, interfaceType,
                ivm::IVInterface::OperationKind::Protected, shared::InterfaceParameter::Direction::IN);
    } break;
    case ::seds::model::InterfaceParameterMode::Async: {
        buildParameter(sedsParameter, InterfaceTranslatorHelper::InterfaceParameterType::Setter, interfaceType,
                ivm::IVInterface::OperationKind::Sporadic, shared::InterfaceParameter::Direction::IN);
    } break;
    default:
        throw UnhandledValueException("InterfaceParameterMode");
        break;
    }
}

void InterfaceParameterTranslator::buildParameter(const ::seds::model::InterfaceParameter &sedsParameter,
        InterfaceTranslatorHelper::InterfaceParameterType interfaceParameterType,
        const ivm::IVInterface::InterfaceType interfaceType, ivm::IVInterface::OperationKind interfaceKind,
        shared::InterfaceParameter::Direction interfaceDirection)
{
    const auto &parameterTypeName = handleParameterTypeName(sedsParameter);

    const auto ivParameter = InterfaceTranslatorHelper::createInterfaceParameter(
            m_ivInterfaceParameterName, parameterTypeName, interfaceDirection);

    const auto interfaceName = InterfaceTranslatorHelper::buildParameterInterfaceName(
            m_sedsInterfaceName, sedsParameter.nameStr(), interfaceParameterType, interfaceType);
    const auto ivInterface = InterfaceTranslatorHelper::createIvInterface(
            interfaceName, interfaceType, interfaceKind, sedsParameter, m_ivFunction);
    ivInterface->addParam(ivParameter);

    if (m_options.isSet(SedsOptions::enableFailureReporting)) {
        handleFailureReporting(ivInterface);
    }

    m_ivFunction->addChild(ivInterface);
}

QString InterfaceParameterTranslator::handleParameterTypeName(
        const ::seds::model::InterfaceParameter &sedsParameter) const
{
    const auto &parameterTypeRef = sedsParameter.type();
    const auto &parameterDimensions = sedsParameter.arrayDimensions();

    return m_typeNameHelper.handleTypeName(parameterTypeRef, parameterDimensions);
}

void InterfaceParameterTranslator::handleFailureReporting(ivm::IVInterface *ivInterface) const
{
    if (!m_options.isSet(SedsOptions::failureReportingType)) {
        throw TranslationException(
                "SEDS failure reporting feature was used but no ASN.1 type for failure parameter was specified");
    }

    const auto failureParamTypeName = m_options.value(SedsOptions::failureReportingType).value();

    // Add transaction name parameter to the IV interface
    const auto ivParameter =
            InterfaceTranslatorHelper::createInterfaceParameter(asn1::translator::seds::Constants::failureParamName,
                    failureParamTypeName, shared::InterfaceParameter::Direction::OUT);
    ivInterface->addParam(ivParameter);
}

} // namespace conversion::iv::translator::seds
