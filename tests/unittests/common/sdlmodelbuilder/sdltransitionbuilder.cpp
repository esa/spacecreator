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

#include "sdltransitionbuilder.h"

#include <sdl/SdlModel/nextstate.h>

using sdl::NextState;

namespace tests::common {

SdlTransitionBuilder::SdlTransitionBuilder(QString name)
    : m_transition(std::make_unique<Transition>())
{
    // m_transition->setName(std::move(name));
    (void)name;
}

std::unique_ptr<Transition> SdlTransitionBuilder::build()
{
    return std::move(m_transition);
}

SdlTransitionBuilder &SdlTransitionBuilder::withNextStateAction(State *nextState)
{
    auto action = std::make_unique<NextState>("", nextState);

    m_transition->addAction(std::move(action));

    return *this;
}

SdlTransitionBuilder &SdlTransitionBuilder::withTask(std::unique_ptr<Task> task)
{
    m_transition->addAction(std::move(task));

    return *this;
}

} // namespace tests::common
