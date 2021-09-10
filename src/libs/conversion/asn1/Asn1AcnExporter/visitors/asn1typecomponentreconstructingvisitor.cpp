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

#include "asn1typecomponentreconstructingvisitor.h"

#include "typeconstraintsreconstructingvisitor.h"

#include <asn1library/asn1/constraints/printingvisitor.h>

using namespace Asn1Acn;

namespace {
const int INDENT_SIZE = 4;
} // namespace

Asn1TypeComponentReconstructingVisitor::Asn1TypeComponentReconstructingVisitor(QTextStream &outStream, int indent)
    : m_outStream(outStream)
    , m_indent(indent)
{
}

void Asn1TypeComponentReconstructingVisitor::visit(const Types::Boolean &type)
{
    valueForStraightType(type);
}
void Asn1TypeComponentReconstructingVisitor::visit(const Types::Null &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Types::BitString &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Types::OctetString &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Types::IA5String &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Types::NumericString &type)
{
    valueForStraightType(type);
}

namespace {
QList<Types::EnumeratedItem> itemsSortedByIndex(const Types::Enumerated &type)
{
    auto items = type.items().values();
    std::sort(items.begin(), items.end(),
            [](const Types::EnumeratedItem &a, const Types::EnumeratedItem &b) { return a.index() < b.index(); });
    return items;
}
} // namespace

void Asn1TypeComponentReconstructingVisitor::visit(const Types::Enumerated &type)
{
    m_outStream << type.typeName() << QStringLiteral("\n");
    m_outStream << QString(m_indent, QChar(' ')) << QStringLiteral("{") << QStringLiteral("\n");

    const auto items = itemsSortedByIndex(type);
    for (auto it = items.begin(); it != items.end(); it++) {
        m_outStream << QString(m_indent + INDENT_SIZE, QChar(' '));
        m_outStream << (*it).name();
        m_outStream << QStringLiteral("(");
        m_outStream << QString::number((*it).value());
        m_outStream << QStringLiteral(")");

        if (std::next(it, 1) != items.end())
            m_outStream << QStringLiteral(",");

        m_outStream << QStringLiteral("\n");
    }

    m_outStream << QString(m_indent, QChar(' ')) << QStringLiteral("}");
}

void Asn1TypeComponentReconstructingVisitor::visit(const Types::Choice &type)
{
    valueForComplexType<Types::Choice>(type, m_indent);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Types::Sequence &type)
{
    valueForComplexType<Types::Sequence>(type, m_indent);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Types::SequenceOf &type)
{
    m_outStream << QStringLiteral("SEQUENCE ") << toString(type.constraints()) << QStringLiteral(" OF ");

    Asn1TypeComponentReconstructingVisitor visitor(m_outStream, m_indent);
    type.itemsType().accept(visitor);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Types::Real &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Types::LabelType &type)
{
    Q_UNUSED(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Types::Integer &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::visit(const Types::UserdefinedType &type)
{
    valueForStraightType(type);
}

void Asn1TypeComponentReconstructingVisitor::valueForStraightType(const Types::Type &type)
{
    TypeConstraintsReconstructingVisitor visitor;
    type.accept(visitor);

    m_outStream << type.typeName() << visitor.value();
}

template<typename T>
void Asn1TypeComponentReconstructingVisitor::valueForComplexType(const T &type, const int indent)
{
    addLine(type.typeName());
    addLine(QStringLiteral("{"), indent);

    const auto &components = type.components();
    for (auto it = components.begin(); it != components.end(); it++) {
        addIndent(indent + INDENT_SIZE);
        addWord((*it)->name());

        Asn1TypeComponentReconstructingVisitor visitor(m_outStream, indent + INDENT_SIZE);
        (*it)->type()->accept(visitor);

        if (std::next(it) != components.end())
            addWord(QStringLiteral(","));

        finishLine();
    }

    addIndent(indent);
    addWord(QStringLiteral("}"));
}

void Asn1TypeComponentReconstructingVisitor::addIndent(int indent)
{
    m_outStream << QString(indent, QChar(' '));
}

void Asn1TypeComponentReconstructingVisitor::addWord(const QString &word)
{
    m_outStream << word << QStringLiteral(" ");
}

void Asn1TypeComponentReconstructingVisitor::finishLine()
{
    m_outStream << QStringLiteral("\n");
}

void Asn1TypeComponentReconstructingVisitor::addLine(const QString &line, int indent)
{
    addIndent(indent);
    addWord(line);
    finishLine();
}
