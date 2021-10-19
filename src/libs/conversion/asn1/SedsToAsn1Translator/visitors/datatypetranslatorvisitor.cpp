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

#include "visitors/datatypetranslatorvisitor.h"

#include "visitors/floatrangetranslatorvisitor.h"
#include "visitors/integerrangetranslatorvisitor.h"

#include <QDebug>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/types/arraydatatype.h>
#include <seds/SedsModel/types/binarydatatype.h>
#include <seds/SedsModel/types/booleandatatype.h>
#include <seds/SedsModel/types/containerdatatype.h>
#include <seds/SedsModel/types/enumerateddatatype.h>
#include <seds/SedsModel/types/floatdatatype.h>
#include <seds/SedsModel/types/integerdatatype.h>
#include <seds/SedsModel/types/stringdatatype.h>
#include <seds/SedsModel/types/subrangedatatype.h>

using conversion::translator::TranslationException;
using conversion::translator::UnhandledValueException;
using conversion::translator::UnsupportedValueException;
using seds::model::ArrayDataType;
using seds::model::BinaryDataType;
using seds::model::BooleanDataType;
using seds::model::ContainerDataType;
using seds::model::EnumeratedDataType;
using seds::model::FloatDataType;
using seds::model::IntegerDataType;
using seds::model::StringDataType;
using seds::model::SubRangeDataType;

namespace conversion::asn1::translator {

template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

void DataTypeTranslatorVisitor::operator()(const ArrayDataType &sedsType)
{
    Q_UNUSED(sedsType);
    throw TranslationException("ArrayDataType translation not implemented");
}

void DataTypeTranslatorVisitor::operator()(const BinaryDataType &sedsType)
{
    const auto &typeName = sedsType.name().value();

    auto type = std::make_unique<Asn1Acn::Types::BitString>(typeName);
    translateBitStringLength(sedsType, type.get());

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const BooleanDataType &sedsType)
{
    const auto &typeName = sedsType.name().value();

    auto type = std::make_unique<Asn1Acn::Types::Boolean>(typeName);
    translateBooleanEncoding(sedsType.encoding(), type.get());

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const ContainerDataType &sedsType)
{
    Q_UNUSED(sedsType);
    throw TranslationException("ContainerDataType translation not implemented");
}

void DataTypeTranslatorVisitor::operator()(const EnumeratedDataType &sedsType)
{
    const auto &typeName = sedsType.name().value();

    auto type = std::make_unique<Asn1Acn::Types::Enumerated>(typeName);
    translateIntegerEncoding(sedsType.encoding(), type.get());
    translateEnumerationList(sedsType.enumerationList(), type.get());

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const FloatDataType &sedsType)
{
    const auto &typeName = sedsType.name().value();

    auto type = std::make_unique<Asn1Acn::Types::Real>(typeName);
    std::visit(FloatRangeTranslatorVisitor { type->constraints() }, sedsType.range());
    translateFloatEncoding(sedsType.encoding(), type.get());

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const IntegerDataType &sedsType)
{
    const auto &typeName = sedsType.name().value();

    auto type = std::make_unique<Asn1Acn::Types::Integer>(typeName);
    std::visit(IntegerRangeTranslatorVisitor { type->constraints() }, sedsType.range());
    translateIntegerEncoding(sedsType.encoding(), type.get());

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const StringDataType &sedsType)
{
    const auto &typeName = sedsType.name().value();

    auto type = std::make_unique<Asn1Acn::Types::IA5String>(typeName);
    translateStringLength(sedsType, type.get());
    translateStringEncoding(sedsType.encoding(), type.get());

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const seds::model::SubRangeDataType &sedsType)
{
    Q_UNUSED(sedsType);
    throw TranslationException("SubRangeDataType translation not implemented");
}

void DataTypeTranslatorVisitor::translateIntegerEncoding(
        const std::optional<seds::model::IntegerDataEncoding> &encoding,
        Asn1Acn::Types::IntegerAcnParameters *asn1Type) const
{
    if (encoding) {
        // clang-format off
        std::visit(overloaded {
            [&](seds::model::CoreIntegerEncoding coreEncoding) {
                translateCoreIntegerEncoding(coreEncoding, asn1Type);
            }
        }, encoding->encoding());
        // clang-format on
        asn1Type->setEndianness(convertByteOrder(encoding->byteOrder()));
        asn1Type->setSize(encoding->bits());
    } else {
        asn1Type->setEncoding(Asn1Acn::Types::IntegerEncoding::unspecified);
        asn1Type->setEndianness(Asn1Acn::Types::Endianness::unspecified);
        asn1Type->setSize(0);
    }
}

void DataTypeTranslatorVisitor::translateFloatEncoding(
        const std::optional<seds::model::FloatDataEncoding> &encoding, Asn1Acn::Types::Real *asn1Type) const
{
    if (encoding) {
        // clang-format off
        std::visit(overloaded {
            [&](seds::model::CoreEncodingAndPrecision coreEncoding) {
                translateCoreEncodingAndPrecision(coreEncoding, asn1Type);
            }
        }, encoding->encoding());
        // clang-format on
        asn1Type->setEndianness(convertByteOrder(encoding->byteOrder()));
    } else {
        asn1Type->setEndianness(Asn1Acn::Types::Endianness::unspecified);
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::unspecified);
    }
}

