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

#include <QString>

namespace sdl {

/**
 * @brief   Represents an answer used in SDL model.
 */
class Answer final
{
public:
    /**
     * @brief   Default constructor
     */
    Answer();

    /**
     * @brief   Deleted copy constructor
     */
    Answer(const Answer &) = delete;

    /**
     * @brief   Default move constructor
     */
    Answer(Answer &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Answer &operator=(const Answer &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    Answer &operator=(Answer &&) = default;

    /**
     * @brief   Getter for the stored action
     *
     * @return  action
     */
    auto action() const -> Action *;

    /**
     * @brief   Setter for the stored value
     *
     * @param   action  action to store
     */
    auto setAction(Action *action) -> void;

private:
    Action *m_action;
    // todo VariableLiteral m_literal; (with getter and setter)
};

} // namespace sdl
