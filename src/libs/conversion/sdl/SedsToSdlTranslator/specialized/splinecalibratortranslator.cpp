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
#include <iostream>

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

    // Create a procedure that initializes sequence values
    auto initTransition = std::make_unique<::sdl::Transition>();

    QString initAction;
    QTextStream initActionStream(&initAction, QIODevice::WriteOnly);
    initActionStream.setRealNumberNotation(QTextStream::FixedNotation);

    initActionStream << variableName << " := {";

    initActionStream << values[0];
    for (std::size_t i = 1; i < values.size(); ++i) {
        initActionStream << ", " << values[i];
    }
    initActionStream << "}";

    auto initTask = std::make_unique<::sdl::Task>("", initAction);
    initTransition->addAction(std::move(initTask));

    const auto initProcName = QString("Init%1").arg(variableName);
    auto initProc = std::make_unique<::sdl::Procedure>(initProcName);
    initProc->setTransition(std::move(initTransition));

    // Add call to the processes start transition
    auto initCall = std::make_unique<::sdl::ProcedureCall>("");
    initCall->setProcedure(initProc.get());
    startTransition->addAction(std::move(initCall));

    m_context.sdlProcess()->addProcedure(std::move(initProc));
}

auto SplineCalibratorTranslator::buildSplineCalibratorBoilerplate(StatementTranslatorVisitor::StatementContext &context)
        -> void
{
    static bool alreadyCreated = false;

    if (alreadyCreated) {
        return;
    }

    buildLinearCalibrationProcedure(context);
    buildSquareCalibrationProcedure(context);
    buildCubicCalibrationProcedure(context);

    alreadyCreated = true;
}

auto SplineCalibratorTranslator::buildFindLinearIntervalProcedure(StatementTranslatorVisitor::StatementContext &context)
        -> ::sdl::Procedure *
{
    const QString procedureName("FindInterval");
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);
    auto procedurePtr = procedure.get();

    auto transition = std::make_unique<::sdl::Transition>();
    procedure->setTransition(std::move(transition));

    context.sdlProcess()->addProcedure(std::move(procedure));

    return procedurePtr;
}

auto SplineCalibratorTranslator::buildLinearCalibrationProcedure(StatementTranslatorVisitor::StatementContext &context)
        -> void
{
    auto findIntervalProc = buildFindLinearIntervalProcedure(context);

    auto transition = std::make_unique<::sdl::Transition>();

    auto findIntervalCall = std::make_unique<::sdl::ProcedureCall>("");
    findIntervalCall->setProcedure(findIntervalProc);
    transition->addAction(std::move(findIntervalCall));

    const QString procedureName("LinearCalibration");
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);
    procedure->setTransition(std::move(transition));

    context.sdlProcess()->addProcedure(std::move(procedure));
}

auto SplineCalibratorTranslator::buildSquareCalibrationProcedure(StatementTranslatorVisitor::StatementContext &context)
        -> void
{
    auto transition = std::make_unique<::sdl::Transition>();

    const QString procedureName("SquareCalibration");
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);
    procedure->setTransition(std::move(transition));

    context.sdlProcess()->addProcedure(std::move(procedure));
}

auto SplineCalibratorTranslator::buildCubicCalibrationProcedure(StatementTranslatorVisitor::StatementContext &context)
        -> void
{
    auto transition = std::make_unique<::sdl::Transition>();

    const QString procedureName("CubicCalibration");
    auto procedure = std::make_unique<::sdl::Procedure>(procedureName);
    procedure->setTransition(std::move(transition));

    context.sdlProcess()->addProcedure(std::move(procedure));
}

} // namespace conversion::sdl::translator
