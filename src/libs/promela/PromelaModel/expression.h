/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 - 2022 N7 Space Sp. z o.o.
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

#include "binaryexpression.h"
#include "booleanconstant.h"
#include "constant.h"
#include "inlinecall.h"
#include "stringconstant.h"
#include "variableref.h"

#include <QString>
#include <variant>

namespace promela::model {
/**
 * @brief Representation of expression in promela.
 */
class Expression final
{
public:
    /**
     * @brief Variant to represent alternatives of expression
     */
    using Value = std::variant<VariableRef, Constant, BinaryExpression, InlineCall, BooleanConstant, StringConstant>;

    /**
     * @brief Constructor.
     *
     * @param content expression content
     */
    Expression(Value content);
    /**
     * @brief Constructor.
     *
     * @param constant expression content
     */
    Expression(int constant);

    /**
     * @brief Getter for expression content
     *
     * @return content of expression
     */
    const Value &getContent() const noexcept;

private:
    Value m_content;
};
}
