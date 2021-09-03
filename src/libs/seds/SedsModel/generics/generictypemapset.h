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

#include "generics/genericalternate.h"
#include "generics/genericalternateset.h"
#include "generics/generictypemap.h"

#include <optional>
#include <vector>

namespace seds::model {

class GenericTypeMapSet final
{
public:
    GenericTypeMapSet() = default;
    GenericTypeMapSet(GenericTypeMapSet &&) = default;
    GenericTypeMapSet &operator=(GenericTypeMapSet &&) = default;

public:
    auto genericTypeMaps() const -> const std::vector<GenericTypeMap> &;
    auto addGenericTypeMap(GenericTypeMap genericTypeMap) -> void;

    auto alternateSet() const -> const GenericAlternateSet &;
    auto setAlternateSet(GenericAlternateSet alternateSet) -> void;

private:
    std::vector<GenericTypeMap> m_genericTypeMaps;
    GenericAlternateSet m_alternateSet;
};

} // namespace seds::model
