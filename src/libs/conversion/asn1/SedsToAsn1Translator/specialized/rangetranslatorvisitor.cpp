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

#include "specialized/rangetranslatorvisitor.h"

namespace conversion::asn1::translator {

template<>
void RangeTranslatorVisitor<Asn1Acn::RealValue>::operator()(const seds::model::MinMaxRange &range)
{
    switch (range.type()) {
    case seds::model::RangeType::ExclusiveMinExclusiveMax:
    case seds::model::RangeType::InclusiveMinExclusiveMax:
    case seds::model::RangeType::ExclusiveMinInclusiveMax:
    case seds::model::RangeType::GreaterThan:
    case seds::model::RangeType::LessThan: {
        auto errorMessage =
                QString("Exclusive min-max ranges are not supported for floating point values (in type \"%1\")")
                        .arg(m_asn1Type->identifier());
        throw conversion::translator::TranslationException(std::move(errorMessage));
    } break;
    case seds::model::RangeType::InclusiveMinInclusiveMax: {
        const auto min = getMin(range);
        const auto max = getMax(range);

        m_constraints.append(createRangeConstraint(min, max));
    } break;
    case seds::model::RangeType::AtLeast: {
        const auto min = getMin(range);
        const auto max = getGreatest();

        m_constraints.append(createRangeConstraint(min, max));
    } break;
    case seds::model::RangeType::AtMost: {
        const auto min = getSmallest();
        const auto max = getMax(range);

        m_constraints.append(createRangeConstraint(min, max));
    } break;
    }
}

template<>
void RangeTranslatorVisitor<Asn1Acn::IntegerValue>::operator()(const seds::model::MinMaxRange &range)
{
    switch (range.type()) {
    case seds::model::RangeType::ExclusiveMinExclusiveMax: {
        const auto min = getMin(range) + 1;
        const auto max = getMax(range) - 1;

        m_constraints.append(createRangeConstraint(min, max));
    } break;
    case seds::model::RangeType::InclusiveMinInclusiveMax: {
        const auto min = getMin(range);
        const auto max = getMax(range);

        m_constraints.append(RangeConstraint::create({ min, max }));
    } break;
    case seds::model::RangeType::InclusiveMinExclusiveMax: {
        const auto min = getMin(range);
        const auto max = getMax(range) - 1;

        m_constraints.append(createRangeConstraint(min, max));
    } break;
    case seds::model::RangeType::ExclusiveMinInclusiveMax: {
        const auto min = getMin(range) + 1;
        const auto max = getMax(range);

        m_constraints.append(createRangeConstraint(min, max));
    } break;
    case seds::model::RangeType::GreaterThan: {
        const auto min = getMin(range) + 1;
        const auto max = getGreatest();

        m_constraints.append(createRangeConstraint(min, max));
    } break;
    case seds::model::RangeType::LessThan: {
        const auto min = getSmallest();
        const auto max = getMax(range) - 1;

        m_constraints.append(createRangeConstraint(min, max));
    } break;
    case seds::model::RangeType::AtLeast: {
        const auto min = getMin(range);
        const auto max = getGreatest();

        m_constraints.append(createRangeConstraint(min, max));
    } break;
    case seds::model::RangeType::AtMost: {
        const auto min = getSmallest();
        const auto max = getMax(range);

        m_constraints.append(createRangeConstraint(min, max));
    } break;
    }
}

} // namespace conversion::asn1::translator
