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

#include "components/activities/splinepoint.h"

#include <set>

namespace seds::model {

class SplineCalibrator final
{
public:
    SplineCalibrator() noexcept;
    SplineCalibrator(SplineCalibrator &&) = default;
    SplineCalibrator &operator=(SplineCalibrator &&) = default;

public:
    auto splinePoints() const -> const std::set<SplinePoint> &;
    auto addSplinePoint(SplinePoint splinePoint) -> void;

    auto extrapolate() const -> bool;
    auto setExtrapolate(bool extrapolate) -> void;

private:
    std::set<SplinePoint> m_splinePoints;
    bool m_extrapolate;
};

} // namespace seds::model
