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

#include "basictypes.h"
#include "mtyperef.h"
#include "utyperef.h"

#include <cstddef>
#include <variant>

namespace promela::model {
/**
 * @brief representation of array type from promela.
 */
class ArrayType final
{
public:
    /**
     * @brief The base type of array, which can be only BasicType or reference to Utype or Mtype
     */
    using Type = std::variant<BasicType, UtypeRef, MtypeRef>;

    /**
     * @brief Constructor directly from @link{Value}
     *
     * @param size  Size of the array
     * @param type  Base type of the array
     */
    ArrayType(size_t size, Type type);

    /**
     * @brief Constructor with size which is named constant
     * The size of array is defined externally, the name of constant is used.
     *
     * @param constantName Name of the preprocessor constant which determines the size
     * @param type  Base type of the array
     */
    ArrayType(QString constantName, Type type);

    /**
     * @brief Getter for constant name, which determines the size
     *
     * @returns reference to constant name
     * @throws std::bad_variant_access if constant is not set
     */
    const QString &getSizeConstantName() const;
    /**
     * @brief Check if size is defined externally using constant name.
     *
     * @returns true if the size is defined by constant name, false if the size is defined here.
     */
    bool hasExternallyDefinedSize() const;
    /**
     * @brief Getter for array size
     *
     * @returns Array size
     */
    size_t getSize() const noexcept;
    /**
     * @brief Getter for base type of array.
     *
     * @return Base type of array
     */
    const Type &getType() const noexcept;

private:
    std::variant<size_t, QString> m_size;
    Type m_type;
};
}
