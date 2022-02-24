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

#include <conversion/common/translation/exceptions.h>

using conversion::UnhandledValueException;
using conversion::translator::TranslationException;

namespace conversion::iv::translator {

const QString InterfaceParameterTranslator::m_ivInterfaceParameterName = "Param";

InterfaceParameterTranslator::InterfaceParameterTranslator(
        ivm::IVFunction *ivFunction, const QString &sedsInterfaceName, const GenericTypeMapper *typeMapper)
    : m_ivFunction(ivFunction)
    , m_sedsInterfaceName(sedsInterfaceName)
    , m_typeMapper(typeMapper)
{
}

void InterfaceParameterTranslator::translateParameter(
        const seds::model::InterfaceParameter &sedsParameter, const ivm::IVInterface::InterfaceType interfaceType)
{
    translateGetterParameter(sedsParameter, interfaceType);

    if (!sedsParameter.isReadOnly()) {
        translateSetterParameter(sedsParameter, interfaceType);
    }
}

void InterfaceParameterTranslator::translateGetterParameter(
        const seds::model::InterfaceParameter &sedsParameter, const ivm::IVInterface::InterfaceType interfaceType)
{
    switch (sedsParameter.mode()) {
    case seds::model::InterfaceParameterMode::Sync: {
        buildParameter(sedsParameter, InterfaceTranslatorHelper::InterfaceParameterType::Getter, interfaceType,
                ivm::IVInterface::OperationKind::Protected, shared::InterfaceParameter::Direction::OUT);
    } break;
    case seds::model::InterfaceParameterMode::Async: {
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
        const seds::model::InterfaceParameter &sedsParameter, const ivm::IVInterface::InterfaceType interfaceType)
{
    switch (sedsParameter.mode()) {
    case seds::model::InterfaceParameterMode::Sync: {
        buildParameter(sedsParameter, InterfaceTranslatorHelper::InterfaceParameterType::Setter, interfaceType,
                ivm::IVInterface::OperationKind::Protected, shared::InterfaceParameter::Direction::IN);
    } break;
    case seds::model::InterfaceParameterMode::Async: {
        buildParameter(sedsParameter, InterfaceTranslatorHelper::InterfaceParameterType::Setter, interfaceType,
                ivm::IVInterface::OperationKind::Sporadic, shared::InterfaceParameter::Direction::IN);
    } break;
    default:
        throw UnhandledValueException("InterfaceParameterMode");
        break;
    }
}

void InterfaceParameterTranslator::buildParameter(const seds::model::InterfaceParameter &sedsParameter,
        InterfaceTranslatorHelper::InterfaceParameterType interfaceParameterType,
        const ivm::IVInterface::InterfaceType interfaceType, ivm::IVInterface::OperationKind interfaceKind,
        shared::InterfaceParameter::Direction interfaceDirection)
{
    const auto &parameterTypeName = handleParameterTypeName(sedsParameter);

    const auto ivParameter = InterfaceTranslatorHelper::createInterfaceParameter(
            m_ivInterfaceParameterName, parameterTypeName, interfaceDirection);

    const auto interfaceName = InterfaceTranslatorHelper::buildParameterInterfaceName(
            m_sedsInterfaceName, sedsParameter.nameStr(), interfaceParameterType, interfaceType);
    const auto ivInterface =
            InterfaceTranslatorHelper::createIvInterface(interfaceName, interfaceType, interfaceKind, m_ivFunction);
    ivInterface->addParam(ivParameter);

    m_ivFunction->addChild(ivInterface);
}

QString InterfaceParameterTranslator::handleParameterTypeName(
        const seds::model::InterfaceParameter &sedsParameter) const
{
    const auto &parameterTypeName = sedsParameter.type().nameStr();

    const auto typeMapping = m_typeMapper->getMapping(parameterTypeName);

    if (typeMapping == nullptr) {
        return parameterTypeName;
    }

    const auto &concreteTypes = typeMapping->concreteTypes;

    if (concreteTypes.empty()) {
        auto errorMessage =
                QString("Type \"%1\" of the parameter \"%2\" is handled as generic, but no mappings was provided")
                        .arg(parameterTypeName)
                        .arg(sedsParameter.nameStr());
        throw TranslationException(std::move(errorMessage));
    } else if (concreteTypes.size() != 1) {
        auto errorMessage = QString(
                "Generic type \"%1\" of the parameter \"%2\" can only be simply mapped (AlternateSet not supported)")
                                    .arg(parameterTypeName)
                                    .arg(sedsParameter.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    return concreteTypes.front().typeName;
}

} // namespace conversion::iv::translator
