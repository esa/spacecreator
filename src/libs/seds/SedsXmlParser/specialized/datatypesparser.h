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

#include <functional>
#include <seds/SedsModel/types/arraydatatype.h>
#include <seds/SedsModel/types/containerdatatype.h>
#include <seds/SedsModel/types/datatype.h>
#include <seds/SedsModel/types/encodings/floatdataencoding.h>
#include <seds/SedsModel/types/encodings/integerdataencoding.h>
#include <seds/SedsModel/types/encodings/stringdataencoding.h>
#include <seds/SedsModel/types/entries/errorcontrolentry.h>
#include <seds/SedsModel/types/floatdatatype.h>
#include <seds/SedsModel/types/integerdatatype.h>
#include <seds/SedsModel/types/ranges/derivedtyperange.h>

class QXmlStreamAttribute;
class QXmlStreamReader;

namespace seds::model {
class ArrayDataType;
class BinaryDataType;
class BooleanDataEncoding;
class BooleanDataType;
class DimensionSize;
class Entry;
class EnumeratedDataType;
class EnumeratedDataTypeRange;
class ExternalField;
class Field;
class FixedValueEntry;
class LengthEntry;
class ListEntry;
class MinMaxRange;
class Package;
class PaddingEntry;
class RootDataType;
class StringDataType;
class SubRangeDataType;
class ValueEnumeration;

enum class ByteOrder;
enum class FalseValue;
enum class FloatPrecisionRange;
enum class RangeType;
} // namespace seds::model

namespace seds::parser {

/**
 * @brief   SEDS parser for data types
 */
class DataTypesParser final
{
private:
    using DimensionAddingFunction = std::function<void(model::DimensionSize)>;
    using DataTypeAddingFunction = std::function<void(model::DataType)>;
    using EntryAddingFunction = std::function<void(model::ContainerDataType::EntryListType)>;

public:
    /**
     * @brief   Adds all data types to given package
     * @param   package     Package to which constructed DataTypes will be added
     */
    static auto readDataTypeSet(DataTypeAddingFunction addDataType, QXmlStreamReader &xmlReader) -> void;

    /**
     * @brief   Processes attribute for ExternalField
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForExternalField(model::ExternalField *object, const QXmlStreamAttribute &xmlReader) -> bool;
    /**
     * @brief   Processes element for ExternalField
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForExternalField(model::ExternalField *object, QXmlStreamReader &xmlReader) -> bool;
    /**
     * @brief   Processes attribute for Field
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForField(model::Field *object, const QXmlStreamAttribute &xmlReader) -> bool;
    /**
     * @brief   Processes element for Field
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForField(model::Field *object, QXmlStreamReader &xmlReader) -> bool;

private:
    /**
     * @brief   Constructs ArrayDataType object
     * @return  ArrayDataType
     */
    static auto readArrayDataType(QXmlStreamReader &xmlReader) -> model::ArrayDataType;
    /**
     * @brief   Constructs BinaryDataType object
     * @return  BinaryDataType
     */
    static auto readBinaryDataType(QXmlStreamReader &xmlReader) -> model::BinaryDataType;
    /**
     * @brief   Constructs BooleanDataType object
     * @return  BooleanDataType
     */
    static auto readBooleanDataType(QXmlStreamReader &xmlReader) -> model::BooleanDataType;
    /**
     * @brief   Constructs ContainerDataType object
     * @return  ContainerDataType
     */
    static auto readContainerDataType(QXmlStreamReader &xmlReader) -> model::ContainerDataType;
    /**
     * @brief   Constructs EnumeratedDataType object
     * @return  EnumeratedDataType
     */
    static auto readEnumeratedDataType(QXmlStreamReader &xmlReader) -> model::EnumeratedDataType;
    /**
     * @brief   Constructs FloatDataType object
     * @return  FloatDataType
     */
    static auto readFloatDataType(QXmlStreamReader &xmlReader) -> model::FloatDataType;
    /**
     * @brief   Constructs IntegerDataType object
     * @return  IntegerDataType
     */
    static auto readIntegerDataType(QXmlStreamReader &xmlReader) -> model::IntegerDataType;
    /**
     * @brief   Constructs StringDataType object
     * @return  StringDataType
     */
    static auto readStringDataType(QXmlStreamReader &xmlReader) -> model::StringDataType;
    /**
     * @brief   Constructs SubRangeDataType object
     * @return  SubRangeDataType
     */
    static auto readSubRangeDataType(QXmlStreamReader &xmlReader) -> model::SubRangeDataType;

