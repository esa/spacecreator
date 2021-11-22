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

#include "mathoperationtranslator.h"

#include "components/activities/coremathoperator.h"
#include "components/activities/operator.h"
#include "translation/exceptions.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/overloaded.h>
#include <conversion/iv/SedsToIvTranslator/specialized/interfacecommandtranslator.h>

using conversion::Escaper;
using conversion::translator::TranslationException;

namespace conversion::sdl::translator {

using seds::model::CoreMathOperator;

auto MathOperationTranslator::translateOperation(const seds::model::MathOperation::Elements &elements) -> QString
{
    bool unused;
    auto mutableExpression = extractMutableExpression(elements);
    return translateMutableExpression(mutableExpression, unused);
}

auto MathOperationTranslator::extractMutableExpression(const seds::model::MathOperation::Elements &elements)
        -> Expression
{
    Expression expression;
    for (const auto &element : elements) {
        expression.push(&element);
    }
    return expression;
}

auto MathOperationTranslator::translateMutableExpression(Expression &expression, bool &outIsComplexValue) -> QString
{
    const auto &head = expression.front();
    expression.pop();
    return std::visit(overloaded { [&outIsComplexValue](const seds::model::ValueOperand &value) {
                                      outIsComplexValue = false;
                                      return value.value().value();
                                  },
                              [&outIsComplexValue](const seds::model::VariableRef &value) {
                                  outIsComplexValue = false;
                                  return Escaper::escapeAsn1FieldName(value.value().value());
                              },
                              [&outIsComplexValue, &expression](const seds::model::Operator &value) {
                                  return translateOperator(value, expression, outIsComplexValue);
                              } },
            *head);
}

auto MathOperationTranslator::translateOperator(
        const seds::model::Operator &op, Expression &expression, bool &outIsComplexValue) -> QString
{
    bool unused;
    bool isLeftComplex;
    bool isRightComplex;
    // Most of the expressions are contained within a call, not requiring parenthesis
    outIsComplexValue = false;
    // This code depends on the presence of built-in operators in SDL software
    // Math operator variant has only one possible type
    switch (std::get<CoreMathOperator>(op.mathOperator())) {
    case CoreMathOperator::Abs:
        return QString("abs(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Acos:
        return QString("acos(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Acosh:
        return QString("acosh(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Add: {
        outIsComplexValue = true;
        const auto left = translateMutableExpression(expression, isLeftComplex);
        const auto right = translateMutableExpression(expression, isRightComplex);
        return formExpression("%1 + %2", left, isLeftComplex, right, isRightComplex);
    }
    case CoreMathOperator::Asin:
        return QString("asin(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Asinh:
        return QString("asinh(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Atan:
        return QString("atan(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Atan2: {
        const auto left = translateMutableExpression(expression, unused);
        const auto right = translateMutableExpression(expression, unused);
        return formExpression("atan2(%1, %2)", left, false, right, false);
    }
    case CoreMathOperator::Atanh:
        return QString("atanh(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Ceil:
        return QString("ceil(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Cos:
        return QString("cos(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Cosh:
        return QString("cosh(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Divide: {
        outIsComplexValue = true;
        const auto left = translateMutableExpression(expression, isLeftComplex);
        const auto right = translateMutableExpression(expression, isRightComplex);
        return formExpression("%1 / %2", left, isLeftComplex, right, isRightComplex);
    }
    case CoreMathOperator::Exp:
        return QString("exp(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Floor:
        return QString("floor(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Inverse: {
        const auto right = translateMutableExpression(expression, isRightComplex);
        return formExpression("%1/%2", "1", false, right, isRightComplex);
    }
    case CoreMathOperator::Ln:
        return QString("ln(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Log:
        return QString("log(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Max: {
        const auto left = translateMutableExpression(expression, unused);
        const auto right = translateMutableExpression(expression, unused);
        return formExpression("max(%1, %2)", left, false, right, false);
    }
    case CoreMathOperator::Min: {
        const auto left = translateMutableExpression(expression, unused);
        const auto right = translateMutableExpression(expression, unused);
        return formExpression("min(%1, %2)", left, false, right, false);
    }
    case CoreMathOperator::Modulus: {
        const auto left = translateMutableExpression(expression, unused);
        const auto right = translateMutableExpression(expression, unused);
        return formExpression("mod(%1, %2)", left, false, right, false);
    }
    case CoreMathOperator::Multiply: {
        outIsComplexValue = true;
        const auto left = translateMutableExpression(expression, isLeftComplex);
        const auto right = translateMutableExpression(expression, isRightComplex);
        return formExpression("%1 * %2", left, isLeftComplex, right, isRightComplex);
    }
    case CoreMathOperator::Pow: {
        const auto left = translateMutableExpression(expression, unused);
        const auto right = translateMutableExpression(expression, unused);
        return formExpression("power(%1, %2)", left, false, right, false);
    }
    case CoreMathOperator::Round:
        return QString("round(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Sin:
        return QString("sin(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Sinh:
        return QString("sinh(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Sqrt:
        return QString("sqrt(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Subtract: {
        outIsComplexValue = true;
        const auto left = translateMutableExpression(expression, isLeftComplex);
        const auto right = translateMutableExpression(expression, isRightComplex);
        return formExpression("%1 - %2", left, isLeftComplex, right, isRightComplex);
    }
    case CoreMathOperator::Swap:
        throw TranslationException("Nested swap operator is not supported in an expression");
    case CoreMathOperator::Tan:
        return QString("tan(%1)").arg(translateMutableExpression(expression, unused));
    case CoreMathOperator::Tanh:
        return QString("tanh(%1)").arg(translateMutableExpression(expression, unused));
    }
    throw TranslationException("MathOperator not implemented");
    return "";
}

auto MathOperationTranslator::formExpression(const QString pattern, const QString left, const bool isLeftComplex,
        const QString right, const bool isRightComplex) -> QString
{
    const auto leftSide = isLeftComplex ? QString("(%1)").arg(left) : left;
    const auto rightSide = isRightComplex ? QString("(%1)").arg(right) : right;
    return QString(pattern).arg(leftSide, rightSide);
}

}
