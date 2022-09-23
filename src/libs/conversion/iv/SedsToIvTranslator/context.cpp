/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "context.h"

#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/package/package.h>

using conversion::translator::UndeclaredInterfaceException;

namespace conversion::iv::translator::seds {

Context::Context(const ::seds::model::Package *package, const ::seds::model::Component *component)
    : m_package(package)
    , m_component(component)
{
}

const ::seds::model::InterfaceDeclaration *Context::findInterfaceDeclaration(
        const QString &interfaceDeclarationName) const
{
    if (m_component != nullptr) {
        const auto &componentInterfaceDeclarations = m_component->declaredInterfaces();
        auto found = std::find_if(componentInterfaceDeclarations.begin(), componentInterfaceDeclarations.end(),
                [&](const auto &interfaceDeclaration) {
                    return interfaceDeclaration.nameStr() == interfaceDeclarationName;
                });
        if (found != componentInterfaceDeclarations.end()) {
            return &(*found);
        }
    }

    const auto &packageInterfaceDeclarations = m_package->declaredInterfaces();
    const auto found = std::find_if(packageInterfaceDeclarations.begin(), packageInterfaceDeclarations.end(),
            [&](const auto &interfaceDeclaration) {
                return interfaceDeclaration.nameStr() == interfaceDeclarationName;
            });
    if (found != packageInterfaceDeclarations.end()) {
        return &(*found);
    }

    throw UndeclaredInterfaceException(interfaceDeclarationName);
}

} // namespace conversion::iv::translator::seds