    /**
     * @brief   Constructs BooleanDataEncoding object
     * @return  BooleanDataEncoding
     */
    static auto readBooleanDataEncoding(QXmlStreamReader &xmlReader) -> model::BooleanDataEncoding;
    /**
     * @brief   Constructs FloatDataEncoding object
     * @return  FloatDataEncoding
     */
    static auto readFloatDataEncoding(QXmlStreamReader &xmlReader) -> model::FloatDataEncoding;
    /**
     * @brief   Constructs IntegerDataEncoding object
     * @return  IntegerDataEncoding
     */
    static auto readIntegerDataEncoding(QXmlStreamReader &xmlReader) -> model::IntegerDataEncoding;
    /**
     * @brief   Constructs StringDataEncoding object
     * @return  StringDataEncoding
     */
    static auto readStringDataEncoding(QXmlStreamReader &xmlReader) -> model::StringDataEncoding;

    /**
     * @brief   Constructs FloatDataRange object
     * @return  FloatDataRange
     */
    static auto readFloatDataRange(QXmlStreamReader &xmlReader) -> model::FloatDataType::Range;
    /**
     * @brief   Constructs IntegerDataRange object
     * @return  IntegerDataRange
     */
    static auto readIntegerDataRange(QXmlStreamReader &xmlReader) -> model::IntegerDataType::Range;

    /**
     * @brief   Constructs DerivedTypeRange object
     * @return  DerivedTypeRange
     */
    static auto readDerivedTypeRange(QXmlStreamReader &xmlReader) -> model::DerivedTypeRange;
    /**
     * @brief   Constructs EnumeratedDataTypeRange object
     * @return  EnumeratedDataTypeRange
     */
    static auto readEnumeratedDataTypeRange(QXmlStreamReader &xmlReader) -> model::EnumeratedDataTypeRange;
    /**
     * @brief   Constructs MinMaxRange object
     * @return  MinMaxRange
     */
    static auto readMinMaxRange(QXmlStreamReader &xmlReader) -> model::MinMaxRange;

    /**
     * @brief   Adds all entries to given container data type
     * @param   containerDataType       EnumeratedDataType to which constructed entries will be added
     */
    static auto readEntryList(EntryAddingFunction addEntry, QXmlStreamReader &xmlReader) -> void;
    /**
     * @brief   Constructs Entry object
     * @return  Entry
     */
    static auto readEntry(QXmlStreamReader &xmlReader) -> model::Entry;
    /**
     * @brief   Constructs ErrorControlEntry object
     * @return  ErrorControlEntry
     */
    static auto readErrorControlEntry(QXmlStreamReader &xmlReader) -> model::ErrorControlEntry;
    /**
     * @brief   Constructs FixedValueEntry object
     * @return  FixedValueEntry
     */
    static auto readFixedValueEntry(QXmlStreamReader &xmlReader) -> model::FixedValueEntry;
    /**
     * @brief   Constructs LengthEntry object
     * @return  LengthEntry
     */
    static auto readLengthEntry(QXmlStreamReader &xmlReader) -> model::LengthEntry;
    /**
     * @brief   Constructs ListEntry object
     * @return  ListEntry
     */
    static auto readListEntry(QXmlStreamReader &xmlReader) -> model::ListEntry;
    /**
     * @brief   Constructs PaddingEntry object
     * @return  PaddingEntry
     */
    static auto readPaddingEntry(QXmlStreamReader &xmlReader) -> model::PaddingEntry;

    /**
     * @brief   Adds all enumeration options to given enumeration data type
     * @param   dataType    EnumeratedDataType to which constructed Enumerations will be added
     */
    static auto readEnumerationList(model::EnumeratedDataType &dataType, QXmlStreamReader &xmlReader) -> void;
    /**
     * @brief   Constructs Enumeration object
     * @return  Enumeration
     */
    static auto readEnumeration(QXmlStreamReader &xmlReader) -> model::ValueEnumeration;

    /**
     * @brief   Adds all dimensions to given array data type
     * @param   dataType    ArrayDataType to which constructed Dimensions will be added
     */
    static auto readDimensionList(DimensionAddingFunction dataType, QXmlStreamReader &xmlReader) -> void;
    /**
     * @brief   Constructs DimensionSize object
     * @return  DimensionSize
     */
    static auto readDimensionSize(QXmlStreamReader &xmlReader) -> model::DimensionSize;

