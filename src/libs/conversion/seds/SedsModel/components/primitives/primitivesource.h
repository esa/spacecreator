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

#include "base/name.h"
#include "components/activities/statement.h"

namespace seds::model {

class PrimitiveSource : public Statement
{
  public:
    PrimitiveSource() noexcept;
    PrimitiveSource(PrimitiveSource&&) = default;
    PrimitiveSource& operator=(PrimitiveSource&&) = default;

  public:
    auto transaction() const -> const std::optional<Name>&;
    auto setTransaction(Name name) -> void;

    auto isFailed() const -> bool;
    auto setFailed(bool failed) -> void;

  private:
    std::optional<Name> m_transaction;
    bool m_failed;
};

} // namespace seds::model
