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

#include "components/primitives/primitivesource.h"

namespace seds::model {

PrimitiveSource::PrimitiveSource() noexcept
    : m_failed(false)
{
}

const std::optional<Name> &PrimitiveSource::transaction() const
{
    return m_transaction;
}

void PrimitiveSource::setTransaction(Name name)
{
    m_transaction = std::move(name);
}

bool PrimitiveSource::isFailed() const
{
    return m_failed;
}

void PrimitiveSource::setFailed(bool failed)
{
    m_failed = failed;
}

} // namespace seds::model
