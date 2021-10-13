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

#include "signal.h"
#include "transition.h"
#include "variablereference.h"

#include <QString>
#include <memory>
#include <variant>
#include <vector>

namespace sdl {

class VariableLiteral final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   value   value of the literal
     */
    VariableLiteral(QString value = "");

    /**
     * @brief   Deleted copy constructor
     */
    VariableLiteral(const VariableLiteral &) = delete;

    /**
     * @brief   Default move constructor
     */
    VariableLiteral(VariableLiteral &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    VariableLiteral &operator=(const VariableLiteral &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    VariableLiteral &operator=(VariableLiteral &&) = default;

    /**
     * @brief   Getter for literal value
     *
     * @returns a literal value
     */
    auto value() const -> QString;

    /**
     * @brief   Setter for literal value
     *
     * @param   value a literal value
     */
    auto setValue(QString value) -> void;

private:
    QString m_value;
};

} // namespace sdl
