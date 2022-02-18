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

#include "sequence.h"
#include "variableref.h"

#include <list>
#include <memory>
#include <variant>

namespace promela::model {
/**
 * @brief Representation of for loop in promela
 */
class ForLoop final
{
public:
    /**
     *
     */
    enum class Type
    {
        RANGE,
        FOR_EACH,
    };
    ForLoop(VariableRef var, int first, int last, std::unique_ptr<Sequence> sequence);

    ForLoop(VariableRef var, VariableRef array, std::unique_ptr<Sequence> sequence);

    Type getType() const;

    const VariableRef &getForVariable() const noexcept;

    int getFirstValue() const;
    int getLastValue() const;

    const VariableRef &getArrayRef() const;

    const std::unique_ptr<Sequence> &getSequence() const noexcept;

private:
    VariableRef m_variable;
    std::variant<std::pair<int, int>, VariableRef> m_data;
    std::unique_ptr<Sequence> m_sequence;
};
}
