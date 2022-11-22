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

#include "realgenerator.h"

namespace promela::translator {
RealGenerator::RealGenerator(const RealSubset &subset, const std::optional<float> &delta)
    : m_subset(subset)
    , m_delta(delta)
{
    reset();
}

void RealGenerator::reset()
{
    m_iter = m_subset.getRanges().begin();
    m_next = std::nullopt;
    if (m_iter != m_subset.getRanges().end()) {
        m_next = m_iter->first;
    }
}

float RealGenerator::next()
{
    auto result = m_next.value();

    if (result == m_iter->second) {
        ++m_iter;
        if (m_iter == m_subset.getRanges().end()) {
            m_next = std::nullopt;
        } else {
            m_next = m_iter->first;
        }
    } else {
        if (m_delta.has_value()) {
            m_next = result + m_delta.value();
        } else {
            m_next = result + 1;
        }
    }
    return result;
}

bool RealGenerator::has_next()
{
    return m_next.has_value();
}

}
