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

#include "base/qualifiedname.h"

#include <optional>

namespace seds::model {

class DataTypeRef final
{
public:
    DataTypeRef() = default;
    DataTypeRef(common::String value) noexcept;
    DataTypeRef(const DataTypeRef &) = default;
    DataTypeRef(DataTypeRef &&) = default;
    DataTypeRef &operator=(const DataTypeRef &) = default;
    DataTypeRef &operator=(DataTypeRef &&) = default;

public:
    auto value() const -> const QualifiedName &;

public:
    auto nameStr() const -> const QString &;
    auto packageStr() const -> const std::optional<QString> &;

    friend auto operator==(const DataTypeRef &lhs, const DataTypeRef &rhs) -> bool;
    friend auto operator!=(const DataTypeRef &lhs, const DataTypeRef &rhs) -> bool;
    friend auto operator<(const DataTypeRef &lhs, const DataTypeRef &rhs) -> bool;

private:
    QualifiedName m_value;
};

} // namespace seds::model
