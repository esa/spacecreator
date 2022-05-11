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

#include "specialized/componentstranslator.h"

#include "specialized/asyncinterfacecommandtranslator.h"
#include "specialized/descriptiontranslator.h"
#include "specialized/interfaceparametertranslator.h"
#include "specialized/syncinterfacecommandtranslator.h"
#include "translator.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <iostream>
#include <ivcore/ivcommonprops.h>
#include <ivcore/ivfunction.h>
#include <seds/SedsModel/package/package.h>

using conversion::Escaper;
using conversion::UnhandledValueException;
using conversion::translator::TranslationException;

namespace conversion::iv::translator {

ComponentsTranslator::ComponentsTranslator(
        const seds::model::Package *sedsPackage, const std::vector<seds::model::Package> &sedsPackages)
    : m_sedsPackage(sedsPackage)
    , m_sedsPackages(sedsPackages)
{
}

QVector<ivm::IVFunction *> ComponentsTranslator::translateComponents()
{
    QVector<ivm::IVFunction *> ivComponents;

    // Each SEDS component translates to a IV function
    for (const auto &sedsComponent : m_sedsPackage->components()) {
        ivComponents.append(translateComponent(sedsComponent));
    }

    return ivComponents;
}

ivm::IVFunction *ComponentsTranslator::translateComponent(const seds::model::Component &sedsComponent)
{
    auto *ivFunction = new ivm::IVFunction();
    ivFunction->setEntityAttribute(
            ivm::meta::Props::token(ivm::meta::Props::Token::name), Escaper::escapeIvName(sedsComponent.nameStr()));

    DescriptionTranslator::translate(sedsComponent, ivFunction);

    for (const auto &sedsInterface : sedsComponent.providedInterfaces()) {
        translateInterface(sedsInterface, sedsComponent, ivm::IVInterface::InterfaceType::Provided, ivFunction);
    }

    for (const auto &sedsInterface : sedsComponent.requiredInterfaces()) {
        translateInterface(sedsInterface, sedsComponent, ivm::IVInterface::InterfaceType::Required, ivFunction);
    }

    return ivFunction;
}

void ComponentsTranslator::translateInterface(const seds::model::Interface &sedsInterface,
        const seds::model::Component &sedsComponent, const ivm::IVInterface::InterfaceType interfaceType,
        ivm::IVFunction *ivFunction)
{
    const auto &sedsInterfaceName = sedsInterface.nameStr();
    const auto &sedsInterfaceTypeRef = sedsInterface.type();

    Context context { m_sedsPackage, &sedsComponent };

    if (sedsInterfaceTypeRef.packageStr()) {
        const auto sedsBaseInterfacePackage =
                SedsToIvTranslator::getSedsPackage(*sedsInterfaceTypeRef.packageStr(), m_sedsPackages);
        context = { sedsBaseInterfacePackage, nullptr };
    }

    auto parentName = QString("%1-%2").arg(sedsComponent.nameStr()).arg(sedsInterface.nameStr());
    const auto sedsInterfaceDeclaration = context.findInterfaceDeclaration(sedsInterfaceTypeRef.nameStr());

    translateInterfaceDeclaration(
            sedsInterfaceDeclaration, sedsInterfaceName, sedsComponent, parentName, interfaceType, ivFunction, context);
}

void ComponentsTranslator::translateInterfaceDeclaration(
        const seds::model::InterfaceDeclaration *sedsInterfaceDeclaration, const QString &sedsInterfaceName,
        const seds::model::Component &sedsComponent, const QString &parentName,
        const ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction, Context context) const
{
    for (const auto &sedsBaseInterface : sedsInterfaceDeclaration->baseInterfaces()) {
        const auto &sedsBaseInterfaceTypeRef = sedsBaseInterface.type();

        auto sedsBaseInterfaceContext = context;
        if (sedsBaseInterfaceTypeRef.packageStr()) {
            const auto sedsBaseInterfacePackage =
                    SedsToIvTranslator::getSedsPackage(*sedsBaseInterfaceTypeRef.packageStr(), m_sedsPackages);
            sedsBaseInterfaceContext = { sedsBaseInterfacePackage, nullptr };
        }

        const auto &sedsBaseInterfaceDeclaration =
                sedsBaseInterfaceContext.findInterfaceDeclaration(sedsBaseInterfaceTypeRef.nameStr());

        translateInterfaceDeclaration(sedsBaseInterfaceDeclaration, sedsInterfaceName, sedsComponent, parentName,
                interfaceType, ivFunction, sedsBaseInterfaceContext);
    }

    InterfaceTypeNameHelper typeNameHelper(context, parentName, sedsInterfaceDeclaration, m_sedsPackages);

    translateParameters(sedsInterfaceName, sedsInterfaceDeclaration, interfaceType, ivFunction, typeNameHelper);
    translateCommands(sedsInterfaceName, sedsInterfaceDeclaration, interfaceType, ivFunction, typeNameHelper);
}

void ComponentsTranslator::translateParameters(const QString &sedsInterfaceName,
        const seds::model::InterfaceDeclaration *sedsInterfaceDeclaration,
        const ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction,
        const InterfaceTypeNameHelper &typeNameHelper) const
{
    InterfaceParameterTranslator parameterTranslator(ivFunction, sedsInterfaceName, typeNameHelper);

    for (const auto &sedsParameter : sedsInterfaceDeclaration->parameters()) {
        parameterTranslator.translateParameter(sedsParameter, interfaceType);
    }
}

void ComponentsTranslator::translateCommands(const QString &sedsInterfaceName,
        const seds::model::InterfaceDeclaration *sedsInterfaceDeclaration,
        const ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction,
        const InterfaceTypeNameHelper &typeNameHelper) const
{
    AsyncInterfaceCommandTranslator asyncCommandTranslator(ivFunction, sedsInterfaceName, typeNameHelper);
    SyncInterfaceCommandTranslator syncCommandTranslator(ivFunction, sedsInterfaceName, typeNameHelper);

    for (const auto &sedsCommand : sedsInterfaceDeclaration->commands()) {
        switch (sedsCommand.mode()) {
        case seds::model::InterfaceCommandMode::Sync:
            syncCommandTranslator.translateCommand(sedsCommand, interfaceType);
            break;
        case seds::model::InterfaceCommandMode::Async:
            asyncCommandTranslator.translateCommand(sedsCommand, interfaceType);
            break;
        default:
            throw UnhandledValueException("InterfaceCommandMode");
            break;
        }
    }
}

} // namespace conversion::iv::translator
