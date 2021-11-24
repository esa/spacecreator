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

#include "specialized/functionstranslator.h"

#include "parameter.h"

#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivinterface.h>
#include <seds/SedsModel/package/package.h>

using conversion::translator::TranslationException;

namespace conversion::seds::translator {

void FunctionsTranslator::translateFunction(const ivm::IVFunction *ivFunction, ::seds::model::Package &sedsPackage)
{
    ::seds::model::Component sedsComponent;

    auto ivFunctionName =
            ivFunction->entityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::name)).value<QString>();
    sedsComponent.setName(std::move(ivFunctionName));

    for (const auto ivInterface : ivFunction->allInterfaces()) {
        translateInterface(ivInterface, sedsComponent);
    }

    sedsPackage.addComponent(std::move(sedsComponent));
}

void FunctionsTranslator::translateInterface(
        const ivm::IVInterface *ivInterface, ::seds::model::Component &sedsComponent)
{
    createInterfaceDeclaration(ivInterface, sedsComponent);
    createInterface(ivInterface, sedsComponent);
}

void FunctionsTranslator::createInterfaceDeclaration(
        const ivm::IVInterface *ivInterface, ::seds::model::Component &sedsComponent)
{
    ::seds::model::InterfaceDeclaration sedsInterfaceDeclaration;
    sedsInterfaceDeclaration.setName(ivInterface->title());

    createInterfaceCommand(ivInterface, sedsInterfaceDeclaration);

    sedsComponent.addInterfaceDeclaration(std::move(sedsInterfaceDeclaration));
}

void FunctionsTranslator::createInterfaceCommand(
        const ivm::IVInterface *ivInterface, ::seds::model::InterfaceDeclaration &sedsInterfaceDeclaration)
{
    ::seds::model::InterfaceCommand sedsInterfaceCommand;
    sedsInterfaceCommand.setName(ivInterface->title());

    switch (ivInterface->kind()) {
    case ivm::IVInterface::OperationKind::Protected:
    case ivm::IVInterface::OperationKind::Unprotected:
        sedsInterfaceCommand.setMode(::seds::model::InterfaceCommandMode::Sync);
        break;
    case ivm::IVInterface::OperationKind::Cyclic:
    case ivm::IVInterface::OperationKind::Sporadic:
        sedsInterfaceCommand.setMode(::seds::model::InterfaceCommandMode::Async);
        break;
    case ivm::IVInterface::OperationKind::Any:
        throw TranslationException("Any interfaces are unsupported");
        break;
    default:
        throw TranslationException("Unhandled OperationKind value");
        break;
    }

    for (const auto &ivInterfaceParameter : ivInterface->params()) {
        createInterfaceArgument(ivInterfaceParameter, sedsInterfaceCommand);
    }

    sedsInterfaceDeclaration.addCommand(std::move(sedsInterfaceCommand));
}

void FunctionsTranslator::createInterfaceArgument(
        const shared::InterfaceParameter &ivInterfaceParameter, ::seds::model::InterfaceCommand &sedsInterfaceCommand)
{
    ::seds::model::CommandArgument sedsInterfaceCommandArgument;
    sedsInterfaceCommandArgument.setName(ivInterfaceParameter.name());
    sedsInterfaceCommandArgument.setType(ivInterfaceParameter.paramTypeName());
    sedsInterfaceCommandArgument.setMode(::seds::model::CommandArgumentMode::In);

    sedsInterfaceCommand.addArgument(std::move(sedsInterfaceCommandArgument));
}

void FunctionsTranslator::createInterface(const ivm::IVInterface *ivInterface, ::seds::model::Component &sedsComponent)
{
    ::seds::model::Interface sedsInterface;
    sedsInterface.setName(ivInterface->title());
    sedsInterface.setType(ivInterface->title());

    switch (ivInterface->direction()) {
    case ivm::IVInterface::InterfaceType::Provided:
        sedsComponent.addProvidedInterface(std::move(sedsInterface));
        break;
    case ivm::IVInterface::InterfaceType::Required:
        sedsComponent.addRequiredInterface(std::move(sedsInterface));
        break;
    case ivm::IVInterface::InterfaceType::Grouped:
        throw TranslationException("Grouped interfaces are not implemented");
        break;
    default:
        throw TranslationException("Unhandled InterfaceType value");
        break;
    }
}

} // namespace conversion::seds::translator
