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
#include <asn1library/asn1/types/acnparameterizablecomposite.h>
#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/integeracnparams.h>
#include <asn1library/asn1/types/null.h>
#include <asn1library/asn1/types/numericstring.h>
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/typereadingvisitor.h>
#include <asn1library/asn1/types/userdefinedtype.h>

namespace Asn1Acn {

class AcnTypeComponentReconstructingVisitor : public Types::TypeReadingVisitor
{
public:
    AcnTypeComponentReconstructingVisitor(QTextStream &outStream, int indent = 0, QString presentWhenValue = QString());
    ~AcnTypeComponentReconstructingVisitor() override = default;

    void visit(const Types::Boolean &type) override;
    void visit(const Types::Null &type) override;
    void visit(const Types::BitString &type) override;
    void visit(const Types::OctetString &type) override;
    void visit(const Types::IA5String &type) override;
    void visit(const Types::NumericString &type) override;
    void visit(const Types::Enumerated &type) override;
    void visit(const Types::Choice &type) override;
    void visit(const Types::Sequence &type) override;
    void visit(const Types::SequenceOf &type) override;
    void visit(const Types::Real &type) override;
    void visit(const Types::LabelType &type) override;
    void visit(const Types::Integer &type) override;
    void visit(const Types::UserdefinedType &type) override;

private:
    void tryAppendIntegerAcnParams(const Types::IntegerAcnParameters &type, QStringList &params) const;
    void tryAppendAsciiStringParams(const Types::AsciiStringAcnParameters &type, QStringList &params) const;

    void tryAppendAlignToNext(const Types::Type &type, QStringList &params) const;
    void tryAppendTrueValue(const Types::Boolean &type, QStringList &params) const;
    void tryAppendFalseValue(const Types::Boolean &type, QStringList &params) const;
    void tryAppendPattern(const Types::Null &type, QStringList &params) const;
    void tryAppendTerminationPattern(const Types::AsciiStringAcnParameters &type, QStringList &params) const;

    void tryAppendPresentWhen(QStringList &params) const;
    void tryAppendSize(const Types::IntegerAcnParameters &type, QStringList &params) const;

    template<typename T>
    void tryAppendSize(const T &type, QStringList &params) const;
    template<typename T>
    void tryAppendEncoding(const T &type, QStringList &params) const;
    template<typename T>
    void tryAppendEndianness(const T &type, QStringList &params) const;

    template<typename T>
    void reconstructComplexType(const T &type, int indent);
    template<typename T>
    void appendParamsFromComplexType(const T &type);
    template<typename T>
    void reconstructComplexTypeComponents(const T &type, int indent);
    template<typename T>
    void reconstructComplexTypeParameters(const T &type);
    void reconstructComplexTypeArguments(const Types::UserdefinedType &type);

    QStringList beginParamsList();
    void endParamsList(QStringList &params);

    QString addIndent(int size) const;

    QTextStream &m_outStream;

    int m_indent;
    QString m_presentWhenValue;
};

} // namespace Asn1Acn
