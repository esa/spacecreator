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

#include "context.h"
#include "dimensiontranslator.h"

#include <simulink/SimulinkModel/common/port.h>

namespace conversion::asn1::translator::simulink {

/**
 * @brief   When port (inport or outport) has "Port Dimensions" parameter different from -1 or greater than 1 or it is a
 * vector [n m], then new ASN1 data type has to be created.
 */
class PortDataTypeTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context     Current translation context
     */
    explicit PortDataTypeTranslator(Context &context);
    /**
     * @brief   Destructor
     */
    ~PortDataTypeTranslator() = default;
    /**
     * @brief   Deleted copy constructor
     */
    PortDataTypeTranslator(const PortDataTypeTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    PortDataTypeTranslator(PortDataTypeTranslator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    PortDataTypeTranslator &operator=(const PortDataTypeTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    PortDataTypeTranslator &operator=(PortDataTypeTranslator &&) = delete;

public:
    /**
     * @brief   Translation port data type to ASN1, if necessary
     *
     * @param   port    port (inport or outport) to translation
     */
    auto translate(const ::simulink::model::Port &port) -> void;

private:
    Context &m_context;
    DimensionTranslator m_dimensionTranslator;
};

} // namespace conversion::asn1::translator::simulink
