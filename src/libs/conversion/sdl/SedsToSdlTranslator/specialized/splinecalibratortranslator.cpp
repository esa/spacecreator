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

#include "splinecalibratortranslator.h"

#include "descriptiontranslator.h"

#include <QTextStream>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/values.h>
#include <conversion/asn1/SedsToAsn1Translator/specialized/rangetranslatorvisitor.h>
#include <conversion/asn1/SedsToAsn1Translator/translator.h>
#include <iostream>
#include <sdl/SdlModel/return.h>
#include <seds/SedsModel/types/ranges/floatprecisionrange.h>

using conversion::asn1::translator::RangeTranslatorVisitor;
using conversion::asn1::translator::SedsToAsn1Translator;

namespace conversion::sdl::translator {

SplineCalibratorTranslator::SplineCalibratorTranslator(StatementTranslatorVisitor::StatementContext &context,
        const seds::model::Calibration &calibration, ::sdl::Transition *sdlTransition)
    : m_context(context)
    , m_calibration(calibration)
    , m_sdlTransition(sdlTransition)
{
}

auto SplineCalibratorTranslator::translate(const seds::model::SplineCalibrator &splineCalibrator) -> void
{
    const auto rawPointsVariableName = m_context.uniqueRawSplinePointsVariableName();
    const auto calibratedPointsVariableName = m_context.uniqueCalibratedSplinePointsVariableName();

    buildSplineCalibratorBoilerplate(m_context);
    buildSplineCalibratorVariables(splineCalibrator, rawPointsVariableName, calibratedPointsVariableName);

    const auto targetName =
            StatementTranslatorVisitor::translateVariableReference(m_calibration.outputVariableRef().value().value());
    const auto sourceName =
            StatementTranslatorVisitor::translateVariableReference(m_calibration.inputVariableRef().value().value());

    const auto action = QString("%1 := call LinearCalibration(%2, %3, %4)")
                                .arg(targetName)
                                .arg(sourceName)
                                .arg(rawPointsVariableName)
                                .arg(calibratedPointsVariableName);
    auto sdlTask = std::make_unique<::sdl::Task>("", action);

    m_sdlTransition->addAction(std::move(sdlTask));
}

auto SplineCalibratorTranslator::buildSplineCalibratorVariables(const seds::model::SplineCalibrator &splineCalibrator,
        const QString &rawPointsVariableName, const QString &calibratedPointsVariableName) -> void
{
    if (m_context.sdlProcess()->startTransition() == nullptr) {
        auto startTransition = std::make_unique<::sdl::Transition>();
        m_context.sdlProcess()->setStartTransition(std::move(startTransition));
    }

    auto startTransition = m_context.sdlProcess()->startTransition();

    const auto &splinePoints = splineCalibrator.splinePoints();

    std::vector<double> rawValues;
    rawValues.reserve(splinePoints.size());

    std::vector<double> calibratedValues;
    calibratedValues.reserve(splinePoints.size());

    for (const auto &splinePoint : splinePoints) {
        rawValues.push_back(splinePoint.raw());
        calibratedValues.push_back(splinePoint.calibrated());
    }

    buildSplinePointsVariable(rawPointsVariableName, rawValues, startTransition);
    buildSplinePointsVariable(calibratedPointsVariableName, calibratedValues, startTransition);
}

auto SplineCalibratorTranslator::buildSplinePointsVariable(
        const QString &variableName, const std::vector<double> &values, ::sdl::Transition *startTransition) -> void
{
    // Create a variable to hold spline values
    auto variable = std::make_unique<::sdl::VariableDeclaration>(variableName, "SplinePointsArray");
    m_context.sdlProcess()->addVariable(std::move(variable));

    // Create a procedure
    const auto initProcName = QString("Init%1").arg(variableName);
    auto initProc = std::make_unique<::sdl::Procedure>(initProcName);

    // Create a transition
    auto initTransition = std::make_unique<::sdl::Transition>();

    // Add initialization action
    QString initAction;
    QTextStream initActionStream(&initAction, QIODevice::WriteOnly);
    initActionStream.setRealNumberNotation(QTextStream::FixedNotation);

    initActionStream << variableName << " := { " << values[0];
    for (std::size_t i = 1; i < values.size(); ++i) {
        initActionStream << ", " << values[i];
    }
    initActionStream << " }";

    auto initTask = std::make_unique<::sdl::Task>("", initAction);
    initTransition->addAction(std::move(initTask));

    // Add call to Init function at the state machine start
    auto initCall = std::make_unique<::sdl::ProcedureCall>("");
    initCall->setProcedure(initProc.get());
    startTransition->addAction(std::move(initCall));

    // Add transition to procedure
    initProc->setTransition(std::move(initTransition));

    // Add procedure to process
    m_context.sdlProcess()->addProcedure(std::move(initProc));
}

auto SplineCalibratorTranslator::buildSplineCalibratorBoilerplate(StatementTranslatorVisitor::StatementContext &context)
        -> void
{
    static bool alreadyCreated = false;

    if (alreadyCreated) {
        return;
    }

    createAsn1Types(context);

    buildFindIntervalProcedure(context);

    buildLinearCalibrationProcedure(context);
    buildSquareCalibrationProcedure(context);
    buildCubicCalibrationProcedure(context);

    alreadyCreated = true;
}

auto SplineCalibratorTranslator::createAsn1Types(StatementTranslatorVisitor::StatementContext &context) -> void
{
    auto asn1Definitions =
            SedsToAsn1Translator::getAsn1Definitions(context.sedsPackage().nameStr(), context.asn1Model()->data());
    Q_UNUSED(asn1Definitions);

    // Create type for spline points values
    auto splinePointType = std::make_unique<Asn1Acn::Types::Real>("SplinePointValue");
    splinePointType->setEncoding(Asn1Acn::Types::RealEncoding::IEEE754_1985_64);
    auto splinePointTypeConstraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::RealValue>::create(
            { std::numeric_limits<double>::min(), std::numeric_limits<double>::max() });
    splinePointType->constraints().append(std::move(splinePointTypeConstraint));

    // Create type for array of spline points values
    auto splinePointsArrayType = std::make_unique<Asn1Acn::Types::SequenceOf>("SplinePointsArray");
    splinePointsArrayType->setItemsType(splinePointType->clone());
    auto splinePointsArrayTypeInnerConstraint =
            Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create({ 0, m_maxSplinePointsArraySize });
    auto splinePointsArrayTypeConstraint =
            std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::IntegerValue>>();
    splinePointsArrayTypeConstraint->setInnerConstraints(std::move(splinePointsArrayTypeInnerConstraint));
    splinePointsArrayType->constraints().append(std::move(splinePointsArrayTypeConstraint));

    // Create type for index of array of spline points values
    auto splinePointsArrayIndexType = std::make_unique<Asn1Acn::Types::Integer>("SplinePointsArrayIndex");
    splinePointsArrayIndexType->setSize(8);
    splinePointsArrayIndexType->setEncoding(Asn1Acn::Types::IntegerEncoding::twos_complement);
    auto splinePointsArrayIndexTypeConstraint =
            Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create({ -1, m_maxSplinePointsArraySize });
    splinePointsArrayIndexType->constraints().append(std::move(splinePointsArrayIndexTypeConstraint));

    // Add types to ASN.1 definitions
    auto splinePointTypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(
            "SplinePointValue", "SplinePointValue", Asn1Acn::SourceLocation(), std::move(splinePointType));
    asn1Definitions->addType(std::move(splinePointTypeAssignment));

    auto splinePointsArrayTypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(
            "SplinePointsArray", "SplinePointsArray", Asn1Acn::SourceLocation(), std::move(splinePointsArrayType));
    asn1Definitions->addType(std::move(splinePointsArrayTypeAssignment));

    auto splinePointsArrayIndexTypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>("SplinePointsArrayIndex",
            "SplinePointsArrayIndex", Asn1Acn::SourceLocation(), std::move(splinePointsArrayIndexType));
    asn1Definitions->addType(std::move(splinePointsArrayIndexTypeAssignment));
}

auto SplineCalibratorTranslator::buildFindIntervalProcedure(StatementTranslatorVisitor::StatementContext &context)
        -> void
{
    // Create procedure
    const QString procedureName("FindInterval");
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);

