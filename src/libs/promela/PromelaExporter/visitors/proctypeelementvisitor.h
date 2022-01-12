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
#include <promela/PromelaModel/proctypeelement.h>

namespace promela::exporter {
/**
 * @brief Visitor for exporting @link{::promela::model::ProctypeElement}
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
    void visit(const ::promela::model::ProctypeElement &element);

    /**
     * @brief Handle Declaration
     *
     * @param declaration declaration to export
     */
    void operator()(const ::promela::model::Declaration &declaration);
    /**
     * @brief Handle ChannelRecv
     *
     * @param channelRecv ChannelRecv to export
     */
    void operator()(const ::promela::model::ChannelRecv &channelRecv);
    /**
     * @brief Handle ChannelSend
     *
     * @param channelSend ChannelSend to export
     */
    void operator()(const ::promela::model::ChannelSend &channelSend);
    /**
     * @brief Handle Expression
     *
     * @param expression Expression to export
     */
    void operator()(const ::promela::model::Expression &expression);
    /**
     * @brief Handle DoLoop
     *
     * @param doLoop DoLoop to export
     */
    void operator()(const ::promela::model::DoLoop &doLoop);
    /**
     * @brief Handle Assignment
     *
     * @param assignment Assignment to export
     */
    void operator()(const ::promela::model::Assignment &assignment);
    /**
     * @brief Handle InlineCall
     *
     * @param inlineCall InlineCall to export
     */
    void operator()(const ::promela::model::InlineCall &inlineCall);

private:
    QTextStream &m_stream;
    const QString m_baseIndent;
    const QString m_sequenceIndent;
    const QString m_indent;
};
}
