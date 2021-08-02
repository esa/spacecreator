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
#include "components/activities/statement.h"
#include "components/variableref.h"

#include <variant>

namespace seds::model {

class Calibration final : public Statement
{
public:
    using Calibrator = std::variant<Polynomial, SplineCalibrator>;

public:
    Calibration() = default;
    Calibration(Calibration &&) = default;
    Calibration &operator=(Calibration &&) = default;

public:
    auto calibrator() const -> const Calibrator &;
    auto setCalibrator(Calibrator calibrator) -> void;

    auto outputVariableRef() const -> const VariableRef &;
    auto setOutputVariableRef(VariableRef outputVariableRef) -> void;

    auto inputVariableRef() const -> const VariableRef &;
    auto setInputVariableRef(VariableRef inputVariableRef) -> void;

private:
    Calibrator m_calibrator;
    VariableRef m_outputVariableRef;
    VariableRef m_inputVariableRef;
};

} // namespace seds::model
