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
 * @brief   Represents an SDL variable declaration
 */
class VariableDeclaration final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   name   name of the declared variable
     * @param   type   type of the declared variable
     */
    VariableDeclaration(QString name = "", QString type = "");

    /**
     * @brief    Getter for the name of the declared variable
     *
     * @return   name of the declared variable
     */
    auto name() const -> const QString &;

    /**
     * @brief    Setter for the name of the declared variable
     *
     * @param    name   name of the declared variable
     */
    auto setName(QString name) -> void;

    /**
     * @brief    Getter for the type
     *
     * @return   type of the declared variable
     */
    auto type() const -> const QString &;

    /**
     * @brief    Setter for the type
     *
     * @param    type   type of the declared variable
     */
    auto setType(QString type) -> void;

private:
    QString m_name;
    QString m_type;
};

} // namespace sdl