void DataTypeTranslatorVisitor::translateBooleanEncoding(
        const std::optional<seds::model::BooleanDataEncoding> &encoding, Asn1Acn::Types::Boolean *asn1Type) const
{
    if (encoding) {
        translateFalseValue(encoding->falseValue(), asn1Type);
    } else {
        asn1Type->setFalseValue("0");
    }
}

void DataTypeTranslatorVisitor::translateStringEncoding(
        const std::optional<seds::model::StringDataEncoding> &encoding, Asn1Acn::Types::IA5String *asn1Type) const
{
    if (encoding) {
        // clang-format off
        std::visit(overloaded {
            [&](seds::model::CoreStringEncoding coreEncoding) {
                translateCoreStringEncoding(coreEncoding, asn1Type);
            }
        }, encoding->encoding());
        // clang-format on

        if (encoding->terminationByte()) {
            asn1Type->setTerminationPattern(QChar(*encoding->terminationByte()));
        }
    } else {
        asn1Type->setEncoding(Asn1Acn::Types::AsciiStringEncoding::unspecified);
    }
}

void DataTypeTranslatorVisitor::translateCoreIntegerEncoding(
        seds::model::CoreIntegerEncoding coreEncoding, Asn1Acn::Types::IntegerAcnParameters *asn1Type) const
{
    switch (coreEncoding) {
    case seds::model::CoreIntegerEncoding::Unsigned:
        asn1Type->setEncoding(Asn1Acn::Types::IntegerEncoding::pos_int);
        break;
    case seds::model::CoreIntegerEncoding::TwosComplement:
        asn1Type->setEncoding(Asn1Acn::Types::IntegerEncoding::twos_complement);
        break;
    case seds::model::CoreIntegerEncoding::Bcd:
        asn1Type->setEncoding(Asn1Acn::Types::IntegerEncoding::BCD);
        break;
    case seds::model::CoreIntegerEncoding::SignMagnitude:
        throw UnsupportedValueException("CoreIntegerEncoding", "SignMagnitude");
        break;
    case seds::model::CoreIntegerEncoding::OnesComplement:
        throw UnsupportedValueException("CoreIntegerEncoding", "OnesComplement");
        break;
    case seds::model::CoreIntegerEncoding::PackedBcd:
        throw UnsupportedValueException("CoreIntegerEncoding", "PackedBcd");
        break;
    default:
        throw UnhandledValueException("CoreIntegerEncoding");
        break;
    }
}

void DataTypeTranslatorVisitor::translateCoreEncodingAndPrecision(
        seds::model::CoreEncodingAndPrecision coreEncoding, Asn1Acn::Types::Real *asn1Type) const
{
    switch (coreEncoding) {
    case seds::model::CoreEncodingAndPrecision::IeeeSingle:
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::IEEE754_1985_32);
        break;
    case seds::model::CoreEncodingAndPrecision::IeeeDouble:
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::IEEE754_1985_64);
        break;
    case seds::model::CoreEncodingAndPrecision::IeeeQuad:
        throw UnsupportedValueException("CoreEncodingAndPrecision", "IeeeQuad");
        break;
    case seds::model::CoreEncodingAndPrecision::MilstdSimple:
        throw UnsupportedValueException("CoreEncodingAndPrecision", "MilstdSimple");
        break;
    case seds::model::CoreEncodingAndPrecision::MilstdExtended:
        throw UnsupportedValueException("CoreEncodingAndPrecision", "MilstdExtended");
        break;
    default:
        throw UnhandledValueException("CoreEncodingAndPrecision");
        break;
    }
}

