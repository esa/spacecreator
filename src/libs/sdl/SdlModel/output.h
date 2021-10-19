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
#include "signal.h"
#include "variableliteral.h"
#include "variablereference.h"

#include <QString>
#include <memory>
#include <variant>

namespace sdl {

/**
 * @brief   Represents an SDL output signal action.
 *
 */
class Output final : public Action
{
public:
    /**
     * @brief   Default constructor
     */
    Output() = default;

    /**
     * @brief   Deleted copy constructor
     */
    Output(const Output &) = delete;

    /**
     * @brief   Default move constructor
     */
    Output(Output &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Output &operator=(const Output &) = delete;

    /**
     * @brief   Default move assignment operator
     */
    Output &operator=(Output &&) = default;

    /**
     * @brief   Getter for the optional output parameter
     *
     * @return  a pointer to variable reference
     */
    auto parameter() const -> VariableReference *;

    /**
     * @brief   Setter for the optional output parameter
     *
     * @param   parameter a pointer to parameter
     */
    auto setParameter(VariableReference *parameter) -> void;

    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    VariableReference *m_parameter;
};

} // namespace sdl
