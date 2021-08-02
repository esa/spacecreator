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

#include "interfaces/commandargument.h"

namespace seds::model {

CommandArgument::CommandArgument() noexcept
    : m_mode(CommandArgumentMode::IN)
    , m_dataUnit(false)
{
}

CommandArgumentMode
CommandArgument::mode() const
{
    return m_mode;
}

void
CommandArgument::setMode(CommandArgumentMode mode)
{
    m_mode = mode;
}

const std::optional<LiteralValue>&
CommandArgument::defaultValue()
{
    return m_defaultValue;
}

void
CommandArgument::setDefaultValue(LiteralValue defaultValue)
{
    m_defaultValue = std::move(defaultValue);
}

bool
CommandArgument::isDataUnit() const
{
    return m_dataUnit;
}

void
CommandArgument::setDataUnit(bool dataUnit)
{
    m_dataUnit = dataUnit;
}

} // namespace seds::model
