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

#include <cstdint>
#include <list>
#include <optional>
#include <utility>

namespace promela::translator {
/**
 * @brief Representation of subset of real values.
 *
 * This class represents a subset of real values using
 * A sorted list of pairs, that the every part
 * represents a continous range of reals.
 * e.g. (0, 10) represents subset of all reals from 0 to 10 included.
 * and (0, 0) represents single number 0.
 *
 * This class allows to calculation of sum and intersection of other subsets.
 * If the result of calculation contains two ranges next to each other,
 * then the ranges will be merged,e.g.
 * if the result of sum or intersection is (0, 10) and (11, 20) it will be merged to (0, 20)
 */
class RealSubset
{
public:
    /** @brief  Typedef for underlying real type */
    using RealType = float;
    /** @brief  Typedef for real range */
    using Range = std::pair<RealType, RealType>;
    /** @brief  Typedef for real range list */
    using RangeList = std::list<Range>;

    /**
     * @brief Constructor.
     *
     * Creates a RealSubset which contains only one number.
     *
     * @param singleValue - number which will be included in subset.
     */
    explicit RealSubset(RealType singleValue);
    /**
     * @brief Constructor.
     *
     * Creates a RealSubset which contains a continuous range of numbers.
     *
     * @param minValue - minimum value of Range
     * @param maxValue - maximum value of Range
     */
    explicit RealSubset(RealType minValue, RealType maxValue);

    /**
     * @brief Calculate sum of two real subsets.
     *
     * @param rsh other RealSubset
     * @return Sum of two subsets.
     */
    RealSubset operator|(const RealSubset &rhs) const;
    /**
     * @brief Calculate intersection of two real subsets.
     *
     * @param rsh other RealSubset
     * @return Intersection of two subsets.
     */
    RealSubset operator&(const RealSubset &rhs) const;

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
    std::optional<RealType> getMin() const noexcept;
    /**
     * @brief Get maximum number of subset.
     *
     * @return Maximum number of subset or nullopt if subset is empty
     */
    std::optional<RealType> getMax() const noexcept;

    /**
     * @brief Get sorted list of continuous ranges included into subset
     *
     * @return List of ranges.
     */
    const RangeList &getRanges() const noexcept;

private:
    RealSubset();
    void mergeOverlappingNeighbours();

private:
    RangeList m_allowedValues;
};

}
