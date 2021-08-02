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

#include "components/activities/calibration.h"

namespace seds::model {

const Calibration::Calibrator&
Calibration::calibrator() const
{
    return m_calibrator;
}

void
Calibration::setCalibrator(Calibration::Calibrator calibrator)
{
    m_calibrator = std::move(calibrator);
}

const VariableRef&
Calibration::outputVariableRef() const
{
    return m_outputVariableRef;
}

void
Calibration::setOutputVariableRef(VariableRef outputVariableRef)
{
    m_outputVariableRef = std::move(outputVariableRef);
}

const VariableRef&
Calibration::inputVariableRef() const
{
    return m_inputVariableRef;
}

void
Calibration::setInputVariableRef(VariableRef inputVariableRef)
{
    m_inputVariableRef = std::move(inputVariableRef);
}

} // namespace seds::model
