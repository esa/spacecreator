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

#include "outport.h"

namespace simulink::model {

const common::String &Outport::initialOutput() const
{
    return m_initialOutput;
}

void Outport::setInitialOutput(common::String initialOutput)
{
    m_initialOutput = std::move(initialOutput);
}

const common::String &Outport::inputSignalNames() const
{
    return m_inputSignalNames;
}

void Outport::setInputSignalNames(common::String inputSignalNames)
{
    m_inputSignalNames = std::move(inputSignalNames);
}

const common::String &Outport::mustResolveToSignalObject() const
{
    return m_mustResolveToSignalObject;
}

void Outport::setMustResolveToSignalObject(common::String mustResolveToSignalObject)
{
    m_mustResolveToSignalObject = std::move(mustResolveToSignalObject);
}

const common::String &Outport::outputWhenDisabled() const
{
    return m_outputWhenDisabled;
}

void Outport::setOutputWhenDisabled(common::String outputWhenDisabled)
{
    m_outputWhenDisabled = std::move(outputWhenDisabled);
}

const common::String &Outport::signalName() const
{
    return m_signalName;
}

void Outport::setSignalName(common::String signalName)
{
    m_signalName = std::move(signalName);
}

const common::String &Outport::signalObject() const
{
    return m_signalObject;
}

void Outport::setSignalObject(common::String signalObject)
{
    m_signalObject = std::move(signalObject);
}

const common::String &Outport::storageClass() const
{
    return m_storageClass;
}

void Outport::setStorageClass(common::String storageClass)
{
    m_storageClass = std::move(storageClass);
}

const common::String &Outport::varSizeSig() const
{
    return m_varSizeSig;
}

void Outport::setVarSizeSig(common::String varSizeSig)
{
    m_varSizeSig = std::move(varSizeSig);
}

} // namespace simulink::model
