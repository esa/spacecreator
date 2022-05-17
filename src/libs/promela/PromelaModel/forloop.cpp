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

#include "forloop.h"

#include "binaryexpression.h"
#include "constant.h"
#include "expression.h"
#include "proctypeelement.h"

#include <stdexcept>
#include <variant>

namespace promela::model {

ForLoop::ForLoop(VariableRef var, const Expression &first, const Expression &last, std::unique_ptr<Sequence> sequence)
    : m_variable(std::move(var))
    , m_data(std::make_pair(first, last))
    , m_sequence(std::move(sequence))
{
}

ForLoop::ForLoop(VariableRef var, VariableRef array, std::unique_ptr<Sequence> sequence)
    : m_variable(std::move(var))
    , m_data(std::move(array))
    , m_sequence(std::move(sequence))
{
}

ForLoop::Type ForLoop::getType() const
{
    return std::holds_alternative<VariableRef>(m_data) ? Type::FOR_EACH : Type::RANGE;
}

const VariableRef &ForLoop::getForVariable() const noexcept
{
    return m_variable;
}

Expression ForLoop::getFirstExpression() const
{
    return std::get<Range>(m_data).first;
}

Expression ForLoop::getLastExpression() const
{
    return std::get<Range>(m_data).second;
}

int ForLoop::getFirstIntValue() const
{
    const Expression &value = std::get<Range>(m_data).first;
    const auto &content = value.getContent();
    if (std::holds_alternative<Constant>(content)) {
        return std::get<Constant>(content).getValue();
    } else {
        throw std::logic_error("Expression does not contain a constant");
    }
}

int ForLoop::getLastIntValue() const
{
    const Expression &value = std::get<Range>(m_data).second;
    const auto &content = value.getContent();
    if (std::holds_alternative<Constant>(content)) {
        return std::get<Constant>(content).getValue();
    } else {
        throw std::logic_error("Expression does not contain a constant");
    }
}

const VariableRef &ForLoop::getArrayRef() const
{
    return std::get<VariableRef>(m_data);
}

const std::unique_ptr<Sequence> &ForLoop::getSequence() const noexcept
{
    return m_sequence;
}

}
