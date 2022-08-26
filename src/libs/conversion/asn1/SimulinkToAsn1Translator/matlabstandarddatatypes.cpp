/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "matlabstandarddatatypes.h"

#include <asn1/constraints/rangeconstraint.h>

using Asn1Acn::IntegerValue;
using Asn1Acn::RealValue;
using Asn1Acn::Constraints::RangeConstraint;
using Asn1Acn::Types::Boolean;
using Asn1Acn::Types::Endianness;
using Asn1Acn::Types::Integer;
using Asn1Acn::Types::IntegerEncoding;
using Asn1Acn::Types::Real;
using Asn1Acn::Types::RealEncoding;

namespace conversion::asn1::translator {

// clang-format off
const QStringList MatLabStandardDataTypes::m_standardLogicalTypes = 
{
    "boolean"
};

const QStringList MatLabStandardDataTypes::m_standardNumericTypes = 
{
    "double", "single", 
    "int8", "uint8", 
    "int16", "uint16",
    "int32", "uint32"
};
// clang-format on

std::unique_ptr<Boolean> MatLabStandardDataTypes::getStandardAsn1BooleanDataType()
{
    const auto asn1TypeName = getStandardDataTypesPrefix() + "Boolean";
    auto asn1Type = std::make_unique<Boolean>(asn1TypeName);

    asn1Type->setTrueValue("1");
    asn1Type->setFalseValue("0");

    return std::move(asn1Type);
}

std::unique_ptr<Real> MatLabStandardDataTypes::getStandardAsn1DoubleDataType()
{
    const auto asn1TypeName = getStandardDataTypesPrefix() + "Double";
    auto asn1Type = std::make_unique<Real>(asn1TypeName);

    asn1Type->setEndianness(Endianness::unspecified);
    asn1Type->setEncoding(RealEncoding::IEEE754_1985_64);

    const auto min = std::numeric_limits<double>::lowest();
    const auto max = std::numeric_limits<double>::max();

    auto rangeConstraint = RangeConstraint<RealValue>::create({ min, max });
    asn1Type->constraints().append(std::move(rangeConstraint));

    return std::move(asn1Type);
}

std::unique_ptr<Real> MatLabStandardDataTypes::getStandardAsn1SingleDataType()
{
    const auto asn1TypeName = getStandardDataTypesPrefix() + "Single";
    auto asn1Type = std::make_unique<Real>(asn1TypeName);

    asn1Type->setEndianness(Endianness::unspecified);
    asn1Type->setEncoding(RealEncoding::IEEE754_1985_32);

    const auto min = static_cast<double>(std::numeric_limits<float>::lowest());
    const auto max = static_cast<double>(std::numeric_limits<float>::max());

    auto rangeConstraint = RangeConstraint<RealValue>::create({ min, max });
    asn1Type->constraints().append(std::move(rangeConstraint));

    return std::move(asn1Type);
}

std::unique_ptr<Integer> MatLabStandardDataTypes::getStandardAsn1Int8DataType()
{
    const auto asn1TypeName = getStandardDataTypesPrefix() + "Int8";
    auto asn1Type = std::make_unique<Integer>(asn1TypeName);

    asn1Type->setSize(8);
    asn1Type->setAcnMinSizeInBits(8);
    asn1Type->setAcnMaxSizeInBits(8);
    asn1Type->setEncoding(IntegerEncoding::twos_complement);
    asn1Type->setEndianness(Endianness::unspecified);

    auto rangeConstraint = RangeConstraint<IntegerValue>::create({ -128, 127 });
    asn1Type->constraints().append(std::move(rangeConstraint));

    return std::move(asn1Type);
}

std::unique_ptr<Integer> MatLabStandardDataTypes::getStandardAsn1UInt8DataType()
{
    const auto asn1TypeName = getStandardDataTypesPrefix() + "Uint8";
    auto asn1Type = std::make_unique<Integer>(asn1TypeName);

    asn1Type->setSize(8);
    asn1Type->setAcnMinSizeInBits(8);
    asn1Type->setAcnMaxSizeInBits(8);
    asn1Type->setEncoding(IntegerEncoding::pos_int);
    asn1Type->setEndianness(Endianness::unspecified);

    auto rangeConstraint = RangeConstraint<IntegerValue>::create({ 0, 255 });
    asn1Type->constraints().append(std::move(rangeConstraint));

    return std::move(asn1Type);
}

std::unique_ptr<Integer> MatLabStandardDataTypes::getStandardAsn1Int16DataType()
{
    const auto asn1TypeName = getStandardDataTypesPrefix() + "Int16";
    auto asn1Type = std::make_unique<Integer>(asn1TypeName);

    asn1Type->setSize(16);
    asn1Type->setAcnMinSizeInBits(16);
    asn1Type->setAcnMaxSizeInBits(16);
    asn1Type->setEncoding(IntegerEncoding::twos_complement);
    asn1Type->setEndianness(Endianness::unspecified);

    auto rangeConstraint = RangeConstraint<IntegerValue>::create({ -32768, 32767 });
    asn1Type->constraints().append(std::move(rangeConstraint));

    return std::move(asn1Type);
}

std::unique_ptr<Integer> MatLabStandardDataTypes::getStandardAsn1UInt16DataType()
{
    const auto asn1TypeName = getStandardDataTypesPrefix() + "Uint16";
    auto asn1Type = std::make_unique<Integer>(asn1TypeName);

    asn1Type->setSize(16);
    asn1Type->setAcnMinSizeInBits(16);
    asn1Type->setAcnMaxSizeInBits(16);
    asn1Type->setEncoding(IntegerEncoding::pos_int);
    asn1Type->setEndianness(Endianness::unspecified);

    auto rangeConstraint = RangeConstraint<IntegerValue>::create({ 0, 65535 });
    asn1Type->constraints().append(std::move(rangeConstraint));

    return std::move(asn1Type);
}

std::unique_ptr<Integer> MatLabStandardDataTypes::getStandardAsn1Int32DataType()
{
    const auto asn1TypeName = getStandardDataTypesPrefix() + "Int32";
    auto asn1Type = std::make_unique<Integer>(asn1TypeName);

    asn1Type->setSize(32);
    asn1Type->setAcnMinSizeInBits(32);
    asn1Type->setAcnMaxSizeInBits(32);
    asn1Type->setEncoding(IntegerEncoding::twos_complement);
    asn1Type->setEndianness(Endianness::unspecified);

    auto rangeConstraint = RangeConstraint<IntegerValue>::create({ -2147483648, 2147483647 });
    asn1Type->constraints().append(std::move(rangeConstraint));

    return std::move(asn1Type);
}

std::unique_ptr<Integer> MatLabStandardDataTypes::getStandardAsn1UInt32DataType()
{
    const auto asn1TypeName = getStandardDataTypesPrefix() + "Uint32";
    auto asn1Type = std::make_unique<Integer>(asn1TypeName);

    asn1Type->setSize(32);
    asn1Type->setAcnMinSizeInBits(32);
    asn1Type->setAcnMaxSizeInBits(32);
    asn1Type->setEncoding(IntegerEncoding::pos_int);
    asn1Type->setEndianness(Endianness::unspecified);

    auto rangeConstraint = RangeConstraint<IntegerValue>::create({ 0, 4294967295 });
    asn1Type->constraints().append(std::move(rangeConstraint));

    return std::move(asn1Type);
}

bool MatLabStandardDataTypes::isBooleanType(const QString &typeName)
{
    return m_standardLogicalTypes.contains(typeName, Qt::CaseSensitive);
}

bool MatLabStandardDataTypes::isNumericType(const QString &typeName)
{
    return m_standardNumericTypes.contains(typeName, Qt::CaseSensitive);
}

} // namespace conversion::asn1::translator
