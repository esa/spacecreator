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

#include "specialized/entrytranslatorvisitor.h"

#include "datatypetranslationhelper.h"
#include "specialized/rangetranslatorvisitor.h"
#include "specialized/sizetranslatorvisitor.h"

#include <asn1library/asn1/acnsequencecomponent.h>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/null.h>
#include <asn1library/asn1/types/numericstring.h>
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/overloaded.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/package/package.h>

using conversion::UnhandledValueException;
using conversion::UnsupportedValueException;
using conversion::translator::TranslationException;

namespace conversion::asn1::translator::seds {

using SizeTranslator = SizeTranslatorVisitor<Asn1Acn::Types::SequenceOf, Asn1Acn::IntegerValue>;

EntryTranslatorVisitor::EntryTranslatorVisitor(
        Context &context, const ::seds::model::ContainerDataType &container, Asn1Acn::Types::Sequence *sequence)
    : m_context(context)
    , m_container(container)
    , m_sequence(sequence)
{
}

void EntryTranslatorVisitor::operator()(const ::seds::model::Entry &sedsEntry)
{
    const auto entryName = Escaper::escapeAsn1FieldName(sedsEntry.nameStr());
    auto entryType = translateEntryType(sedsEntry.type());

    auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(entryName, entryName, false, std::nullopt,
            "", Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(), std::move(entryType));
    m_sequence->addComponent(std::move(sequenceComponent));
}

void EntryTranslatorVisitor::operator()(const ::seds::model::ErrorControlEntry &sedsEntry)
{
    const auto entryName = Escaper::escapeAsn1FieldName(sedsEntry.nameStr());
    auto entryType = translateErrorControl(sedsEntry);
    entryType->setSavePosition(true);

    auto sequenceComponent =
            std::make_unique<Asn1Acn::AcnSequenceComponent>(entryName, entryName, std::move(entryType));
    m_sequence->addComponent(std::move(sequenceComponent));
}

void EntryTranslatorVisitor::operator()(const ::seds::model::FixedValueEntry &sedsEntry)
{
    const auto entryName = Escaper::escapeAsn1FieldName(sedsEntry.nameStr());
    auto entryType = translateEntryType(sedsEntry.type());

    translateFixedValue(sedsEntry, entryType.get());

    auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(entryName, entryName, false, std::nullopt,
            "", Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(), std::move(entryType));
    m_sequence->addComponent(std::move(sequenceComponent));
}

void EntryTranslatorVisitor::operator()(const ::seds::model::LengthEntry &sedsEntry)
{
    const auto entryName = Escaper::escapeAsn1FieldName(sedsEntry.nameStr());
    auto entryType = translateLengthEntry(sedsEntry);
    entryType->setSavePosition(true);

    auto sequenceComponent =
            std::make_unique<Asn1Acn::AcnSequenceComponent>(entryName, entryName, std::move(entryType));
    m_sequence->addComponent(std::move(sequenceComponent));
}

void EntryTranslatorVisitor::operator()(const ::seds::model::ListEntry &sedsEntry)
{
    const auto entryName = Escaper::escapeAsn1FieldName(sedsEntry.nameStr());
    auto entryType = translateEntryType(sedsEntry.type());

    auto asn1SequenceOfType = std::make_unique<Asn1Acn::Types::SequenceOf>();
    asn1SequenceOfType->setItemsType(std::move(entryType));

    addListSizeConstraint(asn1SequenceOfType.get(), sedsEntry);

    auto listSequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(entryName, entryName, false,
            std::nullopt, "", Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
            std::move(asn1SequenceOfType));
    m_sequence->addComponent(std::move(listSequenceComponent));
}

void EntryTranslatorVisitor::operator()(const ::seds::model::PaddingEntry &sedsEntry)
{
    Q_UNUSED(sedsEntry);
    throw TranslationException("PaddingEntry translation not implemented");
}

std::unique_ptr<Asn1Acn::Types::UserdefinedType> EntryTranslatorVisitor::translateEntryType(
        const ::seds::model::DataTypeRef &sedsTypeRef)
{
    auto asn1ReferencedType = m_context.findAsn1Type(sedsTypeRef)->clone();

    DataTypeTranslationHelper::removeConstraints(asn1ReferencedType.get());

    auto asn1EntryType = std::make_unique<Asn1Acn::Types::UserdefinedType>(
            asn1ReferencedType->identifier(), m_context.definitionsName());
    asn1EntryType->setType(std::move(asn1ReferencedType));

    return asn1EntryType;
}

std::unique_ptr<Asn1Acn::Types::Null> EntryTranslatorVisitor::translateErrorControl(
        const ::seds::model::ErrorControlEntry &sedsEntry) const
{
    auto nullType = std::make_unique<Asn1Acn::Types::Null>(sedsEntry.nameStr());
    nullType->setAlignToNext(Asn1Acn::Types::AlignToNext::byte);

    // clang-format off
    std::visit(overloaded {
        [&](::seds::model::CoreErrorControl coreErrorControl) {
            translateCoreErrorControl(coreErrorControl, sedsEntry, nullType.get());
        }
    }, sedsEntry.errorControl());
    // clang-format on

    return nullType;
}

std::unique_ptr<Asn1Acn::Types::Null> EntryTranslatorVisitor::translateLengthEntry(
        const ::seds::model::LengthEntry &sedsEntry) const
{
    const auto &encoding = getLengthEntryEncoding(sedsEntry);

    auto nullType = std::make_unique<Asn1Acn::Types::Null>(sedsEntry.nameStr());
    nullType->setAlignToNext(Asn1Acn::Types::AlignToNext::byte);
    qsizetype encodingBits = static_cast<qsizetype>(encoding.bits());
    nullType->setPattern(QString(encodingBits, '0'));

    return nullType;
}

void EntryTranslatorVisitor::translateFixedValue(
        const ::seds::model::FixedValueEntry &sedsEntry, Asn1Acn::Types::UserdefinedType *asn1Type) const
{
    if (!sedsEntry.fixedValue()) {
        return;
    }

    switch (asn1Type->type()->typeEnum()) {
    case Asn1Acn::Types::Type::ASN1Type::INTEGER: {
        const auto value = Asn1Acn::IntegerValue::fromAstValue(sedsEntry.fixedValue()->value());
        RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue> rangeTranslator(asn1Type->type());
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::REAL: {
        const auto value = Asn1Acn::RealValue::fromAstValue(sedsEntry.fixedValue()->value());
        RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue> rangeTranslator(asn1Type->type());
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::BOOLEAN: {
        const auto value = Asn1Acn::BooleanValue::fromAstValue(sedsEntry.fixedValue()->value());
        RangeTranslatorVisitor<Asn1Acn::Types::Boolean, Asn1Acn::BooleanValue> rangeTranslator(asn1Type->type());
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::SEQUENCE:
        throw UnsupportedValueException("ASN1Type/FixedValueEntry", "SEQUENCE");
        break;
    case Asn1Acn::Types::Type::ASN1Type::SEQUENCEOF:
        throw UnsupportedValueException("ASN1Type/FixedValueEntry", "SEQUENCEOF");
        break;
    case Asn1Acn::Types::Type::ASN1Type::ENUMERATED: {
        const auto value = Asn1Acn::EnumValue::fromAstValue(sedsEntry.fixedValue()->value());
        RangeTranslatorVisitor<Asn1Acn::Types::Enumerated, Asn1Acn::EnumValue> rangeTranslator(asn1Type->type());
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::CHOICE:
        throw UnsupportedValueException("ASN1Type/FixedValueEntry", "CHOICE");
        break;
    case Asn1Acn::Types::Type::ASN1Type::STRING:
        throw UnsupportedValueException("ASN1Type/FixedValueEntry", "STRING");
        break;
    case Asn1Acn::Types::Type::ASN1Type::IA5STRING: {
        const auto value = Asn1Acn::StringValue::fromAstValue(sedsEntry.fixedValue()->value());
        RangeTranslatorVisitor<Asn1Acn::Types::IA5String, Asn1Acn::StringValue> rangeTranslator(asn1Type->type());
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::NUMERICSTRING: {
        const auto value = Asn1Acn::StringValue::fromAstValue(sedsEntry.fixedValue()->value());
        RangeTranslatorVisitor<Asn1Acn::Types::NumericString, Asn1Acn::StringValue> rangeTranslator(asn1Type->type());
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::NULLTYPE:
        throw UnsupportedValueException("ASN1Type/FixedValueEntry", "NULLTYPE");
        break;
    case Asn1Acn::Types::Type::ASN1Type::BITSTRING: {
        const auto value = Asn1Acn::BitStringValue::fromAstValue(sedsEntry.fixedValue()->value());
        RangeTranslatorVisitor<Asn1Acn::Types::BitString, Asn1Acn::BitStringValue> rangeTranslator(asn1Type->type());
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::OCTETSTRING: {
        const auto value = Asn1Acn::OctetStringValue::fromAstValue(sedsEntry.fixedValue()->value());
        RangeTranslatorVisitor<Asn1Acn::Types::OctetString, Asn1Acn::OctetStringValue> rangeTranslator(
                asn1Type->type());
        rangeTranslator.addValueConstraint(value);
    } break;
    case Asn1Acn::Types::Type::ASN1Type::LABELTYPE:
        throw UnsupportedValueException("ASN1Type/FixedValueEntry", "LABELTYPE");
        break;
    case Asn1Acn::Types::Type::ASN1Type::USERDEFINED:
        throw UnsupportedValueException("ASN1Type/FixedValueEntry", "USERDEFINED");
        break;
    default:
        throw UnhandledValueException("ASN1Type/FixedValueEntry");
        break;
    }
}

void EntryTranslatorVisitor::translateCoreErrorControl(::seds::model::CoreErrorControl coreErrorControl,
        const ::seds::model::ErrorControlEntry &sedsEntry, Asn1Acn::Types::Null *asn1Type) const
{
    uint64_t expectedBitCount = 0;

    switch (coreErrorControl) {
    case ::seds::model::CoreErrorControl::Crc8:
        expectedBitCount = m_crc8BitSize;
        break;
    case ::seds::model::CoreErrorControl::Crc16:
        expectedBitCount = m_crc16BitSize;
        break;
    case ::seds::model::CoreErrorControl::Checksum:
        expectedBitCount = m_checksumBitSize;
        break;
    case ::seds::model::CoreErrorControl::ChecksumLongitundinal:
        expectedBitCount = m_checksumLongitundinalSize;
        break;
    default:
        throw UnhandledValueException("CoreErrorControl");
        break;
    }

    const auto bitCount = getErrorControlEntryBitCount(sedsEntry);

    if (bitCount != expectedBitCount) {
        auto errorMessage = QString("Error control entry \"%1\" underlying type doesn't have the same bit size as the "
                                    "requested error control")
                                    .arg(sedsEntry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }
    qsizetype bitCountQsizetype = static_cast<qsizetype>(bitCount);
    asn1Type->setPattern(QString(bitCountQsizetype, '0'));
}

uint64_t EntryTranslatorVisitor::getErrorControlEntryBitCount(const ::seds::model::ErrorControlEntry &entry) const
{
    const auto entryType = m_context.findSedsType(entry.type());
    const auto entryBinaryType = std::get_if<::seds::model::BinaryDataType>(entryType);

    if (!entryBinaryType) {
        auto errorMessage =
                QString("Error control entry \"%1\" has to have binary as an underlying type").arg(entry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    if (!entryBinaryType->hasFixedSize()) {
        auto errorMessage =
                QString("Error control entry \"%1\" underlying type has to be fixed size").arg(entry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    return entryBinaryType->bits();
}

const ::seds::model::IntegerDataEncoding &EntryTranslatorVisitor::getLengthEntryEncoding(
        const ::seds::model::LengthEntry &entry) const
{
    const auto entryType = m_context.findSedsType(entry.type());

    const auto entryIntegerType = std::get_if<::seds::model::IntegerDataType>(entryType);

    if (!entryIntegerType) {
        auto errorMessage =
                QString("Length entry \"%1\" has to have integer as an underlying type").arg(entry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    const auto &entryIntegerEncoding = entryIntegerType->encoding();

    if (!entryIntegerEncoding.has_value()) {
        auto errorMessage = QString("Entry \"%1\" has to have encoding").arg(entry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    return entryIntegerEncoding.value();
}

void EntryTranslatorVisitor::addListSizeConstraint(
        Asn1Acn::Types::SequenceOf *asn1Type, const ::seds::model::ListEntry &sedsEntry) const
{
    const auto listLengthFieldName = sedsEntry.listLengthField().nameStr();
    const auto listLengthField = getListLengthEntry(listLengthFieldName, m_container);

    if (listLengthField == nullptr) {
        auto errorMessage = QString("Entry \"%1\" used as a length of the list entry \"%2\" not found")
                                    .arg(listLengthFieldName)
                                    .arg(sedsEntry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    const auto listLengthEntry = std::get_if<::seds::model::Entry>(listLengthField);

    if (listLengthEntry == nullptr) {
        auto errorMessage = QString(
                "Entry \"%1\" can't be used as a length of the list entry \"%2\". Only plain entry can be used.")
                                    .arg(listLengthFieldName)
                                    .arg(sedsEntry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    // Replace the existing ASN.1 length entry with an ACN-only one
    updateListLengthEntry(listLengthEntry);

    const auto listLengthEntryType = m_context.findSedsType(listLengthEntry->type());

    if (const auto listLengthEntryIntegerType = std::get_if<::seds::model::IntegerDataType>(listLengthEntryType)) {
        SizeTranslator sizeTranslator(asn1Type, SizeTranslator::LengthType::VariableLength,
                SizeTranslator::SourceType::Determinant, m_context.arraySizeThreshold());
        std::visit(sizeTranslator, listLengthEntryIntegerType->range());
    } else {
        auto errorMessage = QString(
                "Entry \"%1\" can't be used as a length of the list entry \"%2\" because it's type is not integer")
                                    .arg(listLengthFieldName)
                                    .arg(sedsEntry.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    asn1Type->setAcnSize(Escaper::escapeAsn1FieldName(listLengthFieldName));
}

const ::seds::model::EntryType *EntryTranslatorVisitor::getListLengthEntry(
        const QString &entryName, const ::seds::model::ContainerDataType &container) const
{
    const auto listLengthField = container.entry(entryName);

    if (listLengthField != nullptr) {
        return listLengthField;
    }

    if (container.baseType()) {
        const auto baseType = m_context.findSedsType(*container.baseType());
        const auto baseContainer = std::get_if<::seds::model::ContainerDataType>(baseType);

        return getListLengthEntry(entryName, *baseContainer);
    }

    return nullptr;
}

void EntryTranslatorVisitor::updateListLengthEntry(const ::seds::model::Entry *sedsEntry) const
{
    auto &listLengthSequenceComponent = getListLengthSequenceComponent(sedsEntry);

    auto name = listLengthSequenceComponent->name();
    auto type = listLengthSequenceComponent->type()->clone();

    const auto &sedsTypeRef = sedsEntry->type();
    if (sedsTypeRef.packageStr().has_value()) {
        auto userdefinedType = dynamic_cast<Asn1Acn::Types::UserdefinedType *>(type.get());

        // We need to add a package qualified to the type name for the ACN-only fields
        const auto qualifiedName =
                QString("%1.%2").arg(Escaper::escapeAsn1PackageName(*sedsTypeRef.packageStr())).arg(type->typeName());
        userdefinedType->setTypeName(qualifiedName);
    }

    listLengthSequenceComponent.reset(new Asn1Acn::AcnSequenceComponent(name, name, std::move(type)));
}

std::unique_ptr<Asn1Acn::SequenceComponent> &EntryTranslatorVisitor::getListLengthSequenceComponent(
        const ::seds::model::Entry *sedsEntry) const
{
    auto &containerComponents = m_sequence->components();

    const auto listLengthFieldName = Escaper::escapeAsn1FieldName(sedsEntry->nameStr());
    auto foundListLengthSequenceComponent = std::find_if(containerComponents.begin(), containerComponents.end(),
            [&](const auto &component) { return component->name() == listLengthFieldName; });

    if (foundListLengthSequenceComponent == containerComponents.end()) {
        auto errorMessage = QString("Entry \"%1\" used as a length of the list entry \"%2\" not found")
                                    .arg(listLengthFieldName)
                                    .arg(sedsEntry->nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    return *foundListLengthSequenceComponent;
}

} // namespace conversion::asn1::translator::seds
