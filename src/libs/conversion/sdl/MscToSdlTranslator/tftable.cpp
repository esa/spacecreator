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

#include "tftable.h"

#include <iostream>

namespace conversion::sdl::translator {

TFTable::TFTable(const std::vector<uint32_t> &sequence, const uint32_t signalCount)
    : m_stateCount(sequence.size())
    , m_signalCount(signalCount)
    , m_table(m_stateCount, std::vector<uint32_t>(m_signalCount))
{
    compute(sequence);
}

uint32_t TFTable::stateCount() const
{
    return m_stateCount;
}

uint32_t TFTable::signalCount() const
{
    return m_signalCount;
}

const std::vector<uint32_t> &TFTable::transitionsForState(uint32_t state) const
{
    return m_table.at(state);
}

void TFTable::compute(const std::vector<uint32_t> &sequence)
{
    for (std::size_t state = 0; state < sequence.size(); ++state) {
        auto &transitions = m_table.at(state);

        for (std::size_t sig = 0; sig < m_signalCount; ++sig) {
            transitions.at(sig) = getNextState(sequence, state, sig);
        }
    }
}

uint32_t TFTable::getNextState(const std::vector<uint32_t> &sequence, const uint32_t state, const uint32_t signal) const
{
    if (state < sequence.size() && sequence.at(state) == signal) {
        return state + 1;
    }

    for (uint32_t nextState = state; nextState > 0; --nextState) {
        if (sequence.at(nextState - 1) == signal) {
            uint32_t i;

            for (i = 0; i < nextState - 1; ++i) {
                if (sequence.at(i) != sequence.at(state - nextState + i + 1)) {
                    break;
                }
            }

            if (i == nextState - 1) {
                return nextState;
            }
        }
    }

    return 0;
}

} // namespace conversion::sdl::translator
