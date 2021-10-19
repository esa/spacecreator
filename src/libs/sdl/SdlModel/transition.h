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
class Transition final
{
public:
    /**
     * @brief   Default constructor
     */
    Transition() = default;

    /**
     * @brief   Deleted copy constructor
     */
    Transition(const Transition &) = delete;

    /**
     * @brief   Default move constructor
     */
    Transition(Transition &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Transition &operator=(const Transition &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    Transition &operator=(Transition &&) = default;

    /**
     * @brief    Getter for the actions
     *
     * @return   const reference to the vector of pointers to actions
     */
    auto actions() -> const std::vector<std::unique_ptr<Action>> &;

    /**
     * @brief    Setter for the actions
     *
     * @param    action  a pointer to action
     */
    auto addAction(std::unique_ptr<Action> action) -> void;

private:
    std::vector<std::unique_ptr<Action>> m_actions;
};

} // namespace sdl
