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

#include "typevisitor.h"

#include "constraintvisitor.h"

#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/labeltype.h>
#include <asn1library/asn1/types/null.h>
#include <asn1library/asn1/types/numericstring.h>
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/types/arraydatatype.h>
#include <seds/SedsModel/types/binarydatatype.h>
#include <seds/SedsModel/types/booleandatatype.h>
#include <seds/SedsModel/types/containerdatatype.h>
#include <seds/SedsModel/types/datatype.h>
#include <seds/SedsModel/types/encodings/coreintegerencoding.h>
#include <seds/SedsModel/types/encodings/falsevalue.h>
#include <seds/SedsModel/types/enumerateddatatype.h>
#include <seds/SedsModel/types/floatdatatype.h>
#include <seds/SedsModel/types/integerdatatype.h>
#include <seds/SedsModel/types/ranges/minmaxrange.h>
#include <seds/SedsModel/types/stringdatatype.h>
#include <seds/SedsModel/types/subrangedatatype.h>

using Asn1Acn::BitStringValue;
using Asn1Acn::BooleanValue;
using Asn1Acn::EnumValue;
using Asn1Acn::IntegerValue;
using Asn1Acn::OctetStringValue;
using Asn1Acn::RealValue;
using Asn1Acn::StringValue;
using Asn1Acn::Types::BitString;
using Asn1Acn::Types::Boolean;
using Asn1Acn::Types::Choice;
using Asn1Acn::Types::Enumerated;
using Asn1Acn::Types::EnumeratedItem;
using Asn1Acn::Types::IA5String;
using Asn1Acn::Types::Integer;
using Asn1Acn::Types::LabelType;
using Asn1Acn::Types::Null;
using Asn1Acn::Types::NumericString;
using Asn1Acn::Types::OctetString;
using Asn1Acn::Types::Real;
using Asn1Acn::Types::Sequence;
using Asn1Acn::Types::SequenceOf;
using Asn1Acn::Types::UserdefinedType;

using conversion::UnsupportedValueException;
using conversion::translator::UnsupportedDataTypeException;

