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

#include "components/activities/statement.h"
#include "components/activities/valueoperand.h"
#include "components/variableref.h"

#include <variant>

namespace seds::model {

class Assignment final : public Statement
{
public:
    using Element = std::variant<VariableRef, ValueOperand>;

public:
    Assignment() = default;
    Assignment(Assignment &&) = default;
    Assignment &operator=(Assignment &&) = default;

public:
    auto element() const -> const Element &;
    auto setElement(Element element) -> void;

    auto outputVariableRef() const -> const VariableRef &;
    auto setOutputVariableRef(VariableRef outputVariableRef) -> void;

private:
    Element m_element;
    VariableRef m_outputVariableRef;
};

} // namespace seds::model
