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

#include "visitors/floatrangetranslatorvisitor.h"

#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <conversion/common/translation/exceptions.h>
#include <limits>
#include <seds/SedsModel/types/ranges/floatprecisionrange.h>
#include <seds/SedsModel/types/ranges/minmaxrange.h>

using Asn1Acn::RealValue;
using conversion::UnhandledValueException;
using conversion::UnsupportedValueException;
using seds::model::FloatPrecisionRange;
using seds::model::MinMaxRange;

namespace conversion::asn1::translator {

void FloatRangeTranslatorVisitor::operator()(const MinMaxRange &range)
{
    const auto min =
            range.min() ? RealValue::fromAstValue(range.min()->value()) : std::numeric_limits<RealValue::Type>::min();
    const auto max =
            range.max() ? RealValue::fromAstValue(range.max()->value()) : std::numeric_limits<RealValue::Type>::max();

    auto constraint = Asn1Acn::Constraints::RangeConstraint<RealValue>::create({ min, max });
    m_constraints.append(std::move(constraint));
}

void FloatRangeTranslatorVisitor::operator()(const FloatPrecisionRange &range)
{
    switch (range) {
    case FloatPrecisionRange::Single: {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<RealValue>::create({ singleRangeMin, singleRangeMax });
        m_constraints.append(std::move(constraint));
        break;
    }
    case FloatPrecisionRange::Double: {
        auto constraint = Asn1Acn::Constraints::RangeConstraint<RealValue>::create({ doubleRangeMin, doubleRangeMax });
        m_constraints.append(std::move(constraint));
        break;
    }
    case FloatPrecisionRange::Quad:
        throw UnsupportedValueException("FloatPrecisionRange", "Quad");
    default:
        throw UnhandledValueException("FloatPrecisionRange");
        break;
    }
}

} // namespace conversion::asn1::translator
