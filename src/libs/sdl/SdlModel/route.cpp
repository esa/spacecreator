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

#include "route.h"

#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

const QString &Route::from() const
{
    return m_from;
}

void Route::setFrom(QString from)
{
    m_from = std::move(from);
}

const QString &Route::to() const
{
    return m_to;
}

void Route::setTo(QString to)
{
    m_to = std::move(to);
}

const QStringList &Route::with() const
{
    return m_with;
}

void Route::addWith(QString with)
{
    m_with.push_back(std::move(with));
}

void Route::setWith(QStringList with)
{
    m_with = std::move(with);
}

void Route::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl {
