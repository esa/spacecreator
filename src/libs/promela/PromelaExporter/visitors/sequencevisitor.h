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

#include <QString>
#include <QTextStream>
#include <promela/PromelaModel/sequence.h>

namespace promela::exporter {
/**
 * @brief Visitor for @link{model::Sequence}
 */
class SequenceVisitor final
{
public:
    /**
     * @brief Constructor
     *
     * @param stream stream to append exported sequence
     * @param baseIndent base identation unit for whole exported model
     * @param sequenceIndent special indent unit for sequences
     * @param indent current indent
     */
    SequenceVisitor(QTextStream &stream, QString baseIndent, QString sequenceIndent, QString indent);

    /**
     * @brief Visit Sequence
     *
     * @param sequence sequence to visit
     * @param useSequenceIndent if true the indent for first element will be sequenceIndent
     */
    void visit(const model::Sequence &sequence, bool useSequenceIndent = false);

    /**
     * @brief Get string prefix for sequence based on sequence type
     *
     * @param sequence sequence
     * @return prefix of sequence e.g. atomic or d_step
     */
    QString getSequencePrefix(const model::Sequence &sequence);

private:
    QTextStream &m_stream;
    const QString m_baseIndent;
    const QString m_sequenceIndent;
    const QString m_indent;
};
}
