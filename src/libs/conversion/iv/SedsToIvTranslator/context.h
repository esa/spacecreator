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
#pragma once

#include <QString>
#include <seds/SedsModel/interfaces/interfacedeclaration.h>

namespace seds::model {
class Package;
class Component;
} // namespace seds::model

namespace conversion::iv::translator::seds {

/**
 * @brief   Context with current SEDS to IV translation state
 */
class Context final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   package     Current package
     * @param   component   Current componen
     */
    Context(const ::seds::model::Package *package, const ::seds::model::Component *component);

public:
    /**
     * @brief   Find interface declaration with given name
     *
     * @param   interfaceDeclarationName    Name to search
     *
     * @return  Pointer to the interface declaration if found, nullptr otherwise
     */
    auto findInterfaceDeclaration(const QString &interfaceDeclarationName) const
            -> const ::seds::model::InterfaceDeclaration *;

private:
    const ::seds::model::Package *m_package;
    const ::seds::model::Component *m_component;
};

} // namespace conversion::iv::translator::seds
