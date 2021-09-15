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

#include "specialized/syncinterfacecommandtranslator.h"

#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
#include <seds/SedsModel/interfaces/argumentscombination.h>
#include <seds/SedsModel/interfaces/interfacecommand.h>

using conversion::translator::TranslationException;
using conversion::translator::UnhandledValueException;
using conversion::translator::UnsupportedValueException;

namespace conversion::iv::translator {

SyncInterfaceCommandTranslator::SyncInterfaceCommandTranslator(
        const seds::model::Interface &interface, ivm::IVFunction *ivFunction)
    : InterfaceCommandTranslator(interface, ivFunction)
{
}

void SyncInterfaceCommandTranslator::translateCommand(
        const seds::model::InterfaceCommand &command, ivm::IVInterface::InterfaceType interfaceType)
{
    switch (command.argumentsCombination()) {
    case seds::model::ArgumentsCombination::InOnly:
    case seds::model::ArgumentsCombination::OutOnly:
    case seds::model::ArgumentsCombination::InAndOut: {
        auto *ivInterface = createIvInterface(command, interfaceType, ivm::IVInterface::OperationKind::Protected);
        translateArguments(command.arguments(), ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case seds::model::ArgumentsCombination::NoArgs:
    case seds::model::ArgumentsCombination::NotifyOnly:
    case seds::model::ArgumentsCombination::InAndNotify:
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

void SyncInterfaceCommandTranslator::translateArguments(
        const std::vector<seds::model::CommandArgument> &arguments, ivm::IVInterface *ivInterface)
{
    for (const auto &argument : arguments) {
        switch (argument.mode()) {
        case seds::model::CommandArgumentMode::In: {
            const auto ivParameter = createIvInterfaceParameter(
                    argument.nameStr(), argument.type().nameStr(), shared::InterfaceParameter::Direction::IN);
            ivInterface->addParam(ivParameter);
        } break;
        case seds::model::CommandArgumentMode::Out: {
            const auto ivParameter = createIvInterfaceParameter(
                    argument.nameStr(), argument.type().nameStr(), shared::InterfaceParameter::Direction::OUT);
            ivInterface->addParam(ivParameter);
        } break;
        case seds::model::CommandArgumentMode::InOut: {
            const auto ivParameterIn = createIvInterfaceParameter(QString("%1_In").arg(argument.nameStr()),
                    argument.type().nameStr(), shared::InterfaceParameter::Direction::IN);
            ivInterface->addParam(ivParameterIn);
            const auto ivParameterOut = createIvInterfaceParameter(QString("%2_Out").arg(argument.nameStr()),
                    argument.type().nameStr(), shared::InterfaceParameter::Direction::OUT);
            ivInterface->addParam(ivParameterOut);
        } break;
        case seds::model::CommandArgumentMode::Notify:
            throw UnsupportedValueException("CommandArgumentMode", "Notify");
            break;
        }
    }
}

shared::InterfaceParameter SyncInterfaceCommandTranslator::createIvInterfaceParameter(
        const QString &name, const QString &typeName, shared::InterfaceParameter::Direction direction)
{
    return shared::InterfaceParameter(
            name, shared::BasicParameter::Type::Other, typeName, m_interfaceParameterEncoding, direction);
}

} // namespace conversion::iv::translator
