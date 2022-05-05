/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "specialized/interfacedeclarationtypecreator.h"

#include "datatypetranslationhelper.h"
#include "specialized/datatypetranslatorvisitor.h"

#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/translation/exceptions.h>

using conversion::UnhandledValueException;
using conversion::translator::TranslationException;
using seds::model::InterfaceCommandMode;

namespace conversion::asn1::translator {

InterfaceDeclarationTypeCreator::InterfaceDeclarationTypeCreator(Context &context)
    : m_context(context)
{
}

void InterfaceDeclarationTypeCreator::createTypes(const seds::model::InterfaceDeclaration &interfaceDeclaration)
{
    // Interface declarations with generic types aren't handled here
    if (!interfaceDeclaration.genericTypes().empty()) {
        return;
    }

    for (const auto &parameter : interfaceDeclaration.parameters()) {
        createTypesForParameter(parameter);
    }

    for (const auto &command : interfaceDeclaration.commands()) {
        switch (command.mode()) {
        case InterfaceCommandMode::Sync:
            createTypesForSyncCommand(command);
            break;
        case InterfaceCommandMode::Async:
            createTypesForAsyncCommand(command, interfaceDeclaration.nameStr());
            break;
        default:
            throw UnhandledValueException("InterfaceCommandMode");
        }
    }
}

void InterfaceDeclarationTypeCreator::createTypesForParameter(const seds::model::InterfaceParameter &parameter)
{
    const auto &arrayDimensions = parameter.arrayDimensions();
    if (!arrayDimensions.empty()) {
        DataTypeTranslationHelper::handleArrayType(m_context, parameter.type(), arrayDimensions);
    }
}

void InterfaceDeclarationTypeCreator::createTypesForSyncCommand(const seds::model::InterfaceCommand &command)
{
    for (const auto &argument : command.arguments()) {
        const auto &arrayDimensions = argument.arrayDimensions();
        if (!arrayDimensions.empty()) {
            DataTypeTranslationHelper::handleArrayType(m_context, argument.type(), arrayDimensions);
        }
    }
}

void InterfaceDeclarationTypeCreator::createTypesForAsyncCommand(
        const seds::model::InterfaceCommand &command, const QString &interfaceName)
{
    const auto commandName = Escaper::escapeAsn1TypeName(command.nameStr());

    // Process command based on its commands
    switch (command.argumentsCombination()) {
    case seds::model::ArgumentsCombination::InOnly: {
        // In arguments are 'native', so they are handles as-is
        const auto bundledTypeName = DataTypeTranslationHelper::buildBundledTypeName(interfaceName, commandName);
        createAsyncCommandBundledType(command, bundledTypeName, seds::model::CommandArgumentMode::In);
    } break;
    case seds::model::ArgumentsCombination::OutOnly: {
        // Out arguments aren't supported by TASTE sporadic interface.
        // We cannot change the argument direction, so we switch interface type (provided <-> required)
        const auto bundledTypeName = DataTypeTranslationHelper::buildBundledTypeName(interfaceName, commandName);
        createAsyncCommandBundledType(command, bundledTypeName, seds::model::CommandArgumentMode::Out);
    } break;
    case seds::model::ArgumentsCombination::InAndNotify: {
        // InAndNotify arguments are separated onto two interfaces
        // In arguments - as-is
        const auto inBundledTypeName =
                DataTypeTranslationHelper::buildBundledTypeName(interfaceName, commandName, "In");
        createAsyncCommandBundledType(command, inBundledTypeName, seds::model::CommandArgumentMode::In);
        // Notify arguments - switched interface type (provided <-> required)
        const auto notifyBundledTypeName =
                DataTypeTranslationHelper::buildBundledTypeName(interfaceName, commandName, "Notify");
        createAsyncCommandBundledType(command, notifyBundledTypeName, seds::model::CommandArgumentMode::Notify);
    } break;
    case seds::model::ArgumentsCombination::NoArgs: {
        // No arguments, no problems
        break;
    }
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

void InterfaceDeclarationTypeCreator::createAsyncCommandBundledType(const seds::model::InterfaceCommand &command,
        const QString &bundledTypeName, const seds::model::CommandArgumentMode requestedArgumentMode)
{
    auto bundledType = std::make_unique<Asn1Acn::Types::Sequence>(bundledTypeName);

    for (const auto &argument : command.arguments()) {
        if (argument.mode() != requestedArgumentMode) {
            continue;
        }

        createAsyncCommandBundledTypeComponent(argument, bundledType.get());
    }

    m_context.addAsn1Type(std::move(bundledType), nullptr);
}

void InterfaceDeclarationTypeCreator::createAsyncCommandBundledTypeComponent(
        const seds::model::CommandArgument &argument, Asn1Acn::Types::Sequence *bundledType)
{
    const auto argumentName = Escaper::escapeAsn1FieldName(argument.nameStr());
    const auto argumentType =
            DataTypeTranslationHelper::handleArrayType(m_context, argument.type(), argument.arrayDimensions());

    auto sequenceComponentType =
            std::make_unique<Asn1Acn::Types::UserdefinedType>(argumentType->identifier(), m_context.definitionsName());
    sequenceComponentType->setType(argumentType->clone());

    std::unique_ptr<Asn1Acn::SequenceComponent> sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            argumentName, argumentName, false, std::nullopt, "", Asn1Acn::AsnSequenceComponent::Presence::NotSpecified,
            Asn1Acn::SourceLocation(), std::move(sequenceComponentType));

    bundledType->addComponent(std::move(sequenceComponent));
}

} // namespace conversion::asn1::translator
