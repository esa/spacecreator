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

#include "decision.h"

#include <memory>
#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

const std::vector<std::unique_ptr<Answer>> &Decision::answers() const
{
    return m_answers;
}

void Decision::addAnswer(std::unique_ptr<Answer> answer)
{
    if (hasAnswer(answer->literal())) {
        throw std::logic_error("Adding two Decision Answers with the same literal is illegal");
    }

    m_answers.push_back(std::move(answer));
}

bool Decision::hasAnswer(const VariableLiteral &answerLiteral)
{
    const auto foundAnswer = std::find_if(
            m_answers.begin(), m_answers.end(), [&](const auto &answer) { return answer->literal() == answerLiteral; });
    return foundAnswer != m_answers.end();
}

Expression *Decision::expression() const
{
    return m_expression.get();
}

void Decision::setExpression(std::unique_ptr<Expression> expression)
{
    m_expression = std::move(expression);
}

void Decision::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl
