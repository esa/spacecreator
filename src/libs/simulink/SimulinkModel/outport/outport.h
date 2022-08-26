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

class Outport final : public Port
{
public:
    Outport() = default;
    Outport(Outport &&) = default;
    Outport &operator=(Outport &&) = default;

public:
    auto initialOutput() const -> const common::String &;
    auto setInitialOutput(common::String initialOutput) -> void;

    auto inputSignalNames() const -> const common::String &;
    auto setInputSignalNames(common::String inputSignalNames) -> void;

    auto mustResolveToSignalObject() const -> const common::String &;
    auto setMustResolveToSignalObject(common::String mustResolveToSignalObject) -> void;

    auto outputWhenDisabled() const -> const common::String &;
    auto setOutputWhenDisabled(common::String outputWhenDisabled) -> void;

    auto signalName() const -> const common::String &;
    auto setSignalName(common::String signalName) -> void;

    auto signalObject() const -> const common::String &;
    auto setSignalObject(common::String signalObject) -> void;

    auto storageClass() const -> const common::String &;
    auto setStorageClass(common::String storageClass) -> void;

    auto varSizeSig() const -> const common::String &;
    auto setVarSizeSig(common::String varSizeSig) -> void;

private:
    common::String m_initialOutput;
    common::String m_inputSignalNames;
    common::String m_mustResolveToSignalObject;
    common::String m_outputWhenDisabled;
    common::String m_signalName;
    common::String m_signalObject;
    common::String m_storageClass;
    common::String m_varSizeSig;
};

} // namespace simulink::model
