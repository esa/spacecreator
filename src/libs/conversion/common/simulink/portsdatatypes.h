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

#include <QString>
#include <conversion/common/escaper/escaper.h>

namespace conversion::simulink {

/**
 * @brief  PortsDataTypes class provides various methods for handling ports (inports and outports) data types
 */
class PortsDataTypes final
{
public:
    /**
     * @brief   Deleted constructor
     */
    PortsDataTypes() = delete;

public:
    /**
     * @brief   Method builds proper ASN.1 type name from port data type name
     *
     * @param   typeName    Name of referenced data type
     *
     * @return  It returns built ASN.1 type name
     */
    static inline auto buildMultiDimensionalPortDataTypeName(const QString &portName) -> QString
    {
        return Escaper::escapeAsn1TypeName(portName + getMultiDimensionalPortsSuffix());
    }

    /**
     * @brief  Return suffix for multidimensional data types of ports
     */
    static inline auto getMultiDimensionalPortsSuffix() -> QString { return "-Type"; }
};

} // namespace conversion::simulink
