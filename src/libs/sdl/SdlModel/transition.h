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

#include "action.h"
#include "node.h"

#include <QString>
#include <memory>
#include <vector>

namespace sdl {

/**
 * @brief   Represents an SDL transition
 */
class Transition : public Node
{
public:
    /**
     * @brief   Constructor
     *
     * @param   name    name of this transition
     * @param   actions reference to the vector of pointers to actions
     */
    Transition(const QString &name, std::vector<std::shared_ptr<Action>> &actions);

    /**
     * @brief    Getter for the actions
     *
     * @return   const reference to the vector of pointers to actions
     */
    auto actions() -> const std::vector<std::shared_ptr<Action>> &;

    /**
     * @brief    Setter for the actions
     *
     * @param    const reference to the vector of pointers to actions
     */
    auto setActions(const std::vector<std::shared_ptr<Action>> &actions) -> void;

    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    auto accept(Visitor &visitor) const -> void;

private:
    std::vector<std::shared_ptr<Action>> m_actions;
};

} // namespace sdl
