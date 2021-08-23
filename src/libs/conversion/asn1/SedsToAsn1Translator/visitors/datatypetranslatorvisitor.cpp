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
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/real.h>
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
    qDebug() << "array";
}

void DataTypeTranslatorVisitor::operator()(const BinaryDataType &sedsType)
{
    qDebug() << "binary";
}

void DataTypeTranslatorVisitor::operator()(const BooleanDataType &sedsType)
{
    const auto &typeName = sedsType.name().value();

    auto type = std::make_unique<Asn1Acn::Types::Boolean>(typeName);
    translateBooleanEncoding(type.get(), sedsType);

    auto typeAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(typeName, typeName, Asn1Acn::SourceLocation(), std::move(type));

    m_definitions->addType(std::move(typeAssignment));
}

void DataTypeTranslatorVisitor::operator()(const ContainerDataType &sedsType)
{
    qDebug() << "container";
}

void DataTypeTranslatorVisitor::operator()(const EnumeratedDataType &sedsType)
{
    qDebug() << "enum";
}

void DataTypeTranslatorVisitor::operator()(const FloatDataType &sedsType)
{
    const auto &typeName = sedsType.name().value();

    auto type = std::make_unique<Asn1Acn::Types::Real>(typeName);
    std::visit(FloatRangeTranslatorVisitor { type->constraints() }, sedsType.range());
    translateFloatEncoding(type.get(), sedsType);

    auto typeAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(typeName, typeName, Asn1Acn::SourceLocation(), std::move(type));

    m_definitions->addType(std::move(typeAssignment));
}

void DataTypeTranslatorVisitor::operator()(const IntegerDataType &sedsType)
{
    const auto &typeName = sedsType.name().value();

    auto type = std::make_unique<Asn1Acn::Types::Integer>(typeName);
    std::visit(IntegerRangeTranslatorVisitor { type->constraints() }, sedsType.range());
    translateIntegerEncoding(type.get(), sedsType);

    auto typeAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(typeName, typeName, Asn1Acn::SourceLocation(), std::move(type));

    m_definitions->addType(std::move(typeAssignment));
}

void DataTypeTranslatorVisitor::operator()(const StringDataType &sedsType)
{
    qDebug() << "string";
}

void DataTypeTranslatorVisitor::operator()(const SubRangeDataType &sedsType)
{
    qDebug() << "subrange";
}

void DataTypeTranslatorVisitor::translateIntegerEncoding(
        Asn1Acn::Types::Integer *asn1Type, const IntegerDataType &sedsType) const
{
    if (const auto &encoding = sedsType.encoding(); encoding) {
        // clang-format off
        std::visit(overloaded {
            [&](seds::model::CoreIntegerEncoding coreEncoding) {
                translateCoreIntegerEncoding(asn1Type, coreEncoding);
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
        Asn1Acn::Types::Real *asn1Type, const FloatDataType &sedsType) const
{
    if (const auto &encoding = sedsType.encoding(); encoding) {
        // clang-format off
        std::visit(overloaded {
            [&](seds::model::CoreEncodingAndPrecision coreEncoding) {
                translateCoreEncodingAndPrecision(asn1Type, coreEncoding);
            }
        }, encoding->encoding());
        // clang-format on
        //
        asn1Type->setEndianness(convertByteOrder(encoding->byteOrder()));
    } else {
        asn1Type->setEndianness(Asn1Acn::Types::Endianness::unspecified);
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::unspecified);
    }
}

void DataTypeTranslatorVisitor::translateBooleanEncoding(
        Asn1Acn::Types::Boolean *asn1Type, const BooleanDataType &sedsType) const
{
    if (const auto &encoding = sedsType.encoding(); encoding) {
        translateFalseValue(asn1Type, encoding->falseValue());
    } else {
        asn1Type->setFalseValue("0");
    }
}

void DataTypeTranslatorVisitor::translateCoreIntegerEncoding(
        Asn1Acn::Types::Integer *asn1Type, seds::model::CoreIntegerEncoding coreEncoding) const
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
    case seds::model::CoreIntegerEncoding::OnesComplement:
    case seds::model::CoreIntegerEncoding::PackedBcd:
        throw UnsupportedValueException("CoreIntegerEncoding");
        break;
    default:
        throw UnhandledValueException("CoreIntegerEncoding");
        break;
    }
}

void DataTypeTranslatorVisitor::translateCoreEncodingAndPrecision(
        Asn1Acn::Types::Real *asn1Type, seds::model::CoreEncodingAndPrecision coreEncoding) const
{
    switch (coreEncoding) {
    case seds::model::CoreEncodingAndPrecision::IeeeSingle:
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::IEEE754_1985_32);
        break;
    case seds::model::CoreEncodingAndPrecision::IeeeDouble:
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::IEEE754_1985_64);
        break;
    case seds::model::CoreEncodingAndPrecision::IeeeQuad:
    case seds::model::CoreEncodingAndPrecision::MilstdSimple:
    case seds::model::CoreEncodingAndPrecision::MilstdExtended:
        throw UnsupportedValueException("CoreEncodingAndPrecision");
        break;
    default:
        throw UnhandledValueException("CoreEncodingAndPrecision");
        break;
    }
}

void DataTypeTranslatorVisitor::translateFalseValue(
        Asn1Acn::Types::Boolean *asn1Type, seds::model::FalseValue falseValue) const
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
