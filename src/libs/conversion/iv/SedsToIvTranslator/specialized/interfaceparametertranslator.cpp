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

#include "specialized/interfaceparametertranslator.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
#include <seds/SedsModel/components/interface.h>
#include <seds/SedsModel/interfaces/interfaceparameter.h>
#include <shared/parameter.h>

using conversion::UnhandledValueException;
using conversion::translator::TranslationException;

namespace conversion::iv::translator {

const QString InterfaceParameterTranslator::m_interfaceParameterEncoding = "ACN";
const QString InterfaceParameterTranslator::m_getterInterfacePrefix = "Get";
const QString InterfaceParameterTranslator::m_setterInterfacePrefix = "Set";
const QString InterfaceParameterTranslator::m_ivInterfaceNameTemplate = "%1_%2_%3_%4";
const QString InterfaceParameterTranslator::m_ivInterfaceParameterName = "Param";

InterfaceParameterTranslator::InterfaceParameterTranslator(
        const seds::model::Interface &sedsInterface, ivm::IVFunction *ivFunction)
    : m_sedsInterface(sedsInterface)
    , m_ivFunction(ivFunction)
{
}

void InterfaceParameterTranslator::translateParameter(
        const seds::model::InterfaceParameter &sedsParameter, const ivm::IVInterface::InterfaceType interfaceType) const
{
    translateGetterParameter(sedsParameter, interfaceType);
    if (!sedsParameter.isReadOnly()) {
        translateSetterParameter(sedsParameter, interfaceType);
    }
}

void InterfaceParameterTranslator::translateGetterParameter(
        const seds::model::InterfaceParameter &sedsParameter, const ivm::IVInterface::InterfaceType interfaceType) const
{
    switch (sedsParameter.mode()) {
    case seds::model::InterfaceParameterMode::Sync:
        createIvInterface(InterfaceMode::Getter, sedsParameter, interfaceType,
                ivm::IVInterface::OperationKind::Protected, shared::InterfaceParameter::Direction::OUT);
        break;
    case seds::model::InterfaceParameterMode::Async:
        createIvInterface(InterfaceMode::Getter, sedsParameter, switchInterfaceType(interfaceType),
                ivm::IVInterface::OperationKind::Sporadic, shared::InterfaceParameter::Direction::IN);
        break;
    default:
        throw UnhandledValueException("InterfaceParameterMode");
        break;
    }
}

void InterfaceParameterTranslator::translateSetterParameter(
        const seds::model::InterfaceParameter &sedsParameter, const ivm::IVInterface::InterfaceType interfaceType) const
{
    switch (sedsParameter.mode()) {
    case seds::model::InterfaceParameterMode::Sync:
        createIvInterface(InterfaceMode::Setter, sedsParameter, interfaceType,
                ivm::IVInterface::OperationKind::Protected, shared::InterfaceParameter::Direction::IN);
        break;
    case seds::model::InterfaceParameterMode::Async:
        createIvInterface(InterfaceMode::Setter, sedsParameter, interfaceType,
                ivm::IVInterface::OperationKind::Sporadic, shared::InterfaceParameter::Direction::IN);
        break;
    default:
        throw UnhandledValueException("InterfaceParameterMode");
        break;
    }
}

void InterfaceParameterTranslator::createIvInterface(const InterfaceParameterTranslator::InterfaceMode mode,
        const seds::model::InterfaceParameter &sedsParameter, ivm::IVInterface::InterfaceType type,
        ivm::IVInterface::OperationKind kind, shared::InterfaceParameter::Direction direction) const
{
    const auto name = getParameterName(mode, m_sedsInterface.nameStr(), type, sedsParameter.nameStr());

    ivm::IVInterface::CreationInfo creationInfo;
    creationInfo.function = m_ivFunction;
    creationInfo.type = type;
    creationInfo.name = name;
    creationInfo.kind = kind;

    auto ivInterface = ivm::IVInterface::createIface(creationInfo);

    auto ivParameter = shared::InterfaceParameter(m_ivInterfaceParameterName, shared::BasicParameter::Type::Other,
            sedsParameter.type().nameStr(), m_interfaceParameterEncoding, direction);
    ivInterface->addParam(ivParameter);

    m_ivFunction->addChild(ivInterface);
}

ivm::IVInterface::InterfaceType InterfaceParameterTranslator::switchInterfaceType(
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

const QString &InterfaceParameterTranslator::interfaceTypeToString(ivm::IVInterface::InterfaceType type)
{
    switch (type) {
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

QString InterfaceParameterTranslator::getParameterName(const InterfaceParameterTranslator::InterfaceMode mode,
        const QString &interfaceName, const ivm::IVInterface::InterfaceType type, const QString &parameterName)
{
    QString namePrefix;

    switch (mode) {
    case InterfaceMode::Getter:
        namePrefix = m_getterInterfacePrefix;
        break;
    case InterfaceMode::Setter:
        namePrefix = m_setterInterfacePrefix;
        break;
    default:
        throw UnhandledValueException("InterfaceMode");
        break;
    }

    return Escaper::escapeIvName(m_ivInterfaceNameTemplate.arg(namePrefix)
                                         .arg(interfaceName)
                                         .arg(parameterName)
                                         .arg(interfaceTypeToString(type)));
}

} // namespace conversion::iv::translator
