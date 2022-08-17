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
#include <vector>

namespace conversion::sdl::translator {

class TFTable final
{
public:
    TFTable(const std::vector<uint32_t> &sequence, const uint32_t signalCount);
    TFTable(const TFTable &) = delete;
    TFTable(TFTable &&) = delete;

    TFTable &operator=(const TFTable &) = delete;
    TFTable &operator=(TFTable &&) = delete;

public:
    auto stateCount() const -> uint32_t;
    auto signalCount() const -> uint32_t;

    auto transitionsForState(uint32_t state) const -> const std::vector<uint32_t> &;

    auto print() const -> void;

private:
    auto compute(const std::vector<uint32_t> &sequence) -> void;
    auto getNextState(const std::vector<uint32_t> &sequence, const uint32_t state, const uint32_t sig) const
            -> uint32_t;

private:
    uint32_t m_stateCount;
    uint32_t m_signalCount;

    std::vector<std::vector<uint32_t>> m_table;
};

} // namespace conversion::sdl::translator
