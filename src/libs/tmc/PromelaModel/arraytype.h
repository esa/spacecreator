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

#include <cstddef>

#include <variant>

#include "basictypes.h"
#include "utyperef.h"

namespace tmc::promelamodel
{
	/**
	 * @brief representation of array type from promela.
	 */
	class ArrayType final
	{
	public:
		/**
		 * @brief The base type of array, which can be only BasicType or reference to Utype
		 */
		using Value = std::variant<BasicType, UtypeRef>;

                /**
                 * @brief Constructor directly from @link{Value}
				 *
				 * @param size  Size of the array
				 * @param type  Base type of the array
                 */
                ArrayType(size_t size, Value type);
                /**
                 * @brief Constructor directly from @link{BasicType}
				 *
				 * @param size  Size of the array
				 * @param type  Base type of the array
                 */
		ArrayType(size_t size, BasicType type);
                /**
                 * @brief Constructor directly from @link{UtypeRef}
				 *
				 * @param size  Size of the array
				 * @param type  Base type of the array
                 */
		ArrayType(size_t size, UtypeRef type);

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
		const Value& getType() const noexcept;

	private:
		size_t m_size;
		Value m_type;
	};
}
