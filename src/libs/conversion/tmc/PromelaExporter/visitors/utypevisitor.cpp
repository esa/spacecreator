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

#include "visitors/utypevisitor.h"

#include "visitors/declarationvisitor.h"

namespace conversion::tmc::exporter {
using ::tmc::promela::model::Declaration;
using ::tmc::promela::model::Utype;

UtypeVisitor::UtypeVisitor(QTextStream &stream, QString indent)
    : m_stream(stream)
    , m_indent(std::move(indent))
{
}

void UtypeVisitor::visit(const Utype &utype)
{
    if (utype.isUnionType()) {
        m_stream << "typedef union ";
    } else {
        m_stream << "typedef ";
    }
    m_stream << utype.getName() << " {\n";
    for (const Declaration &declaration : utype.getFields()) {
        DeclarationVisitor visitor(m_stream, m_indent);
        visitor.visit(declaration);
    }
    m_stream << "}\n\n";
}
}
