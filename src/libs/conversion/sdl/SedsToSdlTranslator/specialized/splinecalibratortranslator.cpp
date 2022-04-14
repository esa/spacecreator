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
#include <conversion/common/translation/exceptions.h>
#include <sdl/SdlModel/return.h>
#include <seds/SedsModel/types/ranges/floatprecisionrange.h>

using conversion::asn1::translator::RangeTranslatorVisitor;
using conversion::asn1::translator::SedsToAsn1Translator;
using conversion::translator::TranslationException;

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
    const auto splineOrder = getSplineOrder(splineCalibrator);

    const auto rawPointsVariableName = m_context.uniqueRawSplinePointsVariableName();
    const auto calibratedPointsVariableName = m_context.uniqueCalibratedSplinePointsVariableName();

    if (!m_context.splineBoilerplateCreated()) {
        buildSplineCalibratorBoilerplate(m_context);
        m_context.setSplineBoilerplateCreated(true);
    }

    buildSplineCalibratorVariables(splineCalibrator, rawPointsVariableName, calibratedPointsVariableName);

    const auto targetName =
            StatementTranslatorVisitor::translateVariableReference(m_calibration.outputVariableRef().value().value());
    const auto sourceName =
            StatementTranslatorVisitor::translateVariableReference(m_calibration.inputVariableRef().value().value());

    addCallToFindInterval(sourceName, rawPointsVariableName);

    const auto extrapolate = splineCalibrator.extrapolate();

    switch (splineOrder) {
    case 1: {
        addCallToCalibration(m_linearCalibrationProcedureName, targetName, sourceName, rawPointsVariableName,
                calibratedPointsVariableName, extrapolate);
    } break;
    case 2: {
        addCallToCalibration(m_squareCalibrationProcedureName, targetName, sourceName, rawPointsVariableName,
                calibratedPointsVariableName, extrapolate);
    } break;
    case 3: {
        addCallToCalibration(m_cubicCalibrationProcedureName, targetName, sourceName, rawPointsVariableName,
                calibratedPointsVariableName, extrapolate);
    } break;
    }
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
    auto variable = std::make_unique<::sdl::VariableDeclaration>(variableName, m_splinePointsArrayTypeName);
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
    createAsn1Types(context);

    buildFindIntervalProcedure(context);

    buildLinearCalibrationProcedure(context);
    buildSquareCalibrationProcedure(context);
    buildCubicCalibrationProcedure(context);

    auto intervalIndexVar =
            std::make_unique<::sdl::VariableDeclaration>("intervalIndex", m_splinePointsArrayIndexTypeName);
    context.sdlProcedure()->addVariable(std::move(intervalIndexVar));
}

auto SplineCalibratorTranslator::createAsn1Types(StatementTranslatorVisitor::StatementContext &context) -> void
{
    auto asn1Definitions =
            SedsToAsn1Translator::getAsn1Definitions(context.sedsPackage().nameStr(), context.asn1Model()->data());
    Q_UNUSED(asn1Definitions);

    // Create type for spline points values
    auto splinePointType = std::make_unique<Asn1Acn::Types::Real>(m_splinePointValueTypeName);
    splinePointType->setEncoding(Asn1Acn::Types::RealEncoding::IEEE754_1985_64);
    auto splinePointTypeConstraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::RealValue>::create(
            { std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max() });
    splinePointType->constraints().append(std::move(splinePointTypeConstraint));

    // Create type for array of spline points values
    auto splinePointsArrayType = std::make_unique<Asn1Acn::Types::SequenceOf>(m_splinePointsArrayTypeName);
    splinePointsArrayType->setItemsType(splinePointType->clone());
    auto splinePointsArrayTypeInnerConstraint =
            Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create({ 0, m_maxSplinePointsArraySize });
    auto splinePointsArrayTypeConstraint =
            std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::IntegerValue>>();
    splinePointsArrayTypeConstraint->setInnerConstraints(std::move(splinePointsArrayTypeInnerConstraint));
    splinePointsArrayType->constraints().append(std::move(splinePointsArrayTypeConstraint));

    // Create type for index of array of spline points values
    auto splinePointsArrayIndexType = std::make_unique<Asn1Acn::Types::Integer>(m_splinePointsArrayIndexTypeName);
    splinePointsArrayIndexType->setSize(8);
    splinePointsArrayIndexType->setEncoding(Asn1Acn::Types::IntegerEncoding::twos_complement);
    auto splinePointsArrayIndexTypeConstraint =
            Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create({ -1, m_maxSplinePointsArraySize });
    splinePointsArrayIndexType->constraints().append(std::move(splinePointsArrayIndexTypeConstraint));

    // Add types to ASN.1 definitions
    auto splinePointTypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(m_splinePointValueTypeName,
            m_splinePointValueTypeName, Asn1Acn::SourceLocation(), std::move(splinePointType));
    asn1Definitions->addType(std::move(splinePointTypeAssignment));

    auto splinePointsArrayTypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(m_splinePointsArrayTypeName,
            m_splinePointsArrayTypeName, Asn1Acn::SourceLocation(), std::move(splinePointsArrayType));
    asn1Definitions->addType(std::move(splinePointsArrayTypeAssignment));

    auto splinePointsArrayIndexTypeAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(m_splinePointsArrayIndexTypeName,
                    m_splinePointsArrayIndexTypeName, Asn1Acn::SourceLocation(), std::move(splinePointsArrayIndexType));
    asn1Definitions->addType(std::move(splinePointsArrayIndexTypeAssignment));
}

