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

#include "system.h"

#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

System::System(QString name)
    : Node(std::move(name))
{
}

const QStringList &System::freeformTexts() const
{
    return m_freeformTexts;
}

void System::addFreeformText(QString text)
{
    m_freeformTexts.push_back(text);
}

const std::vector<std::unique_ptr<Signal>> &System::getSignals() const
{
    return m_signals;
}

void System::addSignal(std::unique_ptr<Signal> signal)
{
    m_signals.push_back(std::move(signal));
}

const std::vector<Channel> &System::channels() const
{
    return m_channels;
}

void System::addChannel(Channel channel)
{
    m_channels.push_back(std::move(channel));
}

const Block &System::block() const
{
    return m_block;
}

void System::setBlock(Block block)
{
    m_block = std::move(block);
}

void System::createRoutes(const QString &channelName, const QString &signalRouteName)
{
    Route route;
    route.setFrom("env");
    route.setTo(m_block.name());
    for (const auto &signal : m_signals) {
        route.addWith(signal->name());
    }

    Channel channel(channelName);
    channel.addRoute(route);
    addChannel(std::move(channel));

    SignalRoute signalRoute(signalRouteName);
    signalRoute.addRoute(route);
    m_block.addSignalRoute(std::move(signalRoute));

    Connection connection(channelName, signalRouteName);
    m_block.addConnection(std::move(connection));
}

void System::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl
