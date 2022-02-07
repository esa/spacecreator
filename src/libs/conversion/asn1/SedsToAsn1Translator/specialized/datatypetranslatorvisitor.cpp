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

#include "specialized/datatypetranslatorvisitor.h"

#include "specialized/containerconstrainttranslatorvisitor.h"
#include "specialized/dimensiontranslator.h"
#include "specialized/entrytranslatorvisitor.h"
#include "specialized/rangetranslatorvisitor.h"

#include <QDebug>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/overloaded.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/package/package.h>

using conversion::Escaper;
using conversion::UnhandledValueException;
using conversion::UnsupportedValueException;
using conversion::translator::MissingAsn1TypeDefinitionException;
using conversion::translator::TranslationException;
using seds::model::ArrayDataType;
using seds::model::BinaryDataType;
using seds::model::BooleanDataType;
using seds::model::ContainerDataType;
using seds::model::DataType;
using seds::model::EnumeratedDataType;
using seds::model::EnumeratedDataTypeRange;
using seds::model::FloatDataType;
using seds::model::IntegerDataType;
using seds::model::StringDataType;
using seds::model::SubRangeDataType;

namespace conversion::asn1::translator {

DataTypeTranslatorVisitor::DataTypeTranslatorVisitor(std::unique_ptr<Asn1Acn::Types::Type> &asn1Type,
        Asn1Acn::Definitions *asn1Definitions, const seds::model::Package *sedsPackage)
    : m_asn1Type(asn1Type)
    , m_asn1Definitions(asn1Definitions)
    , m_sedsPackage(sedsPackage)
{
}

void DataTypeTranslatorVisitor::operator()(const ArrayDataType &sedsType)
{
    const auto &dimensions = sedsType.dimensions();
    if (dimensions.empty()) {
        throw TranslationException("Encountered ArrayDataType without dimensions");
    }

    DimensionTranslator dimensionTranslator(m_sedsPackage);

    if (dimensions.size() == 1) { // Sequence of type with one dimension
        auto type = std::make_unique<Asn1Acn::Types::SequenceOf>(Escaper::escapeAsn1TypeName(sedsType.nameStr()));
        translateArrayType(Escaper::escapeAsn1TypeName(sedsType.type().nameStr()), type.get());
        dimensionTranslator.translateDimension(dimensions[0], type.get());

        m_asn1Type = std::move(type);
    } else { // Sequence of with many dimensions
        // The outermost 'sequence of' element
        auto rootType = std::make_unique<Asn1Acn::Types::SequenceOf>(Escaper::escapeAsn1TypeName(sedsType.nameStr()));
        dimensionTranslator.translateDimension(dimensions[0], rootType.get());

        // Create 'sequence of' chain
        auto *lastType = rootType.get();
        std::for_each(std::next(dimensions.begin()), dimensions.end(), [&](const auto &dimension) {
            auto subType = std::make_unique<Asn1Acn::Types::SequenceOf>();
            dimensionTranslator.translateDimension(dimension, subType.get());
            lastType->setItemsType(std::move(subType));

            lastType = dynamic_cast<Asn1Acn::Types::SequenceOf *>(lastType->itemsType());
        });

        // Add item type to the last element
        translateArrayType(Escaper::escapeAsn1TypeName(sedsType.type().nameStr()), lastType);

        m_asn1Type = std::move(rootType);
    }
}

void DataTypeTranslatorVisitor::operator()(const BinaryDataType &sedsType)
{
    auto type = std::make_unique<Asn1Acn::Types::BitString>(Escaper::escapeAsn1TypeName(sedsType.nameStr()));
    translateBitStringLength(sedsType, type.get());

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const BooleanDataType &sedsType)
{
    auto type = std::make_unique<Asn1Acn::Types::Boolean>(Escaper::escapeAsn1TypeName(sedsType.nameStr()));
    translateBooleanEncoding(sedsType.encoding(), type.get());

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const ContainerDataType &sedsType)
{
    auto type = std::make_unique<Asn1Acn::Types::Sequence>(Escaper::escapeAsn1TypeName(sedsType.nameStr()));

    // Add this type to the cache for future inheritance
    cacheContainerType(sedsType);

    // If type is not abstract, then we want it's entries in the ASN.1 type
    if (!sedsType.isAbstract()) {
        const auto &cachedAsn1Type = m_asn1SequenceComponentsCache[sedsType.nameStr()];

        const auto &cachedAsn1Components = cachedAsn1Type.first->components();
        for (const auto &asn1Component : cachedAsn1Components) {
            type->addComponent(asn1Component->clone());
        }

        const auto &cachedAsn1TrailerComponents = cachedAsn1Type.second->components();
        for (const auto &asn1TrailerComponent : cachedAsn1TrailerComponents) {
            type->addComponent(asn1TrailerComponent->clone());
        }
    }

    // Add realization to the parent component
    if (sedsType.baseType()) {
        applyContainerConstraints(sedsType, type.get());
        updateParentContainer(Escaper::escapeAsn1TypeName(sedsType.baseType()->nameStr()), type.get());
    }

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const EnumeratedDataType &sedsType)
{
    auto type = std::make_unique<Asn1Acn::Types::Enumerated>(Escaper::escapeAsn1TypeName(sedsType.nameStr()));
    translateIntegerEncoding(sedsType.encoding(), type.get());
    translateEnumerationList(sedsType.enumerationList(), type.get());

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const FloatDataType &sedsType)
{
    auto type = std::make_unique<Asn1Acn::Types::Real>(Escaper::escapeAsn1TypeName(sedsType.nameStr()));

    translateFloatEncoding(sedsType.encoding(), type.get());

    RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue> rangeTranslator(type.get());
    std::visit(rangeTranslator, sedsType.range());

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const IntegerDataType &sedsType)
{
    auto type = std::make_unique<Asn1Acn::Types::Integer>(Escaper::escapeAsn1TypeName(sedsType.nameStr()));

    translateIntegerEncoding(sedsType.encoding(), type.get());

    RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue> rangeTranslator(type.get());
    std::visit(rangeTranslator, sedsType.range());

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const StringDataType &sedsType)
{
    auto type = std::make_unique<Asn1Acn::Types::IA5String>(Escaper::escapeAsn1TypeName(sedsType.nameStr()));
    translateStringLength(sedsType, type.get());
    translateStringEncoding(sedsType.encoding(), type.get());

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const SubRangeDataType &sedsType)
{
    const auto &baseTypeName = sedsType.type().nameStr();
    const auto sedsBaseType = m_sedsPackage->dataType(baseTypeName);

    if (sedsBaseType == nullptr) {
        auto errorMessage = QString("SubRangeDataType \"%1\" references unknown type \"%2\"")
                                    .arg(sedsType.nameStr())
                                    .arg(baseTypeName);
        throw TranslationException(std::move(errorMessage));
    }

    if (std::holds_alternative<IntegerDataType>(*sedsBaseType)) {
        translateIntegerSubRangeDataType(sedsType, sedsBaseType);
    } else if (std::holds_alternative<FloatDataType>(*sedsBaseType)) {
        translateFloatSubRangeDataType(sedsType, sedsBaseType);
    } else if (std::holds_alternative<EnumeratedDataType>(*sedsBaseType)) {
        translateEnumSubRangeDataType(sedsType, sedsBaseType);
    } else {
        auto errorMessage =
                QString("SubRangeDataType \"%1\" references type \"%2\" that is neither numeric nor enumerated")
                        .arg(sedsType.nameStr())
                        .arg(baseTypeName);
        throw TranslationException(std::move(errorMessage));
    }
}

void DataTypeTranslatorVisitor::translateIntegerEncoding(
        const std::optional<seds::model::IntegerDataEncoding> &encoding,
        Asn1Acn::Types::IntegerAcnParameters *asn1Type) const
{
    if (encoding) {
        // clang-format off
        std::visit(overloaded {
            [&](seds::model::CoreIntegerEncoding coreEncoding) {
                translateCoreIntegerEncoding(coreEncoding, asn1Type);
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
        const std::optional<seds::model::FloatDataEncoding> &encoding, Asn1Acn::Types::Real *asn1Type) const
{
    if (encoding) {
        // clang-format off
        std::visit(overloaded {
            [&](seds::model::CoreEncodingAndPrecision coreEncoding) {
                translateCoreEncodingAndPrecision(coreEncoding, encoding->bits(), asn1Type);
            }
        }, encoding->encoding());
        // clang-format on
        asn1Type->setEndianness(convertByteOrder(encoding->byteOrder()));
    } else {
        asn1Type->setEndianness(Asn1Acn::Types::Endianness::unspecified);
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::unspecified);
    }
}

void DataTypeTranslatorVisitor::translateBooleanEncoding(
        const std::optional<seds::model::BooleanDataEncoding> &encoding, Asn1Acn::Types::Boolean *asn1Type) const
{
    if (encoding) {
        translateFalseValue(encoding->falseValue(), asn1Type);
    } else {
        asn1Type->setFalseValue("0");
    }
}

void DataTypeTranslatorVisitor::translateStringEncoding(
        const std::optional<seds::model::StringDataEncoding> &encoding, Asn1Acn::Types::IA5String *asn1Type) const
{
    if (encoding) {
        // clang-format off
        std::visit(overloaded {
            [&](seds::model::CoreStringEncoding coreEncoding) {
                translateCoreStringEncoding(coreEncoding, asn1Type);
            }
        }, encoding->encoding());
        // clang-format on

        if (encoding->terminationByte()) {
            asn1Type->setTerminationPattern(QString::fromStdString(std::to_string(*encoding->terminationByte())));
        }
    } else {
        asn1Type->setEncoding(Asn1Acn::Types::AsciiStringEncoding::unspecified);
    }
}

void DataTypeTranslatorVisitor::translateIntegerSubRangeDataType(
        const SubRangeDataType &sedsType, const DataType *sedsBaseType)
{
    if (!std::holds_alternative<seds::model::MinMaxRange>(sedsType.range())) {
        auto errorMessage = QString("Only MinMaxRange can be used as a range in SubRangeDataType \"%1\" because it's "
                                    "base type is an integer")
                                    .arg(sedsType.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    std::visit(*this, *sedsBaseType);

    auto asn1IntegerType = dynamic_cast<Asn1Acn::Types::Integer *>(m_asn1Type.get());
    asn1IntegerType->setIdentifier(sedsType.nameStr());
    asn1IntegerType->constraints().clear();

    auto rangeTranslator = RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue>(m_asn1Type.get());
    std::visit(rangeTranslator, sedsType.range());
}

void DataTypeTranslatorVisitor::translateFloatSubRangeDataType(
        const SubRangeDataType &sedsType, const DataType *sedsBaseType)
{
    if (std::holds_alternative<EnumeratedDataTypeRange>(sedsType.range())) {
        auto errorMessage = QString("EnumeratedDataTypeRange can't be used as a range in SubRangeDataType \"%1\" "
                                    "because it's base type is a float")
                                    .arg(sedsType.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    std::visit(*this, *sedsBaseType);

    auto asn1RealType = dynamic_cast<Asn1Acn::Types::Real *>(m_asn1Type.get());
    asn1RealType->setIdentifier(sedsType.nameStr());
    asn1RealType->constraints().clear();

    auto rangeTranslator = RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue>(m_asn1Type.get());
    std::visit(rangeTranslator, sedsType.range());
}

void DataTypeTranslatorVisitor::translateEnumSubRangeDataType(
        const SubRangeDataType &sedsType, const DataType *sedsBaseType)
{
    if (!std::holds_alternative<EnumeratedDataTypeRange>(sedsType.range())) {
        auto errorMessage = QString("Only EnumeratedDataTypeRange can be used as a range in SubRangeDataType "
                                    "\"%1\" because it's base type is an enum")
                                    .arg(sedsType.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    std::visit(*this, *sedsBaseType);

    auto asn1EnumType = dynamic_cast<Asn1Acn::Types::Enumerated *>(m_asn1Type.get());
    asn1EnumType->setIdentifier(sedsType.nameStr());

    const auto &sedsEnumType = std::get<EnumeratedDataType>(*sedsBaseType);
    const auto &sedsEnumRange = std::get<EnumeratedDataTypeRange>(sedsType.range());
    for (const auto &sedsEnumItem : sedsEnumType.enumerationList()) {
        const auto &sedsEnumItemName = sedsEnumItem.nameStr();

        if (!sedsEnumRange.contains(sedsEnumItemName)) {
            asn1EnumType->items().remove(Escaper::escapeAsn1FieldName(sedsEnumItemName));
        }
    }

    if (asn1EnumType->items().empty()) {
        auto errorMessage = QString("EnumeratedDataTypeRange on SubRangeDataType \"%1\" doesn't allow for any items")
                                    .arg(sedsEnumType.nameStr());
        throw TranslationException(std::move(errorMessage));
    }
}

void DataTypeTranslatorVisitor::translateCoreIntegerEncoding(
        seds::model::CoreIntegerEncoding coreEncoding, Asn1Acn::Types::IntegerAcnParameters *asn1Type) const
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
        throw UnsupportedValueException("CoreIntegerEncoding", "SignMagnitude");
        break;
    case seds::model::CoreIntegerEncoding::OnesComplement:
        throw UnsupportedValueException("CoreIntegerEncoding", "OnesComplement");
        break;
    case seds::model::CoreIntegerEncoding::PackedBcd:
        throw UnsupportedValueException("CoreIntegerEncoding", "PackedBcd");
        break;
    default:
        throw UnhandledValueException("CoreIntegerEncoding");
        break;
    }
}

void DataTypeTranslatorVisitor::translateCoreEncodingAndPrecision(
        seds::model::CoreEncodingAndPrecision coreEncoding, uint64_t bits, Asn1Acn::Types::Real *asn1Type) const
{
    switch (coreEncoding) {
    case seds::model::CoreEncodingAndPrecision::IeeeSingle: {
        if (bits != 32) { // NOLINT
            auto errorMessage = QString("Wrong number of bits specified (%1) for IEEE754_1985_32 encoding").arg(bits);
            throw TranslationException(std::move(errorMessage));
        }
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::IEEE754_1985_32);
    } break;
    case seds::model::CoreEncodingAndPrecision::IeeeDouble: {
        if (bits != 64) { // NOLINT
            auto errorMessage = QString("Wrong number of bits specified (%1) for IEEE754_1985_64 encoding").arg(bits);
            throw TranslationException(std::move(errorMessage));
        }
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::IEEE754_1985_64);
    } break;
    case seds::model::CoreEncodingAndPrecision::IeeeQuad:
        throw UnsupportedValueException("CoreEncodingAndPrecision", "IeeeQuad");
        break;
    case seds::model::CoreEncodingAndPrecision::MilstdSimple:
        throw UnsupportedValueException("CoreEncodingAndPrecision", "MilstdSimple");
        break;
    case seds::model::CoreEncodingAndPrecision::MilstdExtended:
        throw UnsupportedValueException("CoreEncodingAndPrecision", "MilstdExtended");
        break;
    default:
        throw UnhandledValueException("CoreEncodingAndPrecision");
        break;
    }
}

void DataTypeTranslatorVisitor::translateCoreStringEncoding(
        seds::model::CoreStringEncoding coreEncoding, Asn1Acn::Types::IA5String *asn1Type) const
{
    switch (coreEncoding) {
    case seds::model::CoreStringEncoding::Ascii:
        asn1Type->setEncoding(Asn1Acn::Types::AsciiStringEncoding::ASCII);
        break;
    case seds::model::CoreStringEncoding::Utf8:
        throw UnsupportedValueException("CoreStringEncoding", "Utf8");
        break;
    default:
        throw UnhandledValueException("CoreStringEncoding");
        break;
    }
}

void DataTypeTranslatorVisitor::translateBitStringLength(
        const BinaryDataType &sedsType, Asn1Acn::Types::BitString *asn1Type) const
{
    if (sedsType.bits() > std::numeric_limits<Asn1Acn::IntegerValue::Type>::max()) {
        throw TranslationException("Bit string length size overflows ASN.1 range");
    }

    auto sizeConstraint = std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::BitStringValue>>();

    if (sedsType.hasFixedSize()) {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
                { static_cast<Asn1Acn::IntegerValue::Type>(sedsType.bits()) });
        sizeConstraint->setInnerConstraints(std::move(constraint));
    } else {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
                { 0, static_cast<Asn1Acn::IntegerValue::Type>(sedsType.bits()) });
        sizeConstraint->setInnerConstraints(std::move(constraint));
    }

    asn1Type->constraints().append(std::move(sizeConstraint));
}

void DataTypeTranslatorVisitor::translateStringLength(
        const StringDataType &sedsType, Asn1Acn::Types::IA5String *asn1Type) const
{
    if (sedsType.length() > std::numeric_limits<Asn1Acn::IntegerValue::Type>::max()) {
        throw TranslationException("String length size overflows ASN.1 range");
    }

    auto sizeConstraint = std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::StringValue>>();

    if (sedsType.hasFixedLength()) {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
                { static_cast<Asn1Acn::IntegerValue::Type>(sedsType.length()) });
        sizeConstraint->setInnerConstraints(std::move(constraint));
    } else {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
                { 0, static_cast<Asn1Acn::IntegerValue::Type>(sedsType.length()) });
        sizeConstraint->setInnerConstraints(std::move(constraint));
    }

    asn1Type->constraints().append(std::move(sizeConstraint));
}

void DataTypeTranslatorVisitor::translateArrayType(
        const QString &sedsTypeName, Asn1Acn::Types::SequenceOf *asn1Type) const
{
    const auto *asn1ReferencedTypeAssignment = m_asn1Definitions->type(sedsTypeName);
    if (!asn1ReferencedTypeAssignment) {
        throw MissingAsn1TypeDefinitionException(sedsTypeName);
    }

    const auto *asn1ReferencedType = asn1ReferencedTypeAssignment->type();

    auto asn1ItemType = std::make_unique<Asn1Acn::Types::UserdefinedType>(
            asn1ReferencedType->identifier(), m_asn1Definitions->name());
    asn1ItemType->setType(asn1ReferencedType->clone());

    asn1Type->setItemsType(std::move(asn1ItemType));
}

void DataTypeTranslatorVisitor::translateEnumerationList(
        const std::vector<seds::model::ValueEnumeration> &items, Asn1Acn::Types::Enumerated *asn1Type) const
{
    for (std::size_t index = 0; index < items.size(); ++index) {
        const auto &item = items[index];
        const auto name = Escaper::escapeAsn1FieldName(item.label().value());
        const auto enumeratedItem =
                Asn1Acn::Types::EnumeratedItem(index, name, item.value(), Asn1Acn::SourceLocation());

        asn1Type->addItem(enumeratedItem);
    }
}

void DataTypeTranslatorVisitor::translateFalseValue(
        seds::model::FalseValue falseValue, Asn1Acn::Types::Boolean *asn1Type) const
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

void DataTypeTranslatorVisitor::cacheContainerType(const ContainerDataType &sedsType)
{
    auto asn1SequenceComponents = std::make_unique<Asn1Acn::Types::Sequence>();

    // Check if base type is visible
    if (sedsType.baseType()) {
        const auto sedsBaseTypeName = sedsType.baseType()->nameStr();

        if (m_asn1SequenceComponentsCache.count(sedsBaseTypeName) == 0) {
            auto errorMessage = QString("Unable to find base type \"%1\" for container \"%2\" in the current scope")
                                        .arg(sedsBaseTypeName)
                                        .arg(sedsType.nameStr());
            throw TranslationException(std::move(errorMessage));
        }
    }

    // Get parent entries from cache
    if (sedsType.baseType()) {
        const auto sedsBaseTypeName = sedsType.baseType()->nameStr();
        const auto &asn1ParentComponents = m_asn1SequenceComponentsCache[sedsBaseTypeName].first->components();

        for (const auto &asn1Component : asn1ParentComponents) {
            asn1SequenceComponents->addComponent(asn1Component->clone());
        }
    }

    // Translate own entries
    EntryTranslatorVisitor entriesTranslator(asn1SequenceComponents.get(), m_asn1Definitions, &sedsType, m_sedsPackage);
    for (const auto &sedsEntry : sedsType.entries()) {
        std::visit(entriesTranslator, sedsEntry);
    }

    auto asn1SequenceTrailerComponents = std::make_unique<Asn1Acn::Types::Sequence>();

    if (sedsType.isAbstract()) {
        // Translate own trailer entries
        EntryTranslatorVisitor trailerEntriesTranslator(
                asn1SequenceTrailerComponents.get(), m_asn1Definitions, &sedsType, m_sedsPackage);
        for (const auto &sedsTrailerEntry : sedsType.trailerEntries()) {
            std::visit(trailerEntriesTranslator, sedsTrailerEntry);
        }
    }

    // Get parent trailer entries from cache
    if (sedsType.baseType()) {
        const auto sedsBaseTypeName = sedsType.baseType()->nameStr();
        const auto &asn1ParentTrailerComponents = m_asn1SequenceComponentsCache[sedsBaseTypeName].second->components();

        for (const auto &asn1TrailerComponent : asn1ParentTrailerComponents) {
            asn1SequenceTrailerComponents->addComponent(asn1TrailerComponent->clone());
        }
    }

    // Cache this type
    m_asn1SequenceComponentsCache.insert({ sedsType.nameStr(),
            std::make_pair(std::move(asn1SequenceComponents), std::move(asn1SequenceTrailerComponents)) });
}

void DataTypeTranslatorVisitor::createRealizationContainerField(Asn1Acn::Types::Sequence *asn1Sequence)
{
    auto realizationChoice = std::make_unique<Asn1Acn::Types::Choice>();

    if (!asn1Sequence->components().empty()) {
        auto ownRealizationSequence = std::make_unique<Asn1Acn::Types::Sequence>();
        for (const auto &asn1SequenceComponent : asn1Sequence->components()) {
            ownRealizationSequence->addComponent(asn1SequenceComponent->clone());
        }
        asn1Sequence->components().clear();

        const auto ownRealizationChoiceAlternativeName =
                m_realizationComponentsAlternativeNameTemplate.arg(asn1Sequence->identifier());
        auto ownRealizationChoiceAlternative = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
                ownRealizationChoiceAlternativeName, ownRealizationChoiceAlternativeName,
                ownRealizationChoiceAlternativeName, ownRealizationChoiceAlternativeName, "", Asn1Acn::SourceLocation(),
                std::move(ownRealizationSequence));
        realizationChoice->addComponent(std::move(ownRealizationChoiceAlternative));
    }

    auto realizationComponent =
            std::make_unique<Asn1Acn::AsnSequenceComponent>(m_realizationComponentsName, m_realizationComponentsName,
                    false, std::nullopt, "", Asn1Acn::SourceLocation(), std::move(realizationChoice));
    asn1Sequence->addComponent(std::move(realizationComponent));
}

void DataTypeTranslatorVisitor::applyContainerConstraints(
        const ContainerDataType &sedsType, Asn1Acn::Types::Sequence *asn1Type) const
{
    ContainerConstraintTranslatorVisitor translatorVisitor(asn1Type, m_sedsPackage);

    for (const auto &constraint : sedsType.constraints()) {
        std::visit(translatorVisitor, constraint);
    }
}

void DataTypeTranslatorVisitor::updateParentContainer(
        const QString &sedsBaseTypeName, Asn1Acn::Types::Sequence *asn1RealizationSequence)
{
    auto *asn1BaseSequence =
            dynamic_cast<Asn1Acn::Types::Sequence *>(m_asn1Definitions->type(sedsBaseTypeName)->type());
    if (!asn1BaseSequence) {
        throw MissingAsn1TypeDefinitionException(sedsBaseTypeName);
    }

    auto *asn1RealizationComponent = asn1BaseSequence->component(m_realizationComponentsName);
    if (!asn1RealizationComponent) {
        createRealizationContainerField(asn1BaseSequence);
        asn1RealizationComponent = asn1BaseSequence->component(m_realizationComponentsName);
    }

    auto *asn1RealizationChoice = dynamic_cast<Asn1Acn::Types::Choice *>(asn1RealizationComponent->type());
    const auto asn1RealizationChoiceAlternativeName =
            m_realizationComponentsAlternativeNameTemplate.arg(asn1RealizationSequence->identifier());

    auto asn1SequenceReference = std::make_unique<Asn1Acn::Types::UserdefinedType>(
            asn1RealizationSequence->identifier(), m_asn1Definitions->name());
    auto asn1RealizationChoiceAlternative = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
            asn1RealizationChoiceAlternativeName, asn1RealizationChoiceAlternativeName,
            asn1RealizationChoiceAlternativeName, asn1RealizationChoiceAlternativeName, "", Asn1Acn::SourceLocation(),
            std::move(asn1SequenceReference));
    asn1RealizationChoice->addComponent(std::move(asn1RealizationChoiceAlternative));
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
