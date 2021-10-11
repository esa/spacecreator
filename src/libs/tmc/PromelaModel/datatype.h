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

namespace tmc::promela::model {
/**
 * @brief  Representation of data type in promela.
 */
class DataType final
{
public:
    /**
     * @brief variant to represent data type.
     *
     * The data type can be 'unsigned', basic type, reference to user defined type or array
     */
    using Type = std::variant<UnsignedDataType, UtypeRef, BasicType, ArrayType>;

    /**
     * @brief Constructor directly from variant value.
     *
     * @param value data type
     */
    DataType(Type type);

    /**
     * @brief Getter for variant DataType.
     *
     * @return datatype variant
     */
    const Type &getType() const noexcept;

    /**
     * @brief Checks if data type is UnsignedDataType
     *
     * @returns true if data type is UnsignedDataType, otherwise false
     */
    bool isUnsigned() const noexcept;
    /**
     * @brief Checks if data type is UtypeRef
     *
     * @returns true if data type is UtypeRef, otherwise false
     */
    bool isReference() const noexcept;
    /**
     * @brief Checks if data type is BasicType
     *
     * @returns true if data type is BasicType, otherwise false
     */
    bool isBasicType() const noexcept;
    /**
     * @brief Checks if data type is ArrayType
     *
     * @returns true if data type is ArrayType, otherwise false
     */
    bool isArrayType() const noexcept;

    /**
     * @brief Getter for UnsignedDataType
     *
     * @returns reference to UnsginedDataType
     * @throws std::bad_variant_access data type does not contain UnsignedDataType
     */
    const UnsignedDataType &getUnsigned() const;
    /**
     * @brief Getter for UtypeRef
     *
     * @returns reference to UtypeRef
     * @throws std::bad_variant_access data type does not contain UtypeRef
     */
    const UtypeRef &getReference() const;
    /**
     * @brief Getter for BasicType
     *
     * @returns reference to BasicType
     * @throws std::bad_variant_access data type does not contain BasicType
     */
    const BasicType &getBasicType() const;
    /**
     * @brief Getter for ArrayType
     *
     * @returns reference to ArrayType
     * @throws std::bad_variant_access data type does not contain ArrayType
     */
    const ArrayType &getArrayType() const;

private:
    Type m_type;
};
}
