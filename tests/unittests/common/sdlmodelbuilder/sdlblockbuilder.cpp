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

#include "sdlblockbuilder.h"

#include <sdl/SdlModel/connection.h>

using sdl::Block;
using sdl::Connection;
using sdl::Process;
using sdl::SignalRoute;

namespace tests::common {

SdlBlockBuilder::SdlBlockBuilder(QString blockName)
{
    m_block.setName(std::move(blockName));
}

Block SdlBlockBuilder::build()
{
    return std::move(m_block);
}

SdlBlockBuilder &SdlBlockBuilder::withSignalRoute(sdl::SignalRoute signalRoute)
{
    m_block.addSignalRoute(std::move(signalRoute));

    return *this;
}

SdlBlockBuilder &SdlBlockBuilder::withConnection(QString channelName, QString signalRouteName)
{
    Connection connection(std::move(channelName), std::move(signalRouteName));
    m_block.addConnection(std::move(connection));

    return *this;
}

SdlBlockBuilder &SdlBlockBuilder::withProcess(Process process)
{
    m_block.setProcess(std::move(process));

    return *this;
}

} // namespace tests::common