    /**
     * @brief   Adds all constraints to given container data type
     * @param   dataType    ContainerDataType to which constructed Constraints will be added
     */
    static auto readConstraintSet(model::ContainerDataType &dataType, QXmlStreamReader &xmlReader) -> void;
    /**
     * @brief   Constructs ContainerRangeConstraint object
     * @return  ContainerRangeConstraint
     */
    static auto readRangeConstraint(QXmlStreamReader &xmlReader) -> model::ContainerRangeConstraint;
    /**
     * @brief   Constructs ContainerTypeConstraint object
     * @return  ContainerTypeConstraint
     */
    static auto readTypeConstraint(QXmlStreamReader &xmlReader) -> model::ContainerTypeConstraint;
    /**
     * @brief   Constructs ContainerValueConstraint object
     * @return  ContainerValueConstraint
     */
    static auto readValueConstraint(QXmlStreamReader &xmlReader) -> model::ContainerValueConstraint;

    /**
     * @brief   Parses given string as a float encoding
     * @param   encodingStr     String to parse
     * @return  Float encoding
     */
    static auto parseFloatEncoding(QStringRef encodingStr) -> model::FloatDataEncoding::Encoding;
    /**
     * @brief   Parses given string as a integer encoding
     * @param   encodingStr     String to parse
     * @return  Integer encoding
     */
    static auto parseIntegerEncoding(QStringRef encodingStr) -> model::IntegerDataEncoding::Encoding;
    /**
     * @brief   Parses given string as a string encoding
     * @param   encodingStr     String to parse
     * @return  String encoding
     */
    static auto parseStringEncoding(QStringRef encodingStr) -> model::StringDataEncoding::Encoding;

    /**
     * @brief   Parses current element as a float precision range
     * @param   rangeStr        String to parse
     * @return  Float precision range
     */
    static auto parseFloatPrecisionRange(QXmlStreamReader &xmlReader) -> model::FloatPrecisionRange;

    /**
     * @brief   Parses given string as a byte order
     * @param   rangeTypeStr    String to parse
     * @return  Byte order
     */
    static auto parseByteOrder(QStringRef byteOrderStr) -> model::ByteOrder;
    /**
     * @brief   Parses given string as a error control
     * @param   errorControlStr     String to parse
     * @return  Error control value
     */
    static auto parseErrorControl(QStringRef errorControlStr) -> model::ErrorControlEntry::ErrorControl;
    /**
     * @brief   Parses given string as a false value
     * @param   falseValueStr   String to parse
     * @return  False value
     */
    static auto parseFalseValue(QStringRef falseValueStr) -> model::FalseValue;
    /**
     * @brief   Parses given string as a range type
     * @param   rangeTypeStr    String to parse
     * @return  Range type
     */
    static auto parseRangeType(QStringRef rangeTypeStr) -> model::RangeType;

    /**
     * @brief   Processes attribute for ContainerConstraint
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForContainerConstraint(model::ContainerConstraint *object, const QXmlStreamAttribute &attribute)
            -> bool;
    /**
     * @brief   Processes element for ContainerConstraint
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForContainerConstraint(model::ContainerConstraint *object, QXmlStreamReader &xmlReader) -> bool;
    /**
     * @brief   Processes attribute for DataEncoding
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForDataEncoding(model::DataEncoding *object, const QXmlStreamAttribute &attribute) -> bool;
    /**
     * @brief   Processes element for DataEncoding
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForDataEncoding(model::DataEncoding *object, QXmlStreamReader &xmlReader) -> bool;
    /**
     * @brief   Processes attribute for Entry
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForEntry(model::Entry *object, const QXmlStreamAttribute &attribute) -> bool;
    /**
     * @brief   Processes element for Entry
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForEntry(model::Entry *object, QXmlStreamReader &xmlReader) -> bool;
    /**
     * @brief   Processes attribute for RootDataType
     *
     * @param   object          Object to change
     * @param   attribute       Attribute to check
     * @return  True if attribute was used, false otherwise
     */
    static auto processForRootDataType(model::RootDataType *object, const QXmlStreamAttribute &attribute) -> bool;
    /**
     * @brief   Processes element for RootDataType
     *
     * @param   object          Object to change
     * @param   xmlReader       XML reader
     * @return  True if element was used, false otherwise
     */
    static auto processForRootDataType(model::RootDataType *object, QXmlStreamReader &xmlReader) -> bool;
};

} // namespace seds::parser
