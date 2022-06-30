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

#include "expressionvisitor.h"

#include "inlinecallvisitor.h"
#include "variablerefvisitor.h"

using promela::model::BinaryExpression;
using promela::model::BooleanConstant;
using promela::model::Constant;
using promela::model::Expression;
using promela::model::InlineCall;
using promela::model::VariableRef;

namespace promela::exporter {
ExpressionVisitor::ExpressionVisitor(QTextStream &stream)
    : m_stream(stream)
{
}

void ExpressionVisitor::visit(const Expression &expression)
{
    std::visit(*this, expression.getContent());
}

void ExpressionVisitor::operator()(const VariableRef &variableRef)
{
    VariableRefVisitor visitor(m_stream);
    visitor.visit(variableRef);
}

void ExpressionVisitor::operator()(const Constant &constant)
{
    m_stream << constant.getValue();
}

void ExpressionVisitor::operator()(const BinaryExpression &expression)
{
    m_stream << "(";

    ExpressionVisitor leftVisitor(m_stream);
    leftVisitor.visit(*expression.getLeft());

    m_stream << " ";
    switch (expression.getOperator()) {
    case BinaryExpression::Operator::ADD:
        m_stream << "+";
        break;
    case BinaryExpression::Operator::SUBTRACT:
        m_stream << "-";
        break;
    case BinaryExpression::Operator::MULTIPLY:
        m_stream << "*";
        break;
    case BinaryExpression::Operator::DIVIDE:
        m_stream << "/";
        break;
    case BinaryExpression::Operator::EQUAL:
        m_stream << "==";
        break;
    case BinaryExpression::Operator::LESS:
        m_stream << "<";
        break;
    case BinaryExpression::Operator::GREATER:
        m_stream << ">";
        break;
    case BinaryExpression::Operator::LEQUAL:
        m_stream << "<=";
        break;
    case BinaryExpression::Operator::GEQUAL:
        m_stream << ">=";
        break;
    case BinaryExpression::Operator::MODULO:
        m_stream << "%";
        break;
    case BinaryExpression::Operator::NEQUAL:
        m_stream << "!=";
        break;
    case BinaryExpression::Operator::AND:
        m_stream << "&&";
        break;
    case BinaryExpression::Operator::OR:
        m_stream << "||";
        break;
    }

    m_stream << " ";

    ExpressionVisitor rightVisitor(m_stream);
    rightVisitor.visit(*expression.getRight());

    m_stream << ")";
}

void ExpressionVisitor::operator()(const InlineCall &inlineCall)
{
    InlineCallVisitor visitor(m_stream);
    visitor.visit(inlineCall);
}

void ExpressionVisitor::operator()(const BooleanConstant &constant)
{
    if (constant.getValue()) {
        m_stream << "true";
    } else {
        m_stream << "false";
    }
}
}
