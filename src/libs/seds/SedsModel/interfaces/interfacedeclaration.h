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

#include "base/namedentity.h"
#include "generics/generictype.h"
#include "interfaces/interfacecommand.h"
#include "interfaces/interfacelevel.h"
#include "interfaces/interfaceparameter.h"
#include "interfaces/interfaceref.h"

namespace seds::model {

class InterfaceDeclaration final : public NamedEntity
{
public:
    using BaseInterfaceSet = std::vector<InterfaceRef>;
    using CommandSet = std::vector<InterfaceCommand>;
    using GenericTypeSet = std::vector<GenericType>;
    using ParameterSet = std::vector<InterfaceParameter>;

public:
    InterfaceDeclaration() noexcept;
    InterfaceDeclaration(InterfaceDeclaration &&) = default;
    InterfaceDeclaration &operator=(InterfaceDeclaration &&) = default;

public:
    auto isAbstract() const -> bool;
    auto setAbstract(bool abstract) -> void;

    auto level() const -> InterfaceLevel;
    auto setLevel(InterfaceLevel level) -> void;

    auto baseInterfaces() const -> const BaseInterfaceSet &;
    auto addBaseInterface(InterfaceRef baseInterface) -> void;

    auto genericTypes() const -> const GenericTypeSet &;
    auto addGenericType(GenericType genericType) -> void;

    auto parameters() const -> const ParameterSet &;
    auto addParameter(InterfaceParameter parameter) -> void;

    auto commands() const -> const CommandSet &;
    auto addCommand(InterfaceCommand command) -> void;

private:
    bool m_abstract;
    InterfaceLevel m_level;

    BaseInterfaceSet m_baseInterfaces;
    GenericTypeSet m_genericTypes;
    ParameterSet m_parameters;
    CommandSet m_commands;
};

} // namespace seds::model
