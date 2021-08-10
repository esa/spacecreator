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

#include "components/activities/polynomial.h"
#include "components/activities/splinecalibrator.h"
#include "types/field.h"
#include "types/ranges/derivedtyperange.h"

#include <optional>
#include <variant>

namespace seds::model {

class ExternalField : public Field
{
public:
    using Calibrator = std::variant<std::monostate, SplineCalibrator, Polynomial>;

public:
    ExternalField() = default;
    virtual ~ExternalField() = 0;
    ExternalField(ExternalField &&) = default;
    ExternalField &operator=(ExternalField &&) = default;

public:
    auto calibrator() const -> const std::optional<Calibrator> &;
    auto setCalibrator(Calibrator calibrator) -> void;

    auto nominalRange() const -> const std::optional<DerivedTypeRange> &;
    auto setNominalRange(DerivedTypeRange nominalRange) -> void;

    auto safeRange() const -> const std::optional<DerivedTypeRange> &;
    auto setsafeRange(DerivedTypeRange safeRange) -> void;

private:
    std::optional<Calibrator> m_calibrator;
    std::optional<DerivedTypeRange> m_nominalRange;
    std::optional<DerivedTypeRange> m_safeRange;
};

} // namespace seds::model
