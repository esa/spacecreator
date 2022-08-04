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

#include "visitors/inlinecallvisitor.h"

#include "visitors/variablerefvisitor.h"

using promela::model::Constant;
using promela::model::InlineCall;
using promela::model::VariableRef;

namespace promela::exporter {
InlineCallVisitor::InlineCallVisitor(QTextStream &stream)
    : m_stream(stream)
{
}

void InlineCallVisitor::visit(const InlineCall &inlineCall)
{
    m_stream << inlineCall.getName() << "(";
    VariableRefVisitor variableRefVisitor(m_stream);

    bool first = true;
    for (const InlineCall::Argument &argument : inlineCall.getArguments()) {
        if (!first) {
            m_stream << ", ";
        } else {
            first = false;
        }
        if (std::holds_alternative<VariableRef>(argument)) {
            variableRefVisitor.visit(std::get<VariableRef>(argument));
        } else if (std::holds_alternative<Constant>(argument)) {
            m_stream << std::get<Constant>(argument).getValue();
        }
    }
    m_stream << ")";
}
}
