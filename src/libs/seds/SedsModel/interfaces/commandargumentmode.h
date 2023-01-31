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

#include <seds/SedsCommon/enumconversion.h>

namespace seds::model {

enum class CommandArgumentMode : uint8_t
{
    In = 0b001,
    Out = 0b010,
    InOut = 0b011,
    Notify = 0b100,
};

template<>
auto enumFromString(const QStringView &enumStr) -> std::optional<CommandArgumentMode>;

template<>
auto stringFromEnum(CommandArgumentMode mode) -> const QString &;

} // namespace seds::model {
