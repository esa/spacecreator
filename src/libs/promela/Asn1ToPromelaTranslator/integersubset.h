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

#include <list>
#include <optional>
#include <utility>

namespace promela::translator {
/**
 * @brief Representation of subset of integer values.
 *
 * This class represents a subset of integer values using
 * A sorted list of pairs, that the every part
 * represents a continous range of integers.
 * e.g. (0, 10) represents subset of all integers from 0 to 10 included.
 * and (0, 0) represents single number 0.
 *
 * This class allows to calculation of sum and intersection of other subsets.
 * If the result of calculation contains two ranges next to each other,
 * then the ranges will be merged,e.g.
 * if the result of sum or intersection is (0, 10) and (11, 20) it will be merged to (0, 20)
 */
class IntegerSubset
{
public:
    /**
     * @brief Constructor.
     *
     * Creates a IntegerSubset which contains only one number.
     *
     * @param singleValue - number which will be included in subset.
     */
    explicit IntegerSubset(int singleValue);
    /**
     * @brief Constructor.
     *
     * Creates a IntegerSubset which contains a continuous range of numbers.
     *
     * @param minValue - minimumValue of Range
     */
    explicit IntegerSubset(int minValue, int maxValue);

    /**
     * @brief Calculate sum of two integer subsets.
     *
     * @param rsh other IntegerSubset
     * @return Sum of two subsets.
     */
    IntegerSubset operator|(const IntegerSubset &rhs) const;
    /**
     * @brief Calculate intersection of two integer subsets.
     *
     * @param rsh other IntegerSubset
     * @return Intersection of two subsets.
     */
    IntegerSubset operator&(const IntegerSubset &rhs) const;

    /**
     * @brief Check if subset contains only one number.
     *
     * @return true if subset contains only one number, otherwise false
     */
    bool isSingleValue() const noexcept;
    /**
     * @brief Check if subset is empty
     *
     * @return true if subset is empty, otherwise false
     */
    bool isEmpty() const noexcept;

    /**
     * @brief Get mimimum number of subset.
     *
     * @return Minimum number of subset or nullopt if subset is empty
     */
    std::optional<int> getMin() const noexcept;
    /**
     * @brief Get maximum number of subset.
     *
     * @return Maximum number of subset or nullopt if subset is empty
     */
    std::optional<int> getMax() const noexcept;

    /**
     * @brief Get sorted list of continuous ranges included into subset
     *
     * @return List of ranges.
     */
    const std::list<std::pair<int, int>> &getRanges() const noexcept;

private:
    IntegerSubset();
    void mergeOverlappingNeighbours();

private:
    std::list<std::pair<int, int>> m_allowedValues;
};

}
