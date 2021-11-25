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

#include <QDomElement>
#include <seds/SedsModel/package/package.h>

namespace seds::exporter {

/**
 * @brief   Exporter for SEDS components
 */
class ComponentExporter final
{
public:
    /**
     * @brief   Export SEDS components
     *
     * @param   components      Components to export
     * @param   packageElement  Parent package element
     * @param   sedsDocument    SEDS XML document
     */
    static auto exportComponents(const model::Package::ComponentSet &components, QDomElement &packageElement,
            QDomDocument &sedsDocument) -> void;

private:
    static auto exportComponent(
            const model::Component &component, QDomElement &componentSetElement, QDomDocument &sedsDocument) -> void;

    static auto exportProvidedInterfaces(
            const model::Component &component, QDomElement &componentElement, QDomDocument &sedsDocument) -> void;
    static auto exportRequiredInterfaces(
            const model::Component &component, QDomElement &componentElement, QDomDocument &sedsDocument) -> void;
    static auto exportDeclaredInterfaces(
            const model::Component &component, QDomElement &componentElement, QDomDocument &sedsDocument) -> void;
};

} // namespace seds::exporter
