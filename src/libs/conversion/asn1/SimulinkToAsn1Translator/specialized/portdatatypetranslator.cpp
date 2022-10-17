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

#include "portdatatypetranslator.h"

#include "dimensiontranslator.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/simulink/portsdatatypes.h>

using Asn1Acn::SourceLocation;
using Asn1Acn::TypeAssignment;
using conversion::Escaper;
using conversion::simulink::PortsDataTypes;
using simulink::model::Port;
using simulink::model::VectorDimensions;

namespace conversion::asn1::translator::simulink {

PortDataTypeTranslator::PortDataTypeTranslator(Context &context)
    : m_context(context)
    , m_dimensionTranslator(context)
{
}

void PortDataTypeTranslator::translate(const Port &port)
{
    const auto *portDataTypeAsn1Type = m_context.findAsn1Type(port.outDataTypeStr());
    const auto portAsn1TypeName = PortsDataTypes::buildMultiDimensionalPortDataTypeName(port.name());

    if (const auto *scalarDimensionsPtr = std::get_if<int64_t>(&port.portDimensions())) {
        const auto scalarDimensions = *scalarDimensionsPtr;

        if (scalarDimensions == -1 || scalarDimensions == 1) {
            return;
        }

        auto portAsn1SequenceOfType =
                m_dimensionTranslator.translate(portAsn1TypeName, portDataTypeAsn1Type, scalarDimensions);

        m_context.addSimulinkModelAsn1Type(std::move(portAsn1SequenceOfType));

    } else if (const auto *vectorDimensionsPtr = std::get_if<VectorDimensions>(&port.portDimensions())) {
        const auto &vectorDimensions = vectorDimensionsPtr->values();

        if (vectorDimensions.size() == 1 && vectorDimensions[0] == 1)
            return;

        auto portAsn1SequenceOfType = m_dimensionTranslator.translate(portDataTypeAsn1Type, vectorDimensions);

        auto portAsn1TypeAssignment = std::make_unique<TypeAssignment>(
                portAsn1TypeName, portAsn1TypeName, SourceLocation(), std::move(portAsn1SequenceOfType));

        m_context.getSimulinkModelAsn1Definitions()->addType(std::move(portAsn1TypeAssignment));
    }
}

} // namespace conversion::asn1::translator::simulink
