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

#include "action.h"
#include "state.h"

#include <QString>
#include <memory>

namespace sdl {

class NextState : public Action
{
public:
    NextState(const QString &name);
    NextState(const QString &name, const std::shared_ptr<State> &state);
    auto state() const -> const std::shared_ptr<State> &;
    auto accept(Visitor &visitor) const -> void override;

private:
    std::shared_ptr<State> m_state;
};

} // namespace sdl
