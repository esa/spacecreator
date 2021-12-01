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

#include "specialized/dimensiontranslator.h"
#include "specialized/entrytranslatorvisitor.h"
#include "specialized/floatrangetranslatorvisitor.h"
#include "specialized/integerrangetranslatorvisitor.h"

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
using seds::model::EnumeratedDataType;
using seds::model::FloatDataType;
using seds::model::IntegerDataType;
using seds::model::StringDataType;
using seds::model::SubRangeDataType;

namespace conversion::asn1::translator {

DataTypeTranslatorVisitor::DataTypeTranslatorVisitor(
        Asn1Acn::Definitions *asn1Definitions, std::unique_ptr<Asn1Acn::Types::Type> &asn1Type)
    : m_asn1Definitions(asn1Definitions)
    , m_asn1Type(asn1Type)
{
}

void DataTypeTranslatorVisitor::operator()(const ArrayDataType &sedsType)
{
    const auto &dimensions = sedsType.dimensions();
    if (dimensions.empty()) {
        throw TranslationException("Encountered ArrayDataType without dimensions");
    }

    DimensionTranslator dimensionTranslator(m_asn1Definitions);

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
    const auto hasBaseType = sedsType.baseType().has_value();
    auto type = std::make_unique<Asn1Acn::Types::Sequence>(Escaper::escapeAsn1TypeName(sedsType.nameStr()));

    if (sedsType.isAbstract()) {
        cacheAbstractContainerEntries(sedsType);
        createRealizationContainerField(type.get());
    } else {
        EntryTranslatorVisitor visitor { m_asn1Definitions, type.get() };

        // Add parent component entries
        if (hasBaseType) {
            const auto sedsBaseTypeName = Escaper::escapeAsn1TypeName(sedsType.baseType()->nameStr());
            const auto &asn1ParentComponents = m_asn1SequenceComponentsCache[sedsBaseTypeName].first->components();

            for (const auto &asn1ParentComponent : asn1ParentComponents) {
                type->addComponent(asn1ParentComponent->clone());
            }
        }

        for (const auto &sedsEntry : sedsType.entries()) {
            std::visit(visitor, sedsEntry);
        }

        for (const auto &sedsTrailerEntry : sedsType.trailerEntries()) {
            std::visit(visitor, sedsTrailerEntry);
        }

        // Add parent component trailer entries
        if (hasBaseType) {
            const auto sedsBaseTypeName = Escaper::escapeAsn1TypeName(sedsType.baseType()->nameStr());
            const auto &asn1ParentTrailerComponents =
                    m_asn1SequenceComponentsCache[sedsBaseTypeName].second->components();

            for (const auto &asn1ParentTrailerComponent : asn1ParentTrailerComponents) {
                type->addComponent(asn1ParentTrailerComponent->clone());
            }
        }
    }

    // Add realization to the parent component
    if (hasBaseType) {
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
    std::visit(FloatRangeTranslatorVisitor { type->constraints() }, sedsType.range());
    translateFloatEncoding(sedsType.encoding(), type.get());

    m_asn1Type = std::move(type);
}

void DataTypeTranslatorVisitor::operator()(const IntegerDataType &sedsType)
{
    auto type = std::make_unique<Asn1Acn::Types::Integer>(Escaper::escapeAsn1TypeName(sedsType.nameStr()));
    std::visit(IntegerRangeTranslatorVisitor { type->constraints() }, sedsType.range());
    translateIntegerEncoding(sedsType.encoding(), type.get());

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
    Q_UNUSED(sedsType);
    throw TranslationException("SubRangeDataType translation not implemented");
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
                translateCoreEncodingAndPrecision(coreEncoding, asn1Type);
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
        seds::model::CoreEncodingAndPrecision coreEncoding, Asn1Acn::Types::Real *asn1Type) const
{
    switch (coreEncoding) {
    case seds::model::CoreEncodingAndPrecision::IeeeSingle:
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::IEEE754_1985_32);
        break;
    case seds::model::CoreEncodingAndPrecision::IeeeDouble:
        asn1Type->setEncoding(Asn1Acn::Types::RealEncoding::IEEE754_1985_64);
        break;
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
    const auto *asn1ReferencedType = m_asn1Definitions->type(sedsTypeName)->type();
    if (!asn1ReferencedType) {
        throw MissingAsn1TypeDefinitionException(sedsTypeName);
    }

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

void DataTypeTranslatorVisitor::cacheAbstractContainerEntries(const ContainerDataType &sedsType)
{
    auto asn1SequenceComponents = std::make_unique<Asn1Acn::Types::Sequence>();
    EntryTranslatorVisitor componentsVisitor { m_asn1Definitions, asn1SequenceComponents.get() };

    if (sedsType.baseType()) {
        const auto sedsBaseTypeName = Escaper::escapeAsn1TypeName(sedsType.baseType()->nameStr());
        const auto &asn1ParentComponents = m_asn1SequenceComponentsCache[sedsBaseTypeName];

        for (const auto &asn1Component : asn1ParentComponents.first->components()) {
            asn1SequenceComponents->addComponent(asn1Component->clone());
        }
    }
    for (const auto &sedsEntry : sedsType.entries()) {
        std::visit(componentsVisitor, sedsEntry);
    }

    auto asn1SequenceTrailerComponents = std::make_unique<Asn1Acn::Types::Sequence>();
    EntryTranslatorVisitor trailerComponentsVisitor { m_asn1Definitions, asn1SequenceTrailerComponents.get() };

    for (const auto &sedsTrailerEntry : sedsType.trailerEntries()) {
        std::visit(trailerComponentsVisitor, sedsTrailerEntry);
    }
    if (sedsType.baseType()) {
        const auto sedsBaseTypeName = Escaper::escapeAsn1TypeName(sedsType.baseType()->nameStr());
        const auto &asn1ParentComponents = m_asn1SequenceComponentsCache[sedsBaseTypeName];

        for (const auto &asn1TrailerComponent : asn1ParentComponents.second->components()) {
            asn1SequenceTrailerComponents->addComponent(asn1TrailerComponent->clone());
        }
    }

    m_asn1SequenceComponentsCache.insert({ sedsType.nameStr(),
            std::make_pair(std::move(asn1SequenceComponents), std::move(asn1SequenceTrailerComponents)) });
}

void DataTypeTranslatorVisitor::createRealizationContainerField(Asn1Acn::Types::Sequence *asn1Sequence)
{
    auto realizationComponent =
            std::make_unique<Asn1Acn::AsnSequenceComponent>(m_realizationComponentsName, m_realizationComponentsName,
                    false, std::nullopt, "", Asn1Acn::SourceLocation(), std::make_unique<Asn1Acn::Types::Choice>());

    asn1Sequence->addComponent(std::move(realizationComponent));
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
        auto errorMessage = QString("Missing %1 component in the '%2' base component")
                                    .arg(m_realizationComponentsName)
                                    .arg(sedsBaseTypeName);
        throw TranslationException(std::move(errorMessage));
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
