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

#include "interfaces/interfacecommand.h"

namespace seds::model {

InterfaceCommand::InterfaceCommand() noexcept
    : m_mode(InterfaceCommandMode::SYNC)
{
}

InterfaceCommandMode InterfaceCommand::mode() const
{
    return m_mode;
}

void InterfaceCommand::setMode(InterfaceCommandMode mode)
{
    m_mode = mode;
}

const InterfaceCommand::CommandArguments &InterfaceCommand::arguments() const
{
    return m_arguments;
}

void InterfaceCommand::addArgument(CommandArgument argument)
{
    m_arguments.push_back(std::move(argument));
}

} // namespace seds::model
