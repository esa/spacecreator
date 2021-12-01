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

#include "specialized/dimensiontranslator.h"

#include <asn1library/asn1/constraints/logicoperators.h>
#include <asn1library/asn1/types/enumerated.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/types/dimensionsize.h>

using conversion::translator::TranslationException;

namespace conversion::asn1::translator {

DimensionTranslator::DimensionTranslator(Asn1Acn::Definitions *asn1Definitions)
    : m_asn1Definitions(asn1Definitions)
{
}

void DimensionTranslator::translateDimension(
        const seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1Type) const
{
    if (dimension.size()) {
        translateSizeDimension(dimension, asn1Type);
    } else if (dimension.indexTypeRef()) {
        translateIndexDimension(dimension, asn1Type);
    } else {
        throw TranslationException("Array dimension without size nor index type");
    }
}

void DimensionTranslator::translateSizeDimension(
        const seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1Type) const
{
    const auto dimensionSize = dimension.size()->value();

    if (dimensionSize > std::numeric_limits<Asn1Acn::IntegerValue::Type>::max()) {
        const auto message = QString("Dimension size (%1) overflows ASN.1 range").arg(dimensionSize);
        throw TranslationException(message);
    }

    auto rangeConstraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
            { 0, static_cast<Asn1Acn::IntegerValue::Type>(dimensionSize) });

    auto sizeConstraint = std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::IntegerValue>>();
    sizeConstraint->setInnerConstraints(std::move(rangeConstraint));
    asn1Type->constraints().append(std::move(sizeConstraint));
}

void DimensionTranslator::translateIndexDimension(
        const seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1Type) const
{
    const auto indexTypeName = dimension.indexTypeRef()->nameStr();
    const auto indexType = m_asn1Definitions->type(indexTypeName)->type();

    if (const auto integerIndexType = dynamic_cast<const Asn1Acn::Types::Integer *>(indexType); integerIndexType) {
        translateIntegerDimensionIndex(integerIndexType, asn1Type);
    } else if (const auto enumIndexType = dynamic_cast<const Asn1Acn::Types::Enumerated *>(indexType); enumIndexType) {
        throw TranslationException("Enumerated dimension index type not implemented");
    } else {
        throw TranslationException("Only integer and enum dimension index types are supported");
    }
}

void DimensionTranslator::translateIntegerDimensionIndex(
        const Asn1Acn::Types::Integer *indexType, Asn1Acn::Types::SequenceOf *asn1Type) const
{
    const auto &constraints = indexType->constraints().constraints();
    auto resultRange = std::accumulate(std::next(constraints.begin()), constraints.end(),
            mergeRanges(constraints[0].get()), [&](const auto &range, const auto &constraint) {
                auto mergedRange = mergeRanges(constraint.get());

                if (!range) {
                    return mergedRange;
                } else if (!mergedRange) {
                    return range;
                } else {
                    return std::make_optional(range->merge(*mergedRange));
                }
            });

    if (!resultRange) {
        return;
    }

    auto rangeConstraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(*resultRange);

    auto sizeConstraint = std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::IntegerValue>>();
    sizeConstraint->setInnerConstraints(std::move(rangeConstraint));
    asn1Type->constraints().append(std::move(sizeConstraint));
}

std::optional<Asn1Acn::Range<Asn1Acn::IntegerValue::Type>> DimensionTranslator::mergeRanges(
        const Asn1Acn::Constraints::Constraint<Asn1Acn::IntegerValue> *constraint) const
{
    if (!constraint) {
        return std::nullopt;
    }

    if (const auto *andConstraint =
                    dynamic_cast<const Asn1Acn::Constraints::AndConstraint<Asn1Acn::IntegerValue> *>(constraint);
            andConstraint) {
        const auto leftRange = mergeRanges(andConstraint->leftChild());
        const auto rightRange = mergeRanges(andConstraint->rightChild());

        if (!leftRange) {
            return rightRange;
        } else if (!rightRange) {
            return leftRange;
        } else {
            return leftRange->merge(*rightRange);
        }
    } else if (const auto *orConstraint =
                       dynamic_cast<const Asn1Acn::Constraints::OrConstraint<Asn1Acn::IntegerValue> *>(constraint);
               orConstraint) {
        const auto leftRange = mergeRanges(andConstraint->leftChild());
        const auto rightRange = mergeRanges(andConstraint->rightChild());

        if (!leftRange) {
            return rightRange;
        } else if (!rightRange) {
            return leftRange;
        } else {
            return leftRange->merge(*rightRange);
        }
    } else if (const auto *rangeConstraint =
                       dynamic_cast<const Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue> *>(constraint);
               rangeConstraint) {
        return rangeConstraint->range();
    } else {
        return std::nullopt;
    }
}

} // namespace conversion::asn1::translator