auto SplineCalibratorTranslator::buildFindIntervalProcedure(StatementTranslatorVisitor::StatementContext &context)
        -> void
{
    // Create procedure
    const QString procedureName(m_findIntervalProcedureName);
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);

    // Create transition
    auto transition = std::make_unique<::sdl::Transition>();

    // Create procedure local variables
    auto resultVar = std::make_unique<::sdl::VariableDeclaration>("result", m_splinePointsArrayIndexTypeName);

    // Create procedure parameters
    auto valueParameter = std::make_unique<::sdl::ProcedureParameter>("value", m_splinePointValueTypeName, "in");
    auto rawPointsParameter =
            std::make_unique<::sdl::ProcedureParameter>("rawPoints", m_splinePointsArrayTypeName, "in");

    // Check if given value is smaller than the first point
    auto valueSmallerDecision = std::make_unique<::sdl::Decision>();
    auto valueSmallerDecisionExpression = std::make_unique<::sdl::Expression>("value < rawPoints(0)");
    valueSmallerDecision->setExpression(std::move(valueSmallerDecisionExpression));

    // Return -1
    auto valueSmallerDecisionTrueTransition = std::make_unique<::sdl::Transition>();
    auto valueSmallerDecisionTrueReturn = std::make_unique<::sdl::Return>("-1");
    valueSmallerDecisionTrueTransition->addAction(std::move(valueSmallerDecisionTrueReturn));

    // Continue
    auto valueSmallerDecisionFalseTransition = std::make_unique<::sdl::Transition>();

    // Add decision to transition
    auto valueSmallerDecisionTrue = std::make_unique<::sdl::Answer>();
    valueSmallerDecisionTrue->setLiteral(::sdl::VariableLiteral("True"));
    valueSmallerDecisionTrue->setTransition(std::move(valueSmallerDecisionTrueTransition));
    valueSmallerDecision->addAnswer(std::move(valueSmallerDecisionTrue));

    auto valueSmallerDecisionFalse = std::make_unique<::sdl::Answer>();
    valueSmallerDecisionFalse->setLiteral(::sdl::VariableLiteral("False"));
    valueSmallerDecisionFalse->setTransition(std::move(valueSmallerDecisionFalseTransition));
    valueSmallerDecision->addAnswer(std::move(valueSmallerDecisionFalse));

    transition->addAction(std::move(valueSmallerDecision));

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
    const QString procedureName(m_linearCalibrationProcedureName);
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);

    // Create procedure local variables
    auto intervalIndexVar =
            std::make_unique<::sdl::VariableDeclaration>("intervalIndex", m_splinePointsArrayIndexTypeName);
    auto x0Var = std::make_unique<::sdl::VariableDeclaration>("x0", m_splinePointValueTypeName);
    auto x1Var = std::make_unique<::sdl::VariableDeclaration>("x1", m_splinePointValueTypeName);
    auto y0Var = std::make_unique<::sdl::VariableDeclaration>("y0", m_splinePointValueTypeName);
    auto y1Var = std::make_unique<::sdl::VariableDeclaration>("y1", m_splinePointValueTypeName);
    auto l0Var = std::make_unique<::sdl::VariableDeclaration>("l0", m_splinePointValueTypeName);
    auto l1Var = std::make_unique<::sdl::VariableDeclaration>("l1", m_splinePointValueTypeName);
    auto resultVar = std::make_unique<::sdl::VariableDeclaration>("result", m_splinePointValueTypeName);

    // Create procedure parameters
    auto indexParameter = std::make_unique<::sdl::ProcedureParameter>("index", m_splinePointsArrayIndexTypeName, "in");
    auto valueParameter = std::make_unique<::sdl::ProcedureParameter>("value", m_splinePointValueTypeName, "in");
    auto rawPointsParameter =
            std::make_unique<::sdl::ProcedureParameter>("rawPoints", m_splinePointsArrayTypeName, "in");
    auto calibratedPointsParameter =
            std::make_unique<::sdl::ProcedureParameter>("calibratedPoints", m_splinePointsArrayTypeName, "in");

    // Create transition
    auto transition = std::make_unique<::sdl::Transition>();

    // Set interval index
    const auto setIntervalIndexAction = QString("intervalIndex := index");
    auto setIntervalIndexTask = std::make_unique<::sdl::Task>("", setIntervalIndexAction);
    transition->addAction(std::move(setIntervalIndexTask));

    // Handle left extrapolation
    handleLeftExtrapolation(1, transition.get());

    // Handle right extrapolation
    handleRightExtrapolation("length(rawPoints) - 1", transition.get());

    // Return calibrated value is value is a raw point
    addValueEqualRawCheck(transition.get());

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

    // Add parameters to procedure
    procedure->addParameter(std::move(indexParameter));
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
    const QString procedureName(m_squareCalibrationProcedureName);
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);

    // Create procedure local variables
    auto intervalIndexVar =
            std::make_unique<::sdl::VariableDeclaration>("intervalIndex", m_splinePointsArrayIndexTypeName);
    auto x0Var = std::make_unique<::sdl::VariableDeclaration>("x0", m_splinePointValueTypeName);
    auto x1Var = std::make_unique<::sdl::VariableDeclaration>("x1", m_splinePointValueTypeName);
    auto x2Var = std::make_unique<::sdl::VariableDeclaration>("x2", m_splinePointValueTypeName);
    auto y0Var = std::make_unique<::sdl::VariableDeclaration>("y0", m_splinePointValueTypeName);
    auto y1Var = std::make_unique<::sdl::VariableDeclaration>("y1", m_splinePointValueTypeName);
    auto y2Var = std::make_unique<::sdl::VariableDeclaration>("y2", m_splinePointValueTypeName);
    auto l0Var = std::make_unique<::sdl::VariableDeclaration>("l0", m_splinePointValueTypeName);
    auto l1Var = std::make_unique<::sdl::VariableDeclaration>("l1", m_splinePointValueTypeName);
    auto l2Var = std::make_unique<::sdl::VariableDeclaration>("l2", m_splinePointValueTypeName);
    auto resultVar = std::make_unique<::sdl::VariableDeclaration>("result", m_splinePointValueTypeName);

    // Create procedure parameters
    auto indexParameter = std::make_unique<::sdl::ProcedureParameter>("index", m_splinePointsArrayIndexTypeName, "in");
    auto valueParameter = std::make_unique<::sdl::ProcedureParameter>("value", m_splinePointValueTypeName, "in");
    auto rawPointsParameter =
            std::make_unique<::sdl::ProcedureParameter>("rawPoints", m_splinePointsArrayTypeName, "in");
    auto calibratedPointsParameter =
            std::make_unique<::sdl::ProcedureParameter>("calibratedPoints", m_splinePointsArrayTypeName, "in");

    // Create transition
    auto transition = std::make_unique<::sdl::Transition>();

    // Set interval index
    const auto setIntervalIndexAction = QString("intervalIndex := index");
    auto setIntervalIndexTask = std::make_unique<::sdl::Task>("", setIntervalIndexAction);
    transition->addAction(std::move(setIntervalIndexTask));

    // Handle left extrapolation
    handleLeftExtrapolation(1, transition.get());

    // Handle right extrapolation
    handleRightExtrapolation("length(rawPoints) - 2", transition.get());

    // Return calibrated value is value is a raw point
    addValueEqualRawCheck(transition.get());

    // Handle last right interval
    handleLastRightInterval(transition.get());

    // Get x0
    const auto getX0Action = QString("x0 := rawPoints(intervalIndex-1)");
    auto getX0Task = std::make_unique<::sdl::Task>("", getX0Action);
    transition->addAction(std::move(getX0Task));

    // Get x1
    const auto getX1Action = QString("x1 := rawPoints(intervalIndex)");
    auto getX1Task = std::make_unique<::sdl::Task>("", getX1Action);
    transition->addAction(std::move(getX1Task));

    // Get x2
    const auto getX2Action = QString("x2 := rawPoints(intervalIndex+1)");
    auto getX2Task = std::make_unique<::sdl::Task>("", getX2Action);
    transition->addAction(std::move(getX2Task));

    // Get y0
    const auto getY0Action = QString("y0 := calibratedPoints(intervalIndex-1)");
    auto getY0Task = std::make_unique<::sdl::Task>("", getY0Action);
    transition->addAction(std::move(getY0Task));

    // Get y1
    const auto getY1Action = QString("y1 := calibratedPoints(intervalIndex)");
    auto getY1Task = std::make_unique<::sdl::Task>("", getY1Action);
    transition->addAction(std::move(getY1Task));

    // Get y2
    const auto getY2Action = QString("y2 := calibratedPoints(intervalIndex+1)");
    auto getY2Task = std::make_unique<::sdl::Task>("", getY2Action);
    transition->addAction(std::move(getY2Task));

    // Calculate L0
    const auto calculateL0Action = QString("l0 := ((value - x1) * (value - x2)) / ((x0 - x1) * (x0 - x2))");
    auto calculateL0Task = std::make_unique<::sdl::Task>("", calculateL0Action);
    transition->addAction(std::move(calculateL0Task));

    // Calculate L1
    const auto calculateL1Action = QString("l1 := ((value - x0) * (value - x2)) / ((x1 - x0) * (x1 - x2))");
    auto calculateL1Task = std::make_unique<::sdl::Task>("", calculateL1Action);
    transition->addAction(std::move(calculateL1Task));

    // Calculate L2
    const auto calculateL2Action = QString("l2 := ((value - x0) * (value - x1)) / ((x2 - x0) * (x2 - x1))");
    auto calculateL2Task = std::make_unique<::sdl::Task>("", calculateL2Action);
    transition->addAction(std::move(calculateL2Task));

    // Calculate result
    const auto calculateResultAction = QString("result := l0 * y0 + l1 * y1 + l2 * y2");
    auto calculateResultTask = std::make_unique<::sdl::Task>("", calculateResultAction);
    transition->addAction(std::move(calculateResultTask));

    // Set procedure return variable
    auto resultVarRef = std::make_unique<::sdl::VariableReference>(resultVar.get());
    procedure->setReturnVariableReference(std::move(resultVarRef));

    // Add variables to procedure
    procedure->addVariable(std::move(intervalIndexVar));
    procedure->addVariable(std::move(x0Var));
    procedure->addVariable(std::move(x1Var));
    procedure->addVariable(std::move(x2Var));
    procedure->addVariable(std::move(y0Var));
    procedure->addVariable(std::move(y1Var));
    procedure->addVariable(std::move(y2Var));
    procedure->addVariable(std::move(l0Var));
    procedure->addVariable(std::move(l1Var));
    procedure->addVariable(std::move(l2Var));
    procedure->addVariable(std::move(resultVar));

    // Add parameters to procedure
    procedure->addParameter(std::move(indexParameter));
    procedure->addParameter(std::move(valueParameter));
    procedure->addParameter(std::move(rawPointsParameter));
    procedure->addParameter(std::move(calibratedPointsParameter));

    // Add transition to procedure
    procedure->setTransition(std::move(transition));

    // Add procedure to process
    context.sdlProcess()->addProcedure(std::move(procedure));
}