    // Create transition
    auto transition = std::make_unique<::sdl::Transition>();

    // Create procedure local variables
    auto resultVar = std::make_unique<::sdl::VariableDeclaration>("result", "SplinePointsArrayIndex");

    // Create procedure parameters
    auto valueParameter = std::make_unique<::sdl::ProcedureParameter>("value", "SplinePointValue", "in");
    auto rawPointsParameter = std::make_unique<::sdl::ProcedureParameter>("rawPoints", "SplinePointsArray", "in");

    // Initialize result variable
    auto resultVarInitAction = QString("result := 0");
    auto resultVarInitTask = std::make_unique<::sdl::Task>("", resultVarInitAction);
    transition->addAction(std::move(resultVarInitTask));

    // Create iteration start
    auto iterationStartLabel = std::make_unique<::sdl::Label>("find_interval_start");
    transition->addAction(std::move(iterationStartLabel));

    // Iterate over raw points
    auto iterationDecision = std::make_unique<::sdl::Decision>();
    auto iterationDecisionExpression = std::make_unique<::sdl::Expression>("result < length(rawPoints)");
    iterationDecision->setExpression(std::move(iterationDecisionExpression));

    // Exit iteration
    auto iterationDecisionExitTransition = std::make_unique<::sdl::Transition>();
    auto iterationDecisionExitJoin = std::make_unique<::sdl::Join>("find_interval_end");
    iterationDecisionExitTransition->addAction(std::move(iterationDecisionExitJoin));

