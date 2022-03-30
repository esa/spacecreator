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
    buildSplineCalibratorBoilerplate(splineCalibrator);

    const auto targetName =
            StatementTranslatorVisitor::translateVariableReference(m_calibration.outputVariableRef().value().value());
    const auto action = QString("%1 := %2").arg(targetName, targetName);

    auto sdlTask = std::make_unique<::sdl::Task>("", action);
    DescriptionTranslator::translate(m_calibration, sdlTask.get());

    m_sdlTransition->addAction(std::move(sdlTask));
}

auto SplineCalibratorTranslator::buildSplineCalibratorBoilerplate(const seds::model::SplineCalibrator &splineCalibrator)
        -> void
{
    if (m_context.sdlProcess()->startTransition() == nullptr) {
        auto startTransition = std::make_unique<::sdl::Transition>();
        m_context.sdlProcess()->setStartTransition(std::move(startTransition));
    }

    auto startTransition = m_context.sdlProcess()->startTransition();

    const auto rawPointsVariableName = m_context.uniqueRawSplinePointsVariableName();
    buildSplinePointsBoilerplate(rawPointsVariableName, startTransition);

    const auto calibratedPointsVariableName = m_context.uniqueCalibratedSplinePointsVariableName();
    buildSplinePointsBoilerplate(calibratedPointsVariableName, startTransition);
}

auto SplineCalibratorTranslator::buildSplinePointsBoilerplate(
        const QString &variableName, ::sdl::Transition *startTransition) -> void
{
    auto variable = std::make_unique<::sdl::VariableDeclaration>(variableName, "SplinePointsArray");
    m_context.sdlProcess()->addVariable(std::move(variable));

    auto initTransition = std::make_unique<::sdl::Transition>();

    const auto initProcName = QString("Init%1").arg(variableName);
    auto initProc = std::make_unique<::sdl::Procedure>(initProcName);
    initProc->setTransition(std::move(initTransition));

    m_context.sdlProcess()->addProcedure(std::move(initProc));

    const auto initAction = QString("call %1").arg(initProcName);
    auto initTask = std::make_unique<::sdl::Task>("", initAction);
    startTransition->addAction(std::move(initTask));
}

} // namespace conversion::sdl::translator
