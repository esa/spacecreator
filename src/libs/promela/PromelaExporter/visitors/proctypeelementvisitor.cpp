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

#include "proctypeelementvisitor.h"

#include "declarationvisitor.h"
#include "expressionvisitor.h"
#include "sequencevisitor.h"
#include "variablerefvisitor.h"

using promela::model::Assignment;
using promela::model::ChannelRecv;
using promela::model::ChannelSend;
using promela::model::Declaration;
using promela::model::DoLoop;
using promela::model::Expression;
using promela::model::ProctypeElement;
using promela::model::Sequence;
using promela::model::VariableRef;

namespace promela::exporter {
ProctypeElementVisitor::ProctypeElementVisitor(
        QTextStream &stream, QString baseIndent, QString sequenceIndent, QString indent)
    : m_stream(stream)
    , m_baseIndent(std::move(baseIndent))
    , m_sequenceIndent(std::move(sequenceIndent))
    , m_indent(std::move(indent))
{
}

void ProctypeElementVisitor::visit(const ProctypeElement &element)
{
    std::visit(*this, element.getValue());
}

void ProctypeElementVisitor::operator()(const Declaration &declaration)
{
    DeclarationVisitor visitor(m_stream, m_indent);
    visitor.visit(declaration);
}

void ProctypeElementVisitor::operator()(const ChannelRecv &channelRecv)
{
    m_stream << m_indent;
    VariableRefVisitor visitor(m_stream);
    visitor.visit(channelRecv.getChannelRef());
    m_stream << "?";

    bool afterFirst = false;
    for (const VariableRef &arg : channelRecv.getArgs()) {
        if (afterFirst) {
            m_stream << ",";
        }
        visitor.visit(arg);
        afterFirst = true;
    }

    m_stream << ";\n";
}

void ProctypeElementVisitor::operator()(const ChannelSend &channelSend)
{
    m_stream << m_indent;

    VariableRefVisitor visitor(m_stream);
    visitor.visit(channelSend.getChannelRef());
    m_stream << "!";

    bool afterFirst = false;
    for (const VariableRef &arg : channelSend.getArgs()) {
        if (afterFirst) {
            m_stream << ",";
        }
        visitor.visit(arg);
        afterFirst = true;
    }

    m_stream << ";\n";
}

void ProctypeElementVisitor::operator()(const Expression &expression)
{
    m_stream << m_indent;
    ExpressionVisitor visitor(m_stream);
    visitor.visit(expression);
    m_stream << ";\n";
}

void ProctypeElementVisitor::operator()(const DoLoop &doLoop)
{
    m_stream << m_indent << "do\n";
    for (const std::unique_ptr<Sequence> &sequence : doLoop.getSequences()) {
        SequenceVisitor visitor(m_stream, m_baseIndent, m_sequenceIndent, m_indent);
        visitor.visit(*sequence, true);
    }
    m_stream << m_indent << "od;\n";
}

void ProctypeElementVisitor::operator()(const ::promela::model::Assignment &assignment)
{
    m_stream << m_indent;
    VariableRefVisitor variableRefVisitor(m_stream);
    variableRefVisitor.visit(assignment.getVariableRef());
    m_stream << " = ";
    ExpressionVisitor expressionVisitor(m_stream);
    expressionVisitor.visit(assignment.getExpression());
    m_stream << ";\n";
}
}
