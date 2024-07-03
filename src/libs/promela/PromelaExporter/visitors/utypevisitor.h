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
#include <promela/PromelaModel/utype.h>

namespace promela::exporter {
/**
 * @brief Vistor for exporting Utype
 *
 * This visitor exports user defined type to textual representation.
 */
class UtypeVisitor
{
public:
    /**
     * @brief Constructor.
     *
     * @param stream  A stream to append textual representation.
     */
    UtypeVisitor(QTextStream &stream, QString indent);

    /**
     * @brief  Constructor
     *
     * @param utype Utype to visit
     */
    void visit(const model::Utype &utype);

    void operator()(const model::Declaration &declaration);

    void operator()(const model::ConditionalDeclaration &declaration);

private:
    QTextStream &m_stream;
    const QString m_indent;
};
}
