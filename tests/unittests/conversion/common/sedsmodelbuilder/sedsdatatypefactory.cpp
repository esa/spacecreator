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

#include "sedsdatatypefactory.h"

#include "sedscontainerdatatypebuilder.h"
#include "types/arraydatatype.h"
#include "types/binarydatatype.h"
#include "types/booleandatatype.h"
#include "types/containerdatatype.h"
#include "types/enumerateddatatype.h"
#include "types/floatdatatype.h"
#include "types/integerdatatype.h"
#include "types/stringdatatype.h"
#include "types/subrangedatatype.h"

using namespace seds::model;

namespace tests::conversion::common {

ArrayDataType SedsDataTypeFactory::createArray(QString name, QString itemTypeName)
{
    DimensionSize dimension1;
    dimension1.setSize(3);

    DimensionSize dimension2;
    dimension2.setSize(4);

    ArrayDataType dataType;
    dataType.setName(std::move(name));
    dataType.addDimension(std::move(dimension1));
    dataType.addDimension(std::move(dimension2));
    dataType.setType(std::move(itemTypeName));

    return dataType;
}

BinaryDataType SedsDataTypeFactory::createBinary(QString name)
{
    BinaryDataType dataType;
    dataType.setName(std::move(name));
    dataType.setBits(42);
    dataType.setFixedSize(false);

    return dataType;
}

BooleanDataType SedsDataTypeFactory::createBoolean(QString name)
{
    BooleanDataEncoding encoding;
    encoding.setBits(16);
    encoding.setFalseValue(FalseValue::NonZeroIsFalse);

    BooleanDataType dataType;
    dataType.setName(std::move(name));
    dataType.setEncoding(std::move(encoding));

    return dataType;
}

EnumeratedDataType SedsDataTypeFactory::createEnumerated(QString name, QStringList elems)
{
    IntegerDataEncoding encoding;
    encoding.setByteOrder(ByteOrder::BigEndian);
    encoding.setEncoding(CoreIntegerEncoding::TwosComplement);
    encoding.setBits(8);

    EnumeratedDataType dataType;
    dataType.setName(std::move(name));
    dataType.setEncoding(std::move(encoding));

    int32_t index = 0;
    for (const auto &elem : elems) {
        ValueEnumeration value;
        value.setValue(++index);
        value.setLabel(elem);

        dataType.addEnumeration(std::move(value));
    }

    return dataType;
}

FloatDataType SedsDataTypeFactory::createFloat(QString name)
{
    FloatPrecisionRange range = FloatPrecisionRange::Double;

    FloatDataEncoding encoding;
    encoding.setByteOrder(ByteOrder::LittleEndian);
    encoding.setEncoding(CoreEncodingAndPrecision::IeeeDouble);
    encoding.setBits(64);

    FloatDataType dataType;
    dataType.setName(std::move(name));
    dataType.setRange(range);
    dataType.setEncoding(std::move(encoding));

    return dataType;
}

IntegerDataType SedsDataTypeFactory::createInteger(QString name)
{
    MinMaxRange range;
    range.setMin(QString("-42"));
    range.setMax(QString("42"));

    IntegerDataEncoding encoding;
    encoding.setByteOrder(ByteOrder::BigEndian);
    encoding.setEncoding(CoreIntegerEncoding::TwosComplement);
    encoding.setBits(16);

    IntegerDataType dataType;
    dataType.setName(std::move(name));
    dataType.setRange(std::move(range));
    dataType.setEncoding(std::move(encoding));

    return dataType;
}

StringDataType SedsDataTypeFactory::createString(QString name)
{
    StringDataEncoding encoding;
    encoding.setEncoding(CoreStringEncoding::Ascii);
    encoding.setTerminationByte(88);

    StringDataType dataType;
    dataType.setName(std::move(name));
    dataType.setLength(20);
    dataType.setFixedLength(true);
    dataType.setEncoding(std::move(encoding));

    return dataType;
}

} // namespace tests::conversion::common