auto SplineCalibratorTranslator::buildCubicCalibrationProcedure(StatementTranslatorVisitor::StatementContext &context)
        -> void
{
    // Create procedure
    const QString procedureName(m_cubicCalibrationProcedureName);
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);

    // Create procedure local variables
    auto intervalIndexVar =
            std::make_unique<::sdl::VariableDeclaration>("intervalIndex", m_splinePointsArrayIndexTypeName);
    auto x0Var = std::make_unique<::sdl::VariableDeclaration>("x0", m_splinePointValueTypeName);
    auto x1Var = std::make_unique<::sdl::VariableDeclaration>("x1", m_splinePointValueTypeName);
    auto x2Var = std::make_unique<::sdl::VariableDeclaration>("x2", m_splinePointValueTypeName);
    auto x3Var = std::make_unique<::sdl::VariableDeclaration>("x3", m_splinePointValueTypeName);
    auto y0Var = std::make_unique<::sdl::VariableDeclaration>("y0", m_splinePointValueTypeName);
    auto y1Var = std::make_unique<::sdl::VariableDeclaration>("y1", m_splinePointValueTypeName);
    auto y2Var = std::make_unique<::sdl::VariableDeclaration>("y2", m_splinePointValueTypeName);
    auto y3Var = std::make_unique<::sdl::VariableDeclaration>("y3", m_splinePointValueTypeName);
    auto l0Var = std::make_unique<::sdl::VariableDeclaration>("l0", m_splinePointValueTypeName);
    auto l1Var = std::make_unique<::sdl::VariableDeclaration>("l1", m_splinePointValueTypeName);
    auto l2Var = std::make_unique<::sdl::VariableDeclaration>("l2", m_splinePointValueTypeName);
    auto l3Var = std::make_unique<::sdl::VariableDeclaration>("l3", m_splinePointValueTypeName);
    auto resultVar = std::make_unique<::sdl::VariableDeclaration>("result", m_splinePointValueTypeName);

    // Create procedure parameters
    auto indexParameter = std::make_unique<::sdl::ProcedureParameter>("index", m_splinePointsArrayIndexTypeName, "in");
    auto valueParameter = std::make_unique<::sdl::ProcedureParameter>("value", m_splinePointValueTypeName, "in");
    auto rawPointsParameter =
            std::make_unique<::sdl::ProcedureParameter>("rawPoints", m_splinePointsArrayTypeName, "in");
    auto calibratedPointsParameter =
            std::make_unique<::sdl::ProcedureParameter>("calibratedPoints", m_splinePointsArrayTypeName, "in");

    // Create transition
    auto transition = std::make_unique<::sdl::Transition>();

    // Set interval index
    const auto setIntervalIndexAction = QString("intervalIndex := index");
    auto setIntervalIndexTask = std::make_unique<::sdl::Task>("", setIntervalIndexAction);
    transition->addAction(std::move(setIntervalIndexTask));

    // Handle left extrapolation
    handleLeftExtrapolation(2, transition.get());

    // Handle right extrapolation
    handleRightExtrapolation("length(rawPoints) - 2", transition.get());

    // Return calibrated value is value is a raw point
    addValueEqualRawCheck(transition.get());

    // Handle last right interval
    handleFirstLeftInterval(transition.get());

    // Handle last right interval
    handleLastRightInterval(transition.get());

    // Get x0
    const auto getX0Action = QString("x0 := rawPoints(intervalIndex-2)");
    auto getX0Task = std::make_unique<::sdl::Task>("", getX0Action);
    transition->addAction(std::move(getX0Task));

    // Get x1
    const auto getX1Action = QString("x1 := rawPoints(intervalIndex-1)");
    auto getX1Task = std::make_unique<::sdl::Task>("", getX1Action);
    transition->addAction(std::move(getX1Task));

    // Get x2
    const auto getX2Action = QString("x2 := rawPoints(intervalIndex)");
    auto getX2Task = std::make_unique<::sdl::Task>("", getX2Action);
    transition->addAction(std::move(getX2Task));

    // Get x3
    const auto getX3Action = QString("x3 := rawPoints(intervalIndex+1)");
    auto getX3Task = std::make_unique<::sdl::Task>("", getX3Action);
    transition->addAction(std::move(getX3Task));

    // Get y0
    const auto getY0Action = QString("y0 := calibratedPoints(intervalIndex-2)");
    auto getY0Task = std::make_unique<::sdl::Task>("", getY0Action);
    transition->addAction(std::move(getY0Task));

    // Get y1
    const auto getY1Action = QString("y1 := calibratedPoints(intervalIndex-1)");
    auto getY1Task = std::make_unique<::sdl::Task>("", getY1Action);
    transition->addAction(std::move(getY1Task));

    // Get y2
    const auto getY2Action = QString("y2 := calibratedPoints(intervalIndex)");
    auto getY2Task = std::make_unique<::sdl::Task>("", getY2Action);
    transition->addAction(std::move(getY2Task));

    // Get y3
    const auto getY3Action = QString("y3 := calibratedPoints(intervalIndex+1)");
    auto getY3Task = std::make_unique<::sdl::Task>("", getY3Action);
    transition->addAction(std::move(getY3Task));

    // Calculate L0
    const auto calculateL0Action =
            QString("l0 := ((value - x1) * (value - x2) * (value - x3)) / ((x0 - x1) * (x0 - x2) * (x0 - x3))");
    auto calculateL0Task = std::make_unique<::sdl::Task>("", calculateL0Action);
    transition->addAction(std::move(calculateL0Task));

    // Calculate L1
    const auto calculateL1Action =
            QString("l1 := ((value - x0) * (value - x2) * (value - x3)) / ((x1 - x0) * (x1 - x2) * (x1 - x3))");
    auto calculateL1Task = std::make_unique<::sdl::Task>("", calculateL1Action);
    transition->addAction(std::move(calculateL1Task));

    // Calculate L2
    const auto calculateL2Action =
            QString("l2 := ((value - x0) * (value - x1) * (value - x3)) / ((x2 - x0) * (x2 - x1) * (x2 - x3))");
    auto calculateL2Task = std::make_unique<::sdl::Task>("", calculateL2Action);
    transition->addAction(std::move(calculateL2Task));

    // Calculate L3
    const auto calculateL3Action =
            QString("l3 := ((value - x0) * (value - x1) * (value - x2)) / ((x3 - x0) * (x3 - x1) * (x3 - x2))");
    auto calculateL3Task = std::make_unique<::sdl::Task>("", calculateL3Action);
    transition->addAction(std::move(calculateL3Task));

    // Calculate result
    const auto calculateResultAction = QString("result := l0 * y0 + l1 * y1 + l2 * y2 + l3 * y3");
    auto calculateResultTask = std::make_unique<::sdl::Task>("", calculateResultAction);
    transition->addAction(std::move(calculateResultTask));

    // Set procedure return variable
    auto resultVarRef = std::make_unique<::sdl::VariableReference>(resultVar.get());
    procedure->setReturnVariableReference(std::move(resultVarRef));

    // Add variables to procedure
    procedure->addVariable(std::move(intervalIndexVar));
    procedure->addVariable(std::move(x0Var));
    procedure->addVariable(std::move(x1Var));
    procedure->addVariable(std::move(x2Var));
    procedure->addVariable(std::move(x3Var));
    procedure->addVariable(std::move(y0Var));
    procedure->addVariable(std::move(y1Var));
    procedure->addVariable(std::move(y2Var));
    procedure->addVariable(std::move(y3Var));
    procedure->addVariable(std::move(l0Var));
    procedure->addVariable(std::move(l1Var));
    procedure->addVariable(std::move(l2Var));
    procedure->addVariable(std::move(l3Var));
    procedure->addVariable(std::move(resultVar));

    // Add parameters to procedure
    procedure->addParameter(std::move(indexParameter));
    procedure->addParameter(std::move(valueParameter));
    procedure->addParameter(std::move(rawPointsParameter));
    procedure->addParameter(std::move(calibratedPointsParameter));

    // Add transition to procedure
    procedure->setTransition(std::move(transition));

    // Add procedure to process
    context.sdlProcess()->addProcedure(std::move(procedure));
}

