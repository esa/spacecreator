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
#include "specialized/syncinterfacecommandtranslator.h"

#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivcommonprops.h>
#include <ivcore/ivfunction.h>
#include <seds/SedsModel/package/package.h>

using conversion::translator::TranslationException;
using conversion::translator::UndeclaredInterfaceException;
using conversion::translator::UnhandledValueException;

namespace conversion::iv::translator {

ComponentsTranslator::ComponentsTranslator(
        const seds::model::Package &sedsPackage, Asn1Acn::Definitions *asn1Definitions)
    : m_sedsPackage(sedsPackage)
    , m_asn1Definitions(asn1Definitions)
{
}

QVector<ivm::IVFunction *> ComponentsTranslator::translateComponents()
{
    QVector<ivm::IVFunction *> ivComponents;

    // Each SEDS component translates to a IV function
    for (const auto &sedsComponent : m_sedsPackage.components()) {
        ivComponents.append(translateComponent(sedsComponent));
    }

    return ivComponents;
}

ivm::IVFunction *ComponentsTranslator::translateComponent(const seds::model::Component &sedsComponent) const
{
    auto *ivFunction = new ivm::IVFunction();
    ivFunction->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::name), sedsComponent.nameStr());

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
        ivm::IVFunction *ivFunction) const
{
    const auto &sedsInterfaceDeclaration = findInterfaceDeclaration(sedsInterface.type().nameStr(), sedsComponent);

    AsyncInterfaceCommandTranslator asyncCommandTranslator(
            m_sedsPackage, sedsComponent, sedsInterface, m_asn1Definitions, ivFunction);
    SyncInterfaceCommandTranslator syncCommandTranslator(sedsInterface, ivFunction);

    for (const auto &sedsCommand : sedsInterfaceDeclaration.commands()) {
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

const seds::model::InterfaceDeclaration &ComponentsTranslator::findInterfaceDeclaration(
        const QString &name, const seds::model::Component &sedsComponent) const
{
    const auto &sedsComponentInterfaceDeclarations = sedsComponent.declaredInterfaces();
    auto found = std::find_if(sedsComponentInterfaceDeclarations.begin(), sedsComponentInterfaceDeclarations.end(),
            [&name](const seds::model::InterfaceDeclaration &interfaceDeclaration) {
                return interfaceDeclaration.nameStr() == name;
            });
    if (found != sedsComponentInterfaceDeclarations.end()) {
        return *found;
    }

    const auto &sedsPackageInterfaceDeclarations = m_sedsPackage.declaredInterfaces();
    found = std::find_if(sedsPackageInterfaceDeclarations.begin(), sedsPackageInterfaceDeclarations.end(),
            [&name](const seds::model::InterfaceDeclaration &interfaceDeclaration) {
                return interfaceDeclaration.nameStr() == name;
            });
    if (found != sedsPackageInterfaceDeclarations.end()) {
        return *found;
    }

    throw UndeclaredInterfaceException(name);
}

} // namespace conversion::iv::translator
