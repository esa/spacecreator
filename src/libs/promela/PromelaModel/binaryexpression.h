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

#include <memory>

namespace promela::model {
class Expression;

/**
 * @brief Representation of binary expression in promela.
 */
class BinaryExpression final
{
public:
    /**
     * @brief Binary operators in Promela.
     */
    enum class Operator
    {
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE,
        EQUAL,
        LESS,
        GREATER,
        LEQUAL,
        GEQUAL,
        MODULO,
        NEQUAL,
        AND,
        OR,
    };

    /**
     * @brief Constructor.
     *
     * @param oper binary operator
     * @param left left hand side of binary expression
     * @param right right hand side of binary expression
     */
    BinaryExpression(Operator oper, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);

    /**
     * @brief Copy constructor.
     *
     * @param other other BinaryExpression
     */
    BinaryExpression(const BinaryExpression &other);
    /**
     * @brief Move constructor.
     *
     * @param other other BinaryExpression
     */
    BinaryExpression(BinaryExpression &&other);

    /**
     * @brief Copy assignment operator.
     *
     * @param other other BinaryExpression
     */
    const BinaryExpression &operator=(const BinaryExpression &other);
    /**
     * @brief Move assignment operator.
     *
     * @param other other BinaryExpression
     */
    const BinaryExpression &operator=(BinaryExpression &&other);

    /**
     * @brief Getter for operator.
     *
     * @return binary operator
     */
    Operator getOperator() const noexcept;
    /**
     * @brief Getter for left hand side of binary expression.
     *
     * @return left hand side of binary expression.
     */
    const Expression *getLeft() const;
    /**
     * @brief Getter for right hand side of binary expression.
     *
     * @return right hand side of binary expression.
     */
    const Expression *getRight() const;

private:
    Operator m_operator;
    std::unique_ptr<Expression> m_left;
    std::unique_ptr<Expression> m_right;
};
}
