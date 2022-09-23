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

#pragma once

#include "context.h"
#include "interfacetypenamehelper.h"

#include <QVector>
#include <asn1library/asn1/asn1model.h>
#include <ivcore/ivinterface.h>
#include <optional>
#include <seds/SedsModel/interfaces/interfacedeclarationref.h>

namespace Asn1Acn {
class Definitions;
} // namespace Asn1Acn

namespace ivm {
class IVFunction;
} // namespace ivm

namespace seds::model {
class Component;
class Interface;
class InterfaceDeclaration;
class Package;
} // namespace seds::model

namespace conversion::iv::translator::seds {

/**
 * @brief   Translator from SEDS components to InterfaceView functions
 */
class ComponentsTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sedsPackage         Package with components to translate
     * @param   sedsPackages        List of SEDS packages
     */
    ComponentsTranslator(
            const ::seds::model::Package *sedsPackage, const std::vector<::seds::model::Package> &sedsPackages);
    /**
     * @brief   Deleted copy constructor
     */
    ComponentsTranslator(const ComponentsTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    ComponentsTranslator(ComponentsTranslator &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    ComponentsTranslator &operator=(const ComponentsTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    ComponentsTranslator &operator=(ComponentsTranslator &&) = delete;

public:
    /**
     * @brief   Translates SEDS components from the SEDS package to InterfaceView functions
     *
     * @return  Result vector of InterfaceView functions
     */
    auto translateComponents() -> QVector<ivm::IVFunction *>;

private:
    /**
     * @brief   Translates SEDS component to InterfaceView function
     *
     * @param   sedsComponent   Component to translate
     *
     * @return  Result InterfaceView function
     */
    auto translateComponent(const ::seds::model::Component &sedsComponent) -> ivm::IVFunction *;

    /**
     * @brief   Translates SEDS interface to InterfaceView interface
     *
     * @param   sedsInterface       Interface to translate
     * @param   sedsComponent       Parent component
     * @param   interfaceType       Type of the IV interface
     * @param   ivFunction          IV Function where the result should be added
     */
    auto translateInterface(const ::seds::model::Interface &sedsInterface,
            const ::seds::model::Component &sedsComponent, const ivm::IVInterface::InterfaceType interfaceType,
            ivm::IVFunction *ivFunction) -> void;
    auto translateInterfaceDeclaration(const ::seds::model::InterfaceDeclaration *sedsInterfaceDeclaration,
            const QString &sedsInterfaceName, const ::seds::model::Component &sedsComponent, const QString &parentName,
            const ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction, Context context) const
            -> void;
    auto translateParameters(const QString &sedsInterfaceName,
            const ::seds::model::InterfaceDeclaration *sedsInterfaceDeclaration,
            const ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction,
            const InterfaceTypeNameHelper &typeNameHelper) const -> void;
    auto translateCommands(const QString &sedsInterfaceName,
            const ::seds::model::InterfaceDeclaration *sedsInterfaceDeclaration,
            const ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction,
            const InterfaceTypeNameHelper &typeNameHelper) const -> void;

private:
    /// @brief  Parent package
    const ::seds::model::Package *m_sedsPackage;
    /// @brief  List of SEDS packages
    const std::vector<::seds::model::Package> &m_sedsPackages;
};

} // namespace conversion::iv::translator::seds
