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

#include <memory>

#include "base/namedentity.h"
#include "components/interface.h"
#include "components/componentimplementation.h"
#include "interfaces/interfacedeclaration.h"
#include "types/datatype.h"

namespace seds::model {

class Component final : public NamedEntity
{
  public:
    using DataTypeSet = std::vector<DataType>;
    using InterfaceDeclarationSet = std::vector<InterfaceDeclaration>;
    using InterfaceSet = std::vector<Interface>;

    Component() = default;
    Component(Component&&) = default;
    Component& operator=(Component&&) = default;

  public:
    auto providedInterfaces() const -> const InterfaceSet&;
    auto addProvidedInterface(Interface interface) -> void;

    auto requiredInterfaces() const -> const InterfaceSet&;
    auto addRequiredInterface(Interface interface) -> void;

    auto dataTypes() const -> const DataTypeSet&;
    auto addDataType(DataType dataType) -> void;

    auto declaredInterfaces() const -> const InterfaceDeclarationSet&;
    auto addInterfaceDeclaration(InterfaceDeclaration interfaceDeclaration) -> void;

    auto implementation() const -> const ComponentImplementation&;
    auto setImplementation(ComponentImplementation implementation) -> void;

  private:
    InterfaceSet m_providedInterfaces;
    InterfaceSet m_requiredInterfaces;
    DataTypeSet m_dataTypes;
    InterfaceDeclarationSet m_declaredInterfaces;
    ComponentImplementation m_implementation;
};

} // namespace seds::model
