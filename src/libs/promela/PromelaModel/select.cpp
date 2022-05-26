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

#include "binaryexpression.h"
#include "constant.h"
#include "expression.h"
#include "forloop.h"
#include "proctypeelement.h"

#include <stdexcept>
#include <variant>

namespace promela::model {

Select::Select(VariableRef var, const Expression &first, const Expression &last)
    : m_variable(std::move(var))
    , m_range(std::make_pair(first, last))
{
}

const VariableRef &Select::getRecipientVariable() const noexcept
{
    return m_variable;
}

const Expression &Select::getFirstExpression() const
{
    return m_range.first;
}

const Expression &Select::getLastExpression() const
{
    return m_range.second;
}

int Select::getFirstIntValue() const
{
    const Expression &value = m_range.first;
    const auto &content = value.getContent();
    if (std::holds_alternative<Constant>(content)) {
        return std::get<Constant>(content).getValue();
    } else {
        throw std::logic_error("Expression does not contain a constant");
    }
}

int Select::getLastIntValue() const
{
    const Expression &value = m_range.second;
    const auto &content = value.getContent();
    if (std::holds_alternative<Constant>(content)) {
        return std::get<Constant>(content).getValue();
    } else {
        throw std::logic_error("Expression does not contain a constant");
    }
}

}
