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

#include "package/package.h"

namespace seds::model {

const QualifiedName &Package::qualifiedName() const
{
    return m_qualifiedName;
}

void Package::setName(common::String name)
{
    m_qualifiedName = std::move(name);
}

const DataType *Package::dataType(const common::String &name) const
{
    auto result = std::find_if(m_dataTypes.begin(), m_dataTypes.end(),
            [&name](const auto &dataType) { return dataTypeNameStr(dataType) == name; });

    if (result != m_dataTypes.end()) {
        return &(*result);
    }

    for (const auto &component : m_components) {
        result = std::find_if(component.dataTypes().begin(), component.dataTypes().end(),
                [&name](const auto &dataType) { return dataTypeNameStr(dataType) == name; });

        if (result != component.dataTypes().end()) {
            return &(*result);
        }
    }

    return nullptr;
}

const Package::DataTypeSet &Package::dataTypes() const
{
    return m_dataTypes;
}

void Package::addDataType(DataType dataType)
{
    m_dataTypes.push_back(std::move(dataType));
}

const Package::DeclaredInterfaceSet &Package::declaredInterfaces() const
{
    return m_declaredInterfaces;
}

void Package::addInterfaceDeclaration(InterfaceDeclaration interfaceDeclaration)
{
    m_declaredInterfaces.push_back(std::move(interfaceDeclaration));
}

const Package::ComponentSet &Package::components() const
{
    return m_components;
}

void Package::addComponent(Component component)
{
    m_components.push_back(std::move(component));
}

const QString &Package::nameStr() const
{
    return m_qualifiedName.name().value();
}

} // namespace seds::model
