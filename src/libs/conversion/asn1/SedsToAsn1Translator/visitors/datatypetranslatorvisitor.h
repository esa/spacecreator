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

#include <asn1library/asn1/definitions.h>

namespace Asn1Acn::Types {
class Integer;
class Real;
} // namespace Asn1Acn::Types

namespace seds::model {
class ArrayDataType;
class BinaryDataType;
class BooleanDataType;
enum class ByteOrder;
class ContainerDataType;
enum class CoreEncodingAndPrecision;
enum class CoreIntegerEncoding;
class EnumeratedDataType;
enum class FalseValue;
class FloatDataType;
class IntegerDataType;
class Name;
class StringDataType;
class SubRangeDataType;
} // namespace seds::model

namespace conversion::asn1::translator {

struct DataTypeTranslatorVisitor final {
    Asn1Acn::Definitions *m_definitions;

    auto operator()(const seds::model::ArrayDataType &sedsType) -> void;
    auto operator()(const seds::model::BinaryDataType &sedsType) -> void;
    auto operator()(const seds::model::BooleanDataType &sedsType) -> void;
    auto operator()(const seds::model::ContainerDataType &sedsType) -> void;
    auto operator()(const seds::model::EnumeratedDataType &sedsType) -> void;
    auto operator()(const seds::model::FloatDataType &sedsType) -> void;
    auto operator()(const seds::model::IntegerDataType &sedsType) -> void;
    auto operator()(const seds::model::StringDataType &sedsType) -> void;
    auto operator()(const seds::model::SubRangeDataType &sedsType) -> void;

private:
    auto translateIntegerEncoding(Asn1Acn::Types::Integer *asn1Type, const seds::model::IntegerDataType &sedsType) const
            -> void;
    auto translateFloatEncoding(Asn1Acn::Types::Real *asn1Type, const seds::model::FloatDataType &sedsType) const
            -> void;
    auto translateBooleanEncoding(Asn1Acn::Types::Boolean *asn1Type, const seds::model::BooleanDataType &sedsType) const
            -> void;

private:
    auto translateCoreIntegerEncoding(
            Asn1Acn::Types::Integer *asn1Type, seds::model::CoreIntegerEncoding coreEncoding) const -> void;
    auto translateCoreEncodingAndPrecision(
            Asn1Acn::Types::Real *asn1Type, seds::model::CoreEncodingAndPrecision coreEncoding) const -> void;
    auto translateFalseValue(Asn1Acn::Types::Boolean *asn1Type, seds::model::FalseValue falseValue) const -> void;
    auto convertByteOrder(seds::model::ByteOrder sedsByteOrder) const -> Asn1Acn::Types::Endianness;
};

} // namespace conversion::asn1::translator
