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

#include <QString>
#include <memory>
#include <promela/PromelaModel/basictypes.h>
#include <promela/PromelaModel/expression.h>
#include <promela/PromelaModel/proctypeelement.h>
#include <promela/PromelaModel/sequence.h>
#include <promela/PromelaModel/variableref.h>

using promela::model::BasicType;
using promela::model::Expression;
using promela::model::ProctypeElement;
using promela::model::VariableRef;

namespace promela::translator {
/**
 * @brief Make promela proctype element entities
 */
class ProctypeMaker final
{
public:
    /**
     * @brief make ProctypeElement with InlineCall with one argument
     *
     * @param inlineName     name of selected inline
     * @param argumentName   name of a variable to be passed as call argument
     * @param memberName     name of a member of the structure (optional)
     *
     * @return  ProctypeElement with specified InlineCall
     */
    static auto makeInlineCall(const QString &inlineName, const QString &argumentName, const QString &memberName = "")
            -> std::unique_ptr<ProctypeElement>;

    /**
     * @brief make empty promela Sequence of NORMAL type
     *
     * @return promela Sequence
     */
    static auto makeNormalSequence() -> std::unique_ptr<promela::model::Sequence>;

    /**
     * @brief make ProctypeElement with Expression with value "true"
     *
     * @return ProctypeElement with Expression with value "true"
     */
    static auto makeTrueExpressionProctypeElement() -> std::unique_ptr<ProctypeElement>;

    /**
     * @brief make ProctypeElement with Assignment
     *
     * @param variableName variable to which the value will be assigned
     * @param value        value which will be assigned to the variable
     *
     * @return ProctypeElement with specified Assignment
     */
    static auto makeAssignmentProctypeElement(const QString &variableName, int32_t value)
            -> std::unique_ptr<ProctypeElement>;

    /**
     * @brief make ProctypeElement with a variable declaration
     *
     * @param variableType type of the variable
     * @param variableName name of the variable
     *
     * @return ProctypeElement with specified specified variable declaration
     */
    static auto makeVariableDeclaration(const model::BasicType &variableType, const QString &variableName)
            -> std::unique_ptr<ProctypeElement>;

    /**
     * @brief make ProctypeElement with a for loop
     *
     * @param variable reference to iterator variable
     * @param first    start value of iterator value
     * @param last     end value of iterator value (inclusive)
     * @param sequence loop's body
     *
     * @return ProctypeElement with specified specified variable declaration
     */
    static auto makeForLoop(const VariableRef &variable, const Expression &first, const Expression &last,
            std::unique_ptr<model::Sequence> sequence) -> std::unique_ptr<ProctypeElement>;
};

} // namespace promela::translator
