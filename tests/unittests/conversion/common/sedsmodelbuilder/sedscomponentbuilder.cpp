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

#include "sedscomponentbuilder.h"

using namespace seds::model;

namespace tests::conversion::common {

SedsComponentBuilder::SedsComponentBuilder(QString name)
{
    m_component.setName(std::move(name));
}

SedsComponentBuilder::operator Component()
{
    return std::move(m_component);
}

SedsComponentBuilder &SedsComponentBuilder::declaringInterface(InterfaceDeclaration interfaceDeclaration)
{
    m_component.addInterfaceDeclaration(std::move(interfaceDeclaration));

    return *this;
}

SedsComponentBuilder &SedsComponentBuilder::withProvidedInterface(Interface interface)
{
    m_component.addProvidedInterface(std::move(interface));

    return *this;
}

SedsComponentBuilder &SedsComponentBuilder::withRequiredInterface(Interface interface)
{
    m_component.addRequiredInterface(std::move(interface));

    return *this;
}

} // namespace tests::conversion::common
