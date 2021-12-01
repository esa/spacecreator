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
class MinMaxRange;
}

namespace conversion::asn1::translator {

/**
 * @brief   Translator visitor for SEDS integer range
 *
 * Translated range will be added to the passed ASN.1 constraint
 */
struct IntegerRangeTranslatorVisitor final {
    /** @brief  Where translated range will be added */
    Asn1Acn::Constraints::ConstraintList<Asn1Acn::IntegerValue> &m_constraints;

    /**
     * @brief   Translate SEDS min-max range
     *
     * @param   range   Range to translate
     */
    auto operator()(const seds::model::MinMaxRange &range) -> void;
};

} // namespace conversion::asn1::translator
