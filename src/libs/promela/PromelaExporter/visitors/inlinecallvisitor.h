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

#pragma once

#include <QTextStream>
#include <promela/PromelaModel/inlinecall.h>

namespace promela::exporter {
/**
 * @brief Visitor for exporting @link{model::InlineCall}
 */
class InlineCallVisitor final
{
public:
    /**
     * @brief Constructor
     *
     * @param stream stream to append exported expression
     */
    InlineCallVisitor(QTextStream &stream);

    /**
     * @brief Visit InlineCall
     *
     * @param inlineCall InlineCall to visit
     */
    void visit(const model::InlineCall &inlineCall);

private:
    QTextStream &m_stream;
};
}
