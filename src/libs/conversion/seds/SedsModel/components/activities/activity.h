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

#include "base/namedentity.h"
#include "components/activities/activityargument.h"
#include "components/activities/body.h"

namespace seds::model {

class Activity final : public NamedEntity
{
public:
    using Arguments = std::vector<ActivityArgument>;

public:
    Activity() = default;
    Activity(Activity &&) = default;
    Activity &operator=(Activity &&) = default;

public:
    auto arguments() const -> const Arguments &;
    auto addArgument(ActivityArgument argument) -> void;

    auto body() const -> Body *;
    auto setBody(std::unique_ptr<Body> body) -> void;

private:
    Arguments m_arguments;
    std::unique_ptr<Body> m_body;
};

} // namespace seds::model
