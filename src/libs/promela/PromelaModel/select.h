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

#include "expression.h"
#include "sequence.h"
#include "variableref.h"

#include <list>
#include <memory>
#include <variant>

namespace promela::model {
/**
 * @brief Representation of Promela value selection from a range
 */
class Select final
{
public:
    /**
     * @brief Constructor
     *
     * @param var      Recipient variable
     * @param first    First element or range.
     * @param last     Last element of range.
     */
    Select(VariableRef var, const Expression &first, const Expression &last);

    /**
     * @brief Getter for recipient variable.
     *
     * @return Recipient variable.
     */
    const VariableRef &getRecipientVariable() const noexcept;

    /**
     * @brief Getter for first Expression of range.
     *
     * @return First Expression in range.
     */
    Expression getFirstExpression() const;

    /**
     * @brief Getter for last Expression of range.
     *
     * @return Last Expression in range.
     */
    Expression getLastExpression() const;

    /**
     * @brief Getter for first value of range if it is a constant.
     *
     * @return First value in range.
     */
    int getFirstIntValue() const;

    /**
     * @brief Getter for last value of range if it is a constant.
     *
     * @return Last value in range.
     */
    int getLastIntValue() const;

private:
    VariableRef m_variable;
    using Range = std::pair<Expression, Expression>;
    Range m_range;
};
}
