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

#include "utype.h"

namespace promela::model {
Utype::Utype(QString name, bool unionType)
    : m_name(std::move(name))
    , m_unionType(unionType)
{
}

const QString &Utype::getName() const noexcept
{
    return m_name;
}

bool Utype::isUnionType() const noexcept
{
    return m_unionType;
}

void Utype::addField(const Declaration &field)
{
    m_fields.append(field);
}

void Utype::addField(const ConditionalDeclaration &field)
{
    m_fields.append(field);
}

const QList<Utype::Element> &Utype::getFields() const noexcept
{
    return m_fields;
}
}
