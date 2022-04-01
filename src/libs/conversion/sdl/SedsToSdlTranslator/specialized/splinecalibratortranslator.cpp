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
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/values.h>
#include <conversion/asn1/SedsToAsn1Translator/specialized/rangetranslatorvisitor.h>
#include <conversion/asn1/SedsToAsn1Translator/translator.h>
#include <iostream>
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
    buildSplineCalibratorBoilerplate(m_context);
    buildSplineCalibratorVariables(splineCalibrator);

    const auto targetName =
            StatementTranslatorVisitor::translateVariableReference(m_calibration.outputVariableRef().value().value());
    const auto sourceName =
            StatementTranslatorVisitor::translateVariableReference(m_calibration.inputVariableRef().value().value());

    const auto action = QString("%1 := %2").arg(targetName).arg(sourceName);

    auto sdlTask = std::make_unique<::sdl::Task>("", action);
    DescriptionTranslator::translate(m_calibration, sdlTask.get());

    m_sdlTransition->addAction(std::move(sdlTask));
}

auto SplineCalibratorTranslator::buildSplineCalibratorVariables(const seds::model::SplineCalibrator &splineCalibrator)
        -> void
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

    const auto rawPointsVariableName = m_context.uniqueRawSplinePointsVariableName();
    buildSplinePointsVariable(rawPointsVariableName, rawValues, startTransition);

    const auto calibratedPointsVariableName = m_context.uniqueCalibratedSplinePointsVariableName();
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

    // Add transition to procedure
    initProc->setTransition(std::move(initTransition));

    // Add procedure to process
    m_context.sdlProcess()->addProcedure(std::move(initProc));

    // Add call to Init function at the state machine start
    auto initCall = std::make_unique<::sdl::ProcedureCall>("");
    initCall->setProcedure(initProc.get());
    startTransition->addAction(std::move(initCall));
}

auto SplineCalibratorTranslator::buildSplineCalibratorBoilerplate(StatementTranslatorVisitor::StatementContext &context)
        -> void
{
    static bool alreadyCreated = false;

    if (alreadyCreated) {
        return;
    }

    createAsn1Types(context);

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
    splinePointType->setEndianness(Asn1Acn::Types::Endianness::little);
    auto splinePointTypeConstraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::RealValue>::create(
            { std::numeric_limits<double>::min(), std::numeric_limits<double>::max() });
    splinePointType->constraints().append(std::move(splinePointTypeConstraint));

    // Create type for array of spline points values
    auto splinePointsArrayType = std::make_unique<Asn1Acn::Types::SequenceOf>("SplinePointsArray");
    splinePointsArrayType->setItemsType(splinePointType->clone());
    auto splinePointsArrayTypeInnerConstraint =
            Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create({ 0, 20 });
    auto splinePointsArrayTypeConstraint =
            std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::IntegerValue>>();
    splinePointsArrayTypeConstraint->setInnerConstraints(std::move(splinePointsArrayTypeInnerConstraint));
    splinePointsArrayType->constraints().append(std::move(splinePointsArrayTypeConstraint));

    auto splinePointTypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(
            "SplinePointValue", "SplinePointValue", Asn1Acn::SourceLocation(), std::move(splinePointType));
    asn1Definitions->addType(std::move(splinePointTypeAssignment));

    auto splinePointsArrayTypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(
            "SplinePointsArray", "SplinePointsArray", Asn1Acn::SourceLocation(), std::move(splinePointsArrayType));
    asn1Definitions->addType(std::move(splinePointsArrayTypeAssignment));
}

auto SplineCalibratorTranslator::buildFindLinearIntervalProcedure(StatementTranslatorVisitor::StatementContext &context)
        -> ::sdl::Procedure *
{
    // Create procedure
    const QString procedureName("FindInterval");
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);
    auto procedurePtr = procedure.get();

    // Create transition
    auto transition = std::make_unique<::sdl::Transition>();

    // Create parameters
    auto valueParameter = std::make_unique<::sdl::ProcedureParameter>("value", "SplinePointValue", "in");
    procedure->addParameter(std::move(valueParameter));
    auto startParameter = std::make_unique<::sdl::ProcedureParameter>("intervalStart", "SplinePointValue", "in/out");
    procedure->addParameter(std::move(startParameter));
    auto endParameter = std::make_unique<::sdl::ProcedureParameter>("intervalEnd", "SplinePointValue", "in/out");
    procedure->addParameter(std::move(endParameter));

    // Add transition to procedure
    procedure->setTransition(std::move(transition));

    // Add procedure to process
    context.sdlProcess()->addProcedure(std::move(procedure));

    return procedurePtr;
}

auto SplineCalibratorTranslator::buildLinearCalibrationProcedure(StatementTranslatorVisitor::StatementContext &context)
        -> void
{
    // Create procedure
    const QString procedureName("LinearCalibration");
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);

    // Create transition
    auto transition = std::make_unique<::sdl::Transition>();

    // Create helper procedures
    auto findIntervalProc = buildFindLinearIntervalProcedure(context);

    // Add call to FindInterval
    auto findIntervalCall = std::make_unique<::sdl::ProcedureCall>("");
    findIntervalCall->setProcedure(findIntervalProc);
    transition->addAction(std::move(findIntervalCall));

    // Create parameters
    auto valueParameter = std::make_unique<::sdl::ProcedureParameter>("value", "SplinePointValue", "in");
    procedure->addParameter(std::move(valueParameter));

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
