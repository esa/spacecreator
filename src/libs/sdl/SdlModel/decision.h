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

#include "answer.h"
#include "expression.h"

#include <QString>
#include <memory>
#include <vector>

namespace sdl {

/**
 * @brief   Represents a decision used in SDL model.
 */
class Decision final
{
public:
    /**
     * @brief   Default constructor
     */
    Decision();

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

    /**
     * @brief   Getter for a vector of answers
     *
     * @return  vector of answers
     */
    auto answer() const -> const std::vector<std::unique_ptr<Answer>> &;

    /**
     * @brief   Add an answer to a vector
     *
     * @param   answer  answer to add
     */
    auto addAnswer(Answer *answer) -> void;

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

private:
    std::vector<std::unique_ptr<Answer>> m_answers;
    std::unique_ptr<Expression> m_expression;
};

} // namespace sdl
