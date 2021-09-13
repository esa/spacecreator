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

ComponentsTranslator::ComponentsTranslator(const seds::model::Package &package, Asn1Acn::Definitions *asn1Definitions)
    : m_package(package)
    , m_asn1Definitions(asn1Definitions)
{
}

QVector<ivm::IVFunction *> ComponentsTranslator::translateComponents()
{
    QVector<ivm::IVFunction *> components;

    // Each SEDS component translates to a IV function
    for (const auto &component : m_package.components()) {
        components.append(translateComponent(component));
    }

    return components;
}

ivm::IVFunction *ComponentsTranslator::translateComponent(const seds::model::Component &component) const
{
    auto *ivFunction = new ivm::IVFunction();
    ivFunction->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::name), component.nameStr());

    for (const auto &providedInterface : component.providedInterfaces()) {
        translateInterface(providedInterface, component, ivm::IVInterface::InterfaceType::Provided, ivFunction);
    }

    for (const auto &requiredInterface : component.requiredInterfaces()) {
        translateInterface(requiredInterface, component, ivm::IVInterface::InterfaceType::Required, ivFunction);
    }

    return ivFunction;
}

void ComponentsTranslator::translateInterface(const seds::model::Interface &interface,
        const seds::model::Component &component, const ivm::IVInterface::InterfaceType interfaceType,
        ivm::IVFunction *ivFunction) const
{
    const auto &interfaceTypeName = interface.type().nameStr();
    const auto &interfaceDeclaration = findInterfaceDeclaration(interfaceTypeName, component);

    AsyncInterfaceCommandTranslator asyncCommandTranslator(
            m_package, component, interface, ivFunction, m_asn1Definitions);
    SyncInterfaceCommandTranslator syncCommandTranslator(interface, ivFunction);

    for (const auto &command : interfaceDeclaration.commands()) {
        switch (command.mode()) {
        case seds::model::InterfaceCommandMode::Sync:
            throw TranslationException("Sync commands not implemented");
            break;
        case seds::model::InterfaceCommandMode::Async:
            asyncCommandTranslator.translateCommand(command, interfaceType);
            break;
        default:
            throw UnhandledValueException("InterfaceCommandMode");
            break;
        }
    }
}

const seds::model::InterfaceDeclaration &ComponentsTranslator::findInterfaceDeclaration(
        const QString &interfaceTypeName, const seds::model::Component &component) const
{
    const auto &componentInterfacesDeclarations = component.declaredInterfaces();
    auto found = std::find_if(componentInterfacesDeclarations.begin(), componentInterfacesDeclarations.end(),
            [&interfaceTypeName](const seds::model::InterfaceDeclaration &interfaceDeclaration) {
                return interfaceDeclaration.nameStr() == interfaceTypeName;
            });
    if (found != componentInterfacesDeclarations.end()) {
        return *found;
    }

    const auto &packageInterfacesDeclarations = m_package.declaredInterfaces();
    found = std::find_if(packageInterfacesDeclarations.begin(), packageInterfacesDeclarations.end(),
            [&interfaceTypeName](const seds::model::InterfaceDeclaration &interfaceDeclaration) {
                return interfaceDeclaration.nameStr() == interfaceTypeName;
            });
    if (found != packageInterfacesDeclarations.end()) {
        return *found;
    }

    throw UndeclaredInterfaceException(interfaceTypeName);
}

} // namespace conversion::iv::translator
