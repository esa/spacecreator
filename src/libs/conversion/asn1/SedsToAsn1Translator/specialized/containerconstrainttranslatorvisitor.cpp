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

#include "specialized/rangetranslatorvisitor.h"

#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <asn1library/asn1/values.h>
#include <cmath>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/components/entryref.h>
#include <seds/SedsModel/types/constraints/containerrangeconstraint.h>
#include <seds/SedsModel/types/constraints/containertypeconstraint.h>
#include <seds/SedsModel/types/constraints/containervalueconstraint.h>

using ASN1Type = Asn1Acn::Types::Type::ASN1Type;
using conversion::translator::TranslationException;
using seds::model::ContainerRangeConstraint;
using seds::model::ContainerTypeConstraint;
using seds::model::ContainerValueConstraint;

namespace conversion::asn1::translator {

ContainerConstraintTranslatorVisitor::ContainerConstraintTranslatorVisitor(Asn1Acn::Types::Sequence *asn1Sequence)
    : m_asn1Sequence(asn1Sequence)
{
}

void ContainerConstraintTranslatorVisitor::operator()(const ContainerRangeConstraint &rangeConstraint)
{
    auto asn1ConstrainedType = getConstrainedType(rangeConstraint.entry());
    applyContainerRangeConstraint(rangeConstraint, asn1ConstrainedType);
}

void ContainerConstraintTranslatorVisitor::operator()(const ContainerTypeConstraint &typeConstraint)
{
    Q_UNUSED(typeConstraint);
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
        auto asn1IntegerType = dynamic_cast<Asn1Acn::Types::Integer *>(asn1Type);

        auto smallestValue = getSmallestValue(asn1IntegerType);
        auto greatestValue = getGreatestValue(asn1IntegerType);

        RangeTranslatorVisitor<Asn1Acn::IntegerValue> rangeTranslator(
                asn1IntegerType, asn1IntegerType->constraints(), smallestValue, greatestValue);
        std::visit(rangeTranslator, range);
    } break;
    case ASN1Type::REAL: {
        auto asn1RealType = dynamic_cast<Asn1Acn::Types::Real *>(asn1Type);

        auto smallestValue = getSmallestValue(asn1RealType);
        auto greatestValue = getGreatestValue(asn1RealType);

        RangeTranslatorVisitor<Asn1Acn::RealValue> rangeTranslator(
                asn1RealType, asn1RealType->constraints(), smallestValue, greatestValue);
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

void ContainerConstraintTranslatorVisitor::applyContainerValueConstraint(
        const ContainerValueConstraint &valueConstraint, Asn1Acn::Types::Type *asn1Type) const
{
    const auto &value = valueConstraint.value().value();

    switch (asn1Type->typeEnum()) {
    case ASN1Type::INTEGER: {
        auto asn1IntegerType = dynamic_cast<Asn1Acn::Types::Integer *>(asn1Type);

        const auto intValue = Asn1Acn::IntegerValue::fromAstValue(value);
        auto constraint = RangeTranslatorVisitor<Asn1Acn::IntegerValue>::createRangeConstraint(intValue);

        asn1IntegerType->constraints().append(std::move(constraint));
    } break;
    case ASN1Type::REAL: {
        auto asn1RealType = dynamic_cast<Asn1Acn::Types::Real *>(asn1Type);

        const auto realValue = Asn1Acn::RealValue::fromAstValue(value);
        auto constraint = RangeTranslatorVisitor<Asn1Acn::RealValue>::createRangeConstraint(realValue);

        asn1RealType->constraints().append(std::move(constraint));
    } break;
    case ASN1Type::USERDEFINED: {
        auto asn1UserType = dynamic_cast<Asn1Acn::Types::UserdefinedType *>(asn1Type);
        applyContainerValueConstraint(valueConstraint, asn1UserType->type());
    } break;
    default: {
        throw conversion::translator::TranslationException(
                "ContainerRangeConstraint can only be applied to the integer and real entries");
    } break;
    }
}

Asn1Acn::Types::Type *ContainerConstraintTranslatorVisitor::getConstrainedType(const seds::model::EntryRef &entry) const
{
    const auto constrainedEntryName = entry.nameStr();
    const auto asn1ConstrainedComponent = m_asn1Sequence->component(constrainedEntryName);

    if (!asn1ConstrainedComponent) {
        auto errorMessage =
                QString("Container constraint refers to a nonexisting entry \"%1\"").arg(constrainedEntryName);
        throw conversion::translator::TranslationException(std::move(errorMessage));
    }

    return asn1ConstrainedComponent->type();
}

std::int64_t ContainerConstraintTranslatorVisitor::getSmallestValue(Asn1Acn::Types::Integer *type) const
{
    switch (type->encoding()) {
    case Asn1Acn::Types::IntegerEncoding::pos_int:
        return 0;
    case Asn1Acn::Types::IntegerEncoding::twos_complement:
        return -std::pow(2, type->size() - 1);
    case Asn1Acn::Types::IntegerEncoding::ASCII:
        return -std::pow(10, (type->size() / 8));
    case Asn1Acn::Types::IntegerEncoding::BCD:
        return 0;
    case Asn1Acn::Types::IntegerEncoding::unspecified:
    default:
        throw conversion::translator::TranslationException("Unhandled IntegerEncoding for container constraint");
        break;
    }
}

double ContainerConstraintTranslatorVisitor::getSmallestValue(Asn1Acn::Types::Real *type) const
{
    switch (type->encoding()) {
    case Asn1Acn::Types::RealEncoding::IEEE754_1985_32:
        return static_cast<double>(std::numeric_limits<float>::min());
    case Asn1Acn::Types::RealEncoding::IEEE754_1985_64:
        return std::numeric_limits<double>::min();
    case Asn1Acn::Types::RealEncoding::unspecified:
    default:
        throw conversion::translator::TranslationException("Unhandled RealEncoding for container constraint");
        break;
    }
}

std::int64_t ContainerConstraintTranslatorVisitor::getGreatestValue(Asn1Acn::Types::Integer *type) const
{
    switch (type->encoding()) {
    case Asn1Acn::Types::IntegerEncoding::pos_int:
        return std::pow(2, type->size()) - 1;
    case Asn1Acn::Types::IntegerEncoding::twos_complement:
        return std::pow(2, type->size() - 1) - 1;
    case Asn1Acn::Types::IntegerEncoding::ASCII:
        return std::pow(10, (type->size() / 8)) - 1;
    case Asn1Acn::Types::IntegerEncoding::BCD:
        return std::pow(10, (type->size() / 4)) - 1;
    case Asn1Acn::Types::IntegerEncoding::unspecified:
    default:
        throw conversion::translator::TranslationException("Unhandled IntegerEncoding for container constraint");
        break;
    }
}

double ContainerConstraintTranslatorVisitor::getGreatestValue(Asn1Acn::Types::Real *type) const
{
    switch (type->encoding()) {
    case Asn1Acn::Types::RealEncoding::IEEE754_1985_32:
        return static_cast<double>(std::numeric_limits<float>::max());
    case Asn1Acn::Types::RealEncoding::IEEE754_1985_64:
        return std::numeric_limits<double>::max();
    case Asn1Acn::Types::RealEncoding::unspecified:
    default:
        throw conversion::translator::TranslationException("Unhandled RealEncoding for container constraint");
        break;
    }
}

} // namespace conversion::asn1::translator
