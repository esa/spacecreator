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

#include "components/states/transition.h"

namespace seds::model {

const Transition::Primitive &Transition::primitive() const
{
    return m_primitive;
}

void Transition::setPrimitive(Transition::Primitive primitive)
{
    m_primitive = std::move(primitive);
}

const std::optional<BooleanExpression> &Transition::guard() const
{
    return m_guard;
}

void Transition::setGuard(BooleanExpression guard)
{
    m_guard = std::move(guard);
}

const std::optional<ActivityInvocation> &Transition::doActivity() const
{
    return m_do;
}

void Transition::setDoActivity(ActivityInvocation doActivity)
{
    m_do = std::move(doActivity);
}

const StateRef &Transition::fromState() const
{
    return m_fromState;
}

void Transition::setFromState(StateRef fromState)
{
    m_fromState = std::move(fromState);
}

const StateRef &Transition::toState() const
{
    return m_toState;
}

void Transition::setToState(StateRef toState)
{
    m_toState = std::move(toState);
}

} // namespace seds::model
