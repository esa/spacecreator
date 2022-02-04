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

#include "integersubset.h"

#include <algorithm>

namespace promela::translator {

IntegerSubset::IntegerSubset(int singleValue)
{
    m_allowedValues.emplace_back(singleValue, singleValue);
}

IntegerSubset::IntegerSubset(int minValue, int maxValue)
{
    m_allowedValues.emplace_back(minValue, maxValue);
}

IntegerSubset IntegerSubset::operator|(const IntegerSubset &rhs) const
{
    IntegerSubset result;

    auto p = m_allowedValues.begin();
    auto q = rhs.m_allowedValues.begin();

    while (p != m_allowedValues.end() && q != rhs.m_allowedValues.end()) {
        if (p->first < q->first) {
            result.m_allowedValues.push_back(*p);
            ++p;
        } else {
            result.m_allowedValues.push_back(*q);
            ++q;
        }
    }

    std::copy(p, m_allowedValues.end(), std::back_inserter(result.m_allowedValues));
    std::copy(q, rhs.m_allowedValues.end(), std::back_inserter(result.m_allowedValues));

    auto iter = result.m_allowedValues.begin();

    while (iter != result.m_allowedValues.end() && std::next(iter) != result.m_allowedValues.end()) {
        auto successor = std::next(iter);

        if (iter->second + 1 >= successor->first) {
            iter->second = std::max(iter->second, successor->second);
            result.m_allowedValues.erase(successor);
        } else {
            ++iter;
        }
    }

    return result;
}

IntegerSubset IntegerSubset::operator&(const IntegerSubset &rhs) const
{
    IntegerSubset result;

    for (const auto &p : m_allowedValues) {
        for (const auto &q : rhs.m_allowedValues) {
            int left = std::max(p.first, q.first);
            int right = std::min(p.second, q.second);

            if (left <= right) {
                result.m_allowedValues.emplace_back(left, right);
            }
        }
    }

    return result;
}

bool IntegerSubset::isSingleValue() const noexcept
{
    if (m_allowedValues.size() != 1) {
        return false;
    }
    return m_allowedValues.front().first == m_allowedValues.front().second;
}

bool IntegerSubset::isEmpty() const noexcept
{
    return m_allowedValues.empty();
}

std::optional<int> IntegerSubset::getMin() const noexcept
{
    if (m_allowedValues.empty()) {
        return std::nullopt;
    }
    return m_allowedValues.front().first;
}

std::optional<int> IntegerSubset::getMax() const noexcept
{
    if (m_allowedValues.empty()) {
        return std::nullopt;
    }
    return m_allowedValues.back().second;
}

const std::list<std::pair<int, int>> &IntegerSubset::getRanges() const noexcept
{
    return m_allowedValues;
}

IntegerSubset::IntegerSubset() {}
}
