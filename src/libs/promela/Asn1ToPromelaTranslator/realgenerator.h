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

#include "realsubset.h"

#include <list>
#include <optional>

namespace promela::translator {
/**
 * @brief Generator that generates all possible real values from real subset.
 */
class RealGenerator
{
public:
    /**
     * @brief Constructor.
     *
     * @param subset the description of subset of reals to generate
     * @param delta the difference between consecutive reals
     */
    RealGenerator(const RealSubset &subset, const std::optional<float> &delta);

    /**
     * @brief Reset the generator to initial state.
     */
    void reset();
    /**
     * @brief Get the current real and generate the next one.
     *
     * @return Current real value.
     */
    float next();
    /**
     * @brief Check if generator is able to generate next value.
     *
     * @return true if generator can generate next value, otherwise false
     */
    bool has_next();

private:
    const RealSubset &m_subset;
    RealSubset::RangeList::const_iterator m_iter;
    std::optional<RealSubset::RealType> m_next;
    std::optional<float> m_delta;
};
}
