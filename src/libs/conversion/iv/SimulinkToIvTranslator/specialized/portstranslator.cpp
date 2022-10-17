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

#include "portstranslator.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/simulink/matlabstandarddatatypes.h>
#include <conversion/common/simulink/portsdatatypes.h>

using conversion::simulink::MatLabStandardDataTypes;
using conversion::simulink::PortsDataTypes;
using ivm::IVInterface;
using shared::BasicParameter;
using shared::InterfaceParameter;
using simulink::model::Port;
using simulink::model::SimulinkModel;
using simulink::model::VectorDimensions;

namespace conversion::iv::translator::simulink {

PortsTranslator::PortsTranslator(const SimulinkModel &simulinkModel, IVInterface &ivInterface)
    : m_simulinkModel(simulinkModel)
    , m_ivInterface(ivInterface)
{
}

void PortsTranslator::translatePorts()
{
    for (const auto &inport : m_simulinkModel.inports()) {
        translatePort(inport, InterfaceParameter::Direction::IN);
    }

    for (const auto &outport : m_simulinkModel.outports()) {
        translatePort(outport, InterfaceParameter::Direction::OUT);
    }
}

void PortsTranslator::translatePort(const Port &port, InterfaceParameter::Direction direction)
{
    bool isScalar = false;

    if (const auto *scalarDimensionsPtr = std::get_if<int64_t>(&port.portDimensions())) {
        const auto scalarDimensions = *scalarDimensionsPtr;

        if (scalarDimensions == -1 || scalarDimensions == 1) {
            isScalar = true;
        }
    } else if (const auto *vectorDimensionsPtr = std::get_if<VectorDimensions>(&port.portDimensions())) {
        const auto &vectorDimensions = vectorDimensionsPtr->values();

        if (vectorDimensions.size() == 1 && vectorDimensions[0] == 1) {
            isScalar = true;
        }
    }

    const QString &portDataType = port.outDataTypeStr();
    QString interfaceParameterTypeName;

    if (isScalar) {
        if (MatLabStandardDataTypes::isMatLabType(portDataType)) {
            interfaceParameterTypeName = MatLabStandardDataTypes::buildMatLabDataTypeName(portDataType);
        } else {
            interfaceParameterTypeName = Escaper::escapeAsn1TypeName(portDataType);
        }
    } else {
        interfaceParameterTypeName = PortsDataTypes::buildMultiDimensionalPortDataTypeName(port.name());
    }

    const auto ivParameter = InterfaceParameter(port.name(), BasicParameter::Type::Other, interfaceParameterTypeName,
            getEncodingForInterfaceParameter(), direction);

    m_ivInterface.addParam(ivParameter);
}

} // namespace conversion::iv::translator::simulink
