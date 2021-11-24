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

#include "specialized/componentexporter.h"

#include "specialized/interfaceexporter.h"

namespace seds::exporter {

void ComponentExporter::exportComponents(
        const model::Package::ComponentSet &components, QDomElement &packageElement, QDomDocument &sedsDocument)
{
    auto componentSetElement = sedsDocument.createElement("ComponentSet");

    for (const auto &component : components) {
        exportComponent(component, componentSetElement, sedsDocument);
    }

    packageElement.appendChild(std::move(componentSetElement));
}

void ComponentExporter::exportComponent(
        const model::Component &component, QDomElement &componentSetElement, QDomDocument &sedsDocument)
{
    auto componentElement = sedsDocument.createElement("Component");

    const auto &componentName = component.nameStr();
    componentElement.setAttribute(QStringLiteral("name"), componentName);

    auto providedInterfaceSetElement = sedsDocument.createElement("ProvidedInterfaceSet");
    for (const auto &providedInterface : component.providedInterfaces()) {
        InterfaceExporter::exportInterface(providedInterface, providedInterfaceSetElement, sedsDocument);
    }
    componentElement.appendChild(std::move(providedInterfaceSetElement));

    auto requiredInterfaceSetElement = sedsDocument.createElement("RequiredInterfaceSet");
    for (const auto &requiredInterface : component.requiredInterfaces()) {
        InterfaceExporter::exportInterface(requiredInterface, requiredInterfaceSetElement, sedsDocument);
    }
    componentElement.appendChild(std::move(requiredInterfaceSetElement));

    auto declaredInterfaceSetElement = sedsDocument.createElement("DeclaredInterfaceSet");
    for (const auto &interfaceDeclaration : component.declaredInterfaces()) {
        InterfaceExporter::exportInterfaceDeclaration(interfaceDeclaration, declaredInterfaceSetElement, sedsDocument);
    }
    componentElement.appendChild(std::move(declaredInterfaceSetElement));

    componentSetElement.appendChild(std::move(componentElement));
}

} // namespace seds::exporter