namespace conversion::seds::translator {

TypeVisitor::Context::Context(const Asn1Acn::Asn1Model *asn1Model, const Asn1Acn::Definitions *definitions,
        const QString name, ::seds::model::Package *sedsPackage)
    : m_asn1Model(asn1Model)
    , m_definitions(definitions)
    , m_name(name)
    , m_sedsPackage(sedsPackage)
{
}

auto TypeVisitor::Context::model() -> const Asn1Acn::Asn1Model *
{
    return m_asn1Model;
}
auto TypeVisitor::Context::definitions() -> const Asn1Acn::Definitions *
{
    return m_definitions;
}

auto TypeVisitor::Context::name() -> const QString
{
    return m_name;
}

auto TypeVisitor::Context::package() -> ::seds::model::Package *
{
    return m_sedsPackage;
}

TypeVisitor::TypeVisitor(Context &context)
    : m_context(context)
{
}

template<typename EncodingType>
static inline auto setEndianness(EncodingType &encoding, const Asn1Acn::Types::Endianness endianness) -> void
{
    switch (endianness) {
    case Asn1Acn::Types::Endianness::big:
        encoding.setByteOrder(::seds::model::ByteOrder::BigEndian);
        return;
    case Asn1Acn::Types::Endianness::little:
        encoding.setByteOrder(::seds::model::ByteOrder::LittleEndian);
        return;
    case Asn1Acn::Types::Endianness::unspecified:
        throw UnsupportedValueException("Endianness", "unspecified");
        return;
    }
}

template<typename EncodingType>
static inline auto setIntegerEncoding(EncodingType &encoding, const Asn1Acn::Types::IntegerEncoding integerEncoding)
        -> void
{
    switch (integerEncoding) {
    case Asn1Acn::Types::IntegerEncoding::pos_int:
        encoding.setEncoding(::seds::model::CoreIntegerEncoding::Unsigned);
        break;
    case Asn1Acn::Types::IntegerEncoding::twos_complement:
        encoding.setEncoding(::seds::model::CoreIntegerEncoding::TwosComplement);
        break;
    case Asn1Acn::Types::IntegerEncoding::ASCII:
        throw UnsupportedValueException("IntegerEncoding", "ASCII");
        break;
    case Asn1Acn::Types::IntegerEncoding::BCD:
        encoding.setEncoding(::seds::model::CoreIntegerEncoding::Bcd);
        break;
    case Asn1Acn::Types::IntegerEncoding::unspecified:
        throw UnsupportedValueException("IntegerEncoding", "unspecified");
        break;
    }
}

static inline auto isZero(const QString value) -> bool
{
    return value.isEmpty() || value.toULongLong() == 0;
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Boolean &type)
{
    ConstraintVisitor<BooleanValue> constraintVisitor;

    type.constraints().accept(constraintVisitor);

    ::seds::model::BooleanDataType sedsType;

    if (constraintVisitor.isSizeConstraintVisited() || !type.trueValue().isEmpty() || !type.falseValue().isEmpty()) {
        if (constraintVisitor.isVariableSize()) {
            throw UnsupportedDataTypeException("variable size Boolean");
        }

        ::seds::model::BooleanDataEncoding encoding;
        encoding.setBits(constraintVisitor.getMaxSize());
        if (type.trueValue() != "") {
            encoding.setFalseValue(isZero(type.trueValue()) ? ::seds::model::FalseValue::NonZeroIsFalse
                                                            : ::seds::model::FalseValue::ZeroIsFalse);
        } else if (type.falseValue() != "") {
            encoding.setFalseValue(isZero(type.falseValue()) ? ::seds::model::FalseValue::ZeroIsFalse
                                                             : ::seds::model::FalseValue::NonZeroIsFalse);
        }

        sedsType.setEncoding(std::move(encoding));
    }

    sedsType.setName(m_context.name());
    m_context.package()->addDataType(std::move(sedsType));
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Null &type)
{
    Q_UNUSED(type);
    throw UnsupportedDataTypeException("Null");
}

void TypeVisitor::visit(const ::Asn1Acn::Types::BitString &type)
{
    ConstraintVisitor<BitStringValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);

    ::seds::model::BinaryDataType sedsType;

    if (constraintVisitor.isSizeConstraintVisited()) {
        sedsType.setBits(constraintVisitor.getMaxSize());
        sedsType.setFixedSize(!constraintVisitor.isVariableSize());
    }

    sedsType.setName(m_context.name());
    m_context.package()->addDataType(std::move(sedsType));
}

void TypeVisitor::visit(const ::Asn1Acn::Types::OctetString &type)
{
    Q_UNUSED(type);
    throw UnsupportedDataTypeException("OctetString");
}

void TypeVisitor::visit(const ::Asn1Acn::Types::IA5String &type)
{
    ConstraintVisitor<StringValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);

    ::seds::model::StringDataType sedsType;

    if (constraintVisitor.isSizeConstraintVisited()) {
        sedsType.setLength(constraintVisitor.getMaxSize());
        sedsType.setFixedLength(!constraintVisitor.isVariableSize());
    }
    ::seds::model::StringDataEncoding encoding;
    // The closest encoding to IA5
    encoding.setEncoding(::seds::model::CoreStringEncoding::Ascii);

    sedsType.setEncoding(std::move(encoding));
    sedsType.setName(m_context.name());
    m_context.package()->addDataType(std::move(sedsType));
}

