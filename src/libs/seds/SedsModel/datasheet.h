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

#include "device/device.h"
#include "package/package.h"

#include <vector>

namespace seds::model {

class DataSheet final
{
public:
    DataSheet() = default;
    DataSheet(DataSheet &&) = default;
    DataSheet &operator=(DataSheet &&) = default;

public:
    auto device() const -> const Device &;
    auto setDevice(Device device) -> void;

    auto packages() const -> const std::vector<Package> &;
    auto addPackage(Package package) -> void;

private:
    Device m_device;
    std::vector<Package> m_packages;
};

} // namespace seds::model
