/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "member.h"

#include <vector>

namespace simulink::model {

class MemberSet final
{
public:
    using Members = std::vector<Member>;

public:
    MemberSet() = default;
    MemberSet(MemberSet &&) = default;
    MemberSet &operator=(MemberSet &&) = default;

public:
    auto members() const -> const Members &;
    auto addMember(Member member) -> void;

private:
    Members m_members;
};

} // namespace simulink::model
