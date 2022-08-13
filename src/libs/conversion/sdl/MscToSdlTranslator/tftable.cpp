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
    : m_table(sequence.size(), std::vector<uint32_t>(signalCount))
    , m_signalCount(signalCount)
{
    compute(sequence);
}

void TFTable::print() const
{
    for (std::size_t i = 0; i < m_table.size(); ++i) {
        std::cout << i << "\t|";

        const auto &stateTransitions = m_table.at(i);

        for (std::size_t j = 0; j < stateTransitions.size(); ++j) {
            std::cout << " " << stateTransitions.at(j);
        }

        std::cout << '\n';
    }
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

uint32_t TFTable::getNextState(const std::vector<uint32_t> &sequence, const uint32_t state, const uint32_t sig) const
{
    if (state < sequence.size() && sequence.at(state) == sig) {
        return state+1;
    }

    for (uint32_t nextState = state; nextState > 0; --nextState) {
        if(sequence.at(nextState-1) == sig) {
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
