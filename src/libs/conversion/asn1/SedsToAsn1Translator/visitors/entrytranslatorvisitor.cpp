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

#include "visitors/entrytranslatorvisitor.h"

#include "visitors/datatypetranslatorvisitor.h"

#include <asn1library/asn1/acnsequencecomponent.h>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/null.h>
#include <asn1library/asn1/types/numericstring.h>
#include <asn1library/asn1/types/octetstring.h>
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
using conversion::translator::UndeclaredDataTypeException;

namespace conversion::asn1::translator {

void EntryTranslatorVisitor::operator()(const seds::model::Entry &sedsEntry)
{
    auto asn1EntryType = translateEntryType(sedsEntry.type().nameStr());

    const auto entryName = Escaper::escapeAsn1FieldName(sedsEntry.nameStr());
    auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            entryName, entryName, false, std::nullopt, "", Asn1Acn::SourceLocation(), std::move(asn1EntryType));
    m_asn1Sequence->addComponent(std::move(sequenceComponent));
}

void EntryTranslatorVisitor::operator()(const seds::model::ErrorControlEntry &sedsEntry)
{
    auto asn1EntryType = translateErrorControl(sedsEntry);

    const auto entryName = Escaper::escapeAsn1FieldName(sedsEntry.nameStr());
    auto sequenceComponent =
            std::make_unique<Asn1Acn::AcnSequenceComponent>(entryName, entryName, std::move(asn1EntryType));
    m_asn1Sequence->addComponent(std::move(sequenceComponent));
}

void EntryTranslatorVisitor::operator()(const seds::model::FixedValueEntry &sedsEntry)
{
    auto asn1EntryType = translateEntryType(sedsEntry.type().nameStr());
    translateFixedValue(sedsEntry, asn1EntryType.get());

    const auto entryName = Escaper::escapeAsn1FieldName(sedsEntry.nameStr());
    auto sequenceComponent = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            entryName, entryName, false, std::nullopt, "", Asn1Acn::SourceLocation(), std::move(asn1EntryType));
    m_asn1Sequence->addComponent(std::move(sequenceComponent));
}

void EntryTranslatorVisitor::operator()(const seds::model::LengthEntry &sedsEntry)
{
    auto asn1EntryType = translateEntryType(sedsEntry.type().nameStr());

    const auto entryName = Escaper::escapeAsn1FieldName(sedsEntry.nameStr());
    auto sequenceComponent =
            std::make_unique<Asn1Acn::AcnSequenceComponent>(entryName, entryName, std::move(asn1EntryType));
    m_asn1Sequence->addComponent(std::move(sequenceComponent));
}

void EntryTranslatorVisitor::operator()(const seds::model::ListEntry &sedsEntry)
{
    auto asn1EntryType = translateEntryType(sedsEntry.type().nameStr());

    auto asn1SequenceOfType = std::make_unique<Asn1Acn::Types::SequenceOf>();
    asn1SequenceOfType->setItemsType(std::move(asn1EntryType));

    const auto entryName = Escaper::escapeAsn1FieldName(sedsEntry.nameStr());
    auto sequenceComponent =
            std::make_unique<Asn1Acn::AcnSequenceComponent>(entryName, entryName, std::move(asn1SequenceOfType));
    m_asn1Sequence->addComponent(std::move(sequenceComponent));
}

void EntryTranslatorVisitor::operator()(const seds::model::PaddingEntry &sedsEntry)
{
    Q_UNUSED(sedsEntry);
    throw TranslationException("PaddingEntry translation not implemented");
}

std::unique_ptr<Asn1Acn::Types::UserdefinedType> EntryTranslatorVisitor::translateEntryType(
        const QString &sedsTypeName) const
{
    const auto name = Escaper::escapeAsn1TypeName(sedsTypeName);
    const auto *asn1ReferencedType = m_asn1Definitions->type(name);
    if (!asn1ReferencedType || !asn1ReferencedType->type()) {
        throw MissingAsn1TypeDefinitionException(name);
    }

    auto asn1EntryType = std::make_unique<Asn1Acn::Types::UserdefinedType>(
            asn1ReferencedType->type()->identifier(), m_asn1Definitions->name());
    asn1EntryType->setType(asn1ReferencedType->type()->clone());

    return asn1EntryType;
}

