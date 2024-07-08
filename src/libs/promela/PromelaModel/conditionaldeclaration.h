/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2024 N7 Space Sp. z o.o.
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

#include "declaration.h"

#include <QString>

namespace promela::model {
/**
 * @brief Helper class to create two versions of one field in utype.
 * This represents a conditional preprocessor macro with an expression.
 * The spin preprocessor will decide which version of declaration will be used.
 */
class ConditionalDeclaration final
{
public:
    /**
     * @brief Constructor.
     *
     * @param conditionalExpression expression for preprocessor
     * @param declIfTrue declaration to use when expression is evaluated to true
     * @param declIfFalse declaration to use when expression is evaluated to false
     */
    ConditionalDeclaration(QString conditionalExpression, Declaration declIfTrue, Declaration declIfFalse);

    /**
     * @brief Getter for conditional expression
     *
     * @returns reference to conditional expression
     */

    const QString &getConditionalExpression() const noexcept;

    /**
     * @brief Getter for declaration for true case
     *
     * @returns reference to declaration used in true case
     */
    const Declaration &getDeclIfTrue() const noexcept;

    /**
     * @brief Getter for declaration for false case
     *
     * @returns reference to declaration used in false case
     */
    const Declaration &getDeclIfFalse() const noexcept;

private:
    QString m_conditionalExpression;
    Declaration m_declIfTrue;
    Declaration m_declIfFalse;
};
}
