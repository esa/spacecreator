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

#include <QVector>
#include <ivcore/ivinterface.h>

namespace Asn1Acn {
class File;
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

namespace conversion::iv::translator {

/**
 * @brief   Translator from SEDS components to InterfaceView functions
 */
class ComponentsTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   package     Package with components to translate
     * @param   asn1File    ASN.1 file where types of the packed argument will be saved
     */
    ComponentsTranslator(const seds::model::Package &package, Asn1Acn::File &asn1File);
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
    auto translateComponents() -> QVector<ivm::IVFunction *>;

private:
    auto translateComponent(const seds::model::Component &component) const -> ivm::IVFunction *;
    auto translateInterface(const seds::model::Interface &interface, const seds::model::Component &component,
            const ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction) const -> void;

private:
    auto findInterfaceDeclaration(const QString &interfaceTypeName, const seds::model::Component &component) const
            -> const seds::model::InterfaceDeclaration &;

private:
    const seds::model::Package &m_package;
    Asn1Acn::File &m_asn1File;
};

} // namespace conversion::iv::translator
