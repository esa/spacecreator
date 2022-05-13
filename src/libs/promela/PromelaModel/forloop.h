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

#include "expression.h"
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
     * @brief Loop Type.
     *
     * For loop in Promela has two kinds: range based and for each.
     * The range based loop is in the form for(i : 0 .. 10).
     * The for each loop is in the form for(i in array).
     */
    enum class Type
    {
        RANGE,
        FOR_EACH,
    };

    /**
     * @brief Constructor for range based for loop.
     *
     * @param var      loop variable.
     * @param first    First element or range.
     * @param last     Last element of range.
     * @param sequence body.
     */
    ForLoop(VariableRef var, const Expression &first, const Expression &last, std::unique_ptr<Sequence> sequence);

    /**
     * @brief Constructor for array based for loop.
     *
     * @param var loop variable.
     * @param array array to iterate.
     * @param sequence body.
     */
    ForLoop(VariableRef var, VariableRef array, std::unique_ptr<Sequence> sequence);

    /**
     * @brief Getter for type of for loop.
     *
     * @return Type of loop.
     */
    Type getType() const;

    /**
     * @brief Getter for loop variable.
     *
     * @return Loop variable.
     */
    const VariableRef &getForVariable() const noexcept;

    /**
     * @brief Getter for first value of range based loop.
     *
     * @return First value in range.
     * @throws std::bad_optional_access if loop is not range based.
     */
    int getFirstValue() const;

    /**
     * @brief Getter for last value of range based loop.
     *
     * @return Last value in range.
     * @throws std::bad_optional_access if loop is not range based.
     */
    int getLastValue() const;

    /**
     * @brief Getter for array of array based loop.
     *
     * @return reference to array.
     * @throws std::bad_optional_access if loop is array based.
     */
    const VariableRef &getArrayRef() const;

    /**
     * @brief Getter for body of for loop.
     *
     * @return Sequence witch is a body of for loop.
     */
    const std::unique_ptr<Sequence> &getSequence() const noexcept;

private:
    VariableRef m_variable;
    using Range = std::pair<Expression, Expression>;
    std::variant<Range, VariableRef> m_data;
    std::unique_ptr<Sequence> m_sequence;
};
}
