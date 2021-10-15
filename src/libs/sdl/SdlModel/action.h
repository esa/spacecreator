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

#include "label.h"
#include "node.h"

#include <QString>
#include <memory>
#include <optional>

namespace sdl {

/**
 * @brief   Represents an SDL action
 */
class Action : public Node
{
public:
    /**
     * @brief   Constructor
     *
     * @param   name   name of the element
     */
    Action(QString name = "");

    /**
     * @brief   Deleted copy constructor
     */
    Action(const Action &) = delete;

    /**
     * @brief   Default move constructor
     */
    Action(Action &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Action &operator=(const Action &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    Action &operator=(Action &&) = default;

    /**
     * @brief   Default move assignment operator
     *
     * @param   label  pointer to the label that shall be put before this action
     */
    auto setLabel(std::unique_ptr<Label> label) -> void;

private:
    std::optional<std::unique_ptr<Label>> m_label;
};

} // namespace sdl
