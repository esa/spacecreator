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

#include "components/activities/basecondition.h"
#include "components/activities/comparison.h"
#include "components/activities/typecheck.h"

#include <memory>
#include <variant>
#include <vector>

namespace seds::model {

class AndedConditions;

class OredConditions final : public BaseCondition
{
public:
    using Condition = std::variant<Comparison, std::unique_ptr<AndedConditions>, TypeCheck>;
    using Conditions = std::vector<Condition>;

public:
    OredConditions() = default;
    OredConditions(OredConditions &&) = default;
    OredConditions &operator=(OredConditions &&) = default;

public:
    auto conditions() const -> const Conditions &;
    auto addCondition(Condition condition) -> void;

private:
    Conditions m_conditions;
};

} // namespace seds::model
