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

#include "signal.h"

namespace sdl {

/**
 * @brief   Represents an SDL signal renaming
 */
class Rename final : public Signal
{
public:
    /// @brief  Direction of the signals to rename
    enum class Direction
    {
        Input,
        Output
    };

public:
    /**
     * @brief   Default constructor
     */
    Rename() = default;
    /**
     * @brief   Constructor
     *
     * @param   name                    Signal name
     * @param   direction               Renamed signal direction
     * @param   referencedName          Name of the renamed signal
     * @param   referencedFunctionName  Name of the function from the renamed signal
     */
    Rename(QString name, Direction direction, QString referencedName, QString referencedFunctionName);
    /**
     * @brief   Deleted copy constructor
     */
    Rename(const Rename &) = delete;
    /**
     * @brief   Default move constructor
     */
    Rename(Rename &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Rename &operator=(const Rename &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    Rename &operator=(Rename &&) = default;

public:
    /**
     * @brief   Getter for direction
     *
     * @return  Direction
     */
    auto direction() const -> Direction;
    /**
     * @brief   Setter for direction
     *
     * @param   direction   New direction
     */
    auto setDirection(Direction direction) -> void;

    /**
     * @brief   Getter for referenced signal name
     *
     * @return  Referenced name
     */
    auto referencedName() const -> const QString &;
    /**
     * @brief   Setter for referenced signal name
     *
     * @param   name    Referenced signal name
     */
    auto setReferencedName(QString name) -> void;

    /**
     * @brief   Getter for referenced function name
     *
     * @return  Referenced name
     */
    auto referencedFunctionName() const -> const QString &;
    /**
     * @brief   Setter for referenced funcion name
     *
     * @param   name    Referenced function name
     */
    auto setReferencedFunctionName(QString name) -> void;

public:
    /**
     * @brief   Visitor acceptor (shall call visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void;

private:
    Direction m_direction;
    QString m_referencedName;
    QString m_referencedFunctionName;
};

} // namespace sdl
