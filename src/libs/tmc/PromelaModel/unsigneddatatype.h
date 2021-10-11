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

namespace tmc::promela::model {
/**
 * @brief Representation of unsigned type from promela.
 *
 * The unsigned type from promela is a bit-field, with available sizes from 1 to 32.
 */
class UnsignedDataType final
{
public:
    /**
     * @brief Constructor.
     *
     * Construct unsigned data type with given width
     *
     * @param size the size of unsigned data type
     */
    constexpr UnsignedDataType(unsigned int size) noexcept
        : m_size(size)
    {
    }

    /**
     * @brief Getter for size
     *
     * @return size of unsigned data type
     */
    constexpr unsigned int size() const noexcept { return m_size; }

private:
    unsigned int m_size;
};
}
