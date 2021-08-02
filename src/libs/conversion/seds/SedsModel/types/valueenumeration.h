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

#include "base/description.h"
#include "base/name.h"

#include <cstdint>

namespace seds::model {

class ValueEnumeration final : public Description
{
public:
    ValueEnumeration() = default;
    ValueEnumeration(ValueEnumeration &&) = default;
    ValueEnumeration &operator=(ValueEnumeration &&) = default;

public:
    auto value() const -> int32_t;
    auto setValue(int32_t value) -> void;

    auto label() const -> const Name &;
    auto setLabel(common::String label) -> void;

private:
    int32_t m_value;
    Name m_label;
};

} // namespace seds::model
