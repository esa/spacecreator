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

#include "sequencevisitor.h"

#include "proctypeelementvisitor.h"

using promela::model::ProctypeElement;
using promela::model::Sequence;

namespace promela::exporter {
SequenceVisitor::SequenceVisitor(QTextStream &stream, QString baseIndent, QString sequenceIndent, QString indent)
    : m_stream(stream)
    , m_baseIndent(std::move(baseIndent))
    , m_sequenceIndent(std::move(sequenceIndent))
    , m_indent(std::move(indent))
{
}

void SequenceVisitor::visit(const Sequence &sequence, bool useSequenceIndent)
{
    bool afterFirst = false;
    for (const std::unique_ptr<ProctypeElement> &element : sequence.getContent()) {
        if (!afterFirst && useSequenceIndent) {
            ProctypeElementVisitor visitor(m_stream, m_baseIndent, m_sequenceIndent, m_indent + m_sequenceIndent);
            visitor.visit(*element);
            afterFirst = true;
        } else {
            ProctypeElementVisitor visitor(m_stream, m_baseIndent, m_sequenceIndent, m_indent + m_baseIndent);
            visitor.visit(*element);
        }
    }
}
}