auto SplineCalibratorTranslator::addCallToCalibration(const QString &calibrationName, const QString &targetName,
        const QString &sourceName, const QString &rawPointsVariableName, const QString &calibratedPointsVariableName,
        const bool extrapolate) -> void
{
    const auto calibrationAction = QString("%1 := call %2(intervalIndex, %3, %4, %5)")
                                           .arg(targetName)
                                           .arg(calibrationName)
                                           .arg(sourceName)
                                           .arg(rawPointsVariableName)
                                           .arg(calibratedPointsVariableName);
    auto calibrationTask = std::make_unique<::sdl::Task>("", calibrationAction);

    if (extrapolate) {
        m_sdlTransition->addAction(std::move(calibrationTask));
    } else {
        // Check for left extrapolation
        auto isExtrapolationLeft = std::make_unique<::sdl::Decision>();
        auto isExtrapolationLeftExpression = std::make_unique<::sdl::Expression>("intervalIndex = -1");
        isExtrapolationLeft->setExpression(std::move(isExtrapolationLeftExpression));

        // Set first calibrated value on left extrapolation
        auto isExtrapolationLeftTrueTransition = std::make_unique<::sdl::Transition>();

        const auto isExtrapolationLeftTrueSetAction =
                QString("%1 := %2(0)").arg(targetName).arg(calibratedPointsVariableName);
        auto isExtrapolationLeftTrueTask = std::make_unique<::sdl::Task>("", isExtrapolationLeftTrueSetAction);
        isExtrapolationLeftTrueTransition->addAction(std::move(isExtrapolationLeftTrueTask));

        auto isExtrapolationLeftTrue = std::make_unique<::sdl::Answer>();
        isExtrapolationLeftTrue->setLiteral(::sdl::VariableLiteral("True"));
        isExtrapolationLeftTrue->setTransition(std::move(isExtrapolationLeftTrueTransition));
        isExtrapolationLeft->addAnswer(std::move(isExtrapolationLeftTrue));

        auto isExtrapolationLeftFalseTransition = std::make_unique<::sdl::Transition>();

        // Check for right extrapolation
        auto isExtrapolationRight = std::make_unique<::sdl::Decision>();
        auto isExtrapolationRightExpression =
                std::make_unique<::sdl::Expression>(QString("intervalIndex = length(%1)").arg(rawPointsVariableName));
        isExtrapolationRight->setExpression(std::move(isExtrapolationRightExpression));

        // Set last calibrated value of right extrapolation
        auto isExtrapolationRightTrueTransition = std::make_unique<::sdl::Transition>();

        const auto isExtrapolationRightTrueSetAction =
                QString("%1 := %2(length(%2) - 1)").arg(targetName).arg(calibratedPointsVariableName);
        auto isExtrapolationRightTrueTask = std::make_unique<::sdl::Task>("", isExtrapolationRightTrueSetAction);
        isExtrapolationRightTrueTransition->addAction(std::move(isExtrapolationRightTrueTask));

        auto isExtrapolationRightTrue = std::make_unique<::sdl::Answer>();
        isExtrapolationRightTrue->setLiteral(::sdl::VariableLiteral("True"));
        isExtrapolationRightTrue->setTransition(std::move(isExtrapolationRightTrueTransition));
        isExtrapolationRight->addAnswer(std::move(isExtrapolationRightTrue));

        // Do calibration
        auto isExtrapolationRightFalseTransition = std::make_unique<::sdl::Transition>();

        isExtrapolationRightFalseTransition->addAction(std::move(calibrationTask));

        auto isExtrapolationRightFalse = std::make_unique<::sdl::Answer>();
        isExtrapolationRightFalse->setLiteral(::sdl::VariableLiteral("False"));
        isExtrapolationRightFalse->setTransition(std::move(isExtrapolationRightFalseTransition));
        isExtrapolationRight->addAnswer(std::move(isExtrapolationRightFalse));

        isExtrapolationLeftFalseTransition->addAction(std::move(isExtrapolationRight));

        auto isExtrapolationLeftFalse = std::make_unique<::sdl::Answer>();
        isExtrapolationLeftFalse->setLiteral(::sdl::VariableLiteral("False"));
        isExtrapolationLeftFalse->setTransition(std::move(isExtrapolationLeftFalseTransition));
        isExtrapolationLeft->addAnswer(std::move(isExtrapolationLeftFalse));

        m_sdlTransition->addAction(std::move(isExtrapolationLeft));
    }
}

