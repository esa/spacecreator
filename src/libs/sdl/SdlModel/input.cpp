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

#include "input.h"

#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

Input::Input(QString name, Transition *transition)
    : Node(std::move(name))
    , m_transition(transition)
{
}

Transition *Input::transition() const
{
    return m_transition;
}

void Input::setTransition(Transition *transition)
{
    m_transition = transition;
}

const std::vector<std::unique_ptr<VariableReference>> &Input::parameters() const
{
    return m_parameters;
}

void Input::addParameter(std::unique_ptr<VariableReference> parameter)
{
    m_parameters.push_back(std::move(parameter));
}

void Input::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

bool operator==(const Input &lhs, const Input &rhs)
{
    if (QString::compare(lhs.m_name, rhs.m_name, Qt::CaseInsensitive) != 0) {
        return false;
    }

    if (lhs.m_parameters.size() != rhs.m_parameters.size()) {
        return false;
    }

    return std::equal(lhs.m_parameters.begin(), lhs.m_parameters.end(), rhs.m_parameters.begin(),
            [](const auto &lhsParam, const auto &rhsParam) { return *lhsParam == *rhsParam; });
}

bool operator!=(const Input &lhs, const Input &rhs)
{
    return !(lhs == rhs);
}

} // namespace sdl
