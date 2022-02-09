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

#include "expression.h"
#include "variableref.h"

namespace promela::model {
/**
 * @brief Representation of assignment in promela
 */
class Assignment final
{
public:
    /**
     * @brief Constructor.
     *
     * @param variableRef reference to the variable on the left side of assignment
     * @param expression expression on the right side of assignment
     */
    Assignment(VariableRef variableRef, Expression expression);

    /**
     * @brief Getter for variable reference on left side of assignment
     *
     * @return variable reference
     */
    const VariableRef &getVariableRef() const noexcept;

    /**
     * @brief Getter for expression on the right side of assignment
     *
     * @return expression
     */
    const Expression &getExpression() const noexcept;

private:
    VariableRef m_variableRef;
    Expression m_expression;
};
}
