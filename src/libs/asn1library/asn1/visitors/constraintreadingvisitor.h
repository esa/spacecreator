/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Fuzzer - Tool for generating test cases
** based on ASN.1/ACN models and simulating malformed or malicious data.
**
** Tool was developed under a programme and funded by
** European Space Agency.
**
** This Tool is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Tool is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

namespace Asn1Acn {

class ConstraintReadingVisitor : public Types::TypeMutatingVisitor
{
public:
    ConstraintReadingVisitor(QXmlStreamReader &xmlReader)
        : m_xmlReader(xmlReader)
    {}

    void visit(Types::Boolean &type) override { readConstraints<BooleanValue>(type); }

    void visit(Types::Null &type) override { Q_UNUSED(type); }

    void visit(Types::BitString &type) override
    {
        readConstraints<BitStringValue>(type);
    }

    void visit(Types::OctetString &type) override
    {
        readConstraints<OctetStringValue>(type);
    }

    void visit(Types::IA5String &type) override { readConstraints<StringValue>(type); }

    void visit(Types::NumericString &type) override
    {
        readConstraints<StringValue>(type);
    }

    void visit(Types::Enumerated &type) override { readConstraints<EnumValue>(type); }

    void visit(Types::Choice &type) override { Q_UNUSED(type); }

    void visit(Types::Sequence &type) override { Q_UNUSED(type); }

    void visit(Types::SequenceOf &type) override
    {
        // any value constraint would be valid, only size matters
        readConstraints<IntegerValue>(type);
    }

    void visit(Types::Real &type) override { readConstraints<RealValue>(type); }

    void visit(Types::LabelType &type) override { Q_UNUSED(type); }

    void visit(Types::Integer &type) override { readConstraints<IntegerValue>(type); }

    void visit(Types::UserdefinedType &type) override { Q_UNUSED(type); }

private:
    template<typename T>
    void readConstraints(Constraints::WithConstraints<T> &type) const
    {
        AstXmlConstraintParser<T> parser(m_xmlReader, type.constraints());
        parser.parse();
    }

    QXmlStreamReader &m_xmlReader;
};

} // namespace Asn1Acn
