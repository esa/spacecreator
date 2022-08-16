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

#include "block.h"

#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

Block::Block(QString name)
    : Node(std::move(name))
{
}

const std::vector<SignalRoute> &Block::signalRoutes() const
{
    return m_signalRoutes;
}

void Block::addSignalRoute(SignalRoute signalRoute)
{
    m_signalRoutes.push_back(std::move(signalRoute));
}

const std::vector<Connection> &Block::connections() const
{
    return m_connections;
}

void Block::addConnection(Connection connection)
{
    m_connections.push_back(std::move(connection));
}

const Process &Block::process() const
{
    return m_process;
}

void Block::setProcess(Process process)
{
    m_process = std::move(process);
}

void Block::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl
