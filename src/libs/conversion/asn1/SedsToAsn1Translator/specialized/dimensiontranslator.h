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

#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <memory>
#include <optional>

namespace seds::model {
class DimensionSize;
} // namespace seds::model

namespace conversion::asn1::translator {

/**
 * @brief   Translator for SEDS dimensions
 */
class DimensionTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   asn1Definitions     Parent ASN.1 defintions
     */
    explicit DimensionTranslator(Asn1Acn::Definitions *asn1Definitions);

    /**
     * @brief   Translate SEDS array dimension
     *
     * This is translated as ASN.1 range constraint
     *
     * @param   dimension       SEDS array dimension
     * @param   asn1Sequence    ASN.1 type that will be updated
     */
    auto translateDimension(const seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1Sequence) const
            -> void;

private:
    auto translateSizeDimension(
            const seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1Sequence) const -> void;
    auto translateIndexDimension(
            const seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1Sequence) const -> void;

    auto translateIntegerDimensionIndex(
            const Asn1Acn::Types::Integer *indexType, Asn1Acn::Types::SequenceOf *asn1Sequence) const -> void;
    auto translateEnumDimensionIndex(
            const Asn1Acn::Types::Enumerated *indexType, Asn1Acn::Types::SequenceOf *asn1Sequence) const -> void;

    auto mergeRanges(const Asn1Acn::Constraints::Constraint<Asn1Acn::IntegerValue> *constraint) const
            -> std::optional<Asn1Acn::Range<Asn1Acn::IntegerValue::Type>>;

private:
    /// @brief  Parent definitions
    Asn1Acn::Definitions *m_asn1Definitions;
};

} // namespace conversion::asn1::translator