auto SplineCalibratorTranslator::addCallToFindInterval(const QString &sourceName, const QString rawPointsVariableName)
        -> void
{
    const auto callFindIntervalAction = QString("intervalIndex := call %1(%2, %3)")
                                                .arg(m_findIntervalProcedureName)
                                                .arg(sourceName)
                                                .arg(rawPointsVariableName);
    auto callFindIntervalTask = std::make_unique<::sdl::Task>("", callFindIntervalAction);
    m_sdlTransition->addAction(std::move(callFindIntervalTask));
}

auto SplineCalibratorTranslator::addValueEqualRawCheck(::sdl::Transition *transition) -> void
{
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
}

auto SplineCalibratorTranslator::handleLeftExtrapolation(const uint8_t newIndex, ::sdl::Transition *transition) -> void
{
    auto leftExtrapolationDecision = std::make_unique<::sdl::Decision>();
    auto leftExtrapolationDecisionExpression = std::make_unique<::sdl::Expression>("intervalIndex = -1");
    leftExtrapolationDecision->setExpression(std::move(leftExtrapolationDecisionExpression));

    auto leftExtrapolationDecisionTrueTransition = std::make_unique<::sdl::Transition>();

    const auto leftExtrapolationDecisionTrueAction = QString("intervalIndex := %1").arg(newIndex);
    auto leftExtrapolationDecisionTrueTask = std::make_unique<::sdl::Task>("", leftExtrapolationDecisionTrueAction);
    leftExtrapolationDecisionTrueTransition->addAction(std::move(leftExtrapolationDecisionTrueTask));

    auto leftExtrapolationDecisionTrue = std::make_unique<::sdl::Answer>();
    leftExtrapolationDecisionTrue->setLiteral(::sdl::VariableLiteral("True"));
    leftExtrapolationDecisionTrue->setTransition(std::move(leftExtrapolationDecisionTrueTransition));
    leftExtrapolationDecision->addAnswer(std::move(leftExtrapolationDecisionTrue));

    auto leftExtrapolationDecisionFalseTransition = std::make_unique<::sdl::Transition>();
    auto leftExtrapolationDecisionFalse = std::make_unique<::sdl::Answer>();
    leftExtrapolationDecisionFalse->setLiteral(::sdl::VariableLiteral("False"));
    leftExtrapolationDecisionFalse->setTransition(std::move(leftExtrapolationDecisionFalseTransition));
    leftExtrapolationDecision->addAnswer(std::move(leftExtrapolationDecisionFalse));

    transition->addAction(std::move(leftExtrapolationDecision));
}