    // Compare value with current raw value
    auto iterationDecisionDoTransition = std::make_unique<::sdl::Transition>();

    auto comparisonDecision = std::make_unique<::sdl::Decision>();
    auto comparisonDecisionExpression = std::make_unique<::sdl::Expression>("rawPoints(result) >= value");
    comparisonDecision->setExpression(std::move(comparisonDecisionExpression));

    // Return current index
    auto comparisonDecisionReturnTransition = std::make_unique<::sdl::Transition>();
    auto comparisonDecisionReturnReturn = std::make_unique<::sdl::Return>("result");
    comparisonDecisionReturnTransition->addAction(std::move(comparisonDecisionReturnReturn));

    auto comparisonDecisionTrue = std::make_unique<::sdl::Answer>();
    comparisonDecisionTrue->setLiteral(::sdl::VariableLiteral("True"));
    comparisonDecisionTrue->setTransition(std::move(comparisonDecisionReturnTransition));
    comparisonDecision->addAnswer(std::move(comparisonDecisionTrue));

    // Continue iteration
    auto comparisonDecisionContinueTransition = std::make_unique<::sdl::Transition>();

    const auto iterationDecisionContinueIncrementAction = QString("result := result + 1");
    auto iterationDecisionContinueIncrementTask =
            std::make_unique<::sdl::Task>("", iterationDecisionContinueIncrementAction);
    comparisonDecisionContinueTransition->addAction(std::move(iterationDecisionContinueIncrementTask));

    auto iterationDecisionContinueJoin = std::make_unique<::sdl::Join>("find_interval_start");
    comparisonDecisionContinueTransition->addAction(std::move(iterationDecisionContinueJoin));

    auto comparisonDecisionFalse = std::make_unique<::sdl::Answer>();
    comparisonDecisionFalse->setLiteral(::sdl::VariableLiteral("False"));
    comparisonDecisionFalse->setTransition(std::move(comparisonDecisionContinueTransition));
    comparisonDecision->addAnswer(std::move(comparisonDecisionFalse));

    // Add comparison to do transition
    iterationDecisionDoTransition->addAction(std::move(comparisonDecision));

    // Add iteration to transition
    auto iterationDecisionTrue = std::make_unique<::sdl::Answer>();
    iterationDecisionTrue->setLiteral(::sdl::VariableLiteral("True"));
    iterationDecisionTrue->setTransition(std::move(iterationDecisionDoTransition));
    iterationDecision->addAnswer(std::move(iterationDecisionTrue));

    auto iterationDecisionFalse = std::make_unique<::sdl::Answer>();
    iterationDecisionFalse->setLiteral(::sdl::VariableLiteral("False"));
    iterationDecisionFalse->setTransition(std::move(iterationDecisionExitTransition));
    iterationDecision->addAnswer(std::move(iterationDecisionFalse));

