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

#include "sedsmodelbuilder.h"

using namespace seds::model;

namespace tests::conversion::common {

SedsModelBuilder::SedsModelBuilder(QString name)
{
    m_package.setName(std::move(name));
}

std::unique_ptr<SedsModel> SedsModelBuilder::build()
{
    PackageFile packageFile;
    packageFile.setPackage(std::move(m_package));

    return std::make_unique<SedsModel>(std::move(packageFile));
}

SedsModelBuilder &SedsModelBuilder::withBitStringDataType(QString name)
{
    BinaryDataType dataType;
    dataType.setName(std::move(name));
    dataType.setBits(42);
    dataType.setFixedSize(false);

    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withBooleanDataType(QString name)
{
    BooleanDataEncoding encoding;
    encoding.setBits(16);
    encoding.setFalseValue(FalseValue::NonZeroIsFalse);

    BooleanDataType dataType;
    dataType.setName(std::move(name));
    dataType.setEncoding(std::move(encoding));

    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withEnumeratedDataType(QString name, QStringList elems)
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

    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withFloatDataType(QString name)
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

    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withIntegerDataType(QString name)
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

    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withStringDataType(QString name)
{
    StringDataEncoding encoding;
    encoding.setEncoding(CoreStringEncoding::Ascii);
    encoding.setTerminationByte(88);

    StringDataType dataType;
    dataType.setName(std::move(name));
    dataType.setLength(20);
    dataType.setFixedLength(true);
    dataType.setEncoding(std::move(encoding));

    m_package.addDataType(std::move(dataType));

    return *this;
}

SedsModelBuilder &SedsModelBuilder::withComponent(Component component)
{
    m_package.addComponent(std::move(component));

    return *this;
}

} // namespace tests::conversion::common
