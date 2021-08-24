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
#include <optional>

namespace Asn1Acn::Types {
class BitString;
class IntegerAcnParameters;
class Real;
class IA5String;
} // namespace Asn1Acn::Types

namespace seds::model {
class ArrayDataType;
class BinaryDataType;
class BooleanDataEncoding;
class BooleanDataType;
enum class ByteOrder;
class ContainerDataType;
enum class CoreEncodingAndPrecision;
enum class CoreIntegerEncoding;
enum class CoreStringEncoding;
class EnumeratedDataType;
enum class FalseValue;
class FloatDataEncoding;
class FloatDataType;
class IntegerDataEncoding;
class IntegerDataType;
class Name;
class StringDataEncoding;
class StringDataType;
class SubRangeDataType;
class ValueEnumeration;
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
    auto translateIntegerEncoding(const std::optional<seds::model::IntegerDataEncoding> &encoding,
            Asn1Acn::Types::IntegerAcnParameters *asn1Type) const -> void;
    auto translateFloatEncoding(const std::optional<seds::model::FloatDataEncoding> &encoding,
            Asn1Acn::Types::Real *asn1Type) const -> void;
    auto translateBooleanEncoding(const std::optional<seds::model::BooleanDataEncoding> &encoding,
            Asn1Acn::Types::Boolean *asn1Type) const -> void;
    auto translateStringEncoding(const std::optional<seds::model::StringDataEncoding> &encoding,
            Asn1Acn::Types::IA5String *asn1Type) const -> void;

    auto translateCoreIntegerEncoding(seds::model::CoreIntegerEncoding coreEncoding,
            Asn1Acn::Types::IntegerAcnParameters *asn1Type) const -> void;
    auto translateCoreEncodingAndPrecision(
            seds::model::CoreEncodingAndPrecision coreEncoding, Asn1Acn::Types::Real *asn1Type) const -> void;
    auto translateCoreStringEncoding(
            seds::model::CoreStringEncoding coreEncoding, Asn1Acn::Types::IA5String *asn1Type) const -> void;
    auto translateStringLength(const seds::model::StringDataType &sedsType, Asn1Acn::Types::IA5String *asn1Type) const
            -> void;
    auto translateBitStringLength(
            const seds::model::BinaryDataType &sedsType, Asn1Acn::Types::BitString *asn1Type) const -> void;
    auto translateEnumerationList(const std::vector<seds::model::ValueEnumeration> &items,
            Asn1Acn::Types::Enumerated *asn1Type) const -> void;
    auto translateFalseValue(seds::model::FalseValue falseValue, Asn1Acn::Types::Boolean *asn1Type) const -> void;

    auto convertByteOrder(seds::model::ByteOrder sedsByteOrder) const -> Asn1Acn::Types::Endianness;
};

} // namespace conversion::asn1::translator
