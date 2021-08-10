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

#include "components/activities/coremathoperator.h"

namespace seds::model {

template<>
auto enumFromString(const QStringRef enumStr) -> std::optional<CoreMathOperator>
{
    if (enumStr.compare(QStringLiteral("abs"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Abs;
    } else if (enumStr.compare(QStringLiteral("acos"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Acos;
    } else if (enumStr.compare(QStringLiteral("acosh"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Acosh;
    } else if (enumStr.compare(QStringLiteral("add"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Add;
    } else if (enumStr.compare(QStringLiteral("asin"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Asin;
    } else if (enumStr.compare(QStringLiteral("asinh"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Asinh;
    } else if (enumStr.compare(QStringLiteral("atan"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Atan;
    } else if (enumStr.compare(QStringLiteral("atan2"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Atan2;
    } else if (enumStr.compare(QStringLiteral("atanh"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Atanh;
    } else if (enumStr.compare(QStringLiteral("ceil"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Ceil;
    } else if (enumStr.compare(QStringLiteral("cos"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Cos;
    } else if (enumStr.compare(QStringLiteral("cosh"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Cosh;
    } else if (enumStr.compare(QStringLiteral("divide"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Divide;
    } else if (enumStr.compare(QStringLiteral("exp"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Exp;
    } else if (enumStr.compare(QStringLiteral("floor"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Floor;
    } else if (enumStr.compare(QStringLiteral("inverse"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Inverse;
    } else if (enumStr.compare(QStringLiteral("ln"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Ln;
    } else if (enumStr.compare(QStringLiteral("log"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Log;
    } else if (enumStr.compare(QStringLiteral("max"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Max;
    } else if (enumStr.compare(QStringLiteral("min"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Min;
    } else if (enumStr.compare(QStringLiteral("modulus"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Modulus;
    } else if (enumStr.compare(QStringLiteral("multiply"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Multiply;
    } else if (enumStr.compare(QStringLiteral("pow"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Pow;
    } else if (enumStr.compare(QStringLiteral("round"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Round;
    } else if (enumStr.compare(QStringLiteral("sin"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Sin;
    } else if (enumStr.compare(QStringLiteral("sinh"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Sinh;
    } else if (enumStr.compare(QStringLiteral("sqrt"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Sqrt;
    } else if (enumStr.compare(QStringLiteral("subtract"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Subtract;
    } else if (enumStr.compare(QStringLiteral("swap"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Swap;
    } else if (enumStr.compare(QStringLiteral("tan"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Tan;
    } else if (enumStr.compare(QStringLiteral("tanh"), Qt::CaseInsensitive) == 0) {
        return CoreMathOperator::Tanh;
    } else {
        return std::nullopt;
    }
}

} // namespace seds::model
