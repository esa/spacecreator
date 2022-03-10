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

#include "generictypemapper.h"
#include "specialized/asyncinterfacecommandtranslator.h"
#include "specialized/descriptiontranslator.h"
#include "specialized/interfaceparametertranslator.h"
#include "specialized/syncinterfacecommandtranslator.h"
#include "translator.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivcommonprops.h>
#include <ivcore/ivfunction.h>
#include <seds/SedsModel/package/package.h>

using conversion::Escaper;
using conversion::UnhandledValueException;
using conversion::translator::TranslationException;
using conversion::translator::UndeclaredInterfaceException;

namespace conversion::iv::translator {

ComponentsTranslator::ComponentsTranslator(const seds::model::Package *sedsPackage,
        Asn1Acn::Definitions *asn1Definitions, const Asn1Acn::Asn1Model::Data &asn1Files,
        const std::vector<seds::model::Package> &sedsPackages)
    : m_sedsPackage(sedsPackage)
    , m_asn1Definitions(asn1Definitions)
    , m_asn1Files(asn1Files)
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

    const auto &sedsInterfaceDeclaration =
            findInterfaceDeclaration(sedsInterface.type(), sedsComponent, m_sedsPackage, m_sedsPackages);
    translateInterfaceDeclaration(sedsInterfaceDeclaration, sedsInterfaceName, sedsInterface.genericTypeMapSet(),
            sedsComponent, interfaceType, ivFunction);
}

void ComponentsTranslator::translateInterfaceDeclaration(
        const seds::model::InterfaceDeclaration &sedsInterfaceDeclaration, const QString &sedsInterfaceName,
        const std::optional<seds::model::GenericTypeMapSet> &genericTypeMapSet,
        const seds::model::Component &sedsComponent, const ivm::IVInterface::InterfaceType interfaceType,
        ivm::IVFunction *ivFunction) const
{
    for (const auto &sedsBaseInterface : sedsInterfaceDeclaration.baseInterfaces()) {
        const auto &sedsBaseInterfaceDeclaration =
                findInterfaceDeclaration(sedsBaseInterface.type(), sedsComponent, m_sedsPackage, m_sedsPackages);
        translateInterfaceDeclaration(sedsBaseInterfaceDeclaration, sedsInterfaceName,
                sedsBaseInterface.genericTypeMapSet(), sedsComponent, interfaceType, ivFunction);
    }

    GenericTypeMapper typeMapper(sedsInterfaceName, genericTypeMapSet);
    translateParameters(sedsInterfaceName, sedsInterfaceDeclaration, interfaceType, ivFunction, &typeMapper);
    translateCommands(sedsInterfaceName, sedsInterfaceDeclaration, interfaceType, ivFunction, &typeMapper);
}

void ComponentsTranslator::translateParameters(const QString &sedsInterfaceName,
        const seds::model::InterfaceDeclaration &sedsInterfaceDeclaration,
        const ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction,
        const GenericTypeMapper *typeMapper) const
{
    InterfaceParameterTranslator parameterTranslator(ivFunction, sedsInterfaceName, typeMapper);

    for (const auto &sedsParameter : sedsInterfaceDeclaration.parameters()) {
        parameterTranslator.translateParameter(sedsParameter, interfaceType);
    }
}

void ComponentsTranslator::translateCommands(const QString &sedsInterfaceName,
        const seds::model::InterfaceDeclaration &sedsInterfaceDeclaration,
        const ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction,
        const GenericTypeMapper *typeMapper) const
{
    AsyncInterfaceCommandTranslator asyncCommandTranslator(
            ivFunction, sedsInterfaceName, m_asn1Definitions, m_sedsPackage, m_asn1Files, m_sedsPackages, typeMapper);
    SyncInterfaceCommandTranslator syncCommandTranslator(
            ivFunction, sedsInterfaceName, m_asn1Definitions, m_sedsPackage, m_asn1Files, m_sedsPackages, typeMapper);

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
        const seds::model::InterfaceDeclarationRef &interfaceDeclarationRef,
        const seds::model::Component &sedsComponent, const seds::model::Package *sedsPackage,
        const std::vector<seds::model::Package> &sedsPackages)
{
    const auto &name = interfaceDeclarationRef.nameStr();

    if (interfaceDeclarationRef.packageStr()) {
        const auto otherSedsPackage =
                SedsToIvTranslator::getSedsPackage(*interfaceDeclarationRef.packageStr(), sedsPackages);

        const auto &sedsPackageInterfaceDeclarations = otherSedsPackage->declaredInterfaces();
        const auto found =
                std::find_if(sedsPackageInterfaceDeclarations.begin(), sedsPackageInterfaceDeclarations.end(),
                        [&name](const seds::model::InterfaceDeclaration &interfaceDeclaration) {
                            return interfaceDeclaration.nameStr() == name;
                        });
        if (found != sedsPackageInterfaceDeclarations.end()) {
            return *found;
        }
    } else {
        const auto &sedsComponentInterfaceDeclarations = sedsComponent.declaredInterfaces();
        auto found = std::find_if(sedsComponentInterfaceDeclarations.begin(), sedsComponentInterfaceDeclarations.end(),
                [&name](const seds::model::InterfaceDeclaration &interfaceDeclaration) {
                    return interfaceDeclaration.nameStr() == name;
                });
        if (found != sedsComponentInterfaceDeclarations.end()) {
            return *found;
        }

        const auto &sedsPackageInterfaceDeclarations = sedsPackage->declaredInterfaces();
        found = std::find_if(sedsPackageInterfaceDeclarations.begin(), sedsPackageInterfaceDeclarations.end(),
                [&name](const seds::model::InterfaceDeclaration &interfaceDeclaration) {
                    return interfaceDeclaration.nameStr() == name;
                });
        if (found != sedsPackageInterfaceDeclarations.end()) {
            return *found;
        }
    }

    throw UndeclaredInterfaceException(interfaceDeclarationRef.value().pathStr());
}

} // namespace conversion::iv::translator
