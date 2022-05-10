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

#include "specialized/asyncinterfacecommandtranslator.h"

#include "interfacetranslatorhelper.h"

#include <asn1library/asn1/acnparameter.h>
#include <asn1library/asn1/acnsequencecomponent.h>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/asn1/SedsToAsn1Translator/datatypetranslationhelper.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>

using conversion::asn1::translator::DataTypeTranslationHelper;
using conversion::translator::MissingAsn1TypeDefinitionException;
using conversion::translator::TranslationException;
using seds::model::ArgumentsCombination;
using seds::model::CommandArgumentMode;

namespace conversion::iv::translator {

AsyncInterfaceCommandTranslator::AsyncInterfaceCommandTranslator(ivm::IVFunction *ivFunction,
        const seds::model::InterfaceDeclaration &sedsInterfaceDeclaration, const QString &sedsComponentName,
        const QString &sedsInterfaceName)
    : m_ivFunction(ivFunction)
    , m_sedsInterfaceDeclaration(sedsInterfaceDeclaration)
    , m_sedsComponentName(sedsComponentName)
    , m_sedsInterfaceName(sedsInterfaceName)
{
}

void AsyncInterfaceCommandTranslator::translateCommand(
        const seds::model::InterfaceCommand &sedsCommand, ivm::IVInterface::InterfaceType interfaceType)
{
    // Process command based on its commands
    switch (sedsCommand.argumentsCombination()) {
    case ArgumentsCombination::InOnly: {
        // In arguments are 'native', so they are handles as-is
        const auto interfaceName = InterfaceTranslatorHelper::buildCommandInterfaceName(
                m_sedsInterfaceName, sedsCommand.nameStr(), interfaceType);
        auto *ivInterface = InterfaceTranslatorHelper::createIvInterface(
                interfaceName, interfaceType, ivm::IVInterface::OperationKind::Sporadic, sedsCommand, m_ivFunction);
        translateArguments(sedsCommand, CommandArgumentMode::In, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case ArgumentsCombination::OutOnly: {
        // Out arguments aren't supported by TASTE sporadic interface.
        // We cannot change the argument direction, so we switch interface type (provided <-> required)
        const auto interfaceName = InterfaceTranslatorHelper::buildCommandInterfaceName(m_sedsInterfaceName,
                sedsCommand.nameStr(), InterfaceTranslatorHelper::switchInterfaceType(interfaceType));
        auto *ivInterface = InterfaceTranslatorHelper::createIvInterface(interfaceName,
                InterfaceTranslatorHelper::switchInterfaceType(interfaceType),
                ivm::IVInterface::OperationKind::Sporadic, sedsCommand, m_ivFunction);
        translateArguments(sedsCommand, CommandArgumentMode::Out, ivInterface);
        m_ivFunction->addChild(ivInterface);
    } break;
    case ArgumentsCombination::InAndNotify: {
        // InAndNotify arguments are separated onto two interfaces
        // In arguments - as-is
        // Notify arguments - switched interface type (provided <-> required)
        const auto inInterfaceName = InterfaceTranslatorHelper::buildCommandInterfaceName(
                m_sedsInterfaceName, sedsCommand.nameStr(), interfaceType);
        auto *ivInterfaceIn = InterfaceTranslatorHelper::createIvInterface(
                inInterfaceName, interfaceType, ivm::IVInterface::OperationKind::Sporadic, sedsCommand, m_ivFunction);
        translateArguments(sedsCommand, CommandArgumentMode::In, ivInterfaceIn);
        m_ivFunction->addChild(ivInterfaceIn);

        const auto notifyInterfaceName = InterfaceTranslatorHelper::buildCommandInterfaceName(m_sedsInterfaceName,
                sedsCommand.nameStr(), InterfaceTranslatorHelper::switchInterfaceType(interfaceType));
        auto *ivInterfaceNotify = InterfaceTranslatorHelper::createIvInterface(notifyInterfaceName,
                InterfaceTranslatorHelper::switchInterfaceType(interfaceType),
                ivm::IVInterface::OperationKind::Sporadic, sedsCommand, m_ivFunction);
        translateArguments(sedsCommand, CommandArgumentMode::Notify, ivInterfaceNotify);
        m_ivFunction->addChild(ivInterfaceNotify);
    } break;
    case ArgumentsCombination::NoArgs: {
        // No arguments, no problems
        const auto interfaceName = InterfaceTranslatorHelper::buildCommandInterfaceName(
                m_sedsInterfaceName, sedsCommand.nameStr(), interfaceType);
        auto *ivInterface = InterfaceTranslatorHelper::createIvInterface(
                interfaceName, interfaceType, ivm::IVInterface::OperationKind::Sporadic, sedsCommand, m_ivFunction);
        m_ivFunction->addChild(ivInterface);
        break;
    }
    case ArgumentsCombination::NotifyOnly:
    case ArgumentsCombination::InAndOut:
    case ArgumentsCombination::OutAndNotify:
    case ArgumentsCombination::All: {
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

void AsyncInterfaceCommandTranslator::translateArguments(const seds::model::InterfaceCommand &sedsCommand,
        const CommandArgumentMode requestedArgumentMode, ivm::IVInterface *ivInterface)
{
    // Async commands are translated to sporadic interfaces, which can accept only one argument
    // To satisfy this we need to pack all command arguments into one
    const auto bundledTypeName = handleArgumentTypeName(sedsCommand, requestedArgumentMode);

    const auto ivParameter = InterfaceTranslatorHelper::createInterfaceParameter(
            m_ivInterfaceParameterName, bundledTypeName, shared::InterfaceParameter::Direction::IN);
    ivInterface->addParam(ivParameter);
}

QString AsyncInterfaceCommandTranslator::handleArgumentTypeName(
        const seds::model::InterfaceCommand &sedsCommand, const CommandArgumentMode requestedArgumentMode)
{
    return "ASYNC_STUB";
}

} // namespace conversion::iv::translator