    transition->addAction(std::move(iterationDecision));

    // Create iteration exit
    auto iterationEndLabel = std::make_unique<::sdl::Label>("find_interval_end");
    transition->addAction(std::move(iterationEndLabel));

    // Assign to result
    const auto assignToResultAction = QString("result := result + 1");
    auto assignToResultTask = std::make_unique<::sdl::Task>("", assignToResultAction);
    transition->addAction(std::move(assignToResultTask));

    // Set procedure return variable
    auto resultVarRef = std::make_unique<::sdl::VariableReference>(resultVar.get());
    procedure->setReturnVariableReference(std::move(resultVarRef));

    // Add variables to procedure
    procedure->addVariable(std::move(resultVar));

    // Add parameters to procedure
    procedure->addParameter(std::move(valueParameter));
    procedure->addParameter(std::move(rawPointsParameter));

    // Add transition to procedure
    procedure->setTransition(std::move(transition));

    // Add procedure to process
    context.sdlProcess()->addProcedure(std::move(procedure));
}

auto SplineCalibratorTranslator::buildLinearCalibrationProcedure(StatementTranslatorVisitor::StatementContext &context)
        -> void
{
    // Create procedure
    const QString procedureName("LinearCalibration");
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);

    // Create procedure local variables
    auto intervalIndexVar = std::make_unique<::sdl::VariableDeclaration>("intervalIndex", "SplinePointsArrayIndex");
    auto x0Var = std::make_unique<::sdl::VariableDeclaration>("x0", "SplinePointValue");
    auto x1Var = std::make_unique<::sdl::VariableDeclaration>("x1", "SplinePointValue");
    auto y0Var = std::make_unique<::sdl::VariableDeclaration>("y0", "SplinePointValue");
    auto y1Var = std::make_unique<::sdl::VariableDeclaration>("y1", "SplinePointValue");
    auto l0Var = std::make_unique<::sdl::VariableDeclaration>("l0", "SplinePointValue");
    auto l1Var = std::make_unique<::sdl::VariableDeclaration>("l1", "SplinePointValue");
    auto resultVar = std::make_unique<::sdl::VariableDeclaration>("result", "SplinePointValue");

    // Create procedure parameters
    auto valueParameter = std::make_unique<::sdl::ProcedureParameter>("value", "SplinePointValue", "in");
    auto rawPointsParameter = std::make_unique<::sdl::ProcedureParameter>("rawPoints", "SplinePointsArray", "in");
    auto calibratedPointsParameter =
            std::make_unique<::sdl::ProcedureParameter>("calibratedPoints", "SplinePointsArray", "in");

    // Create transition
    auto transition = std::make_unique<::sdl::Transition>();

    // Add call to FindInterval
    const auto callFindIntervalAction = QString("intervalIndex := call FindInterval(value, rawPoints)");
    auto callFindIntervalTask = std::make_unique<::sdl::Task>("", callFindIntervalAction);
    transition->addAction(std::move(callFindIntervalTask));

    // Return calibrated value is value is a raw point
    auto isValueEqualRawDecision = std::make_unique<::sdl::Decision>();
    auto isValueEqualRawDecisionExpression = std::make_unique<::sdl::Expression>("value = rawPoints(intervalIndex)");
    isValueEqualRawDecision->setExpression(std::move(isValueEqualRawDecisionExpression));

    auto isValueEqualRawDecisionTrueTransition = std::make_unique<::sdl::Transition>();
    auto isValueEqualRawDecisionTrueTransitionReturn =
            std::make_unique<::sdl::Return>("calibratedPoints(intervalIndex)");
    isValueEqualRawDecisionTrueTransition->addAction(std::move(isValueEqualRawDecisionTrueTransitionReturn));
    auto isValueEqualRawDecisionTrue = std::make_unique<::sdl::Answer>();
    isValueEqualRawDecisionTrue->setLiteral(::sdl::VariableLiteral("True"));
    isValueEqualRawDecisionTrue->setTransition(std::move(isValueEqualRawDecisionTrueTransition));
    isValueEqualRawDecision->addAnswer(std::move(isValueEqualRawDecisionTrue));

    auto isValueEqualRawDecisionFalseTransition = std::make_unique<::sdl::Transition>();
    auto isValueEqualRawDecisionFalse = std::make_unique<::sdl::Answer>();
    isValueEqualRawDecisionFalse->setLiteral(::sdl::VariableLiteral("False"));
    isValueEqualRawDecisionFalse->setTransition(std::move(isValueEqualRawDecisionFalseTransition));
    isValueEqualRawDecision->addAnswer(std::move(isValueEqualRawDecisionFalse));

    transition->addAction(std::move(isValueEqualRawDecision));

    // Get x0
    const auto getX0Action = QString("x0 := rawPoints(intervalIndex-1)");
    auto getX0Task = std::make_unique<::sdl::Task>("", getX0Action);
    transition->addAction(std::move(getX0Task));

    // Get x1
    const auto getX1Action = QString("x1 := rawPoints(intervalIndex)");
    auto getX1Task = std::make_unique<::sdl::Task>("", getX1Action);
    transition->addAction(std::move(getX1Task));

    // Get y0
    const auto getY0Action = QString("y0 := calibratedPoints(intervalIndex-1)");
    auto getY0Task = std::make_unique<::sdl::Task>("", getY0Action);
    transition->addAction(std::move(getY0Task));

    // Get y1
    const auto getY1Action = QString("y1 := calibratedPoints(intervalIndex)");
    auto getY1Task = std::make_unique<::sdl::Task>("", getY1Action);
    transition->addAction(std::move(getY1Task));

    // Calculate L0
    const auto calculateL0Action = QString("l0 := (value - x1) / (x0 - x1)");
    auto calculateL0Task = std::make_unique<::sdl::Task>("", calculateL0Action);
    transition->addAction(std::move(calculateL0Task));

    // Calculate L1
    const auto calculateL1Action = QString("l1 := (value - x0) / (x1 - x0)");
    auto calculateL1Task = std::make_unique<::sdl::Task>("", calculateL1Action);
    transition->addAction(std::move(calculateL1Task));

    // Calculate result
    const auto calculateResultAction = QString("result := l0 * y0 + l1 * y1");
    auto calculateResultTask = std::make_unique<::sdl::Task>("", calculateResultAction);
    transition->addAction(std::move(calculateResultTask));

    // Set procedure return variable
    auto resultVarRef = std::make_unique<::sdl::VariableReference>(resultVar.get());
    procedure->setReturnVariableReference(std::move(resultVarRef));

    // Add variables to procedure
    procedure->addVariable(std::move(intervalIndexVar));
    procedure->addVariable(std::move(x0Var));
    procedure->addVariable(std::move(x1Var));
    procedure->addVariable(std::move(y0Var));
    procedure->addVariable(std::move(y1Var));
    procedure->addVariable(std::move(l0Var));
    procedure->addVariable(std::move(l1Var));
    procedure->addVariable(std::move(resultVar));

    // Add paramters to procedure
    procedure->addParameter(std::move(valueParameter));
    procedure->addParameter(std::move(rawPointsParameter));
    procedure->addParameter(std::move(calibratedPointsParameter));

    // Add transition to procedure
    procedure->setTransition(std::move(transition));

    // Add procedure to process
    context.sdlProcess()->addProcedure(std::move(procedure));
}

auto SplineCalibratorTranslator::buildSquareCalibrationProcedure(StatementTranslatorVisitor::StatementContext &context)
        -> void
{
    // Create procedure
    const QString procedureName("SquareCalibration");
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);

    // Create transition
    auto transition = std::make_unique<::sdl::Transition>();

    // Add transition to procedure
    procedure->setTransition(std::move(transition));

    // Add procedure to process
    context.sdlProcess()->addProcedure(std::move(procedure));
}

auto SplineCalibratorTranslator::buildCubicCalibrationProcedure(StatementTranslatorVisitor::StatementContext &context)
        -> void
{
    // Create procedure
    const QString procedureName("CubicCalibration");
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);

    // Create transition
    auto transition = std::make_unique<::sdl::Transition>();

    // Add transition to procedure
    procedure->setTransition(std::move(transition));

    // Add procedure to process
    context.sdlProcess()->addProcedure(std::move(procedure));
}

} // namespace conversion::sdl::translator
