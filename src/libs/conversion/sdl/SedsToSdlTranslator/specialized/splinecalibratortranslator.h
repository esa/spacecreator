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

#pragma once

#include "specialized/statementtranslatorvisitor.h"

#include <seds/SedsModel/components/activities/calibration.h>

namespace conversion::sdl::translator {

class SplineCalibratorTranslator final
{
public:
    SplineCalibratorTranslator(StatementTranslatorVisitor::StatementContext &context,
            const seds::model::Calibration &calibration, ::sdl::Transition *sdlTransition);

public:
    auto translate(const seds::model::SplineCalibrator &splineCalibrator) -> void;

private:
    auto buildSplineCalibratorVariables(const seds::model::SplineCalibrator &splineCalibrator,
            const QString &rawPointsVariableName, const QString &calibratedPointsVariableName) -> void;
    auto buildSplinePointsVariable(
            const QString &variableName, const std::vector<double> &values, ::sdl::Transition *startTransition) -> void;

    static auto buildSplineCalibratorBoilerplate(StatementTranslatorVisitor::StatementContext &context) -> void;
    static auto createAsn1Types(StatementTranslatorVisitor::StatementContext &context) -> void;
    static auto buildLinearCalibrationProcedure(StatementTranslatorVisitor::StatementContext &context) -> void;
    static auto buildFindIntervalProcedure(StatementTranslatorVisitor::StatementContext &context) -> void;
    static auto buildSquareCalibrationProcedure(StatementTranslatorVisitor::StatementContext &context) -> void;
    static auto buildCubicCalibrationProcedure(StatementTranslatorVisitor::StatementContext &context) -> void;
    static auto buildLinearExtrapolationProcedure(StatementTranslatorVisitor::StatementContext &context) -> void;

    auto addCallToCalibration(const QString &calibrationName, const QString &targetName, const QString &sourceName,
            const QString &rawPointsVariableName, const QString &calibratedPointsVariableName, const bool extrapolate)
            -> void;
    auto addCallToFindInterval(const QString &sourceName, const QString rawPointsVariableName) -> void;
    static auto addValueEqualRawCheck(::sdl::Transition *transition) -> void;
    static auto handleLeftExtrapolation(const uint8_t newIndex, ::sdl::Transition *transition) -> void;
    static auto handleRightExtrapolation(const QString &newIndex, ::sdl::Transition *transition) -> void;
    static auto handleFirstLeftInterval(::sdl::Transition *transition) -> void;
    static auto handleLastRightInterval(::sdl::Transition *transition) -> void;

    static auto getSplineOrder(const seds::model::SplineCalibrator &splineCalibrator) -> uint8_t;

private:
    StatementTranslatorVisitor::StatementContext &m_context;
    const seds::model::Calibration &m_calibration;
    ::sdl::Transition *m_sdlTransition;

    static constexpr int m_maxSplinePointsArraySize = 20;
};

} // namespace conversion::sdl::translator