void DataTypeTranslatorVisitor::translateCoreStringEncoding(
        seds::model::CoreStringEncoding coreEncoding, Asn1Acn::Types::IA5String *asn1Type) const
{
    switch (coreEncoding) {
    case seds::model::CoreStringEncoding::Ascii:
        asn1Type->setEncoding(Asn1Acn::Types::AsciiStringEncoding::ASCII);
        break;
    case seds::model::CoreStringEncoding::Utf8:
        throw UnsupportedValueException("CoreStringEncoding", "Utf8");
        break;
    default:
        throw UnhandledValueException("CoreStringEncoding");
        break;
    }
}

void DataTypeTranslatorVisitor::translateBitStringLength(
        const BinaryDataType &sedsType, Asn1Acn::Types::BitString *asn1Type) const
{
    if (sedsType.bits() > std::numeric_limits<Asn1Acn::IntegerValue::Type>::max()) {
        throw TranslationException("Bit string length size overflows ASN.1 range");
    }

    auto sizeConstraint = std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::BitStringValue>>();

    if (sedsType.hasFixedSize()) {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
                { static_cast<Asn1Acn::IntegerValue::Type>(sedsType.bits()) });
        sizeConstraint->setInnerConstraints(std::move(constraint));
    } else {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
                { 0, static_cast<Asn1Acn::IntegerValue::Type>(sedsType.bits()) });
        sizeConstraint->setInnerConstraints(std::move(constraint));
    }

    asn1Type->constraints().append(std::move(sizeConstraint));
}

void DataTypeTranslatorVisitor::translateStringLength(
        const StringDataType &sedsType, Asn1Acn::Types::IA5String *asn1Type) const
{
    if (sedsType.length() > std::numeric_limits<Asn1Acn::IntegerValue::Type>::max()) {
        throw TranslationException("String length size overflows ASN.1 range");
    }

    auto sizeConstraint = std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::StringValue>>();

    if (sedsType.hasFixedLength()) {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
                { static_cast<Asn1Acn::IntegerValue::Type>(sedsType.length()) });
        sizeConstraint->setInnerConstraints(std::move(constraint));
    } else {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
                { 0, static_cast<Asn1Acn::IntegerValue::Type>(sedsType.length()) });
        sizeConstraint->setInnerConstraints(std::move(constraint));
    }

    asn1Type->constraints().append(std::move(sizeConstraint));
}

void DataTypeTranslatorVisitor::translateEnumerationList(
        const std::vector<seds::model::ValueEnumeration> &items, Asn1Acn::Types::Enumerated *asn1Type) const
{
    for (std::size_t index = 0; index < items.size(); ++index) {
        const auto &item = items[index];

        asn1Type->addItem(
                Asn1Acn::Types::EnumeratedItem(index, item.label().value(), item.value(), Asn1Acn::SourceLocation()));
    }
}

void DataTypeTranslatorVisitor::translateFalseValue(
        seds::model::FalseValue falseValue, Asn1Acn::Types::Boolean *asn1Type) const
{
    switch (falseValue) {
    case seds::model::FalseValue::ZeroIsFalse:
        asn1Type->setFalseValue("0");
        break;
    case seds::model::FalseValue::NonZeroIsFalse:
        asn1Type->setTrueValue("0");
        break;
    default:
        throw UnhandledValueException("FalseValue");
        break;
    }
}

Asn1Acn::Types::Endianness DataTypeTranslatorVisitor::convertByteOrder(seds::model::ByteOrder sedsByteOrder) const
{
    switch (sedsByteOrder) {
    case seds::model::ByteOrder::BigEndian:
        return Asn1Acn::Types::Endianness::big;
    case seds::model::ByteOrder::LittleEndian:
        return Asn1Acn::Types::Endianness::little;
    default:
        throw UnhandledValueException("ByteOrder");
        break;
    }
}

} // namespace conversion::asn1::translator
