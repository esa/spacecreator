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

#include "variablerefvisitor.h"

#include "expressionvisitor.h"

using promela::model::VariableRef;

namespace promela::exporter {
VariableRefVisitor::VariableRefVisitor(QTextStream &stream)
    : m_stream(stream)
{
}

void VariableRefVisitor::visit(const VariableRef &variableRef)
{
    const std::list<VariableRef::Element> &elements = variableRef.getElements();

    bool first = true;
    for (const auto &element : elements) {
        if (!first) {
            m_stream << ".";
        }
        first = false;
        m_stream << element.m_name;
        if (element.m_index) {
            m_stream << "[";
            ExpressionVisitor expressionVisitor(m_stream);
            expressionVisitor.visit(*element.m_index);
            m_stream << "]";
        }
    }
}
}
