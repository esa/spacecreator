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

#include "proctypeelementvisitor.h"

#include "binaryexpression.h"
#include "constant.h"
#include "declarationvisitor.h"
#include "expressionvisitor.h"
#include "inlinecallvisitor.h"
#include "sequencevisitor.h"
#include "variablerefvisitor.h"

using promela::model::AssertCall;
using promela::model::Assignment;
using promela::model::ChannelRecv;
using promela::model::ChannelSend;
using promela::model::Conditional;
using promela::model::Declaration;
using promela::model::DoLoop;
using promela::model::ExitLoop;
using promela::model::Expression;
using promela::model::ForLoop;
using promela::model::InlineCall;
using promela::model::ProctypeElement;
using promela::model::Select;
using promela::model::Sequence;
using promela::model::Skip;
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
        if (sequence->getType() != Sequence::Type::NORMAL) {
            SequenceVisitor visitor(m_stream, m_baseIndent, m_sequenceIndent, m_indent);
            m_stream << m_indent << m_sequenceIndent << visitor.getSequencePrefix(*sequence) << "{\n";
            visitor.visit(*sequence, false);
            m_stream << m_indent << "}\n";
        } else {
            SequenceVisitor visitor(m_stream, m_baseIndent, m_sequenceIndent, m_indent);
            visitor.visit(*sequence, true);
        }
    }
    m_stream << m_indent << "od;\n";
}

void ProctypeElementVisitor::operator()(const Assignment &assignment)
{
    m_stream << m_indent;
    VariableRefVisitor variableRefVisitor(m_stream);
    variableRefVisitor.visit(assignment.getVariableRef());
    m_stream << " = ";
    ExpressionVisitor expressionVisitor(m_stream);
    expressionVisitor.visit(assignment.getExpression());
    m_stream << ";\n";
}

void ProctypeElementVisitor::operator()(const InlineCall &inlineCall)
{
    m_stream << m_indent;
    InlineCallVisitor visitor(m_stream);
    visitor.visit(inlineCall);
    m_stream << ";\n";
}

void ProctypeElementVisitor::operator()(const Skip &skip)
{
    Q_UNUSED(skip);
    m_stream << m_indent;
    m_stream << "skip;\n";
}

void ProctypeElementVisitor::operator()(const ExitLoop &exitLoop)
{
    Q_UNUSED(exitLoop);
    m_stream << m_indent;
    m_stream << "break;\n";
}

void ProctypeElementVisitor::operator()(const Conditional &conditional)
{
    m_stream << m_indent << "if\n";
    for (const std::unique_ptr<Sequence> &sequence : conditional.getAlternatives()) {
        if (sequence->getType() != Sequence::Type::NORMAL) {
            SequenceVisitor visitor(m_stream, m_baseIndent, m_sequenceIndent, m_indent);
            m_stream << m_indent << m_sequenceIndent << visitor.getSequencePrefix(*sequence) << "{\n";
            visitor.visit(*sequence, false);
            m_stream << m_indent << "}\n";
        } else {
            SequenceVisitor visitor(m_stream, m_baseIndent, m_sequenceIndent, m_indent);
            visitor.visit(*sequence, true);
        }
    }
    m_stream << m_indent << "fi;\n";
}

void ProctypeElementVisitor::operator()(const Sequence &sequence)
{
    SequenceVisitor visitor(m_stream, m_baseIndent, m_sequenceIndent, m_indent);
    m_stream << m_indent << visitor.getSequencePrefix(sequence) << "{\n";
    visitor.visit(sequence, false);
    m_stream << m_indent << "}\n";
}

void ProctypeElementVisitor::operator()(const ForLoop &loop)
{
    VariableRefVisitor variableRefVisitor(m_stream);
    if (loop.getType() == ForLoop::Type::RANGE) {
        m_stream << m_indent << "for(";
        variableRefVisitor.visit(loop.getForVariable());

        const QString beginExpr = expressionContentToString(loop.getFirstExpression());
        const QString endExpr = expressionContentToString(loop.getLastExpression());
        m_stream << " : " << beginExpr << " .. " << endExpr << ")\n";
    } else {
        m_stream << m_indent << "for(";
        variableRefVisitor.visit(loop.getForVariable());
        m_stream << " in ";
        variableRefVisitor.visit(loop.getArrayRef());
        m_stream << ")\n";
    }
    SequenceVisitor visitor(m_stream, m_baseIndent, m_sequenceIndent, m_indent);
    m_stream << m_indent << visitor.getSequencePrefix(*loop.getSequence()) << "{\n";
    visitor.visit(*loop.getSequence(), false);
    m_stream << m_indent << "}\n";
}

void ProctypeElementVisitor::operator()(const Select &select)
{
    VariableRefVisitor variableRefVisitor(m_stream);

    m_stream << m_indent << "select (";
    variableRefVisitor.visit(select.getRecipientVariable());

    const QString beginExpr = expressionContentToString(select.getFirstExpression());
    const QString endExpr = expressionContentToString(select.getLastExpression());
    m_stream << " : " << beginExpr << " .. " << endExpr << ");\n";
}

void ProctypeElementVisitor::operator()(const AssertCall &call)
{
    m_stream << m_indent;

    m_stream << "assert(";

    ExpressionVisitor visitor(m_stream);
    visitor.visit(call.expression());

    m_stream << ");\n";
}

QString ProctypeElementVisitor::expressionContentToString(const model::Expression &expression)
{
    const Expression::Value &content = expression.getContent();

    QString str;
    QTextStream stream(&str);

    ProctypeElementVisitor visitor(stream, "", "", ""); // none indent shall be used
    visitor(Expression(content));

    const int lineEndingLen = 2;
    str.truncate(str.length() - lineEndingLen);

    return str;
}

}
