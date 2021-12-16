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
class ProctypeElementVisitor
{
public:
    ProctypeElementVisitor(QTextStream &stream, QString baseIndent, QString sequenceIndent, QString indent);

    void visit(const ::promela::model::ProctypeElement &element);

    void operator()(const ::promela::model::Declaration &declaration);
    void operator()(const ::promela::model::ChannelRecv &channelRecv);
    void operator()(const ::promela::model::ChannelSend &channelSend);
    void operator()(const ::promela::model::Expression &expression);
    void operator()(const ::promela::model::DoLoop &doLoop);
    void operator()(const ::promela::model::Assignment &assignment);
    void operator()(const ::promela::model::InlineCall &inlineCall);

private:
    QTextStream &m_stream;
    const QString m_baseIndent;
    const QString m_sequenceIndent;
    const QString m_indent;
};
}
