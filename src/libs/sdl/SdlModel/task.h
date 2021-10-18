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
#include <memory>

namespace sdl {

/**
 * @brief   Represents a task action in SDL model.
 *
 * Task content is a string
 */
class Task final : public Action
{
public:
    /**
     * @brief   Constructor
     *
     * @param   name      name of the action
     * @param   content   content of the task
     */
    Task(QString name = "", QString content = "");

    /**
     * @brief   Deleted copy constructor
     */
    Task(const Task &) = delete;

    /**
     * @brief   Default move constructor
     */
    Task(Task &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Task &operator=(const Task &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    Task &operator=(Task &&) = default;

    /**
     * @brief   Getter for the task content
     *
     * @return  task content
     */
    auto content() const -> QString;

    /**
     * @brief   Setter for the task content
     *
     * @param   content task content
     */
    auto setContent(QString content) -> void;

    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    QString m_content;
};

} // namespace sdl
