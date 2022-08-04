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
#include <sdl/SdlModel/output.h>
#include <sdl/SdlModel/state.h>
#include <sdl/SdlModel/task.h>
#include <sdl/SdlModel/transition.h>

namespace tests::common {

class SdlTransitionBuilder final
{
public:
    SdlTransitionBuilder();

    auto build() -> std::unique_ptr<sdl::Transition>;

    auto withNextStateAction(sdl::State *nextState = nullptr) -> SdlTransitionBuilder &;
    auto withTask(std::unique_ptr<sdl::Task> task) -> SdlTransitionBuilder &;
    auto withOutput(std::unique_ptr<sdl::Output> output) -> SdlTransitionBuilder &;
    auto withAction(std::unique_ptr<sdl::Action> action) -> SdlTransitionBuilder &;

private:
    std::unique_ptr<sdl::Transition> m_transition;
};

} // namespace tests::common