auto SplineCalibratorTranslator::handleRightExtrapolation(const QString &newIndex, ::sdl::Transition *transition)
        -> void
{
    auto rightExtrapolationDecision = std::make_unique<::sdl::Decision>();
    auto rightExtrapolationDecisionExpression =
            std::make_unique<::sdl::Expression>("intervalIndex = length(rawPoints)");
    rightExtrapolationDecision->setExpression(std::move(rightExtrapolationDecisionExpression));

    auto rightExtrapolationDecisionTrueTransition = std::make_unique<::sdl::Transition>();

    const auto rightExtrapolationDecisionTrueAction = QString("intervalIndex := %1").arg(newIndex);
    auto rightExtrapolationDecisionTrueTask = std::make_unique<::sdl::Task>("", rightExtrapolationDecisionTrueAction);
    rightExtrapolationDecisionTrueTransition->addAction(std::move(rightExtrapolationDecisionTrueTask));

    auto rightExtrapolationDecisionTrue = std::make_unique<::sdl::Answer>();
    rightExtrapolationDecisionTrue->setLiteral(::sdl::VariableLiteral("True"));
    rightExtrapolationDecisionTrue->setTransition(std::move(rightExtrapolationDecisionTrueTransition));
    rightExtrapolationDecision->addAnswer(std::move(rightExtrapolationDecisionTrue));

    auto rightExtrapolationDecisionFalseTransition = std::make_unique<::sdl::Transition>();
    auto rightExtrapolationDecisionFalse = std::make_unique<::sdl::Answer>();
    rightExtrapolationDecisionFalse->setLiteral(::sdl::VariableLiteral("False"));
    rightExtrapolationDecisionFalse->setTransition(std::move(rightExtrapolationDecisionFalseTransition));
    rightExtrapolationDecision->addAnswer(std::move(rightExtrapolationDecisionFalse));

    transition->addAction(std::move(rightExtrapolationDecision));
}

