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

#include "sedsinterfacecommandbuilder.h"

using namespace seds::model;

namespace tests::conversion::common {

SedsInterfaceCommandBuilder::SedsInterfaceCommandBuilder(QString name, InterfaceCommandMode mode)
{
    m_command.setName(std::move(name));
    m_command.setMode(mode);
}

InterfaceCommand SedsInterfaceCommandBuilder::build()
{
    return std::move(m_command);
}

SedsInterfaceCommandBuilder &SedsInterfaceCommandBuilder::withArgument(
        QString name, QString type, CommandArgumentMode mode)
{
    CommandArgument argument;
    argument.setName(std::move(name));
    argument.setType(std::move(type));
    argument.setMode(mode);

    m_command.addArgument(std::move(argument));

    return *this;
}

} // namespace tests::conversion::common
