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

#include "mtyperef.h"

namespace tmc::promela::model {
MtypeRef::MtypeRef()
    : m_named(false)
    , m_ref()
{
}

MtypeRef::MtypeRef(QString ref)
    : m_named(true)
    , m_ref(std::move(ref))
{
}

const QString &MtypeRef::getName() const noexcept
{
    return m_ref;
}

bool MtypeRef::isNamed() const noexcept
{
    return m_named;
}
}