auto SplineCalibratorTranslator::handleFirstLeftInterval(::sdl::Transition *transition) -> void
{
    auto lastRightIntervalDecision = std::make_unique<::sdl::Decision>();
    auto lastRightIntervalDecisionExpression = std::make_unique<::sdl::Expression>("intervalIndex = 1");
    lastRightIntervalDecision->setExpression(std::move(lastRightIntervalDecisionExpression));

    auto lastRightIntervalDecisionTrueTransition = std::make_unique<::sdl::Transition>();

    const auto lastRightIntervalDecisionTrueAction = QString("intervalIndex := 2");
    auto lastRightIntervalDecisionTrueTask = std::make_unique<::sdl::Task>("", lastRightIntervalDecisionTrueAction);
    lastRightIntervalDecisionTrueTransition->addAction(std::move(lastRightIntervalDecisionTrueTask));

    auto lastRightIntervalDecisionTrue = std::make_unique<::sdl::Answer>();
    lastRightIntervalDecisionTrue->setLiteral(::sdl::VariableLiteral("True"));
    lastRightIntervalDecisionTrue->setTransition(std::move(lastRightIntervalDecisionTrueTransition));
    lastRightIntervalDecision->addAnswer(std::move(lastRightIntervalDecisionTrue));

    auto lastRightIntervalDecisionFalseTransition = std::make_unique<::sdl::Transition>();
    auto lastRightIntervalDecisionFalse = std::make_unique<::sdl::Answer>();
    lastRightIntervalDecisionFalse->setLiteral(::sdl::VariableLiteral("False"));
    lastRightIntervalDecisionFalse->setTransition(std::move(lastRightIntervalDecisionFalseTransition));
    lastRightIntervalDecision->addAnswer(std::move(lastRightIntervalDecisionFalse));

    transition->addAction(std::move(lastRightIntervalDecision));
}

