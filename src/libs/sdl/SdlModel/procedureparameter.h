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

#include "variabledeclaration.h"

#include <QString>

namespace sdl {

/**
 * @brief   Represents an input/output SDL Procedure parameter
 */
class ProcedureParameter final : public VariableDeclaration
{
public:
    /**
     * @brief   Constructor
     *
     * @param   name         name of the parameter
     * @param   type         type of the parameter
     * @param   direction    direction (input or output) of the parameter
     */
    ProcedureParameter(QString name = "", QString type = "", QString direction = "");

    /**
     * @brief   Deleted copy constructor
     */
    ProcedureParameter(const ProcedureParameter &) = delete;

    /**
     * @brief   Default move constructor
     */
    ProcedureParameter(ProcedureParameter &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    ProcedureParameter &operator=(const ProcedureParameter &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    ProcedureParameter &operator=(ProcedureParameter &&) = default;

    /**
     * @brief    Getter for the direction
     *
     * @return   direction of the parameter
     */
    auto direction() const -> const QString &;

    /**
     * @brief    Setter for the direction
     *
     * @param    type   type of the declared variable
     */
    auto setDirection(QString type) -> void;

    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    QString m_direction;
};

} // namespace sdl
