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

#include "binaryexpression.h"

#include "expression.h"

namespace promela::model {
BinaryExpression::BinaryExpression(
        BinaryExpression::Operator oper, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
    : m_operator(oper)
    , m_left(std::move(left))
    , m_right(std::move(right))
{
}

BinaryExpression::BinaryExpression(const BinaryExpression &other)
    : m_operator(other.m_operator)
    , m_left(std::make_unique<Expression>(*other.m_left))
    , m_right(std::make_unique<Expression>(*other.m_right))
{
}

BinaryExpression::BinaryExpression(BinaryExpression &&other)
    : m_operator(other.m_operator)
    , m_left(std::move(other.m_left))
    , m_right(std::move(other.m_right))
{
}

const BinaryExpression &BinaryExpression::operator=(const BinaryExpression &other)
{
    m_operator = other.m_operator;
    m_left = std::make_unique<Expression>(*other.m_left);
    m_right = std::make_unique<Expression>(*other.m_right);

    return *this;
}

const BinaryExpression &BinaryExpression::operator=(BinaryExpression &&other)
{
    m_operator = other.m_operator;
    m_left = std::move(other.m_left);
    m_right = std::move(other.m_right);

    return *this;
}

BinaryExpression::Operator BinaryExpression::getOperator() const noexcept
{
    return m_operator;
}

const Expression *BinaryExpression::getLeft() const
{
    return m_left.get();
}
const Expression *BinaryExpression::getRight() const
{
    return m_right.get();
}

}
