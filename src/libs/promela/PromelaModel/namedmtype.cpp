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

#include "namedmtype.h"

namespace promela::model {
NamedMtype::NamedMtype(QString name)
    : m_name(std::move(name))
{
}

const QString &NamedMtype::getName() const noexcept
{
    return m_name;
}

void NamedMtype::addValue(const QString &value)
{
    m_values.insert(value);
}

void NamedMtype::removeValue(const QString &value)
{
    m_values.remove(value);
}

bool NamedMtype::hasValue(const QString &value) const
{
    return m_values.contains(value);
}

const QSet<QString> &NamedMtype::values() const noexcept
{
    return m_values;
}
}
