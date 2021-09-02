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
#include "interfaces/argumentscombination.h"
#include "interfaces/commandargument.h"
#include "interfaces/interfacecommandmode.h"

#include <set>

namespace seds::model {

class InterfaceCommand final : public NamedEntity
{
public:
    using CommandArguments = std::vector<CommandArgument>;

public:
    InterfaceCommand() noexcept;
    InterfaceCommand(InterfaceCommand &&) = default;
    InterfaceCommand &operator=(InterfaceCommand &&) = default;

public:
    auto mode() const -> InterfaceCommandMode;
    auto setMode(InterfaceCommandMode mode) -> void;

    auto arguments() const -> const CommandArguments &;
    auto addArgument(CommandArgument argument) -> void;

public:
    auto argumentsCombination() const -> ArgumentsCombination;

private:
    InterfaceCommandMode m_mode;
    CommandArguments m_arguments;

    std::set<CommandArgumentMode> m_argumentsModes;
};

} // namespace seds::model
