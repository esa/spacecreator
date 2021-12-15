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

#include "specialized/integerrangetranslatorvisitor.h"

#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <limits>
#include <seds/SedsModel/types/ranges/minmaxrange.h>

using Asn1Acn::IntegerValue;

namespace conversion::asn1::translator {

void IntegerRangeTranslatorVisitor::operator()(const seds::model::MinMaxRange &range)
{
    const auto min = range.min() ? IntegerValue::fromAstValue(range.min()->value())
                                 : std::numeric_limits<IntegerValue::Type>::min();
    const auto max = range.max() ? IntegerValue::fromAstValue(range.max()->value())
                                 : std::numeric_limits<IntegerValue::Type>::max();

    auto constraint = Asn1Acn::Constraints::RangeConstraint<IntegerValue>::create({ min, max });
    m_constraints.append(std::move(constraint));
}

} // namespace conversion::asn1::translator
