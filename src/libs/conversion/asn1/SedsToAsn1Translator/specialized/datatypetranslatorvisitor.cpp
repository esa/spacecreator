/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "specialized/datatypetranslatorvisitor.h"

#include "specialized/rangetranslatorvisitor.h"

#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/overloaded.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/package/package.h>

using conversion::Escaper;
using conversion::UnhandledValueException;
using conversion::UnsupportedValueException;
using conversion::translator::MissingAsn1TypeDefinitionException;
using conversion::translator::TranslationException;
using seds::model::ArrayDataType;
using seds::model::BinaryDataType;
using seds::model::BooleanDataType;
using seds::model::ContainerDataType;
using seds::model::DataType;
using seds::model::EnumeratedDataType;
using seds::model::EnumeratedDataTypeRange;
using seds::model::FloatDataType;
using seds::model::IntegerDataType;
using seds::model::StringDataType;
using seds::model::SubRangeDataType;

namespace conversion::asn1::translator::seds {

DataTypeTranslatorVisitor::DataTypeTranslatorVisitor(Context &context)
    : m_context(context)
    , m_arrayTranslator(m_context)
    , m_containerTranslator(m_context)
{
}

void DataTypeTranslatorVisitor::operator()(const ::seds::model::ArrayDataType &sedsType)
{
    m_arrayTranslator.translate(sedsType);
}

void DataTypeTranslatorVisitor::operator()(const ::seds::model::BinaryDataType &sedsType)
{
    const auto asn1TypeName = Escaper::escapeAsn1TypeName(sedsType.nameStr());
    auto asn1Type = std::make_unique<Asn1Acn::Types::BitString>(asn1TypeName);

    translateBitStringLength(sedsType, asn1Type.get());

    m_context.addAsn1Type(std::move(asn1Type), &sedsType);
}

void DataTypeTranslatorVisitor::operator()(const ::seds::model::BooleanDataType &sedsType)
{
    const auto asn1TypeName = Escaper::escapeAsn1TypeName(sedsType.nameStr());
    auto asn1Type = std::make_unique<Asn1Acn::Types::Boolean>(asn1TypeName);

    translateBooleanEncoding(sedsType, asn1Type.get());

    m_context.addAsn1Type(std::move(asn1Type), &sedsType);
}

void DataTypeTranslatorVisitor::operator()(const ::seds::model::ContainerDataType &sedsType)
{
    m_containerTranslator.translate(sedsType);
}

void DataTypeTranslatorVisitor::operator()(const ::seds::model::EnumeratedDataType &sedsType)
{
    const auto asn1TypeName = Escaper::escapeAsn1TypeName(sedsType.nameStr());
    auto asn1Type = std::make_unique<Asn1Acn::Types::Enumerated>(asn1TypeName);

    translateIntegerEncoding(sedsType.encoding(), asn1Type.get());
    translateEnumerationList(sedsType, asn1Type.get());

    m_context.addAsn1Type(std::move(asn1Type), &sedsType);
}

void DataTypeTranslatorVisitor::operator()(const ::seds::model::FloatDataType &sedsType)
{
    const auto asn1TypeName = Escaper::escapeAsn1TypeName(sedsType.nameStr());
    auto asn1Type = std::make_unique<Asn1Acn::Types::Real>(asn1TypeName);

    translateFloatEncoding(sedsType, asn1Type.get());

    RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue> rangeTranslator(asn1Type.get());
    std::visit(rangeTranslator, sedsType.range());

    m_context.addAsn1Type(std::move(asn1Type), &sedsType);
}

void DataTypeTranslatorVisitor::operator()(const ::seds::model::IntegerDataType &sedsType)
{
    const auto asn1TypeName = Escaper::escapeAsn1TypeName(sedsType.nameStr());
    auto asn1Type = std::make_unique<Asn1Acn::Types::Integer>(asn1TypeName);

    translateIntegerEncoding(sedsType.encoding(), asn1Type.get());

    RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue> rangeTranslator(asn1Type.get());
    std::visit(rangeTranslator, sedsType.range());

    m_context.addAsn1Type(std::move(asn1Type), &sedsType);
}

void DataTypeTranslatorVisitor::operator()(const ::seds::model::StringDataType &sedsType)
{
    const auto asn1TypeName = Escaper::escapeAsn1TypeName(sedsType.nameStr());
    auto asn1Type = std::make_unique<Asn1Acn::Types::IA5String>(asn1TypeName);

    translateStringLength(sedsType, asn1Type.get());
    translateStringEncoding(sedsType, asn1Type.get());

    m_context.addAsn1Type(std::move(asn1Type), &sedsType);
}

void DataTypeTranslatorVisitor::operator()(const ::seds::model::SubRangeDataType &sedsType)
{
    const auto baseType = m_context.findSedsType(sedsType.type());

    if (std::holds_alternative<IntegerDataType>(*baseType)) {
        translateIntegerSubRangeDataType(sedsType, std::get<::seds::model::IntegerDataType>(*baseType));
    } else if (std::holds_alternative<FloatDataType>(*baseType)) {
        translateFloatSubRangeDataType(sedsType, std::get<::seds::model::FloatDataType>(*baseType));
    } else if (std::holds_alternative<EnumeratedDataType>(*baseType)) {
        translateEnumSubRangeDataType(sedsType, std::get<::seds::model::EnumeratedDataType>(*baseType));
    } else {
        auto errorMessage =
                QString("SubRangeDataType \"%1\" references type \"%2\" that is neither numeric nor enumerated")
                        .arg(sedsType.nameStr())
                        .arg(sedsType.type().value().pathStr());
        throw TranslationException(std::move(errorMessage));
    }
}

void DataTypeTranslatorVisitor::translateBitStringLength(
        const BinaryDataType &sedsType, Asn1Acn::Types::BitString *asn1Type) const
{
    if (sedsType.bits() > static_cast<::seds::model::PositiveLong::Value>(
                                  std::numeric_limits<Asn1Acn::IntegerValue::Type>::max())) {
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
    if (sedsType.length() > static_cast<::seds::model::PositiveLong::Value>(
                                    std::numeric_limits<Asn1Acn::IntegerValue::Type>::max())) {
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

void DataTypeTranslatorVisitor::translateBooleanEncoding(
        const ::seds::model::BooleanDataType &sedsType, Asn1Acn::Types::Boolean *asn1Type) const
{
    if (const auto &encoding = sedsType.encoding(); encoding.has_value()) {
        asn1Type->setAcnSize(encoding->bits());
        translateFalseValue(encoding->falseValue(), asn1Type);
    } else {
        asn1Type->setFalseValue("0");
    }
}

void DataTypeTranslatorVisitor::translateIntegerEncoding(
        const std::optional<::seds::model::IntegerDataEncoding> &encoding,
        Asn1Acn::Types::IntegerAcnParameters *asn1Type) const
{
    if (encoding) {
        // clang-format off
        std::visit(overloaded {
            [&](::seds::model::CoreIntegerEncoding coreEncoding) {
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
        const ::seds::model::FloatDataType &sedsType, Asn1Acn::Types::Real *asn1Type) const
{
    if (const auto &encoding = sedsType.encoding(); encoding.has_value()) {
        // clang-format off
        std::visit(overloaded {
            [&](::seds::model::CoreEncodingAndPrecision coreEncoding) {
                translateCoreEncodingAndPrecision(coreEncoding, encoding->bits(), asn1Type);
            }
        }, encoding->encoding());
        // clang-format on
        asn1Type->setEndianness(convertByteOrder(encoding->byteOrder()));
    } else {
        asn1Type->setEndianness(Asn1Acn::Types::Endianness::unspecified);
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::unspecified);
    }
}

void DataTypeTranslatorVisitor::translateStringEncoding(
        const ::seds::model::StringDataType &sedsType, Asn1Acn::Types::IA5String *asn1Type) const
{
    if (const auto &encoding = sedsType.encoding(); encoding.has_value()) {
        // clang-format off
        std::visit(overloaded {
            [&](::seds::model::CoreStringEncoding coreEncoding) {
                translateCoreStringEncoding(coreEncoding, asn1Type);
            }
        }, encoding->encoding());
        // clang-format on

        if (encoding->terminationByte()) {
            asn1Type->setTerminationPattern(QString::fromStdString(std::to_string(*encoding->terminationByte())));
        }
    } else {
        asn1Type->setEncoding(Asn1Acn::Types::AsciiStringEncoding::unspecified);
    }
}

void DataTypeTranslatorVisitor::translateEnumerationList(
        const ::seds::model::EnumeratedDataType &sedsType, Asn1Acn::Types::Enumerated *asn1Type) const
{
    const auto &items = sedsType.enumerationList();

    for (std::size_t index = 0; index < items.size(); ++index) {
        const auto &item = items[index];
        const auto name = Escaper::escapeAsn1FieldName(item.label().value());
        const auto enumeratedItem =
                Asn1Acn::Types::EnumeratedItem(index, name, item.value(), Asn1Acn::SourceLocation());

        asn1Type->addItem(enumeratedItem);
    }
}

void DataTypeTranslatorVisitor::translateIntegerSubRangeDataType(
        const ::seds::model::SubRangeDataType &sedsType, const ::seds::model::IntegerDataType &sedsBaseType)
{
    if (!std::holds_alternative<::seds::model::MinMaxRange>(sedsType.range())) {
        auto errorMessage = QString("Only MinMaxRange can be used as a range in SubRangeDataType \"%1\" because it's "
                                    "base type is an integer")
                                    .arg(sedsType.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    const auto asn1TypeName = Escaper::escapeAsn1TypeName(sedsType.nameStr());
    auto asn1Type = std::make_unique<Asn1Acn::Types::Integer>(asn1TypeName);

    translateIntegerEncoding(sedsBaseType.encoding(), asn1Type.get());

    auto rangeTranslator = RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue>(asn1Type.get());
    std::visit(rangeTranslator, sedsType.range());

    m_context.addAsn1Type(std::move(asn1Type), &sedsType);
}

void DataTypeTranslatorVisitor::translateFloatSubRangeDataType(
        const ::seds::model::SubRangeDataType &sedsType, const ::seds::model::FloatDataType &sedsBaseType)
{
    if (std::holds_alternative<EnumeratedDataTypeRange>(sedsType.range())) {
        auto errorMessage = QString("EnumeratedDataTypeRange can't be used as a range in SubRangeDataType \"%1\" "
                                    "because it's base type is a float")
                                    .arg(sedsType.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    const auto asn1TypeName = Escaper::escapeAsn1TypeName(sedsType.nameStr());
    auto asn1Type = std::make_unique<Asn1Acn::Types::Real>(asn1TypeName);

    translateFloatEncoding(sedsBaseType, asn1Type.get());

    auto rangeTranslator = RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue>(asn1Type.get());
    std::visit(rangeTranslator, sedsType.range());

    m_context.addAsn1Type(std::move(asn1Type), &sedsType);
}

void DataTypeTranslatorVisitor::translateEnumSubRangeDataType(
        const ::seds::model::SubRangeDataType &sedsType, const ::seds::model::EnumeratedDataType &sedsBaseType)
{
    if (!std::holds_alternative<EnumeratedDataTypeRange>(sedsType.range())) {
        auto errorMessage = QString("Only EnumeratedDataTypeRange can be used as a range in SubRangeDataType "
                                    "\"%1\" because it's base type is an enum")
                                    .arg(sedsType.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    const auto asn1TypeName = Escaper::escapeAsn1TypeName(sedsType.nameStr());
    auto asn1Type = std::make_unique<Asn1Acn::Types::Enumerated>(asn1TypeName);

    translateIntegerEncoding(sedsBaseType.encoding(), asn1Type.get());

    const auto &enumRange = std::get<EnumeratedDataTypeRange>(sedsType.range());
    const auto &items = sedsBaseType.enumerationList();

    for (std::size_t index = 0; index < items.size(); ++index) {
        const auto &item = items[index];
        const auto itemName = item.label().value();

        if (!enumRange.contains(itemName)) {
            continue;
        }

        const auto enumeratedItem = Asn1Acn::Types::EnumeratedItem(
                index, Escaper::escapeAsn1FieldName(itemName), item.value(), Asn1Acn::SourceLocation());
        asn1Type->addItem(enumeratedItem);
    }

    if (asn1Type->items().empty()) {
        auto errorMessage = QString("EnumeratedDataTypeRange on SubRangeDataType \"%1\" doesn't allow for any items")
                                    .arg(sedsBaseType.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    m_context.addAsn1Type(std::move(asn1Type), &sedsType);
}

void DataTypeTranslatorVisitor::translateCoreIntegerEncoding(
        ::seds::model::CoreIntegerEncoding coreEncoding, Asn1Acn::Types::IntegerAcnParameters *asn1Type) const
{
    switch (coreEncoding) {
    case ::seds::model::CoreIntegerEncoding::Unsigned:
        asn1Type->setEncoding(Asn1Acn::Types::IntegerEncoding::pos_int);
        break;
    case ::seds::model::CoreIntegerEncoding::TwosComplement:
        asn1Type->setEncoding(Asn1Acn::Types::IntegerEncoding::twos_complement);
        break;
    case ::seds::model::CoreIntegerEncoding::Bcd:
        asn1Type->setEncoding(Asn1Acn::Types::IntegerEncoding::BCD);
        break;
    case ::seds::model::CoreIntegerEncoding::SignMagnitude:
        throw UnsupportedValueException("CoreIntegerEncoding", "SignMagnitude");
        break;
    case ::seds::model::CoreIntegerEncoding::OnesComplement:
        throw UnsupportedValueException("CoreIntegerEncoding", "OnesComplement");
        break;
    case ::seds::model::CoreIntegerEncoding::PackedBcd:
        throw UnsupportedValueException("CoreIntegerEncoding", "PackedBcd");
        break;
    default:
        throw UnhandledValueException("CoreIntegerEncoding");
        break;
    }
}

void DataTypeTranslatorVisitor::translateCoreEncodingAndPrecision(
        ::seds::model::CoreEncodingAndPrecision coreEncoding, uint64_t bits, Asn1Acn::Types::Real *asn1Type) const
{
    switch (coreEncoding) {
    case ::seds::model::CoreEncodingAndPrecision::IeeeSingle: {
        if (bits != 32) { // NOLINT(readability-magic-numbers)
            auto errorMessage = QString("Wrong number of bits specified (%1) for IEEE754_1985_32 encoding").arg(bits);
            throw TranslationException(std::move(errorMessage));
        }
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::IEEE754_1985_32);
    } break;
    case ::seds::model::CoreEncodingAndPrecision::IeeeDouble: {
        if (bits != 64) { // NOLINT(readability-magic-numbers)
            auto errorMessage = QString("Wrong number of bits specified (%1) for IEEE754_1985_64 encoding").arg(bits);
            throw TranslationException(std::move(errorMessage));
        }
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::IEEE754_1985_64);
    } break;
    case ::seds::model::CoreEncodingAndPrecision::IeeeQuad:
        throw UnsupportedValueException("CoreEncodingAndPrecision", "IeeeQuad");
        break;
    case ::seds::model::CoreEncodingAndPrecision::MilstdSimple:
        throw UnsupportedValueException("CoreEncodingAndPrecision", "MilstdSimple");
        break;
    case ::seds::model::CoreEncodingAndPrecision::MilstdExtended:
        throw UnsupportedValueException("CoreEncodingAndPrecision", "MilstdExtended");
        break;
    default:
        throw UnhandledValueException("CoreEncodingAndPrecision");
        break;
    }
}

void DataTypeTranslatorVisitor::translateCoreStringEncoding(
        ::seds::model::CoreStringEncoding coreEncoding, Asn1Acn::Types::IA5String *asn1Type) const
{
    switch (coreEncoding) {
    case ::seds::model::CoreStringEncoding::Ascii:
        asn1Type->setEncoding(Asn1Acn::Types::AsciiStringEncoding::ASCII);
        break;
    case ::seds::model::CoreStringEncoding::Utf8:
        throw UnsupportedValueException("CoreStringEncoding", "Utf8");
        break;
    default:
        throw UnhandledValueException("CoreStringEncoding");
        break;
    }
}

void DataTypeTranslatorVisitor::translateFalseValue(
        ::seds::model::FalseValue falseValue, Asn1Acn::Types::Boolean *asn1Type) const
{
    switch (falseValue) {
    case ::seds::model::FalseValue::ZeroIsFalse:
        asn1Type->setFalseValue(QString(asn1Type->acnSize(), '0'));
        break;
    case ::seds::model::FalseValue::NonZeroIsFalse:
        asn1Type->setTrueValue(QString(asn1Type->acnSize(), '0'));
        break;
    default:
        throw UnhandledValueException("FalseValue");
        break;
    }
}

Asn1Acn::Types::Endianness DataTypeTranslatorVisitor::convertByteOrder(::seds::model::ByteOrder sedsByteOrder) const
{
    switch (sedsByteOrder) {
    case ::seds::model::ByteOrder::BigEndian:
        return Asn1Acn::Types::Endianness::big;
    case ::seds::model::ByteOrder::LittleEndian:
        return Asn1Acn::Types::Endianness::little;
    default:
        throw UnhandledValueException("ByteOrder");
        break;
    }
}

} // namespace conversion::asn1::translator::seds
