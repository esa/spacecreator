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

#include <QTextStream>
#include <promela/PromelaModel/expression.h>

namespace promela::exporter {
/**
 * @brief Visitor for exporting @link{::romela::model::Expression}
 */
class ExpressionVisitor final
{
public:
    /**
     * @brief Constructor
     *
     * @param stream stream to append exported expression
     */
    ExpressionVisitor(QTextStream &stream);

    /**
     * @brief Visit Expression
     *
     * @param expression to visit
     */
    void visit(const ::promela::model::Expression &expression);

    /**
     * @brief Handle VariableRef
     *
     * @param variableRef variable reference to export
     */
    void operator()(const ::promela::model::VariableRef &variableRef);
    /**
     * @brief Handle Constant
     *
     * @param constant constant to export
     */
    void operator()(const ::promela::model::Constant &constant);

private:
    QTextStream &m_stream;
};
}
