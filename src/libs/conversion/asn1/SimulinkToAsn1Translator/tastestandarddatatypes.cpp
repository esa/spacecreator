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

#include "tastestandarddatatypes.h"

namespace conversion::asn1::translator {

// clang-format off
const QStringList TasteStandardDataTypes::m_tasteStandardDataTypes = 
{
    "T-Null-Record",
    "T-Boolean",
    "T-Int8",
    "T-UInt8",
    "T-Int32",
    "T-UInt32",
};
// clang-format on

bool TasteStandardDataTypes::isTasteType(const QString &typeName)
{
    return m_tasteStandardDataTypes.contains(typeName, Qt::CaseSensitive);
}

} // namespace conversion::asn1::translator
