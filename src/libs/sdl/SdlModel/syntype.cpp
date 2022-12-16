/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "syntype.h"

#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

Syntype::Syntype(QString name, QString baseTypeName)
    : Node(std::move(name))
    , m_baseTypeName(std::move(baseTypeName))
{
}

const QString &Syntype::baseTypeName() const
{
    return m_baseTypeName;
}

void Syntype::setBaseTypeName(QString baseTypeName)
{
    m_baseTypeName = std::move(baseTypeName);
}

const std::vector<QString> &Syntype::constants() const
{
    return m_constants;
}

void Syntype::addValueConstant(uint64_t value)
{
    m_constants.push_back(m_valueConstantTemplate.arg(value));
}

void Syntype::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl
