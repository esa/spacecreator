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

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
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
        translateInterface(ivInterface, ivFunction, sedsComponent);
    }

    sedsPackage.addComponent(std::move(sedsComponent));
}

void FunctionsTranslator::translateInterface(
        const ivm::IVInterface *ivInterface, const ivm::IVFunction *ivFunction, ::seds::model::Component &sedsComponent)
{
    createInterfaceDeclaration(ivInterface, ivFunction, sedsComponent);
    createInterface(ivInterface, ivFunction, sedsComponent);
}

void FunctionsTranslator::createInterfaceDeclaration(
        const ivm::IVInterface *ivInterface, const ivm::IVFunction *ivFunction, ::seds::model::Component &sedsComponent)
{
    auto sedsInterfaceDeclarationName = getInterfaceDeclarationName(ivInterface, ivFunction);

    ::seds::model::InterfaceDeclaration sedsInterfaceDeclaration;
    sedsInterfaceDeclaration.setName(std::move(sedsInterfaceDeclarationName));

    createInterfaceCommand(ivInterface, sedsInterfaceDeclaration);

    sedsComponent.addInterfaceDeclaration(std::move(sedsInterfaceDeclaration));
}

void FunctionsTranslator::createInterfaceCommand(
        const ivm::IVInterface *ivInterface, ::seds::model::InterfaceDeclaration &sedsInterfaceDeclaration)
{
    auto sedsInterfaceCommandName = getInterfaceCommandName(ivInterface);

    ::seds::model::InterfaceCommand sedsInterfaceCommand;
    sedsInterfaceCommand.setName(std::move(sedsInterfaceCommandName));

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
    sedsInterfaceCommandArgument.setType(Escaper::escapeIvName(ivInterfaceParameter.paramTypeName()));

    switch (ivInterfaceParameter.direction()) {
    case shared::InterfaceParameter::Direction::IN:
        sedsInterfaceCommandArgument.setMode(::seds::model::CommandArgumentMode::In);
        break;
    case shared::InterfaceParameter::Direction::OUT:
        sedsInterfaceCommandArgument.setMode(::seds::model::CommandArgumentMode::Out);
        break;
    }

    sedsInterfaceCommand.addArgument(std::move(sedsInterfaceCommandArgument));
}

void FunctionsTranslator::createInterface(
        const ivm::IVInterface *ivInterface, const ivm::IVFunction *ivFunction, ::seds::model::Component &sedsComponent)
{
    auto sedsInterfaceName = getInterfaceName(ivInterface);
    auto sedsInterfaceDeclarationName = getInterfaceDeclarationName(ivInterface, ivFunction);

    ::seds::model::Interface sedsInterface;
    sedsInterface.setName(std::move(sedsInterfaceName));
    sedsInterface.setType(std::move(sedsInterfaceDeclarationName));

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

QString FunctionsTranslator::getInterfaceDeclarationName(
        const ivm::IVInterface *ivInterface, const ivm::IVFunction *ivFunction)
{
    return QString("%1%2%3")
            .arg(ivFunction->title())
            .arg(ivInterface->title())
            .arg(interfaceTypeToString(ivInterface->direction()));
}

QString FunctionsTranslator::getInterfaceName(const ivm::IVInterface *ivInterface)
{
    return QString("%1%2").arg(ivInterface->title()).arg(interfaceTypeToString(ivInterface->direction()));
}

QString FunctionsTranslator::getInterfaceCommandName(const ivm::IVInterface *ivInterface)
{
    return QString("%1%2").arg(ivInterface->title()).arg(interfaceTypeToString(ivInterface->direction()));
}

const QString &FunctionsTranslator::interfaceTypeToString(ivm::IVInterface::InterfaceType type)
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

} // namespace conversion::seds::translator
