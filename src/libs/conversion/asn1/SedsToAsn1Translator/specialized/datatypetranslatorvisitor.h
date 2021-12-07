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

#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/type.h>
#include <conversion/common/qstringhash.h>
#include <optional>
#include <seds/SedsModel/types/datatype.h>
#include <unordered_map>

namespace Asn1Acn {
class Definitions;
namespace Types {
class BitString;
class IntegerAcnParameters;
class Real;
class IA5String;
} // namespace Types
} // namespace Asn1Acn

namespace seds::model {
class ArrayDataType;
class BinaryDataType;
class BooleanDataEncoding;
class BooleanDataType;
class Component;
class ContainerDataType;
class DataTypeRef;
class DimensionSize;
class EnumeratedDataType;
class FloatDataEncoding;
class FloatDataType;
class IntegerDataEncoding;
class IntegerDataType;
class Name;
class Package;
class StringDataEncoding;
class StringDataType;
class SubRangeDataType;
class ValueEnumeration;
enum class ByteOrder;
enum class CoreEncodingAndPrecision;
enum class CoreIntegerEncoding;
enum class CoreStringEncoding;
enum class FalseValue;
} // namespace seds::model

namespace conversion::asn1::translator {

/**
 * @brief   Translator visitor for SEDS data type
 *
 * Translated data type will be added to the passed ASN.1 Definitions
 */
struct DataTypeTranslatorVisitor final {
    /**
     * @brief   Constructor
     *
     * @param   asn1Definitions     Parent ASN.1 defintions
     * @param   asn1Type            Type where translated types are saved
     */
    DataTypeTranslatorVisitor(Asn1Acn::Definitions *asn1Definitions, std::unique_ptr<Asn1Acn::Types::Type> &asn1Type);

    /**
     * @brief   Translates SEDS array data type
     *
     * @param   sedsType    Type to translate
     */
    auto operator()(const seds::model::ArrayDataType &sedsType) -> void;
    /**
     * @brief   Translates SEDS binary data type
     *
     * @param   sedsType    Type to translate
     */
    auto operator()(const seds::model::BinaryDataType &sedsType) -> void;
    /**
     * @brief   Translates SEDS boolean data type
     *
     * @param   sedsType    Type to translate
     */
    auto operator()(const seds::model::BooleanDataType &sedsType) -> void;
    /**
     * @brief   Translates SEDS container data type
     *
     * @param   sedsType    Type to translate
     */
    auto operator()(const seds::model::ContainerDataType &sedsType) -> void;
    /**
     * @brief   Translates SEDS enumerated data type
     *
     * @param   sedsType    Type to translate
     */
    auto operator()(const seds::model::EnumeratedDataType &sedsType) -> void;
    /**
     * @brief   Translates SEDS float data type
     *
     * @param   sedsType    Type to translate
     */
    auto operator()(const seds::model::FloatDataType &sedsType) -> void;
    /**
     * @brief   Translates SEDS integer data type
     *
     * @param   sedsType    Type to translate
     */
    auto operator()(const seds::model::IntegerDataType &sedsType) -> void;
    /**
     * @brief   Translates SEDS string data type
     *
     * @param   sedsType    Type to translate
     */
    auto operator()(const seds::model::StringDataType &sedsType) -> void;
    /**
     * @brief   Translates SEDS subrange data type
     *
     * @param   sedsType    Type to translate
     */
    auto operator()(const seds::model::SubRangeDataType &sedsType) -> void;

private:
    /**
     * @brief   Translate SEDS integer encoding
     *
     * @param   encoding    Encoding to translate
     * @param   asn1Type    ASN.1 type that will be updated
     */
    auto translateIntegerEncoding(const std::optional<seds::model::IntegerDataEncoding> &encoding,
            Asn1Acn::Types::IntegerAcnParameters *asn1Type) const -> void;
    /**
     * @brief   Translate SEDS float encoding
     *
     * @param   encoding    Encoding to translate
     * @param   asn1Type    ASN.1 type that will be updated
     */
    auto translateFloatEncoding(const std::optional<seds::model::FloatDataEncoding> &encoding,
            Asn1Acn::Types::Real *asn1Type) const -> void;
    /**
     * @brief   Translate SEDS boolean encoding
     *
     * @param   encoding    Encoding to translate
     * @param   asn1Type    ASN.1 type that will be updated
     */
    auto translateBooleanEncoding(const std::optional<seds::model::BooleanDataEncoding> &encoding,
            Asn1Acn::Types::Boolean *asn1Type) const -> void;
    /**
     * @brief   Translate SEDS string encoding
     *
     * @param   encoding    Encoding to translate
     * @param   asn1Type    ASN.1 type that will be updated
     */
    auto translateStringEncoding(const std::optional<seds::model::StringDataEncoding> &encoding,
            Asn1Acn::Types::IA5String *asn1Type) const -> void;

