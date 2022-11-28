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

#include "newtype.h"

#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

Newtype::Newtype(QString name, QString indexingTypeName, QString elementTypeName)
    : Node(std::move(name))
    , m_indexingTypeName(std::move(indexingTypeName))
    , m_elementTypeName(std::move(elementTypeName))
{
}

const QString &Newtype::indexingTypeName() const
{
    return m_indexingTypeName;
}

void Newtype::setIndexingTypeName(QString indexingTypeName)
{
    m_indexingTypeName = std::move(indexingTypeName);
}

const QString &Newtype::elementTypeName() const
{
    return m_elementTypeName;
}

void Newtype::setElementTypeName(QString elementTypeName)
{
    m_elementTypeName = std::move(elementTypeName);
}

void Newtype::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl

