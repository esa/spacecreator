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

#include "visitors/declarationvisitor.h"

#include "visitors/datatypeprefixvisitor.h"
#include "visitors/datatypesuffixvisitor.h"

namespace conversion::tmc::exporter {
using ::tmc::promela::model::ArrayType;
using ::tmc::promela::model::Declaration;
using ::tmc::promela::model::UnsignedDataType;
using ::tmc::promela::model::UtypeRef;

DeclarationVisitor::DeclarationVisitor(QTextStream &stream, QString prefix)
    : m_stream(stream)
    , m_prefix(std::move(prefix))
{
}

void DeclarationVisitor::visit(const Declaration &declaration)
{
    m_stream << m_prefix;
    m_stream << getVisibilityString(declaration);
    DataTypePrefixVisitor prefixVisitor(m_stream);
    prefixVisitor.visit(declaration.getType());
    m_stream << " " << declaration.getName();
    DataTypeSuffixVisitor suffixVisitor(m_stream);
    suffixVisitor.visit(declaration.getType());
    m_stream << ";\n";
}

QString DeclarationVisitor::getVisibilityString(const Declaration &declaration)
{
    switch (declaration.getVisibility()) {
    case Declaration::Visibility::NORMAL:
        return "";
    case Declaration::Visibility::HIDDEN:
        return "hidden ";
    case Declaration::Visibility::SHOW:
        return "show ";
    }

    return "";
}

}