void EntryTranslatorVisitor::translateFixedValue(
        const seds::model::FixedValueEntry &sedsEntry, Asn1Acn::Types::UserdefinedType *asn1Type) const
{
    if (!sedsEntry.fixedValue()) {
        return;
    }

    switch (asn1Type->type()->typeEnum()) {
    case Asn1Acn::Types::Type::ASN1Type::INTEGER:
        createValueConstraint<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue>(
                sedsEntry.fixedValue()->value(), asn1Type->type());
        break;
    case Asn1Acn::Types::Type::ASN1Type::REAL:
        createValueConstraint<Asn1Acn::Types::Real, Asn1Acn::RealValue>(
                sedsEntry.fixedValue()->value(), asn1Type->type());
        break;
    case Asn1Acn::Types::Type::ASN1Type::BOOLEAN:
        createValueConstraint<Asn1Acn::Types::Boolean, Asn1Acn::BooleanValue>(
                sedsEntry.fixedValue()->value(), asn1Type->type());
        break;
    case Asn1Acn::Types::Type::ASN1Type::SEQUENCE:
        throw UnsupportedValueException("ASN1Type/FixedValueEntry", "SEQUENCE");
        break;
    case Asn1Acn::Types::Type::ASN1Type::SEQUENCEOF:
        throw UnsupportedValueException("ASN1Type/FixedValueEntry", "SEQUENCEOF");
        break;
    case Asn1Acn::Types::Type::ASN1Type::ENUMERATED:
        createValueConstraint<Asn1Acn::Types::Enumerated, Asn1Acn::EnumValue>(
                Escaper::escapeAsn1FieldName(sedsEntry.fixedValue()->value()), asn1Type->type());
        break;
    case Asn1Acn::Types::Type::ASN1Type::CHOICE:
        throw UnsupportedValueException("ASN1Type/FixedValueEntry", "CHOICE");
        break;
    case Asn1Acn::Types::Type::ASN1Type::STRING:
        throw UnsupportedValueException("ASN1Type/FixedValueEntry", "STRING");
        break;
    case Asn1Acn::Types::Type::ASN1Type::IA5STRING:
        createValueConstraint<Asn1Acn::Types::IA5String, Asn1Acn::StringValue>(
                sedsEntry.fixedValue()->value(), asn1Type->type());
        break;
    case Asn1Acn::Types::Type::ASN1Type::NUMERICSTRING:
        createValueConstraint<Asn1Acn::Types::NumericString, Asn1Acn::StringValue>(
                sedsEntry.fixedValue()->value(), asn1Type->type());
        break;
    case Asn1Acn::Types::Type::ASN1Type::NULLTYPE:
        throw UnsupportedValueException("ASN1Type/FixedValueEntry", "NULLTYPE");
        break;
    case Asn1Acn::Types::Type::ASN1Type::BITSTRING:
        createValueConstraint<Asn1Acn::Types::BitString, Asn1Acn::BitStringValue>(
                sedsEntry.fixedValue()->value(), asn1Type->type());
        break;
    case Asn1Acn::Types::Type::ASN1Type::OCTETSTRING:
        createValueConstraint<Asn1Acn::Types::OctetString, Asn1Acn::OctetStringValue>(
                sedsEntry.fixedValue()->value(), asn1Type->type());
        break;
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

std::unique_ptr<Asn1Acn::Types::Null> EntryTranslatorVisitor::translateErrorControl(
        const seds::model::ErrorControlEntry &sedsEntry) const
{
    auto nullType = std::make_unique<Asn1Acn::Types::Null>(sedsEntry.nameStr());
    nullType->setAlignToNext(Asn1Acn::Types::AlignToNext::byte);

    // clang-format off
    std::visit(overloaded {
        [&](seds::model::CoreErrorControl coreErrorControl) {
            translateCoreErrorControl(coreErrorControl, nullType.get());
        }
    }, sedsEntry.errorControl());
    // clang-format on

    return nullType;
}

void EntryTranslatorVisitor::translateCoreErrorControl(
        seds::model::CoreErrorControl coreErrorControl, Asn1Acn::Types::Null *asn1Type) const
{
    switch (coreErrorControl) {
    case seds::model::CoreErrorControl::Crc8:
        asn1Type->setPattern(QString(m_crc8BitSize, '0'));
        break;
    case seds::model::CoreErrorControl::Crc16:
        asn1Type->setPattern(QString(m_crc16BitSize, '0'));
        break;
    case seds::model::CoreErrorControl::Checksum:
    case seds::model::CoreErrorControl::ChecksumLongitundinal:
        asn1Type->setPattern(QString(m_checksumBitSize, '0'));
        break;
    default:
        throw UnhandledValueException("CoreErrorControl");
        break;
    }
}

} // namespace conversion::asn1::translator
