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

class TypeAttributesAssigningVisitor : public Types::TypeMutatingVisitor
{
public:
    TypeAttributesAssigningVisitor(const QXmlStreamAttributes &attributes)
        : m_attributes(attributes)
    {}

    ~TypeAttributesAssigningVisitor() override {}

    void visit(Types::Boolean &type) override
    {
        type.setTrueValue(m_attributes.value(QStringLiteral("true-value")).toString());
        type.setFalseValue(m_attributes.value(QStringLiteral("false-value")).toString());
    }

    void visit(Types::Null &type) override
    {
        type.setPattern(m_attributes.value(QStringLiteral("pattern")).toString());
    }

    void visit(Types::BitString &type) override
    {
        type.setAcnSize(m_attributes.value(QStringLiteral("size")).toString());
    }

    void visit(Types::OctetString &type) override
    {
        type.setAcnSize(m_attributes.value(QStringLiteral("size")).toString());
    }

    void visit(Types::IA5String &type) override
    {
        using namespace Types;

        type.setAcnSize(m_attributes.value(QStringLiteral("size")).toString());
        type.setTerminationPattern(
            m_attributes.value(QStringLiteral("termination-pattern")).toString());
        type.setEncoding(IA5String::mapEncoding(m_attributes.value(QStringLiteral("encoding"))));
    }

    void visit(Types::NumericString &type) override
    {
        using namespace Types;

        type.setAcnSize(m_attributes.value(QStringLiteral("size")).toString());
        type.setTerminationPattern(
            m_attributes.value(QStringLiteral("termination-pattern")).toString());
        type.setEncoding(NumericString::mapEncoding(m_attributes.value(QStringLiteral("encoding"))));
    }

    void visit(Types::Enumerated &type) override
    {
        setIntegerAcnParameters(type);

        const auto encodeValues = m_attributes.value(QStringLiteral("encode-values"));
        type.setEncodeValues(encodeValues.toString().toLower() == "true");
    }

    void visit(Types::Choice &type) override
    {
        type.setDeterminant(m_attributes.value(QStringLiteral("determinant")).toString());
    }

    void visit(Types::Sequence &type) override { Q_UNUSED(type); }

    void visit(Types::SequenceOf &type) override
    {
        type.setAcnSize(m_attributes.value(QStringLiteral("size")).toString());
    }

    void visit(Types::Real &type) override
    {
        using namespace Types;

        type.setEndianness(Type::mapEndianess(m_attributes.value(QStringLiteral("endianness"))));
        type.setEncoding(Real::mapEncoding(m_attributes.value(QStringLiteral("encoding"))));
    }

    void visit(Types::LabelType &type) override { Q_UNUSED(type); }

    void visit(Types::Integer &type) override { setIntegerAcnParameters(type); }

    void visit(Types::UserdefinedType &type) override { Q_UNUSED(type); }

private:
    void setIntegerAcnParameters(Types::IntegerAcnParameters &type)
    {
        using namespace Types;

        type.setSize(m_attributes.value(QStringLiteral("size")).toInt());
        type.setEndianness(Type::mapEndianess(m_attributes.value(QStringLiteral("endianness"))));
        type.setEncoding(Integer::mapEncoding(m_attributes.value(QStringLiteral("encoding"))));
        type.setAcnMinSizeInBits(m_attributes.value(QStringLiteral("acnMinSizeInBits")).toInt());
        type.setAcnMaxSizeInBits(m_attributes.value(QStringLiteral("acnMaxSizeInBits")).toInt());
    }

    const QXmlStreamAttributes &m_attributes;
};

} // namespace Asn1Acn
