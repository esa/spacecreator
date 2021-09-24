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

#pragma once

#include "arraytype.h"
#include "basictypes.h"
#include "unsigneddatatype.h"
#include "utyperef.h"

#include <QString>
#include <variant>

namespace tmc::promelamodel {
class DataType final
{
public:
    using Value = std::variant<UnsignedDataType, UtypeRef, BasicType, ArrayType>;

    DataType(Value value);
    DataType(UnsignedDataType value);
    DataType(UtypeRef value);
    DataType(BasicType value);
    DataType(ArrayType value);

    const Value &getValue() const;

    bool isUnsigned() const;
    bool isReference() const;
    bool isBasicType() const;
    bool isArrayType() const;

    const UnsignedDataType &getUnsigned() const;
    const UtypeRef &getReference() const;
    const BasicType &getBasicType() const;
    const ArrayType &getArrayType() const;

private:
    Value m_value;
};
}
