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

#include <memory>

#include <seds/SedsCommon/basetypesmappings.h>
#include <seds/SedsModel/components/activities/mathoperator.h>

class QXmlStreamAttribute;
class QXmlStreamReader;

namespace seds::model {
class Activity;
class ActivityArgument;
class ActivityInvocation;
class AndedConditions;
class Assignment;
class Body;
class BooleanExpression;
class Calibration;
class Comparison;
class ComponentImplementation;
class Conditional;
class Iteration;
class MathOperation;
class Operand;
class Operator;
class OredConditions;
class Polynomial;
class PolynomialTerm;
class SplineCalibrator;
class SplinePoint;
class Statement;
class TypeCheck;
class ValueOperand;
class VariableRefOperand;

enum class ComparisonOperator;
} // namespace seds::model

namespace seds::parser {

/**
 * @brief   SEDS parser for component activities
 */
class ComponentActivitiesParser final
{
  public:
    /**
     * @brief   Adds all activities to given component implementation
     * @param   implementation    Component implementation to which constructed Activities will be added
     */
    static auto readActivitySet(model::ComponentImplementation& implementation, QXmlStreamReader& xmlReader) -> void;

    /**
     * @brief   Constructs Body object
     * @return  Body
     */
    static auto readBody(QXmlStreamReader& xmlReader) -> std::unique_ptr<model::Body>;
    /**
     * @brief   Constructs ActivityInvocation object
     * @return  ActivityInvocation
     */
    static auto readActivityInvocation(QXmlStreamReader& xmlReader) -> model::ActivityInvocation;

    /**
     * @brief   Constructs Polynomial object
     * @return  Polynomial
     */
    static auto readPolynomial(QXmlStreamReader& xmlReader) -> model::Polynomial;
    /**
     * @brief   Constructs SplineCalibrator object
     * @return  SplineCalibrator
     */
    static auto readSplineCalibrator(QXmlStreamReader& xmlReader) -> model::SplineCalibrator;

    /**
     * @brief   Constructs BooleanExpression object
     * @return  BooleanExpression
     */
    static auto readBooleanExpression(QXmlStreamReader& xmlReader) -> model::BooleanExpression;

    /**
     * @brief   Constructs Operand object
     * @return  Operand
     */
    static auto readOperand(QXmlStreamReader& xmlReader) -> model::Operand;

    /**
     * @brief   Processes attribute for Operand
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForOperand(model::Operand* object, const QXmlStreamAttribute& attribute) -> bool;
    /**
     * @brief   Processes element for Operand
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForOperand(model::Operand* object, QXmlStreamReader& xmlReader) -> bool;
    /**
     * @brief   Processes attribute for Statement
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForStatement(model::Statement* object, const QXmlStreamAttribute& attribute) -> bool;
    /**
     * @brief   Processes element for Statement
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForStatement(model::Statement* object, QXmlStreamReader& xmlReader) -> bool;

  private:
    /**
     * @brief   Constructs Activity object
     * @return  Activity
     */
    static auto readActivity(QXmlStreamReader& xmlReader) -> model::Activity;
    /**
     * @brief   Constructs ActivityArgument object
     * @return  ActivityArgument
     */
    static auto readActivityArgument(QXmlStreamReader& xmlReader) -> model::ActivityArgument;

    /**
     * @brief   Constructs Assignment object
     * @return  Assignment
     */
    static auto readAssignment(QXmlStreamReader& xmlReader) -> model::Assignment;
    /**
     * @brief   Constructs Calibration object
     * @return  Calibration
     */
    static auto readCalibration(QXmlStreamReader& xmlReader) -> model::Calibration;
    /**
     * @brief   Constructs Conditional object
     * @return  Conditional
     */
    static auto readConditional(QXmlStreamReader& xmlReader) -> model::Conditional;
    /**
     * @brief   Constructs Iteration object
     * @return  Iteration
     */
    static auto readIteration(QXmlStreamReader& xmlReader) -> model::Iteration;
    /**
     * @brief   Constructs MathOperation object
     * @return  MathOperation
     */
    static auto readMathOperation(QXmlStreamReader& xmlReader) -> model::MathOperation;
    /**
     * @brief   Constructs Operator object
     * @return  Operator
     */
    static auto readOperator(QXmlStreamReader& xmlReader) -> model::Operator;

    /**
     * @brief   Constructs ValueOperand object
     * @return  ValueOperand
     */
    static auto readValueOperand(QXmlStreamReader& xmlReader) -> model::ValueOperand;
    /**
     * @brief   Constructs VariableRefOperand object
     * @return  VariableRefOperand
     */
    static auto readVariableRefOperand(QXmlStreamReader& xmlReader) -> model::VariableRefOperand;

    /**
     * @brief   Constructs PolynomialTerm object
     * @return  PolynomialTerm
     */
    static auto readPolynomialTerm(QXmlStreamReader& xmlReader) -> model::PolynomialTerm;
    /**
     * @brief   Constructs SplinePoint object
     * @return  SplinePoint
     */
    static auto readSplinePoint(QXmlStreamReader& xmlReader) -> model::SplinePoint;

    /**
     * @brief   Constructs Comparsion object
     * @return  Comparsion
     */
    static auto readComparison(QXmlStreamReader& xmlReader) -> model::Comparison;
    /**
     * @brief   Constructs Comparsion object
     * @return  Comparsion
     */
    static auto readAndedConditions(QXmlStreamReader& xmlReader) -> std::unique_ptr<model::AndedConditions>;
    /**
     * @brief   Constructs Comparsion object
     * @return  Comparsion
     */
    static auto readOredConditions(QXmlStreamReader& xmlReader) -> std::unique_ptr<model::OredConditions>;
    /**
     * @brief   Constructs TypeCheck object
     * @return  TypeCheck
     */
    static auto readTypeCheck(QXmlStreamReader& xmlReader) -> model::TypeCheck;

    /**
     * @brief   Parses given string as a comparison operator
     * @param   valueStr        String to parse
     * @return  Corresponding value
     * @throws  ParserException
     */
    static auto parseComparisonOperator(common::String comparisonOperatorStr) -> model::ComparisonOperator;
    /**
     * @brief   Parses given string as a math operator
     * @param   valueStr        String to parse
     * @return  Corresponding value
     * @throws  ParserException
     */
    static auto parseMathOperator(QStringRef mathOperatorStr) -> model::MathOperator;
};

} // namespace seds::parser
