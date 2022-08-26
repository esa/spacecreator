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

#include "inport.h"

namespace simulink::model {

const common::String &Inport::interpolate() const
{
    return m_interpolate;
}

void Inport::setInterpolate(common::String interpolate)
{
    m_interpolate = std::move(interpolate);
}

const common::String &Inport::latchByDelayingOutsideSignal() const
{
    return m_latchByDelayingOutsideSignal;
}

void Inport::setLatchByDelayingOutsideSignal(common::String latchByDelayingOutsideSignal)
{
    m_latchByDelayingOutsideSignal = std::move(latchByDelayingOutsideSignal);
}

const common::String &Inport::latchInputForFeedbackSignals() const
{
    return m_latchInputForFeedbackSignals;
}

void Inport::setLatchInputForFeedbackSignals(common::String latchInputForFeedbackSignals)
{
    m_latchInputForFeedbackSignals = std::move(latchInputForFeedbackSignals);
}

const common::String &Inport::outputSignalNames() const
{
    return m_outputSignalNames;
}

void Inport::setOutputSignalNames(common::String outputSignalNames)
{
    m_outputSignalNames = std::move(outputSignalNames);
}

} // namespace simulink::model
