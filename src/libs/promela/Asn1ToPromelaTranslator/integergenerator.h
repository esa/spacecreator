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

#include "integersubset.h"

#include <list>
#include <optional>

namespace promela::translator {
/**
 * @brief Generator that generates all possible integer values from integer subset.
 */
class IntegerGenerator
{
public:
    /**
     * @brief Constructor.
     *
     * @param subset the description of subset of integers to generate
     */
    IntegerGenerator(const IntegerSubset &subset);

    /**
     * @brief Reset the generator to initial state.
     */
    void reset();
    /**
     * @brief Get the current integer and generate the next one.
     *
     * @return Current integer value.
     */
    int next();
    /**
     * @brief Check if generator is able to generate next value.
     *
     * @return true if generator can generate next value, otherwise false
     */
    bool has_next();

private:
    const IntegerSubset &m_subset;
    IntegerSubset::RangeList::const_iterator m_iter;
    std::optional<IntegerSubset::IntegerType> m_next;
};
}
