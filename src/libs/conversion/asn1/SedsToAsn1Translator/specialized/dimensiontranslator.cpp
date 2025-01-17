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

#include "specialized/dimensiontranslator.h"

#include "specialized/sizetranslatorvisitor.h"

#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/translation/exceptions.h>
#include <limits>
#include <seds/SedsModel/package/package.h>

using conversion::translator::TranslationException;

namespace conversion::asn1::translator::seds {

using SizeTranslator = SizeTranslatorVisitor<Asn1Acn::Types::SequenceOf, Asn1Acn::IntegerValue>;

DimensionTranslator::DimensionTranslator(Context &context)
    : m_context(context)
    , m_threshold(m_context.arraySizeThreshold())
{
}

void DimensionTranslator::translate(
        const ::seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1Type) const
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
        const ::seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1Type) const
{
    auto dimensionSize = dimension.size()->value();

    if (m_threshold.has_value() && dimensionSize > *m_threshold) {
        dimensionSize = *m_threshold;
    }

    if (dimensionSize > static_cast<::seds::model::PositiveLong::Value>(
                                std::numeric_limits<Asn1Acn::IntegerValue::Type>::max())) {
        const auto message = QString("Dimension size (%1) overflows ASN.1 range").arg(dimensionSize);
        throw TranslationException(message);
    }

    if (dimensionSize <= 0) {
        auto errorMessage = QString("Dimension size of %1 must be greater than zero").arg(asn1Type->identifier());
        throw TranslationException(std::move(errorMessage));
    }

    auto rangeConstraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
            { static_cast<Asn1Acn::IntegerValue::Type>(dimensionSize),
                    static_cast<Asn1Acn::IntegerValue::Type>(dimensionSize) });
    auto sizeConstraint = std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::IntegerValue>>();
    sizeConstraint->setInnerConstraints(std::move(rangeConstraint));

    asn1Type->constraints().append(std::move(sizeConstraint));
}

void DimensionTranslator::translateIndexDimension(
        const ::seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1Type) const
{
    const auto indexType = m_context.findSedsType(*dimension.indexTypeRef());

    if (const auto integerIndexType = std::get_if<::seds::model::IntegerDataType>(indexType); integerIndexType) {
        SizeTranslator sizeTranslator(
                asn1Type, SizeTranslator::LengthType::FixedLength, SizeTranslator::SourceType::Index, m_threshold);
        std::visit(sizeTranslator, integerIndexType->range());

        if (sizeTranslator.getLastSetMin() != 0) {
            auto errorMessage = QString("The lowest value of integer %1 used as an index type for %2 is not 0, and due "
                                        "to ASN.1 limitations it is not supported")
                                        .arg(integerIndexType->nameStr())
                                        .arg(asn1Type->identifier());
            throw TranslationException(std::move(errorMessage));
        }
    } else if (const auto enumIndexType = std::get_if<::seds::model::EnumeratedDataType>(indexType); enumIndexType) {
        translateEnumDimensionIndex(*enumIndexType, asn1Type);
    } else {
        throw TranslationException("Only integer and enum dimension index types are supported");
    }
}

void DimensionTranslator::translateEnumDimensionIndex(
        const ::seds::model::EnumeratedDataType &indexType, Asn1Acn::Types::SequenceOf *asn1Type) const
{
    const auto &enumItems = indexType.enumerationList();

    if (enumItems.empty()) {
        auto errorMessage = QString("Enumeration %1 used as an index type for %2 doesn't contain any values")
                                    .arg(indexType.nameStr())
                                    .arg(asn1Type->identifier());
        throw TranslationException(std::move(errorMessage));
    }

    std::vector<long> enumValues;
    std::transform(enumItems.begin(), enumItems.end(), std::back_inserter(enumValues),
            [](const auto &enumItem) { return enumItem.value(); });
    std::sort(enumValues.begin(), enumValues.end());

    if (enumValues.front() < 0) {
        auto errorMessage = QString("Enumeration %1 used as an index type for %2 contains a negative values")
                                    .arg(indexType.nameStr())
                                    .arg(asn1Type->identifier());
        throw TranslationException(std::move(errorMessage));
    }

    SizeTranslator sizeTranslator(
            asn1Type, SizeTranslator::LengthType::FixedLength, SizeTranslator::SourceType::Index, m_threshold);
    if (enumValues.front() != 0) {
        auto errorMessage = QString("The lowest value of enumeration %1 used as an index type for %2 is not 0, and due "
                                    "to ASN.1 limitations it is not supported")
                                    .arg(indexType.nameStr())
                                    .arg(asn1Type->identifier());
        throw TranslationException(std::move(errorMessage));
    }
    sizeTranslator.addSizeConstraint(enumValues.back() - enumValues.front());
}

} // namespace conversion::asn1::translator::seds
