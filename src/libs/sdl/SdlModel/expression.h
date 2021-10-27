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

#include <QString>

namespace sdl {

/**
 * @brief   Represents an expression used in SDL model.
 */
class Expression final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   content    content to store
     */
    Expression(QString content = "");

    /**
     * @brief   Deleted copy constructor
     */
    Expression(const Expression &) = delete;

    /**
     * @brief   Default move constructor
     */
    Expression(Expression &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Expression &operator=(const Expression &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    Expression &operator=(Expression &&) = default;

    /**
     * @brief   Getter for the stored value
     *
     * @return  const reference to the stored value
     */
    auto content() const -> const QString &;

    /**
     * @brief   Setter for the stored value
     *
     * @param   content stored expression string
     */
    auto setContent(QString content) -> void;

private:
    QString m_value;
};

} // namespace sdl
