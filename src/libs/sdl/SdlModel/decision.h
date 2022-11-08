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
#include "answer.h"
#include "expression.h"

#include <QString>
#include <memory>
#include <vector>

namespace sdl {

/**
 * @brief   Represents a decision used in SDL model.
 */
class Decision final : public Action
{
public:
    /**
     * @brief   Default constructor
     */
    Decision() = default;
    /**
     * @brief   Deleted copy constructor
     */
    Decision(const Decision &) = delete;
    /**
     * @brief   Default move constructor
     */
    Decision(Decision &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Decision &operator=(const Decision &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    Decision &operator=(Decision &&) = default;

public:
    /**
     * @brief   Getter for a vector of answers
     *
     * @return  vector of answers
     */
    auto answers() const -> const std::vector<std::unique_ptr<Answer>> &;
    /**
     * @brief   Add an answer to a vector
     *
     * @param   answer  answer to add
     */
    auto addAnswer(std::unique_ptr<Answer> answer) -> void;
    /**
     * @brief   Checks if decision contains answer for given literal
     *
     * @param   answerLiteral   Literal to look for
     *
     * @return  True if answer was found, false otherwise
     */
    auto hasAnswer(const VariableLiteral &answerLiteral) -> bool;

    /**
     * @brief   Getter for expression
     *
     * @return  expression
     */
    auto expression() const -> Expression *;
    /**
     * @brief   Setter for expression
     *
     * @param   expression expression to set
     */
    auto setExpression(std::unique_ptr<Expression> expression) -> void;

public:
    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    std::vector<std::unique_ptr<Answer>> m_answers;
    std::unique_ptr<Expression> m_expression;
};

} // namespace sdl
