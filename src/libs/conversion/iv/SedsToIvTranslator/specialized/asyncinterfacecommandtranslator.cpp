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

#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
#include <seds/SedsModel/interfaces/interfacecommand.h>

using conversion::translator::TranslationException;
using conversion::translator::UnhandledValueException;

namespace conversion::iv::translator {

AsyncInterfaceCommandTranslator::AsyncInterfaceCommandTranslator(
        const ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction)
    : m_interfaceType(interfaceType)
    , m_ivFunction(ivFunction)
{
}

void AsyncInterfaceCommandTranslator::translateCommand(const seds::model::InterfaceCommand &command) const
{
    switch (command.argumentsCombination()) {
    case seds::model::ArgumentsCombination::InOnly:
        throw TranslationException("Interface command with in arguments only not implemented");
        break;
    case seds::model::ArgumentsCombination::OutOnly:
        throw TranslationException("Interface command with out arguments only not implemented");
        break;
    case seds::model::ArgumentsCombination::InAndNotify:
        throw TranslationException("Interface command with in and notify arguments not implemented");
        break;
    case seds::model::ArgumentsCombination::NoArgs:
    case seds::model::ArgumentsCombination::NotifyOnly:
    case seds::model::ArgumentsCombination::InAndOut:
    case seds::model::ArgumentsCombination::OutAndNotify:
    case seds::model::ArgumentsCombination::All: {
        const auto message =
                QString("Interface command arguments combination '%1' is not supported for TASTE InterfaceView")
                        .arg(argumentsCombinationToString(command.argumentsCombination()));
        throw TranslationException(message);
    } break;
    default:
        throw UnhandledValueException("ArgumentsCombination");
        break;
    }
}

void AsyncInterfaceCommandTranslator::createIvInterface(
        const QString &name, ivm::IVInterface::OperationKind operationKind) const
{
    ivm::IVInterface::CreationInfo creationInfo;
    creationInfo.function = m_ivFunction;
    creationInfo.type = m_interfaceType;
    creationInfo.name = name;
    creationInfo.kind = operationKind;

    auto *ivInterface = ivm::IVInterface::createIface(creationInfo);
}

} // namespace conversion::iv::translator
