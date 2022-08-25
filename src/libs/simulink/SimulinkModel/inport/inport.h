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

#include <simulink/SimulinkCommon/basetypesmappings.h>
#include <simulink/SimulinkModel/common/port.h>

namespace simulink::model {

class Inport final : public Port
{
public:
    Inport() = default;
    Inport(Inport &&) = default;
    Inport &operator=(Inport &&) = default;

public:
    auto interpolate() const -> const common::String &;
    auto setInterpolate(common::String interpolate) -> void;

    auto latchByDelayingOutsideSignal() const -> const common::String &;
    auto setLatchByDelayingOutsideSignal(common::String latchByDelayingOutsideSignal) -> void;

    auto latchInputForFeedbackSignals() const -> const common::String &;
    auto setLatchInputForFeedbackSignals(common::String latchInputForFeedbackSignals) -> void;

    auto outputSignalNames() const -> const common::String &;
    auto setOutputSignalNames(common::String outputSignalNames) -> void;

private:
    common::String m_interpolate;
    common::String m_latchByDelayingOutsideSignal;
    common::String m_latchInputForFeedbackSignals;
    common::String m_outputSignalNames;
};

} // namespace simulink::model