void TypeVisitor::visit(const ::Asn1Acn::Types::NumericString &type)
{
    Q_UNUSED(type);
    throw UnsupportedDataTypeException("NumericString");
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Enumerated &type)
{
    ConstraintVisitor<EnumValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);

    ::seds::model::EnumeratedDataType sedsType;

    // Encoding in ASN.1 model is not optional, but may be unset
    if (type.size() > 0) {
        ::seds::model::IntegerDataEncoding encoding;
        encoding.setBits(static_cast<uint64_t>(type.size()));

        setEndianness(encoding, type.endianness());
        setIntegerEncoding(encoding, type.encoding());

        sedsType.setEncoding(std::move(encoding));
    }

    int computedValue = 0;
    for (const auto &item : type.items()) {
        ::seds::model::ValueEnumeration valueEnumeration;
        valueEnumeration.setLabel(item.name());
        valueEnumeration.setValue(type.encodeValues() ? item.value() : computedValue++);
        sedsType.addEnumeration(std::move(valueEnumeration));
    }

    sedsType.setName(m_context.name());
    m_context.package()->addDataType(std::move(sedsType));
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Choice &type)
{
    Q_UNUSED(type);
    throw UnsupportedDataTypeException("Choice");
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Sequence &type)
{
    Q_UNUSED(type);
    throw UnsupportedDataTypeException("Sequence");
}

void TypeVisitor::visit(const ::Asn1Acn::Types::SequenceOf &type)
{
    Q_UNUSED(type);
    throw UnsupportedDataTypeException("SequenceOf");
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Real &type)
{
    ConstraintVisitor<RealValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);

    ::seds::model::FloatDataType sedsType;

    if (constraintVisitor.isRangeConstraintVisited()) {
        ::seds::model::MinMaxRange range;
        range.setType(::seds::model::RangeType::InclusiveMinInclusiveMax);
        range.setMax(RealValue::asString(constraintVisitor.getRange().end()));
        range.setMin(RealValue::asString(constraintVisitor.getRange().begin()));
        sedsType.setRange(std::move(range));
    }

    // Encoding in ASN.1 model is not optional, but may be unset
    if (type.encoding() != Asn1Acn::Types::RealEncoding::unspecified) {
        ::seds::model::FloatDataEncoding encoding;
        switch (type.encoding()) {
        case Asn1Acn::Types::RealEncoding::IEEE754_1985_32:
            encoding.setEncoding(::seds::model::CoreEncodingAndPrecision::IeeeSingle);
            encoding.setBits(32);
            break;
        case Asn1Acn::Types::RealEncoding::IEEE754_1985_64:
            encoding.setEncoding(::seds::model::CoreEncodingAndPrecision::IeeeDouble);
            encoding.setBits(64);
            break;
        case Asn1Acn::Types::RealEncoding::unspecified:
            break;
        }

        setEndianness(encoding, type.endianness());

        sedsType.setEncoding(std::move(encoding));
    }
    sedsType.setName(m_context.name());
    m_context.package()->addDataType(std::move(sedsType));
}

void TypeVisitor::visit(const ::Asn1Acn::Types::LabelType &type)
{
    Q_UNUSED(type);
    throw UnsupportedDataTypeException("LabelType");
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Integer &type)
{
    ConstraintVisitor<IntegerValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);

    ::seds::model::IntegerDataType sedsType;

    if (constraintVisitor.isRangeConstraintVisited()) {
        ::seds::model::MinMaxRange range;
        range.setType(::seds::model::RangeType::InclusiveMinInclusiveMax);
        range.setMax(IntegerValue::asString(constraintVisitor.getRange().end()));
        range.setMin(IntegerValue::asString(constraintVisitor.getRange().begin()));
        sedsType.setRange(std::move(range));
    }

    // Encoding in ASN.1 model is not optional, but may be unset
    if (type.size() > 0) {
        ::seds::model::IntegerDataEncoding encoding;
        encoding.setBits(static_cast<uint64_t>(type.size()));

        setEndianness(encoding, type.endianness());
        setIntegerEncoding(encoding, type.encoding());

        sedsType.setEncoding(std::move(encoding));
    }
    sedsType.setName(m_context.name());
    m_context.package()->addDataType(std::move(sedsType));
}

void TypeVisitor::visit(const ::Asn1Acn::Types::UserdefinedType &type)
{
    Context context(m_context.model(), m_context.definitions(), m_context.name(), m_context.package());
    type.type()->accept(*this);
}

} // namespace conversion::seds::translator
