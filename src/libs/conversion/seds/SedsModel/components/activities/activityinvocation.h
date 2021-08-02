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
#include "components/primitives/namedargumentvalue.h"

#include <vector>

namespace seds::model {

class ActivityInvocation final : public Statement
{
public:
    using ArgumentValues = std::vector<NamedArgumentValue>;

public:
    ActivityInvocation() = default;
    ActivityInvocation(ActivityInvocation &&) = default;
    ActivityInvocation &operator=(ActivityInvocation &&) = default;

public:
    auto argumentValues() const -> const ArgumentValues &;
    auto addArgumentValue(NamedArgumentValue argumentValue) -> void;

    auto activity() const -> const Name &;
    auto setActivity(Name activity) -> void;

private:
    ArgumentValues m_argumentValues;
    Name m_activity;
};

} // namespace seds::model
