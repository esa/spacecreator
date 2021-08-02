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

#pragma once

#include <optional>

#include "types/externalfield.h"

namespace seds::model {

enum class CommandArgumentMode
{
    IN,
    OUT,
    INOUT,
    NOTIFY
};

class CommandArgument final : public ExternalField
{
  public:
    CommandArgument() noexcept;
    CommandArgument(CommandArgument&&) = default;
    CommandArgument& operator=(CommandArgument&&) = default;

  public:
    auto mode() const -> CommandArgumentMode;
    auto setMode(CommandArgumentMode mode) -> void;

    auto defaultValue() -> const std::optional<LiteralValue>&;
    auto setDefaultValue(LiteralValue defaultValue) -> void;

    auto isDataUnit() const -> bool;
    auto setDataUnit(bool dataUnit) -> void;

  private:
    CommandArgumentMode m_mode;
    std::optional<LiteralValue> m_defaultValue;
    bool m_dataUnit;
};

} // namespace seds::model
