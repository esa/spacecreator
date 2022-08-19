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

#include "signalroute.h"

#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

SignalRoute::SignalRoute(QString name)
    : Node(std::move(name))
{
}

const std::vector<Route> &SignalRoute::routes() const
{
    return m_routes;
}

void SignalRoute::addRoute(Route route)
{
    m_routes.push_back(std::move(route));
}

void SignalRoute::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl
