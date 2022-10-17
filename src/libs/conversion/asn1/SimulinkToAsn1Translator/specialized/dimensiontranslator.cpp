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

#include "dimensiontranslator.h"

#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/types/type.h>
#include <asn1library/asn1/types/userdefinedtype.h>

using Asn1Acn::IntegerValue;
using Asn1Acn::Range;
using Asn1Acn::Constraints::RangeConstraint;
using Asn1Acn::Constraints::SizeConstraint;
using Asn1Acn::Types::SequenceOf;
using Asn1Acn::Types::Type;
using Asn1Acn::Types::UserdefinedType;
using simulink::model::VectorDimensions;

namespace conversion::asn1::translator::simulink {

DimensionTranslator::DimensionTranslator(Context &context)
    : m_context(context)
{
}

std::unique_ptr<SequenceOf> DimensionTranslator::translate(
        const QString &sequenceOfAsn1TypeName, const Type *itemAsn1Type, int64_t scalarDimensions)
{
    auto asn1SequenceOfType = std::make_unique<SequenceOf>(sequenceOfAsn1TypeName);
    auto asn1UserdefinedType =
            std::make_unique<UserdefinedType>(itemAsn1Type->identifier(), m_context.simulinkModelDefinitionsName());

    asn1SequenceOfType->setItemsType(std::move(asn1UserdefinedType));

    addSizeConstraint(asn1SequenceOfType.get(), scalarDimensions);

    return asn1SequenceOfType;
}

std::unique_ptr<SequenceOf> DimensionTranslator::translate(
        const Type *itemAsn1Type, const VectorDimensions::Vector &vectorDimensions)
{
    auto asn1SequenceOfType = std::make_unique<SequenceOf>(itemAsn1Type->identifier());

    addSizeConstraint(asn1SequenceOfType.get(), vectorDimensions[0]);

    // Create 'sequence of' chain
    auto *lastAsn1SequenceOfType = asn1SequenceOfType.get();
    std::for_each(std::next(vectorDimensions.begin()), vectorDimensions.end(), [&](const auto &dimension) {
        auto asn1SequenceOfSubType = std::make_unique<SequenceOf>();

        addSizeConstraint(asn1SequenceOfSubType.get(), dimension);

        lastAsn1SequenceOfType->setItemsType(std::move(asn1SequenceOfSubType));

        lastAsn1SequenceOfType = dynamic_cast<SequenceOf *>(lastAsn1SequenceOfType->itemsType());
    });

    auto asn1UserdefinedType = std::make_unique<UserdefinedType>(
            asn1SequenceOfType->identifier(), m_context.simulinkModelDefinitionsName());

    lastAsn1SequenceOfType->setItemsType(std::move(asn1UserdefinedType));

    return asn1SequenceOfType;
}

void DimensionTranslator::addSizeConstraint(Asn1Acn::Types::SequenceOf *sequenceOf, const int64_t size)
{
    auto range =
            Range<IntegerValue::Type>({ static_cast<IntegerValue::Type>(size), static_cast<IntegerValue::Type>(size) });
    auto rangeConstraint = RangeConstraint<IntegerValue>::create(range);
    auto sizeConstraint = std::make_unique<SizeConstraint<IntegerValue>>();
    sizeConstraint->setInnerConstraints(std::move(rangeConstraint));

    sequenceOf->constraints().append(std::move(sizeConstraint));
}

} // namespace conversion::asn1::translator::simulink
