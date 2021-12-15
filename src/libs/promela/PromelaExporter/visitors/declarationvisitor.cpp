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

#include "declarationvisitor.h"

#include "datatypeprefixvisitor.h"
#include "datatypesuffixvisitor.h"

using promela::model::ArrayType;
using promela::model::ChannelInit;
using promela::model::Declaration;
using promela::model::UnsignedDataType;
using promela::model::UtypeRef;

namespace promela::exporter {

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

    if (declaration.hasInit()) {
        generateChannelInit(declaration.getInit().value());
    }

    m_stream << ";\n";
}

void DeclarationVisitor::generateChannelInit(const ChannelInit &channelInit)
{
    m_stream << " = [" << channelInit.getSize() << "] of {";

    bool first = true;
    for (const ChannelInit::Type &type : channelInit.getTypes()) {
        if (!first) {
            m_stream << ", ";
        }
        first = true;
        DataTypePrefixVisitor typeVisitor(m_stream);
        typeVisitor.visit(type);
    }

    m_stream << "}";
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
