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

#include "base/qualifiedname.h"

namespace seds::model {

QualifiedName::QualifiedName(common::String value) noexcept
    : m_path(std::move(value))
{
    const auto index = m_path.lastIndexOf('/');

    if (index == -1) {
        m_name = m_path;
    } else {
        m_name = m_path.mid(index + 1);
        m_namespaceName = m_path.left(index);
    }
}

const Name &QualifiedName::name() const
{
    return m_name;
}

const std::optional<common::String> &QualifiedName::namespaceName() const
{
    return m_namespaceName;
}

const QString &QualifiedName::pathStr() const
{
    return m_path;
}

bool operator==(const QualifiedName &lhs, const QualifiedName &rhs)
{
    return lhs.m_path == rhs.m_path;
}

bool operator!=(const QualifiedName &lhs, const QualifiedName &rhs)
{
    return !(lhs == rhs);
}

bool operator<(const QualifiedName &lhs, const QualifiedName &rhs)
{
    return lhs.m_path < rhs.m_path;
}

} // namespace seds::model
