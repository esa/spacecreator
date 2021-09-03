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
#include <data/types/acnparameterizablecomposite.h>
#include <data/types/bitstring.h>
#include <data/types/boolean.h>
#include <data/types/choice.h>
#include <data/types/enumerated.h>
#include <data/types/ia5string.h>
#include <data/types/integer.h>
#include <data/types/integeracnparams.h>
#include <data/types/null.h>
#include <data/types/numericstring.h>
#include <data/types/octetstring.h>
#include <data/types/real.h>
#include <data/types/sequence.h>
#include <data/types/sequenceof.h>
#include <data/types/typereadingvisitor.h>
#include <data/types/userdefinedtype.h>

namespace Asn1Acn {

class Asn1TypeComponentReconstructingVisitor : public Data::Types::TypeReadingVisitor
{
public:
    Asn1TypeComponentReconstructingVisitor(QTextStream &outStream, int indent = 0);
    ~Asn1TypeComponentReconstructingVisitor() override = default;

    void visit(const Data::Types::Boolean &type) override;
    void visit(const Data::Types::Null &type) override;
    void visit(const Data::Types::BitString &type) override;
    void visit(const Data::Types::OctetString &type) override;
    void visit(const Data::Types::IA5String &type) override;
    void visit(const Data::Types::NumericString &type) override;
    void visit(const Data::Types::Enumerated &type) override;
    void visit(const Data::Types::Choice &type) override;
    void visit(const Data::Types::Sequence &type) override;
    void visit(const Data::Types::SequenceOf &type) override;
    void visit(const Data::Types::Real &type) override;
    void visit(const Data::Types::LabelType &type) override;
    void visit(const Data::Types::Integer &type) override;
    void visit(const Data::Types::UserdefinedType &type) override;

private:
    void valueForStraightType(const Data::Types::Type &type);

    template<typename T>
    void valueForComplexType(const T &type, const int indent);

    void addIndent(int indent);
    void addWord(const QString &word);
    void finishLine();
    void addLine(QString line, int indent = 0);

    QTextStream &m_outStream;
    int m_indent;
};

} // namespace Asn1Acn