auto SplineCalibratorTranslator::handleLastRightInterval(::sdl::Transition *transition) -> void
{
    auto lastRightIntervalDecision = std::make_unique<::sdl::Decision>();
    auto lastRightIntervalDecisionExpression =
            std::make_unique<::sdl::Expression>("intervalIndex = length(rawPoints) - 1");
    lastRightIntervalDecision->setExpression(std::move(lastRightIntervalDecisionExpression));

    auto lastRightIntervalDecisionTrueTransition = std::make_unique<::sdl::Transition>();

    const auto lastRightIntervalDecisionTrueAction = QString("intervalIndex := length(rawPoints) - 2");
    auto lastRightIntervalDecisionTrueTask = std::make_unique<::sdl::Task>("", lastRightIntervalDecisionTrueAction);
    lastRightIntervalDecisionTrueTransition->addAction(std::move(lastRightIntervalDecisionTrueTask));

    auto lastRightIntervalDecisionTrue = std::make_unique<::sdl::Answer>();
    lastRightIntervalDecisionTrue->setLiteral(::sdl::VariableLiteral("True"));
    lastRightIntervalDecisionTrue->setTransition(std::move(lastRightIntervalDecisionTrueTransition));
    lastRightIntervalDecision->addAnswer(std::move(lastRightIntervalDecisionTrue));

    auto lastRightIntervalDecisionFalseTransition = std::make_unique<::sdl::Transition>();
    auto lastRightIntervalDecisionFalse = std::make_unique<::sdl::Answer>();
    lastRightIntervalDecisionFalse->setLiteral(::sdl::VariableLiteral("False"));
    lastRightIntervalDecisionFalse->setTransition(std::move(lastRightIntervalDecisionFalseTransition));
    lastRightIntervalDecision->addAnswer(std::move(lastRightIntervalDecisionFalse));

    transition->addAction(std::move(lastRightIntervalDecision));
}

auto SplineCalibratorTranslator::getSplineOrder(const seds::model::SplineCalibrator &splineCalibrator) -> uint8_t
{
    const auto &splinePoints = splineCalibrator.splinePoints();

    if (splinePoints.empty()) {
        throw TranslationException("Encountered spline calibrator without points");
    }

    const uint8_t splineOrder = splinePoints.begin()->order();
    for (const auto &splinePoint : splinePoints) {
        if (splinePoint.order() != splineOrder) {
            throw TranslationException(
                    "Calibration splines with varying point orders are not supported to avoid numerical issues");
        }
    }

    switch (splineOrder) {
    case 1:
        if (splinePoints.size() < 2) {
            throw TranslationException("Linear calibration requires at least 2 points");
        }
        break;
    case 2:
        if (splinePoints.size() < 3) {
            throw TranslationException("Square calibration requires at least 3 points");
        }
        break;
    case 3:
        if (splinePoints.size() < 4) {
            throw TranslationException("Cubic calibration requires at least 4 points");
        }
        break;
    }

    return splineOrder;
}

} // namespace conversion::sdl::translator
