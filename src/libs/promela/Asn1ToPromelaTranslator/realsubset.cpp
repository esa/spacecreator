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

#include "realsubset.h"

#include <algorithm>

namespace promela::translator {

RealSubset::RealSubset(RealSubset::RealType singleValue)
{
    m_allowedValues.emplace_back(singleValue, singleValue);
}

RealSubset::RealSubset(RealSubset::RealType minValue, RealSubset::RealType maxValue)
{
    m_allowedValues.emplace_back(minValue, maxValue);
}

RealSubset RealSubset::operator|(const RealSubset &rhs) const
{
    RealSubset result;

    std::merge(m_allowedValues.begin(), m_allowedValues.end(), rhs.m_allowedValues.begin(), rhs.m_allowedValues.end(),
            std::back_inserter(result.m_allowedValues),
            [](const auto &left, const auto &right) { return left.first < right.first; });

    result.mergeOverlappingNeighbours();

    return result;
}

RealSubset RealSubset::operator&(const RealSubset &rhs) const
{
    RealSubset result;

    for (const auto &p : m_allowedValues) {
        for (const auto &q : rhs.m_allowedValues) {
            auto left = std::max(p.first, q.first);
            auto right = std::min(p.second, q.second);

            if (left <= right) {
                result.m_allowedValues.emplace_back(left, right);
            }
        }
    }

    return result;
}

bool RealSubset::isSingleValue() const noexcept
{
    if (m_allowedValues.size() != 1) {
        return false;
    }
    return m_allowedValues.front().first == m_allowedValues.front().second;
}

bool RealSubset::isEmpty() const noexcept
{
    return m_allowedValues.empty();
}

std::optional<RealSubset::RealType> RealSubset::getMin() const noexcept
{
    if (m_allowedValues.empty()) {
        return std::nullopt;
    }
    return m_allowedValues.front().first;
}

std::optional<RealSubset::RealType> RealSubset::getMax() const noexcept
{
    if (m_allowedValues.empty()) {
        return std::nullopt;
    }
    return m_allowedValues.back().second;
}

const RealSubset::RangeList &RealSubset::getRanges() const noexcept
{
    return m_allowedValues;
}

RealSubset::RealSubset() {}

void RealSubset::mergeOverlappingNeighbours()
{
    auto iter = m_allowedValues.begin();

    while (iter != m_allowedValues.end() && std::next(iter) != m_allowedValues.end()) {
        auto successor = std::next(iter);

        if (iter->second + 1 >= successor->first) {
            iter->second = std::max(iter->second, successor->second);
            m_allowedValues.erase(successor);
        } else {
            ++iter;
        }
    }
}
}
