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

#include "specialized/componentactivitiesparser.h"

#include <QXmlStreamReader>

#include <seds/ThirdParty/magicenum.h>
#include <seds/SedsModel/components/componentimplementation.h>
#include <seds/SedsModel/components/activities/activity.h>
#include <seds/SedsModel/components/activities/activityargument.h>
#include <seds/SedsModel/components/activities/activityinvocation.h>
#include <seds/SedsModel/components/activities/andedconditions.h>
#include <seds/SedsModel/components/activities/assignment.h>
#include <seds/SedsModel/components/activities/body.h>
#include <seds/SedsModel/components/activities/booleanexpression.h>
#include <seds/SedsModel/components/activities/calibration.h>
#include <seds/SedsModel/components/activities/comparison.h>
#include <seds/SedsModel/components/activities/comparisonoperator.h>
#include <seds/SedsModel/components/activities/conditional.h>
#include <seds/SedsModel/components/activities/mathoperation.h>
#include <seds/SedsModel/components/activities/operand.h>
#include <seds/SedsModel/components/activities/operator.h>
#include <seds/SedsModel/components/activities/oredconditions.h>
#include <seds/SedsModel/components/activities/polynomial.h>
#include <seds/SedsModel/components/activities/polynomialterm.h>
#include <seds/SedsModel/components/activities/splinecalibrator.h>
#include <seds/SedsModel/components/activities/splinepoint.h>
#include <seds/SedsModel/components/activities/statement.h>
#include <seds/SedsModel/components/activities/typecheck.h>
#include <seds/SedsModel/components/activities/valueoperand.h>
#include <seds/SedsModel/components/activities/variablerefoperand.h>

#include "exceptions.h"
#include "specialized/componentprimitivesparser.h"
#include "specialized/componentsparser.h"
#include "specialized/coreparser.h"
#include "specialized/datatypesparser.h"

namespace seds::parser {

void
ComponentActivitiesParser::readActivitySet(model::ComponentImplementation& implementation, QXmlStreamReader& xmlReader)
{
    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("Activity")) {
            implementation.addActivity(readActivity(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "ActivitySet");
        }
    }
}

