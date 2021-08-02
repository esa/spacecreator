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

#include "base/description.h"

namespace seds::model {

Description::~Description() = default;

const std::optional<common::String>&
Description::shortDescription() const
{
    return m_shortDescription;
}

void
Description::setShortDescription(common::String shortDescription)
{
    m_shortDescription = std::move(shortDescription);
}

const std::optional<common::String>&
Description::longDescription() const
{
    return m_longDescription;
}

void
Description::setLongDescription(common::String longDescription)
{
    m_longDescription = std::move(longDescription);
}

} // namespace seds::model
