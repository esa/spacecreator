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

#pragma once

#include <asn1library/asn1/constraints/constraintlist.h>
#include <asn1library/asn1/values.h>

namespace seds::model {
enum class FloatPrecisionRange;
class MinMaxRange;
}

namespace conversion::asn1::translator {

struct FloatRangeTranslatorVisitor final {
    Asn1Acn::Constraints::ConstraintList<Asn1Acn::RealValue> &m_constraints;

    auto operator()(const seds::model::FloatPrecisionRange &range) -> void;
    auto operator()(const seds::model::MinMaxRange &range) -> void;

    constexpr static double singleRangeMin = 1.17549E-38;
    constexpr static double singleRangeMax = 3.40282e+38;
    constexpr static double doubleRangeMin = 2.22507e-308;
    constexpr static double doubleRangeMax = 1.79769e+308;
};

} // namespace conversion::asn1::translator
