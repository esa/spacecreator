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

#include <memory>

#include "components/activities/booleanexpression.h"
#include "components/activities/statement.h"

namespace seds::model {

class Body;

class Conditional final : public Statement
{
  public:
    Conditional() = default;
    Conditional(Conditional&&) = default;
    Conditional& operator=(Conditional&&) = default;

  public:
    auto condition() const -> const BooleanExpression&;
    auto setCondition(BooleanExpression condition) -> void;

    auto onConditionTrue() const -> Body*;
    auto setOnConditionTrue(std::unique_ptr<Body> onConditionTrue) -> void;

    auto onConditionFalse() const -> Body*;
    auto setOnConditionFalse(std::unique_ptr<Body> onConditionFalse) -> void;

  private:
    BooleanExpression m_condition;
    std::unique_ptr<Body> m_onConditionTrue;
    std::unique_ptr<Body> m_onConditionFalse;
};

} // namespace seds::model