std::unique_ptr<model::Body>
ComponentActivitiesParser::readBody(QXmlStreamReader& xmlReader)
{
    auto body = std::make_unique<model::Body>();

    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("Assignment")) {
            body->addStatement(readAssignment(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Call")) {
            body->addStatement(readActivityInvocation(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Calibration")) {
            body->addStatement(readCalibration(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Conditional")) {
            body->addStatement(readConditional(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Iteration")) {
            body->addStatement(readIteration(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("MathOperation")) {
            body->addStatement(readMathOperation(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("SendCommandPrimitive")) {
            body->addStatement(ComponentPrimitivesParser::readSendCommandPrimitive(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("SendParameterPrimitive")) {
            body->addStatement(ComponentPrimitivesParser::readSendParameterPrimitive(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Body");
        }
    }

    return body;
}

model::ActivityInvocation
ComponentActivitiesParser::readActivityInvocation(QXmlStreamReader& xmlReader)
{
    model::ActivityInvocation activityInvocation;

    for(const auto& attribute : xmlReader.attributes()) {
        if(processForStatement(&activityInvocation, attribute)) {
            continue;
        } else if(attribute.name() == QStringLiteral("activity")) {
            activityInvocation.setActivity(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(processForStatement(&activityInvocation, xmlReader)) {
            continue;
        } else if(xmlReader.name() == QStringLiteral("ArgumentValue")) {
            activityInvocation.addArgumentValue(ComponentPrimitivesParser::readNamedArgumentValue(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "ActivityInvocation");
        }
    }

    return activityInvocation;
}

model::Polynomial
ComponentActivitiesParser::readPolynomial(QXmlStreamReader& xmlReader)
{
    model::Polynomial polynomial;

    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("Term")) {
            polynomial.addTerm(readPolynomialTerm(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Polynomial");
        }
    }

    return polynomial;
}

model::SplineCalibrator
ComponentActivitiesParser::readSplineCalibrator(QXmlStreamReader& xmlReader)
{
    model::SplineCalibrator splineCalibrator;

    for(const auto& attribute : xmlReader.attributes()) {
        if(attribute.name() == QStringLiteral("extrapolate")) {
            splineCalibrator.setExtrapolate(CoreParser::parseBool(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("SplinePoint")) {
            splineCalibrator.addSplinePoint(readSplinePoint(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "SplineCalibrator");
        }
    }

    return splineCalibrator;
}

model::BooleanExpression
ComponentActivitiesParser::readBooleanExpression(QXmlStreamReader& xmlReader)
{
    model::BooleanExpression booleanExpression;

    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("Condition")) {
            booleanExpression.setCondition(readComparison(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("ANDedConditions")) {
            booleanExpression.setCondition(readAndedConditions(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("ORedConditions")) {
            booleanExpression.setCondition(readOredConditions(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("TypeCondition")) {
            booleanExpression.setCondition(readTypeCheck(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "BooleanExpression");
        }
    }

    return booleanExpression;
}

model::Operand
ComponentActivitiesParser::readOperand(QXmlStreamReader& xmlReader)
{
    model::Operand operand;

    for(const auto& attribute : xmlReader.attributes()) {
        if(processForOperand(&operand, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(processForOperand(&operand, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "Operand");
        }
    }

    return operand;
}

bool
ComponentActivitiesParser::processForOperand(model::Operand* object, const QXmlStreamAttribute& attribute)
{
    (void)object;
    (void)attribute;

    return false;
}

bool
ComponentActivitiesParser::processForOperand(model::Operand* object, QXmlStreamReader& xmlReader)
{
    if(xmlReader.name() == QStringLiteral("Value")) {
        object->setValue(readValueOperand(xmlReader));
    } else if(xmlReader.name() == QStringLiteral("VariableRef")) {
        object->setValue(readVariableRefOperand(xmlReader));
    } else {
        return false;
    }

    return true;
}

bool
ComponentActivitiesParser::processForStatement(model::Statement* object, const QXmlStreamAttribute& attribute)
{
    return CoreParser::processForDescription(object, attribute);
}

bool
ComponentActivitiesParser::processForStatement(model::Statement* object, QXmlStreamReader& xmlReader)
{
    return CoreParser::processForDescription(object, xmlReader);
}

model::Activity
ComponentActivitiesParser::readActivity(QXmlStreamReader& xmlReader)
{
    model::Activity activity;

    for(const auto& attribute : xmlReader.attributes()) {
        if(CoreParser::processForNamedEntity(&activity, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(CoreParser::processForNamedEntity(&activity, xmlReader)) {
            continue;
        } else if(xmlReader.name() == QStringLiteral("Argument")) {
            activity.addArgument(readActivityArgument(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Body")) {
            activity.setBody(readBody(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Activity");
        }
    }

    return activity;
}

model::ActivityArgument
ComponentActivitiesParser::readActivityArgument(QXmlStreamReader& xmlReader)
{
    model::ActivityArgument argument;

    for(const auto& attribute : xmlReader.attributes()) {
        if(DataTypesParser::processForField(&argument, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(DataTypesParser::processForField(&argument, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "ActivityArgument");
        }
    }

    return argument;
}

model::Assignment
ComponentActivitiesParser::readAssignment(QXmlStreamReader& xmlReader)
{
    model::Assignment assignment;

    for(const auto& attribute : xmlReader.attributes()) {
        if(processForStatement(&assignment, attribute)) {
            continue;
        } else if(attribute.name() == QStringLiteral("outputVariableRef")) {
            assignment.setOutputVariableRef(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(processForStatement(&assignment, xmlReader)) {
            continue;
        } else if(xmlReader.name() == QStringLiteral("VariableRef")) {
            assignment.setElement(ComponentsParser::readVariableRef(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Value")) {
            assignment.setElement(readValueOperand(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Assignment");
        }
    }

    return assignment;
}

model::Calibration
ComponentActivitiesParser::readCalibration(QXmlStreamReader& xmlReader)
{
    model::Calibration calibration;

    for(const auto& attribute : xmlReader.attributes()) {
        if(processForStatement(&calibration, attribute)) {
            continue;
        } else if(attribute.name() == QStringLiteral("outputVariableRef")) {
            calibration.setOutputVariableRef(attribute.value().toString());
        } else if(attribute.name() == QStringLiteral("inputVariableRef")) {
            calibration.setInputVariableRef(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(processForStatement(&calibration, xmlReader)) {
            continue;
        } else if(xmlReader.name() == QStringLiteral("PolynomialCalibrator")) {
            calibration.setCalibrator(readPolynomial(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("SplineCalibrator")) {
            calibration.setCalibrator(readSplineCalibrator(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Calibration");
        }
    }

    return calibration;
}

model::Conditional
ComponentActivitiesParser::readConditional(QXmlStreamReader& xmlReader)
{
    model::Conditional conditional;

    for(const auto& attribute : xmlReader.attributes()) {
        if(processForStatement(&conditional, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(processForStatement(&conditional, xmlReader)) {
            continue;
        } else if(xmlReader.name() == QStringLiteral("Condition")) {
            conditional.setCondition(readBooleanExpression(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("OnConditionTrue")) {
            conditional.setOnConditionTrue(readBody(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("OnConditionFalse")) {
            conditional.setOnConditionFalse(readBody(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Conditional");
        }
    }

    return conditional;
}

model::Iteration
ComponentActivitiesParser::readIteration(QXmlStreamReader& xmlReader)
{
    model::Iteration iteration;

    for(const auto& attribute : xmlReader.attributes()) {
        if(ComponentActivitiesParser::processForStatement(&iteration, attribute)) {
            continue;
        } else if(attribute.name() == QStringLiteral("iteratorVariableRef")) {
            iteration.setIteratorVariableRef(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    model::Iteration::NumericRange numericRange;
    auto useNumericRange = false;

    while(xmlReader.readNextStartElement()) {
        if(processForStatement(&iteration, xmlReader)) {
            continue;
        } else if(xmlReader.name() == QStringLiteral("StartAt")) {
            useNumericRange = true;
            numericRange.setStartAt(readOperand(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Step")) {
            useNumericRange = true;
            numericRange.setStep(readOperand(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("EndAt")) {
            useNumericRange = true;
            numericRange.setEndAt(readOperand(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("OverArray")) {
            iteration.setRange(ComponentsParser::readVariableRef(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Do")) {
            iteration.setDoBody(readBody(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Iteration");
        }
    }

    if(useNumericRange) {
        iteration.setRange(std::move(numericRange));
    }

    return iteration;
}

model::MathOperation
ComponentActivitiesParser::readMathOperation(QXmlStreamReader& xmlReader)
{
    model::MathOperation mathOperation;

    for(const auto& attribute : xmlReader.attributes()) {
        if(processForStatement(&mathOperation, attribute)) {
            continue;
        } else if(attribute.name() == QStringLiteral("outputVariableRef")) {
            mathOperation.setOutputVariableRef(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(processForStatement(&mathOperation, xmlReader)) {
            continue;
        } else if(xmlReader.name() == QStringLiteral("ValueOperand")) {
            mathOperation.addElement(readValueOperand(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("VariableRef")) {
            mathOperation.addElement(ComponentsParser::readVariableRef(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Operator")) {
            mathOperation.addElement(readOperator(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "MathOperation");
        }
    }

    return mathOperation;
}

model::Operator
ComponentActivitiesParser::readOperator(QXmlStreamReader& xmlReader)
{
    model::Operator op;

    for(const auto& attribute : xmlReader.attributes()) {
        if(attribute.name() == QStringLiteral("operator")) {
            op.setMathOperator(parseMathOperator(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        throw UnhandledElement(xmlReader.name(), "Operator");
    }

    return op;
}

model::ValueOperand
ComponentActivitiesParser::readValueOperand(QXmlStreamReader& xmlReader)
{
    model::ValueOperand valueOperand;

    for(const auto& attribute : xmlReader.attributes()) {
        if(attribute.name() == QStringLiteral("value")) {
            valueOperand.setValue(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        throw UnhandledElement(xmlReader.name(), "ValueOperand");
    }

    return valueOperand;
}

model::VariableRefOperand
ComponentActivitiesParser::readVariableRefOperand(QXmlStreamReader& xmlReader)
{
    model::VariableRefOperand variableRefOperand;

    for(const auto& attribute : xmlReader.attributes()) {
        if(attribute.name() == QStringLiteral("variableRef")) {
            variableRefOperand.setVariableRef(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        throw UnhandledElement(xmlReader.name(), "VariableRefOperand");
    }

    return variableRefOperand;
}

model::PolynomialTerm
ComponentActivitiesParser::readPolynomialTerm(QXmlStreamReader& xmlReader)
{
    model::PolynomialTerm polynomialTerm;

    for(const auto& attribute : xmlReader.attributes()) {
        if(attribute.name() == QStringLiteral("coefficient")) {
            polynomialTerm.setCoefficient(CoreParser::parseDouble(attribute.value()));
        } else if(attribute.name() == QStringLiteral("exponent")) {
            polynomialTerm.setExponent(CoreParser::parseUInt64(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        throw UnhandledElement(xmlReader.name(), "PolynomialTerm");
    }

    return polynomialTerm;
}

model::SplinePoint
ComponentActivitiesParser::readSplinePoint(QXmlStreamReader& xmlReader)
{
    model::SplinePoint splinePoint;

    for(const auto& attribute : xmlReader.attributes()) {
        if(attribute.name() == QStringLiteral("order")) {
            splinePoint.setOrder(CoreParser::parseInt32(attribute.value()));
        } else if(attribute.name() == QStringLiteral("raw")) {
            splinePoint.setRaw(CoreParser::parseDouble(attribute.value()));
        } else if(attribute.name() == QStringLiteral("calibrated")) {
            splinePoint.setCalibrated(CoreParser::parseDouble(attribute.value()));
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        throw UnhandledElement(xmlReader.name(), "SplinePoint");
    }

    return splinePoint;
}

model::Comparison
ComponentActivitiesParser::readComparison(QXmlStreamReader& xmlReader)
{
    model::Comparison comparison;

    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("FirstOperand")) {
            comparison.setFirstOperand(readVariableRefOperand(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("ComparisonOperator")) {
            comparison.setComparisonOperator(parseComparisonOperator(xmlReader.readElementText()));
        } else if(xmlReader.name() == QStringLiteral("SecondOperand")) {
            comparison.setSecondOperand(readVariableRefOperand(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Value")) {
            comparison.setSecondOperand(readValueOperand(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Comparison");
        }
    }

    return comparison;
}

std::unique_ptr<model::AndedConditions>
ComponentActivitiesParser::readAndedConditions(QXmlStreamReader& xmlReader)
{
    auto conditions = std::make_unique<model::AndedConditions>();

    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("Condition")) {
            conditions->addCondition(readComparison(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("ORedConditions")) {
            conditions->addCondition(readOredConditions(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("TypeCondition")) {
            conditions->addCondition(readTypeCheck(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "AndedConditions");
        }
    }

    return conditions;
}

std::unique_ptr<model::OredConditions>
ComponentActivitiesParser::readOredConditions(QXmlStreamReader& xmlReader)
{
    auto conditions = std::make_unique<model::OredConditions>();

    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("Condition")) {
            conditions->addCondition(readComparison(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("ANDedConditions")) {
            conditions->addCondition(readAndedConditions(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("TypeCondition")) {
            conditions->addCondition(readTypeCheck(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "OredConditions");
        }
    }

    return conditions;
}

model::TypeCheck
ComponentActivitiesParser::readTypeCheck(QXmlStreamReader& xmlReader)
{
    model::TypeCheck typeCheck;

    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("FirstOperand")) {
            typeCheck.setFirstOperand(readVariableRefOperand(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("TypeOperand")) {
            typeCheck.setTypeOperand(xmlReader.readElementText());
        } else {
            throw UnhandledElement(xmlReader.name(), "TypeCheck");
        }
    }

    return typeCheck;
}

model::ComparisonOperator
ComponentActivitiesParser::parseComparisonOperator(QString comparisonOperatorStr)
{
    auto comparisonOperatorStdStr = comparisonOperatorStr.toStdString();
    std::transform(comparisonOperatorStdStr.begin(),
                   comparisonOperatorStdStr.end(),
                   comparisonOperatorStdStr.begin(),
                   ::toupper);

    auto coreComparisonOperator = magic_enum::enum_cast<model::ComparisonOperator>(comparisonOperatorStdStr);

    if(coreComparisonOperator) {
        return *coreComparisonOperator;
    }

    throw ParserException(QString("Unable to parse comparison operator '%1'").arg(comparisonOperatorStr));
}

model::MathOperator
ComponentActivitiesParser::parseMathOperator(QStringRef mathOperatorStr)
{
    auto mathOperatorStdStr = mathOperatorStr.toString().toStdString();
    std::transform(mathOperatorStdStr.begin(), mathOperatorStdStr.end(), mathOperatorStdStr.begin(), ::toupper);

    auto coreMathOperator = magic_enum::enum_cast<model::CoreMathOperator>(mathOperatorStdStr);

    if(coreMathOperator) {
        return *coreMathOperator;
    }

    throw ParserException(QString("Unable to parse math operator '%1'").arg(mathOperatorStr));
}

} // namespace seds::parser
