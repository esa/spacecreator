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

#include "datatypeexporter.h"

#include <conversion/common/export/exceptions.h>
#include <conversion/common/overloaded.h>

using namespace seds::model;
using conversion::exporter::UnsupportedElementException;

namespace seds::exporter {

auto DataTypeExporter::exportDataType(
        const model::DataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void
{
    std::visit(overloaded { [&setElement, &sedsDocument](const ArrayDataType &arrayType) {
                               DataTypeExporter::exportArrayDataType(arrayType, setElement, sedsDocument);
                           },
                       [&setElement, &sedsDocument](const BinaryDataType &binaryType) {
                           DataTypeExporter::exportBinaryDataType(binaryType, setElement, sedsDocument);
                       },
                       [&setElement, &sedsDocument](const BooleanDataType &booleanType) {
                           DataTypeExporter::exportBooleanDataType(booleanType, setElement, sedsDocument);
                       },
                       [&setElement, &sedsDocument](const ContainerDataType &containerType) {
                           DataTypeExporter::exportContainerDataType(containerType, setElement, sedsDocument);
                       },
                       [&setElement, &sedsDocument](const EnumeratedDataType &enumeratedType) {
                           DataTypeExporter::exportEnumeratedDataType(enumeratedType, setElement, sedsDocument);
                       },
                       [&setElement, &sedsDocument](const FloatDataType &floatType) {
                           DataTypeExporter::exportFloatDataType(floatType, setElement, sedsDocument);
                       },
                       [&setElement, &sedsDocument](const IntegerDataType &integerType) {
                           DataTypeExporter::exportIntegerDataType(integerType, setElement, sedsDocument);
                       },
                       [&setElement, &sedsDocument](const StringDataType &stringType) {
                           DataTypeExporter::exportStringDataType(stringType, setElement, sedsDocument);
                       },
                       [&setElement, &sedsDocument](const SubRangeDataType &subrangeType) {
                           DataTypeExporter::exportSubRangeDataType(subrangeType, setElement, sedsDocument);
                       } },
            dataType);
}

auto DataTypeExporter::exportArrayDataType(
        const model::ArrayDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void
{
    Q_UNUSED(dataType);
    Q_UNUSED(setElement);
    Q_UNUSED(sedsDocument);
    throw UnsupportedElementException("ArrayDataType");
}

auto DataTypeExporter::exportBinaryDataType(
        const model::BinaryDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void
{
    Q_UNUSED(dataType);
    Q_UNUSED(setElement);
    Q_UNUSED(sedsDocument);
    throw UnsupportedElementException("BinaryDataType");
}

auto DataTypeExporter::exportBooleanDataType(
        const model::BooleanDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void
{
    auto typeElement = sedsDocument.createElement(QStringLiteral("BooleanDataType"));
    typeElement.setAttribute(QStringLiteral("name"), dataType.nameStr());
    if (dataType.encoding().has_value()) {
        const auto &encoding = *dataType.encoding();
        auto encodingElement = sedsDocument.createElement(QStringLiteral("BooleanDataEncoding"));
        encodingElement.setAttribute(QStringLiteral("sizeInBits"), static_cast<qulonglong>(encoding.bits()));
        switch (encoding.falseValue()) {
        case FalseValue::ZeroIsFalse:
            encodingElement.setAttribute(QStringLiteral("falseValue"), QStringLiteral("zeroIsFalse"));
            break;
        case FalseValue::NonZeroIsFalse:
            encodingElement.setAttribute(QStringLiteral("falseValue"), QStringLiteral("nonZeroIsFalse"));
            break;
        }
        typeElement.appendChild(std::move(encodingElement));
    }
    setElement.appendChild(std::move(typeElement));
}

auto DataTypeExporter::exportContainerDataType(
        const model::ContainerDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void
{
    Q_UNUSED(dataType);
    Q_UNUSED(setElement);
    Q_UNUSED(sedsDocument);
    throw UnsupportedElementException("ContainerDataType");
}

auto DataTypeExporter::exportEnumeratedDataType(
        const model::EnumeratedDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void
{
    Q_UNUSED(dataType);
    Q_UNUSED(setElement);
    Q_UNUSED(sedsDocument);
    throw UnsupportedElementException("EnumeratedDataType");
}

auto DataTypeExporter::exportFloatDataType(
        const model::FloatDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void
{
    Q_UNUSED(dataType);
    Q_UNUSED(setElement);
    Q_UNUSED(sedsDocument);
    throw UnsupportedElementException("FloatDataType");
}

auto DataTypeExporter::exportIntegerDataType(
        const model::IntegerDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void
{
    auto typeElement = sedsDocument.createElement(QStringLiteral("IntegerDataType"));
    typeElement.setAttribute(QStringLiteral("name"), dataType.nameStr());
    if (dataType.encoding().has_value()) {
        const auto &encoding = *dataType.encoding();
        auto encodingElement = sedsDocument.createElement(QStringLiteral("IntegerDataEncoding"));
        encodingElement.setAttribute(QStringLiteral("sizeInBits"), static_cast<qulonglong>(encoding.bits()));

        std::visit(overloaded { [&encodingElement](const CoreIntegerEncoding &coreEncoding) {
            exportCoreIntegerEncoding(coreEncoding, encodingElement);
        } },
                encoding.encoding());

        switch (encoding.byteOrder()) {
        case ByteOrder::BigEndian:
            encodingElement.setAttribute(QStringLiteral("byteOrder"), QStringLiteral("bigEndian"));
            break;
        case ByteOrder::LittleEndian:
            encodingElement.setAttribute(QStringLiteral("byteOrder"), QStringLiteral("littleEndian"));
            break;
        }

        typeElement.appendChild(std::move(encodingElement));
    }

    std::visit(overloaded { [&typeElement, &sedsDocument](const MinMaxRange &range) {
        exportMinMaxRange(range, typeElement, sedsDocument);
    } },
            dataType.range());

    setElement.appendChild(std::move(typeElement));
}

auto DataTypeExporter::exportStringDataType(
        const model::StringDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void
{
    Q_UNUSED(dataType);
    Q_UNUSED(setElement);
    Q_UNUSED(sedsDocument);
    throw UnsupportedElementException("StringDataType");
}

auto DataTypeExporter::exportSubRangeDataType(
        const model::SubRangeDataType &dataType, QDomElement &setElement, QDomDocument &sedsDocument) -> void
{
    Q_UNUSED(dataType);
    Q_UNUSED(setElement);
    Q_UNUSED(sedsDocument);
    throw UnsupportedElementException("SubRangeDataType");
}

auto DataTypeExporter::exportCoreIntegerEncoding(model::CoreIntegerEncoding encoding, QDomElement &setElement) -> void
{
    switch (encoding) {
    case CoreIntegerEncoding::Unsigned:
        setElement.setAttribute(QStringLiteral("encoding"), QStringLiteral("unsigned"));
        break;
    case CoreIntegerEncoding::SignMagnitude:
        setElement.setAttribute(QStringLiteral("encoding"), QStringLiteral("signMagnitude"));
        break;
    case CoreIntegerEncoding::TwosComplement:
        setElement.setAttribute(QStringLiteral("encoding"), QStringLiteral("twosComplement"));
        break;
    case CoreIntegerEncoding::OnesComplement:
        setElement.setAttribute(QStringLiteral("encoding"), QStringLiteral("onesComplement"));
        break;
    case CoreIntegerEncoding::Bcd:
        setElement.setAttribute(QStringLiteral("encoding"), QStringLiteral("BCD"));
        break;
    case CoreIntegerEncoding::PackedBcd:
        setElement.setAttribute(QStringLiteral("encoding"), QStringLiteral("packedBCD"));
        break;
    }
    throw UnsupportedElementException("CoreIntegerEncoding");
}

auto DataTypeExporter::exportMinMaxRange(
        const model::MinMaxRange &range, QDomElement &setElement, QDomDocument &sedsDocument) -> void
{
    auto rangeElement = sedsDocument.createElement(QStringLiteral("Range"));
    auto minMaxRangeElement = sedsDocument.createElement(QStringLiteral("MinMaxRange"));
    switch (range.type()) {
    case RangeType::ExclusiveMinExclusiveMax:
        minMaxRangeElement.setAttribute(QStringLiteral("rangeType"), QStringLiteral("exclusiveMinExclusiveMax"));
        minMaxRangeElement.setAttribute(QStringLiteral("min"), range.min()->value());
        minMaxRangeElement.setAttribute(QStringLiteral("max"), range.max()->value());
        break;
    case RangeType::InclusiveMinInclusiveMax:
        minMaxRangeElement.setAttribute(QStringLiteral("rangeType"), QStringLiteral("inclusiveMinInclusiveMax"));
        minMaxRangeElement.setAttribute(QStringLiteral("min"), range.min()->value());
        minMaxRangeElement.setAttribute(QStringLiteral("max"), range.max()->value());
        break;
    case RangeType::InclusiveMinExclusiveMax:
        minMaxRangeElement.setAttribute(QStringLiteral("rangeType"), QStringLiteral("inclusiveMinExclusiveMax"));
        minMaxRangeElement.setAttribute(QStringLiteral("min"), range.min()->value());
        minMaxRangeElement.setAttribute(QStringLiteral("max"), range.max()->value());
        break;
    case RangeType::ExclusiveMinInclusiveMax:
        minMaxRangeElement.setAttribute(QStringLiteral("rangeType"), QStringLiteral("exclusiveMinInclusiveMax"));
        minMaxRangeElement.setAttribute(QStringLiteral("min"), range.min()->value());
        minMaxRangeElement.setAttribute(QStringLiteral("max"), range.max()->value());
        break;
    case RangeType::GreaterThan:
        minMaxRangeElement.setAttribute(QStringLiteral("rangeType"), QStringLiteral("greaterThan"));
        minMaxRangeElement.setAttribute(QStringLiteral("min"), range.min()->value());
        break;
    case RangeType::LessThan:
        minMaxRangeElement.setAttribute(QStringLiteral("rangeType"), QStringLiteral("lessThan"));
        minMaxRangeElement.setAttribute(QStringLiteral("max"), range.max()->value());
        break;
    case RangeType::AtLeast:
        minMaxRangeElement.setAttribute(QStringLiteral("rangeType"), QStringLiteral("atLeast"));
        minMaxRangeElement.setAttribute(QStringLiteral("min"), range.min()->value());
        break;
    case RangeType::AtMost:
        minMaxRangeElement.setAttribute(QStringLiteral("rangeType"), QStringLiteral("atMost"));
        minMaxRangeElement.setAttribute(QStringLiteral("max"), range.max()->value());
        break;
    }

    rangeElement.appendChild(std::move(minMaxRangeElement));
    setElement.appendChild(std::move(rangeElement));
}

} // namespace seds::exporter
