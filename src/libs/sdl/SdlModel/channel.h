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

#pragma once

#include "node.h"
#include "route.h"

#include <vector>

namespace sdl {

/**
 * @brief   Represents an SDL channel
 */
class Channel final : public Node
{
public:
    /**
     * @brief   Constructor
     *
     * @param   name    Channel name
     */
    Channel(QString name = "");
    /**
     * @brief   Deleted copy constructor
     */
    Channel(const Channel &) = delete;
    /**
     * @brief   Default move constructor
     */
    Channel(Channel &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Channel &operator=(const Channel &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    Channel &operator=(Channel &&) = default;

public:
    /**
     * @brief   Getter for routes
     *
     * @return  Routes
     */
    auto routes() const -> const std::vector<Route> &;
    /**
     * @brief   Adds route
     *
     * @param   route       Route to add
     */
    auto addRoute(Route route) -> void;

public:
    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    std::vector<Route> m_routes;
};

} // namespace sdl
