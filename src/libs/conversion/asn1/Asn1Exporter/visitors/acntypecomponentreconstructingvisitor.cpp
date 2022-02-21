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

#include "acntypecomponentreconstructingvisitor.h"

#include "typeconstraintsreconstructingvisitor.h"

using namespace Asn1Acn;

namespace {
const int INDENT_SIZE = 4;
} // namespace

AcnTypeComponentReconstructingVisitor::AcnTypeComponentReconstructingVisitor(
        QTextStream &outStream, int indent, QString presentWhenValue)
    : m_outStream(outStream)
    , m_indent(indent)
    , m_presentWhenValue(std::move(presentWhenValue))
{
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::Boolean &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);

    tryAppendTrueValue(type, params);
    tryAppendFalseValue(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::Null &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendPattern(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::BitString &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendSize(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::OctetString &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendSize(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::IA5String &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendSize(type, params);
    tryAppendAsciiStringParams(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::NumericString &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendSize(type, params);
    tryAppendAsciiStringParams(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::Enumerated &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendIntegerAcnParams(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::Choice &type)
{
    // TODO: append determinant, when it will be fixed
    reconstructComplexType(type, m_indent);
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::Sequence &type)
{
    reconstructComplexType(type, m_indent);
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::SequenceOf &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendSize(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::Real &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendEncoding(type, params);
    tryAppendEndianness(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::LabelType &type)
{
    Q_UNUSED(type);
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::Integer &type)
{
    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    tryAppendIntegerAcnParams(type, params);

    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::visit(const Types::UserdefinedType &type)
{
    reconstructComplexTypeArguments(type);

    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);
    endParamsList(params);
}

void AcnTypeComponentReconstructingVisitor::tryAppendIntegerAcnParams(
        const Types::IntegerAcnParameters &type, QStringList &params) const
{
    tryAppendSize(type, params);
    tryAppendEncoding(type, params);

    // Querying size on the tryAppendEndianness() function level is infesible, as it
    // requires a major rewrite (separate specializations for different types
    // and re-ordering the file so that instantiations are after specialization)
    const auto size = type.size();

    // NOLINTNEXTLINE
    if ((type.endianness() != Types::Endianness::little) || (size == 0) || ((size % 8 == 0) && (size > 8))) {
        // Apply endianess if it is not little
        // or if size is undefined
        // or if size is multibyte.
        // Otherwise ASN1SCC will complain.
        tryAppendEndianness(type, params);
    }
}

void AcnTypeComponentReconstructingVisitor::tryAppendAsciiStringParams(
        const Types::AsciiStringAcnParameters &type, QStringList &params) const
{
    tryAppendEncoding(type, params);
    tryAppendTerminationPattern(type, params);
}

void AcnTypeComponentReconstructingVisitor::tryAppendAlignToNext(const Types::Type &type, QStringList &params) const
{
    auto alignToNext = type.alignToNext();
    if (alignToNext != Types::AlignToNext::unspecified) {
        params << QStringLiteral("align-to-next ") + Types::Type::alignToNextToString(alignToNext);
    }
}

void AcnTypeComponentReconstructingVisitor::tryAppendTrueValue(const Types::Boolean &type, QStringList &params) const
{
    const auto &trueValue = type.trueValue();
    if (!trueValue.isEmpty()) {
        params << QStringLiteral("true-value ") + BitStringValue::asString(trueValue);
    }
}

void AcnTypeComponentReconstructingVisitor::tryAppendFalseValue(const Types::Boolean &type, QStringList &params) const
{
    const auto &falseValue = type.falseValue();
    if (!falseValue.isEmpty()) {
        params << QStringLiteral("false-value ") + BitStringValue::asString(falseValue);
    }
}

void AcnTypeComponentReconstructingVisitor::tryAppendPattern(const Types::Null &type, QStringList &params) const
{
    const auto &pattern = type.pattern();
    if (!pattern.isEmpty()) {
        params << QStringLiteral("pattern ") + BitStringValue::asString(pattern);
    }
}

void AcnTypeComponentReconstructingVisitor::tryAppendTerminationPattern(
        const Types::AsciiStringAcnParameters &type, QStringList &params) const
{
    const auto &terminationPattern = type.terminationPattern();
    if (!terminationPattern.isEmpty()) {
        params << QStringLiteral("termination-pattern ") + OctetStringValue::asString(terminationPattern);
    }
}

void AcnTypeComponentReconstructingVisitor::tryAppendPresentWhen(QStringList &params) const
{
    auto val = m_presentWhenValue;
    if (!val.isEmpty()) {
        params << QStringLiteral("present-when ") + val.replace(QStringLiteral(" = "), QStringLiteral(" == "));
    }
}

template<typename T>
void AcnTypeComponentReconstructingVisitor::tryAppendSize(const T &type, QStringList &params) const
{
    const auto size = type.acnSize();
    if (!size.isEmpty()) {
        params << QStringLiteral("size ") + size;
    }
}

void AcnTypeComponentReconstructingVisitor::tryAppendSize(
        const Types::IntegerAcnParameters &type, QStringList &params) const
{
    const auto size = type.size();
    if (size != 0) {
        params << QStringLiteral("size ") + QString::number(size);
    }
}

template<typename T>
void AcnTypeComponentReconstructingVisitor::tryAppendEncoding(const T &type, QStringList &params) const
{
    const auto encodingString = type.encodingToString(type.encoding());
    if (encodingString.isEmpty()) {
        return;
    }

    params << QStringLiteral("encoding ") + encodingString;
}

template<typename T>
void AcnTypeComponentReconstructingVisitor::tryAppendEndianness(const T &type, QStringList &params) const
{
    auto endianness = type.endianness();
    if (endianness != Types::Endianness::unspecified) {
        params.append(QStringLiteral("endianness ") + Types::Type::endiannessToString(endianness));
    }
}

template<typename T>
void AcnTypeComponentReconstructingVisitor::reconstructComplexType(const T &type, const int indent)
{
    reconstructComplexTypeParameters(type);

    auto params = beginParamsList();

    tryAppendAlignToNext(type, params);

    endParamsList(params);

    reconstructComplexTypeComponents(type, indent);
}

template<typename T>
void AcnTypeComponentReconstructingVisitor::reconstructComplexTypeComponents(const T &type, const int indent)
{
    if (type.components().empty()) {
        return;
    }

    m_outStream << QStringLiteral("\n") << addIndent(indent) << QStringLiteral("{") << QStringLiteral("\n");

    const auto &components = type.components();
    for (auto it = components.begin(); it != components.end(); it++) {
        m_outStream << addIndent(indent + INDENT_SIZE) << (*it)->definitionAsString() << QStringLiteral(" ");

        const auto &acnParameters = (*it)->acnParameters();
        if (!acnParameters.empty()) {
            m_outStream << QStringLiteral("<");

            for (auto param = acnParameters.begin(); param != acnParameters.end(); param++) {
                m_outStream << *param;

                if (std::next(param, 1) != acnParameters.end()) {
                    m_outStream << QStringLiteral(", ");
                }
            }

            m_outStream << QStringLiteral(">");
        }

        AcnTypeComponentReconstructingVisitor visitor(m_outStream, indent + INDENT_SIZE, (*it)->presentWhen());
        (*it)->type()->accept(visitor);

        if (std::next(it, 1) != components.end()) {
            m_outStream << QStringLiteral(",");
        }
        m_outStream << QStringLiteral("\n");
    }

    m_outStream << addIndent(indent) << QStringLiteral("}");
}

template<typename T>
void AcnTypeComponentReconstructingVisitor::reconstructComplexTypeParameters(const T &type)
{
    const auto &acnParameters = type.acnParameters();
    if (acnParameters.empty()) {
        return;
    }

    m_outStream << QStringLiteral("<");
    for (auto it = acnParameters.begin(); it != acnParameters.end(); it++) {
        m_outStream << (*it)->type() << QStringLiteral(":") << (*it)->name();

        if (std::next(it, 1) != acnParameters.end()) {
            m_outStream << QStringLiteral(", ");
        }
    }
    m_outStream << QStringLiteral("> ");
}

void AcnTypeComponentReconstructingVisitor::reconstructComplexTypeArguments(const Types::UserdefinedType &type)
{
    const auto &acnArguments = type.acnArguments();
    if (acnArguments.empty()) {
        return;
    }

    m_outStream << QStringLiteral("<");
    for (auto it = acnArguments.begin(); it != acnArguments.end(); it++) {
        m_outStream << (*it)->id();

        if (std::next(it, 1) != acnArguments.end()) {
            m_outStream << QStringLiteral(", ");
        }
    }

    m_outStream << QStringLiteral("> ");
}

QStringList AcnTypeComponentReconstructingVisitor::beginParamsList()
{
    m_outStream << QStringLiteral("[");
    return {};
}

void AcnTypeComponentReconstructingVisitor::endParamsList(QStringList &params)
{
    tryAppendPresentWhen(params);

    if (!params.isEmpty()) {
        m_outStream << params.join(QStringLiteral(", "));
    }

    m_outStream << QStringLiteral("]");
}

QString AcnTypeComponentReconstructingVisitor::addIndent(int size) const
{
    return QString(size, QChar(' '));
}
