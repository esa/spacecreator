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

#pragma once

#include "context.h"
#include "specialized/arraydatatypetranslator.h"
#include "specialized/containerdatatypetranslator.h"

#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/integeracnparams.h>
#include <optional>
#include <seds/SedsModel/types/datatype.h>
#include <seds/SedsModel/types/encodings/coreencodingandprecision.h>
#include <seds/SedsModel/types/encodings/coreintegerencoding.h>
#include <seds/SedsModel/types/encodings/corestringencoding.h>
#include <seds/SedsModel/types/encodings/falsevalue.h>
#include <seds/SedsModel/types/encodings/integerdataencoding.h>

namespace conversion::asn1::translator {

/**
 * @brief   Translator visitor for SEDS data type
 *
 * Translated data type will be added to the passed ASN.1 Definitions
 */
class DataTypeTranslatorVisitor final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context                 Current translation context
     */
    explicit DataTypeTranslatorVisitor(Context &context);
    /**
     * @brief   Deleted copy constructor
     */
    DataTypeTranslatorVisitor(const DataTypeTranslatorVisitor &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    DataTypeTranslatorVisitor(DataTypeTranslatorVisitor &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    DataTypeTranslatorVisitor &operator=(const DataTypeTranslatorVisitor &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    DataTypeTranslatorVisitor &operator=(DataTypeTranslatorVisitor &&) = delete;

public:
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
    auto translateBitStringLength(
            const seds::model::BinaryDataType &sedsType, Asn1Acn::Types::BitString *asn1Type) const -> void;
    auto translateStringLength(const seds::model::StringDataType &sedsType, Asn1Acn::Types::IA5String *asn1Type) const
            -> void;
    auto translateBooleanEncoding(const seds::model::BooleanDataType &sedsType, Asn1Acn::Types::Boolean *asn1Type) const
            -> void;
    auto translateIntegerEncoding(const std::optional<seds::model::IntegerDataEncoding> &encoding,
            Asn1Acn::Types::IntegerAcnParameters *asn1Type) const -> void;
    auto translateFloatEncoding(const seds::model::FloatDataType &sedsType, Asn1Acn::Types::Real *asn1Type) const
            -> void;
    auto translateStringEncoding(const seds::model::StringDataType &sedsType, Asn1Acn::Types::IA5String *asn1Type) const
            -> void;
    auto translateEnumerationList(
            const seds::model::EnumeratedDataType &sedsType, Asn1Acn::Types::Enumerated *asn1Type) const -> void;
    auto translateIntegerSubRangeDataType(
            const seds::model::SubRangeDataType &sedsType, const seds::model::IntegerDataType &sedsBaseType) -> void;
    auto translateFloatSubRangeDataType(
            const seds::model::SubRangeDataType &sedsType, const seds::model::FloatDataType &sedsBaseType) -> void;
    auto translateEnumSubRangeDataType(
            const seds::model::SubRangeDataType &sedsType, const seds::model::EnumeratedDataType &sedsBaseType) -> void;

    auto translateCoreIntegerEncoding(seds::model::CoreIntegerEncoding coreEncoding,
            Asn1Acn::Types::IntegerAcnParameters *asn1Type) const -> void;
    auto translateCoreEncodingAndPrecision(seds::model::CoreEncodingAndPrecision coreEncoding, uint64_t bits,
            Asn1Acn::Types::Real *asn1Type) const -> void;
    auto translateCoreStringEncoding(
            seds::model::CoreStringEncoding coreEncoding, Asn1Acn::Types::IA5String *asn1Type) const -> void;
    auto translateFalseValue(seds::model::FalseValue falseValue, Asn1Acn::Types::Boolean *asn1Type) const -> void;

    auto convertByteOrder(seds::model::ByteOrder sedsByteOrder) const -> Asn1Acn::Types::Endianness;

private:
    Context &m_context;

    ArrayDataTypeTranslator m_arrayTranslator;
    ContainerDataTypeTranslator m_containerTranslator;
};

} // namespace conversion::asn1::translator
