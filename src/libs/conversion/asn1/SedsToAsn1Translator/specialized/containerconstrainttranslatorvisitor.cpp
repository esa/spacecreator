/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "specialized/containerconstrainttranslatorvisitor.h"

#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/numericstring.h>
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/overloaded.h>
#include <iostream>
#include <seds/SedsModel/components/entryref.h>
#include <seds/SedsModel/package/package.h>
#include <seds/SedsModel/types/constraints/containerrangeconstraint.h>
#include <seds/SedsModel/types/constraints/containertypeconstraint.h>
#include <seds/SedsModel/types/constraints/containervalueconstraint.h>

using ASN1Type = Asn1Acn::Types::Type::ASN1Type;
using conversion::translator::TranslationException;
using seds::model::ContainerRangeConstraint;
using seds::model::ContainerTypeConstraint;
using seds::model::ContainerValueConstraint;

namespace conversion::asn1::translator {

ContainerConstraintTranslatorVisitor::ContainerConstraintTranslatorVisitor(
        Asn1Acn::Types::Sequence *asn1Sequence, Asn1Acn::Definitions *asn1Definitions)
    : m_asn1Sequence(asn1Sequence)
    , m_asn1Definitions(asn1Definitions)
{
}

void ContainerConstraintTranslatorVisitor::operator()(const ContainerRangeConstraint &rangeConstraint)
{
    auto asn1ConstrainedType = getConstrainedType(rangeConstraint.entry());
    applyContainerRangeConstraint(rangeConstraint, asn1ConstrainedType);
}

void ContainerConstraintTranslatorVisitor::operator()(const ContainerTypeConstraint &typeConstraint)
{
    auto asn1ConstrainedType = getConstrainedType(typeConstraint.entry());
    applyContainerTypeConstraint(typeConstraint, asn1ConstrainedType);
}

void ContainerConstraintTranslatorVisitor::operator()(const ContainerValueConstraint &valueConstraint)
{
    auto asn1ConstrainedType = getConstrainedType(valueConstraint.entry());
    applyContainerValueConstraint(valueConstraint, asn1ConstrainedType);
}

void ContainerConstraintTranslatorVisitor::applyContainerRangeConstraint(
        const ContainerRangeConstraint &rangeConstraint, Asn1Acn::Types::Type *asn1Type) const
{
    const auto &range = rangeConstraint.range();

    switch (asn1Type->typeEnum()) {
    case ASN1Type::INTEGER: {
        RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue> rangeTranslator(asn1Type);
        std::visit(rangeTranslator, range);
    } break;
    case ASN1Type::REAL: {
        RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue> rangeTranslator(asn1Type);
        std::visit(rangeTranslator, range);
    } break;
    case ASN1Type::USERDEFINED: {
        auto asn1UserType = dynamic_cast<Asn1Acn::Types::UserdefinedType *>(asn1Type);
        applyContainerRangeConstraint(rangeConstraint, asn1UserType->type());
    } break;
    default:
        throw conversion::translator::TranslationException(
                "ContainerRangeConstraint can only be applied to the integer and real entries");
        break;
    }
}

void ContainerConstraintTranslatorVisitor::applyContainerTypeConstraint(
        const ContainerTypeConstraint &typeConstraint, Asn1Acn::Types::Type *asn1Type) const
{
    const auto &referencedTypeName = typeConstraint.type().nameStr();
    const auto referencedTypeAssignment = m_asn1Definitions->type(referencedTypeName);

    if (!referencedTypeAssignment) {
        auto errorMessage = QString("ContainerTypeConstraint for \"%1\" refers to an unknown type \"%2\"")
                                    .arg(typeConstraint.entry().nameStr())
                                    .arg(referencedTypeName);
        throw conversion::translator::TranslationException(std::move(errorMessage));
    }

    auto referencedType = referencedTypeAssignment->type();
    while (referencedType->typeEnum() == ASN1Type::USERDEFINED) {
        auto referencedUserType = dynamic_cast<Asn1Acn::Types::UserdefinedType *>(referencedType);
        referencedType = referencedUserType->type();
    }

    switch (asn1Type->typeEnum()) {
    case ASN1Type::INTEGER: {
        if (referencedType->typeEnum() != ASN1Type::INTEGER) {
            auto errorMessage = QString("ContainerTypeConstraint cannot be applied to \"%1\" entry because referenced "
                                        "type \"%2\" is not of the same type")
                                        .arg(typeConstraint.entry().nameStr())
                                        .arg(referencedTypeName);
            throw conversion::translator::TranslationException(std::move(errorMessage));
        }

        const auto referencedIntegerType = dynamic_cast<const Asn1Acn::Types::Integer *>(referencedType);
        addTypeRangeConstraint<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue>(asn1Type, referencedIntegerType);
    } break;
    case ASN1Type::REAL: {
        if (referencedType->typeEnum() != ASN1Type::REAL) {
            auto errorMessage = QString("ContainerTypeConstraint cannot be applied to \"%1\" entry because referenced "
                                        "type \"%2\" is not of the same type")
                                        .arg(typeConstraint.entry().nameStr())
                                        .arg(referencedTypeName);
            throw conversion::translator::TranslationException(std::move(errorMessage));
        }
        const auto referencedRealType = dynamic_cast<const Asn1Acn::Types::Real *>(referencedType);
        addTypeRangeConstraint<Asn1Acn::Types::Real, Asn1Acn::RealValue>(asn1Type, referencedRealType);
    } break;
    case ASN1Type::USERDEFINED: {
        auto asn1UserType = dynamic_cast<Asn1Acn::Types::UserdefinedType *>(asn1Type);
        applyContainerTypeConstraint(typeConstraint, asn1UserType->type());
    } break;
    default: {
        auto errorMessage = QString("ContainerTypeConstraint cannot be applied to \"%1\" entry because it's neither an "
                                    "integer nor float data type")
                                    .arg(typeConstraint.entry().nameStr());
        throw conversion::translator::TranslationException(std::move(errorMessage));
    } break;
    }
}

void ContainerConstraintTranslatorVisitor::applyContainerValueConstraint(
        const ContainerValueConstraint &valueConstraint, Asn1Acn::Types::Type *asn1Type) const
{
    const auto &value = valueConstraint.value().value();

    switch (asn1Type->typeEnum()) {
    case ASN1Type::INTEGER: {
        const auto intValue = Asn1Acn::IntegerValue::fromAstValue(value);

        RangeTranslatorVisitor<Asn1Acn::Types::Integer, Asn1Acn::IntegerValue> rangeTranslator(asn1Type);
        rangeTranslator.addValueConstraint(intValue);
    } break;
    case ASN1Type::REAL: {
        const auto realValue = Asn1Acn::RealValue::fromAstValue(value);

        RangeTranslatorVisitor<Asn1Acn::Types::Real, Asn1Acn::RealValue> rangeTranslator(asn1Type);
        rangeTranslator.addValueConstraint(realValue);
    } break;
    case ASN1Type::ENUMERATED: {
        const auto enumValue = Escaper::escapeAsn1FieldName(Asn1Acn::EnumValue::fromAstValue(value));

        RangeTranslatorVisitor<Asn1Acn::Types::Enumerated, Asn1Acn::EnumValue> rangeTranslator(asn1Type);
        rangeTranslator.addValueConstraint(enumValue);
    } break;
    case ASN1Type::BITSTRING: {
        const auto bitStringValue = Asn1Acn::BitStringValue::fromAstValue(value);

        RangeTranslatorVisitor<Asn1Acn::Types::BitString, Asn1Acn::BitStringValue> rangeTranslator(asn1Type);
        rangeTranslator.addValueConstraint(bitStringValue);
    } break;
    case ASN1Type::IA5STRING: {
        const auto ia5StringValue = Asn1Acn::StringValue::fromAstValue(value);

        RangeTranslatorVisitor<Asn1Acn::Types::IA5String, Asn1Acn::StringValue> rangeTranslator(asn1Type);
        rangeTranslator.addValueConstraint(ia5StringValue);
    } break;
    case ASN1Type::NUMERICSTRING: {
        const auto numericStringValue = Asn1Acn::StringValue::fromAstValue(value);

        RangeTranslatorVisitor<Asn1Acn::Types::NumericString, Asn1Acn::StringValue> rangeTranslator(asn1Type);
        rangeTranslator.addValueConstraint(numericStringValue);
    } break;
    case ASN1Type::OCTETSTRING: {
        const auto octetStringValue = Asn1Acn::OctetStringValue::fromAstValue(value);

        RangeTranslatorVisitor<Asn1Acn::Types::OctetString, Asn1Acn::OctetStringValue> rangeTranslator(asn1Type);
        rangeTranslator.addValueConstraint(octetStringValue);
    } break;
    case ASN1Type::USERDEFINED: {
        auto asn1UserType = dynamic_cast<Asn1Acn::Types::UserdefinedType *>(asn1Type);
        applyContainerValueConstraint(valueConstraint, asn1UserType->type());
    } break;
    case ASN1Type::BOOLEAN:
    case ASN1Type::SEQUENCE:
    case ASN1Type::SEQUENCEOF:
    case ASN1Type::CHOICE:
    case ASN1Type::STRING:
    case ASN1Type::NULLTYPE:
    case ASN1Type::LABELTYPE: {
        auto errorMessage = QString("ContainerValueConstraints cannot be applied to \"%1\" with type %2")
                                    .arg(valueConstraint.entry().nameStr())
                                    .arg(asn1Type->typeName());
        throw conversion::translator::TranslationException(std::move(errorMessage));
    } break;
    default: {
        auto errorMessage =
                QString("Unhandled ASN1Type \"%1\" in ContainerValueConstraint translation").arg(asn1Type->typeName());
        throw conversion::translator::TranslationException(std::move(errorMessage));
    } break;
    }
}

Asn1Acn::Types::Type *ContainerConstraintTranslatorVisitor::getConstrainedType(const seds::model::EntryRef &entry) const
{
    const auto constrainedEntryName = Escaper::escapeAsn1FieldName(entry.nameStr());
    const auto asn1ConstrainedComponent = m_asn1Sequence->component(constrainedEntryName);

    if (!asn1ConstrainedComponent) {
        auto errorMessage =
                QString("Container constraint refers to a nonexisting entry \"%1\"").arg(constrainedEntryName);
        throw conversion::translator::TranslationException(std::move(errorMessage));
    }

    return asn1ConstrainedComponent->type();
}

} // namespace conversion::asn1::translator
