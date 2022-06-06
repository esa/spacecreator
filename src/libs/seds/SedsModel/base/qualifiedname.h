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

#include "base/name.h"

#include <optional>
#include <seds/SedsCommon/basetypesmappings.h>

namespace seds::model {

class QualifiedName final
{
public:
    QualifiedName() = default;
    QualifiedName(common::String value) noexcept;
    QualifiedName(const QualifiedName &) = default;
    QualifiedName(QualifiedName &&) = default;
    QualifiedName &operator=(const QualifiedName &) = default;
    QualifiedName &operator=(QualifiedName &&) = default;

public:
    auto name() const -> const Name &;
    auto namespaceName() const -> const std::optional<common::String> &;

public:
    auto pathStr() const -> const QString &;

    friend auto operator==(const QualifiedName &lhs, const QualifiedName &rhs) -> bool;
    friend auto operator!=(const QualifiedName &lhs, const QualifiedName &rhs) -> bool;
    friend auto operator<(const QualifiedName &lhs, const QualifiedName &rhs) -> bool;

private:
    Name m_name;
    std::optional<common::String> m_namespaceName;

    QString m_path;
};

} // namespace seds::model
