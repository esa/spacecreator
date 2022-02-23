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

#include "specialized/rangetranslatorvisitor.h"
#include "specialized/sizetranslatorvisitor.h"
#include "translator.h"

#include <asn1library/asn1/values.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/package/package.h>
#include <seds/SedsModel/types/dimensionsize.h>
#include <seds/SedsModel/types/enumerateddatatype.h>
#include <seds/SedsModel/types/integerdatatype.h>

using conversion::translator::MissingAsn1TypeDefinitionException;
using conversion::translator::TranslationException;

namespace conversion::asn1::translator {

using SizeTranslator = SizeTranslatorVisitor<Asn1Acn::Types::SequenceOf, Asn1Acn::IntegerValue>;

DimensionTranslator::DimensionTranslator(
        const seds::model::Package *sedsPackage, const std::vector<seds::model::Package> &sedsPackages)
    : m_sedsPackage(sedsPackage)
    , m_sedsPackages(sedsPackages)
{
}

void DimensionTranslator::translateDimension(
        const seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1SequenceOf) const
{
    if (dimension.size()) {
        translateSizeDimension(dimension, asn1SequenceOf);
    } else if (dimension.indexTypeRef()) {
        translateIndexDimension(dimension, asn1SequenceOf);
    } else {
        throw TranslationException("Array dimension without size nor index type");
    }
}

void DimensionTranslator::translateSizeDimension(
        const seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1SequenceOf) const
{
    const auto dimensionSize = dimension.size()->value();

    if (dimensionSize > std::numeric_limits<Asn1Acn::IntegerValue::Type>::max()) {
        const auto message = QString("Dimension size (%1) overflows ASN.1 range").arg(dimensionSize);
        throw TranslationException(message);
    }

    if (dimensionSize <= 0) {
        auto errorMessage = QString("Dimension size of %1 must be greater than zero").arg(asn1SequenceOf->identifier());
        throw TranslationException(std::move(errorMessage));
    }

    auto rangeConstraint = Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>::create(
            { static_cast<Asn1Acn::IntegerValue::Type>(dimensionSize),
                    static_cast<Asn1Acn::IntegerValue::Type>(dimensionSize) });

    auto sizeConstraint = std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::IntegerValue>>();
    sizeConstraint->setInnerConstraints(std::move(rangeConstraint));
    asn1SequenceOf->constraints().append(std::move(sizeConstraint));
}

void DimensionTranslator::translateIndexDimension(
        const seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1SequenceOf) const
{
    const auto &indexTypeRef = dimension.indexTypeRef();
    const auto &indexTypeName = indexTypeRef->nameStr();

    const auto sedsPackage = indexTypeRef->packageStr()
            ? SedsToAsn1Translator::getSedsPackage(*indexTypeRef->packageStr(), m_sedsPackages)
            : m_sedsPackage;

    const auto indexType = sedsPackage->dataType(indexTypeName);

    if (!indexType) {
        throw MissingAsn1TypeDefinitionException(indexTypeName);
    }

    if (const auto integerIndexType = std::get_if<seds::model::IntegerDataType>(indexType); integerIndexType) {
        SizeTranslator sizeTranslator(
                asn1SequenceOf, SizeTranslator::LengthType::FixedLength, SizeTranslator::SourceType::Index);
        std::visit(sizeTranslator, integerIndexType->range());

        if (sizeTranslator.getLastSetMin() != 0) {
            auto errorMessage = QString("The lowest value of integer %1 used as an index type for %2 is not 0, and due "
                                        "to ASN.1 limitations it is not supported")
                                        .arg(integerIndexType->nameStr())
                                        .arg(asn1SequenceOf->identifier());
            throw TranslationException(std::move(errorMessage));
        }

    } else if (const auto enumIndexType = std::get_if<seds::model::EnumeratedDataType>(indexType); enumIndexType) {
        translateEnumDimensionIndex(*enumIndexType, asn1SequenceOf);
    } else {
        throw TranslationException("Only integer and enum dimension index types are supported");
    }
}

void DimensionTranslator::translateEnumDimensionIndex(
        const seds::model::EnumeratedDataType &indexType, Asn1Acn::Types::SequenceOf *asn1SequenceOf) const
{
    const auto &enumItems = indexType.enumerationList();

    if (enumItems.empty()) {
        auto errorMessage = QString("Enumeration %1 used as an index type for %2 doesn't contain any values")
                                    .arg(indexType.nameStr())
                                    .arg(asn1SequenceOf->identifier());
        throw TranslationException(std::move(errorMessage));
    }

    std::vector<long> enumValues;
    std::transform(enumItems.begin(), enumItems.end(), std::back_inserter(enumValues),
            [](const auto &enumItem) { return enumItem.value(); });
    std::sort(enumValues.begin(), enumValues.end());

    if (enumValues.front() < 0) {
        auto errorMessage = QString("Enumeration %1 used as an index type for %2 contains a negative values")
                                    .arg(indexType.nameStr())
                                    .arg(asn1SequenceOf->identifier());
        throw TranslationException(std::move(errorMessage));
    }

    SizeTranslator sizeTranslator(
            asn1SequenceOf, SizeTranslator::LengthType::FixedLength, SizeTranslator::SourceType::Index);
    if (enumValues.front() != 0) {
        auto errorMessage = QString("The lowest value of enumeration %1 used as an index type for %2 is not 0, and due "
                                    "to ASN.1 limitations it is not supported")
                                    .arg(indexType.nameStr())
                                    .arg(asn1SequenceOf->identifier());
        throw TranslationException(std::move(errorMessage));
    }
    sizeTranslator.addSizeConstraint(enumValues.back() - enumValues.front());
}

} // namespace conversion::asn1::translator
