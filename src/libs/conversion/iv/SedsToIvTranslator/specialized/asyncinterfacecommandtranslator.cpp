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
#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
#include <ivcore/parameter.h>
#include <seds/SedsModel/generics/generictypemapset.h>
#include <seds/SedsModel/interfaces/argumentscombination.h>
#include <seds/SedsModel/interfaces/interfacecommand.h>

using conversion::translator::MissingGenericTypeMappingException;
using conversion::translator::TranslationException;
using conversion::translator::UnhandledValueException;

namespace conversion::iv::translator {

AsyncInterfaceCommandTranslator::AsyncInterfaceCommandTranslator(
        const seds::model::GenericTypeMapSet &typeMappings, ivm::IVFunction *ivFunction)
    : m_typeMappings(typeMappings)
    , m_ivFunction(ivFunction)
{
}

void AsyncInterfaceCommandTranslator::translateCommand(
        const seds::model::InterfaceCommand &command, ivm::IVInterface::InterfaceType interfaceType) const
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
        seds::model::CommandArgumentMode requestedArgumentMode, ivm::IVInterface *ivInterface) const
{
    for (const auto &argument : arguments) {
        if (argument.mode() != requestedArgumentMode) {
            continue;
        }

        translateArgument(argument, ivInterface);
    }
}

void AsyncInterfaceCommandTranslator::translateArgument(
        const seds::model::CommandArgument &argument, ivm::IVInterface *ivInterface) const
{
    const auto &genericArgumentTypeName = argument.type().nameStr();
    const auto concreteArgumentTypeName = findMappedType(genericArgumentTypeName).nameStr();

    const auto ivParameter = ivm::InterfaceParameter(argument.nameStr(), ivm::BasicParameter::Type::Other,
            concreteArgumentTypeName, QStringLiteral("ACN"), ivm::InterfaceParameter::Direction::IN);

    ivInterface->addParam(ivParameter);
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

const seds::model::DataTypeRef &AsyncInterfaceCommandTranslator::findMappedType(const QString &genericTypeName) const
{
    const auto &genericTypeMaps = m_typeMappings.genericTypeMaps();

    const auto found = std::find_if(genericTypeMaps.begin(), genericTypeMaps.end(),
            [&genericTypeName](
                    const seds::model::GenericTypeMap &typeMap) { return typeMap.nameStr() == genericTypeName; });

    if (found != genericTypeMaps.end()) {
        return found->type();
    }

    throw MissingGenericTypeMappingException(genericTypeName);
}

} // namespace conversion::iv::translator
