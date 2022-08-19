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

#include "sdlchannelbuilder.h"

#include <sdl/SdlModel/route.h>

using sdl::Channel;
using sdl::Route;

namespace tests::common {

SdlChannelBuilder::SdlChannelBuilder(QString channelName)
{
    m_channel.setName(std::move(channelName));
}

Channel SdlChannelBuilder::build()
{
    return std::move(m_channel);
}

SdlChannelBuilder &SdlChannelBuilder::withRoute(QString from, QString to, QStringList with)
{
    Route route;
    route.setFrom(std::move(from));
    route.setTo(std::move(to));
    route.setWith(std::move(with));

    m_channel.addRoute(std::move(route));

    return *this;
}

} // namespace tests::common