    /**
     * @brief   Translate SEDS core integer encoding
     *
     * @param   coreEncoding    Encoding to translatoe
     * @param   asn1Type        ASN.1 type that will be updated
     */
    auto translateCoreIntegerEncoding(seds::model::CoreIntegerEncoding coreEncoding,
            Asn1Acn::Types::IntegerAcnParameters *asn1Type) const -> void;
    /**
     * @brief   Translate SEDS core float encoding and precision
     *
     * @param   coreEncoding    Encoding to translatoe
     * @param   asn1Type        ASN.1 type that will be updated
     */
    auto translateCoreEncodingAndPrecision(
            seds::model::CoreEncodingAndPrecision coreEncoding, Asn1Acn::Types::Real *asn1Type) const -> void;
    /**
     * @brief   Translate SEDS core string encoding
     *
     * @param   coreEncoding    Encoding to translatoe
     * @param   asn1Type        ASN.1 type that will be updated
     */
    auto translateCoreStringEncoding(
            seds::model::CoreStringEncoding coreEncoding, Asn1Acn::Types::IA5String *asn1Type) const -> void;
    /**
     * @brief   Translate SEDS string length
     *
     * @param   sedsType    Type to translate
     * @param   asn1Type    ASN.1 type that will be updated
     */
    auto translateStringLength(const seds::model::StringDataType &sedsType, Asn1Acn::Types::IA5String *asn1Type) const
            -> void;
    /**
     * @brief   Translate SEDS bit string length
     *
     * @param   sedsType    Type to translate
     * @param   asn1Type    ASN.1 type that will be updated
     */
    auto translateBitStringLength(
            const seds::model::BinaryDataType &sedsType, Asn1Acn::Types::BitString *asn1Type) const -> void;
    /**
     * @brief   Translate SEDS array type
     *
     * @param   sedsTypeName    SEDS array element type name
     * @param   asn1Type        ASN.1 type that will be updated
     */
    auto translateArrayType(const QString &sedsTypeName, Asn1Acn::Types::SequenceOf *asn1Type) const -> void;
    /**
     * @brief   Translate SEDS enumeration list
     *
     * @param   items       Enum items to translate
     * @param   asn1Type    ASN.1 type that will be updated
     */
    auto translateEnumerationList(const std::vector<seds::model::ValueEnumeration> &items,
            Asn1Acn::Types::Enumerated *asn1Type) const -> void;
    /**
     * @brief   Translate SEDS false value
     *
     * @param   falseValue      Value to translate
     * @param   asn1Type        ASN.1 type that will be updated
     */
    auto translateFalseValue(seds::model::FalseValue falseValue, Asn1Acn::Types::Boolean *asn1Type) const -> void;

    /**
     * @brief   Adds entries from given container data type to the entries cache
     *
     * @param   sedsType    Type which entries should be cached
     */
    auto cacheAbstractContainerEntries(const seds::model::ContainerDataType &sedsType) -> void;
    /**
     * @brief   Adds a choice field to the passed ASN.1 sequence for realization fields
     *
     * @param   asn1Sequence    Sequence to which field should be added
     */
    auto createRealizationContainerField(Asn1Acn::Types::Sequence *asn1Sequence) -> void;
    /**
     * @brief   Adds a reference to the realization in the given parent container
     *
     * @param   sedsBaseTypeName            Name of the base container which should be updated
     * @param   asn1RealizationSeuqnece     Sequence that is a realization of the based container
     */
    auto updateParentContainer(const QString &sedsBaseTypeName, Asn1Acn::Types::Sequence *asn1RealizationSequence)
            -> void;

    /**
     * @brief   Converts SEDS byte order
     *
     * @param   sedsByteOrder   Value to convert
     *
     * @return  ASN.1 endiannes
     */
    auto convertByteOrder(seds::model::ByteOrder sedsByteOrder) const -> Asn1Acn::Types::Endianness;

private:
    using ContainerEntriesCacheValue =
            std::pair<std::unique_ptr<Asn1Acn::Types::Sequence>, std::unique_ptr<Asn1Acn::Types::Sequence>>;
    using ContainerEntriesCacheMap = std::unordered_map<QString, ContainerEntriesCacheValue>;

    /// @brief  Parent definitions
    Asn1Acn::Definitions *m_asn1Definitions;
    /// @brief  Where translated data type will be saved
    std::unique_ptr<Asn1Acn::Types::Type> &m_asn1Type;
    /// @brief  Cache for sequence components
    ContainerEntriesCacheMap m_asn1SequenceComponentsCache;

    inline static const QString m_realizationComponentsName = "realization";
    inline static const QString m_realizationComponentsAlternativeNameTemplate = "realization%1";
};

} // namespace conversion::asn1::translator
