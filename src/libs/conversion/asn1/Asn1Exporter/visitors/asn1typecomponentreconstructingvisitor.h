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

class Asn1TypeComponentReconstructingVisitor : public Types::TypeReadingVisitor
{
public:
    Asn1TypeComponentReconstructingVisitor(QTextStream &outStream, int indent = 0);
    ~Asn1TypeComponentReconstructingVisitor() override = default;

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
    void valueForStraightType(const Types::Type &type);

    template<typename T>
    void valueForComplexType(const T &type, int indent);

    void addIndent(int indent);
    void addWord(const QString &word);
    void finishLine();
    void addLine(QString line, int indent = 0);

    QTextStream &m_outStream;
    int m_indent;
};

} // namespace Asn1Acn
