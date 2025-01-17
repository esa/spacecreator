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

#include "node.h"

#include <QString>

namespace sdl {

/**
 * @brief   Represents an SDL variable declaration
 */
class VariableDeclaration : public Node
{
public:
    /**
     * @brief   Constructor
     *
     * @param   name        name of the declared variable
     * @param   type        type of the declared variable
     * @param   isMonitor   whether this variable is a monitor
     */
    VariableDeclaration(QString name = "", QString type = "", bool isMonitor = false);
    /**
     * @brief   Deleted copy constructor
     */
    VariableDeclaration(const VariableDeclaration &) = delete;
    /**
     * @brief   Default move constructor
     */
    VariableDeclaration(VariableDeclaration &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    VariableDeclaration &operator=(const VariableDeclaration &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    VariableDeclaration &operator=(VariableDeclaration &&) = default;

public:
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

    /**
     * @brief   Getter for monitor status
     *
     * @return  True if this variable is a monitor, false otherwise
     */
    auto isMonitor() const -> bool;
    /**
     * @brief   Sets whether this variable is a monitor
     *
     * @param   isMonitor       Is monitor
     */
    auto setMonitor(bool isMonitor) -> void;

public:
    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    QString m_type;
    bool m_isMonitor;
};

} // namespace sdl
