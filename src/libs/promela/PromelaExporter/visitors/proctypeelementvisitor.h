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

#include <QTextStream>
#include <promela/PromelaModel/proctypeelement.h>

namespace promela::exporter {
/**
 * @brief Visitor for exporting @link{model::ProctypeElement}
 */
class ProctypeElementVisitor
{
public:
    /**
     * @brief Constructor
     *
     * @param stream stream to append exported proctype element
     * @param baseIndent base indentation unit for whole exported model
     * @param sequenceIndent special indent unit for sequences
     * @param indent current indent
     */
    ProctypeElementVisitor(QTextStream &stream, QString baseIndent, QString sequenceIndent, QString indent);

    /**
     * @brief Visit ProctypeElement
     *
     * @param element ProctypeElement to visit*/
    void visit(const model::ProctypeElement &element);

    /**
     * @brief Handle Declaration
     *
     * @param declaration declaration to export
     */
    void operator()(const model::Declaration &declaration);
    /**
     * @brief Handle ChannelRecv
     *
     * @param channelRecv ChannelRecv to export
     */
    void operator()(const model::ChannelRecv &channelRecv);
    /**
     * @brief Handle ChannelSend
     *
     * @param channelSend ChannelSend to export
     */
    void operator()(const model::ChannelSend &channelSend);
    /**
     * @brief Handle Expression
     *
     * @param expression Expression to export
     */
    void operator()(const model::Expression &expression);
    /**
     * @brief Handle DoLoop
     *
     * @param doLoop DoLoop to export
     */
    void operator()(const model::DoLoop &doLoop);
    /**
     * @brief Handle Assignment
     *
     * @param assignment Assignment to export
     */
    void operator()(const model::Assignment &assignment);
    /**
     * @brief Handle InlineCall
     *
     * @param inlineCall InlineCall to export
     */
    void operator()(const model::InlineCall &inlineCall);
    /**
     * @brief Handle Skip
     *
     * @param skip Skip to export
     */
    void operator()(const model::Skip &skip);
    /**
     * @brief Handle ExitLoop (break)
     *
     * @param skip ExitLoop to export
     */
    void operator()(const model::ExitLoop &exitLoop);
    /**
     * @brief Handle Conditional
     *
     * @param ifStatement Conditional to export
     */
    void operator()(const model::Conditional &conditional);
    /**
     * @brief Handle Sequence
     *
     * @param sequence Sequence to export
     */
    void operator()(const model::Sequence &sequence);
    /**
     * @brief Handle For Loop
     *
     * @param loop ForLoop to export
     */
    void operator()(const model::ForLoop &loop);
    /**
     * @brief Handle select
     *
     * @param select Select to export
     */
    void operator()(const model::Select &select);
    /**
     * @brief Handle Assert Call
     *
     * @param call AssertCall to export
     */
    void operator()(const model::AssertCall &call);
    /**
     * @brief Handle Else Statement
     *
     * @param statement ElseStatement to export
     */
    void operator()(const model::ElseStatement &statement);
    /**
     * @brief Handle Label Statement
     *
     * @param label Label to export
     */
    void operator()(const model::Label &label);
    /**
     * @brief Handle Else Statement
     *
     * @param statement GoTo to export
     */
    void operator()(const model::GoTo &statement);
    /**
     * @brief Handle PrintfStatement
     *
     * @param statement PrintfStatement to export
     */
    void operator()(const model::PrintfStatement &statement);

    void operator()(const model::CCode &code);

private:
    QString expressionContentToString(const model::Expression &expression);

    QTextStream &m_stream;
    const QString m_baseIndent;
    const QString m_sequenceIndent;
    const QString m_indent;
};
}
