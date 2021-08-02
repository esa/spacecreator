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

#include <variant>
#include <vector>

#include "components/activities/activityinvocation.h"
#include "components/activities/assignment.h"
#include "components/activities/calibration.h"
#include "components/activities/conditional.h"
#include "components/activities/iteration.h"
#include "components/activities/mathoperation.h"
#include "components/primitives/sendcommandprimitive.h"
#include "components/primitives/sendparameterprimitive.h"

namespace seds::model {

class Body final
{
  public:
    using Statement = std::variant<ActivityInvocation,
                                   Assignment,
                                   Calibration,
                                   Conditional,
                                   Iteration,
                                   MathOperation,
                                   SendCommandPrimitive,
                                   SendParameterPrimitive>;
    using Statements = std::vector<Statement>;

  public:
    Body() = default;
    Body(Body&&) = default;
    Body& operator=(Body&&) = default;

  public:
    auto statements() const -> const Statements&;
    auto addStatement(Statement statement) -> void;

  private:
    std::vector<Statement> m_statements;
};

} // namespace seds::model
