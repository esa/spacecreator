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

#include <cstdint>

class QString;

namespace seds::model {

/**
 * @brief   Represents all available permutation of interface argument types
 */
enum class ArgumentsCombination : uint8_t
{
    NoArgs = 0b000,
    InOnly = 0b001,
    OutOnly = 0b010,
    NotifyOnly = 0b100,
    InAndOut = 0b011,
    InAndNotify = 0b101,
    OutAndNotify = 0b110,
    All = 0b111,
};

/**
 * @brief   Converts given arguments combination to string
 *
 * @param   argumentsCombination   Arguments combination to convert
 *
 * @param   String with arguments combination name
 */
auto argumentsCombinationToString(ArgumentsCombination argumentsCombination) -> QString;

} // namespace seds::model
