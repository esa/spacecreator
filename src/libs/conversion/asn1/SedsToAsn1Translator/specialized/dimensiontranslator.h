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

#pragma once

#include "context.h"

#include <asn1library/asn1/types/sequenceof.h>
#include <seds/SedsModel/types/dimensionsize.h>

namespace conversion::asn1::translator {

class DimensionTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context     Current translation context
     */
    explicit DimensionTranslator(Context &context);
    /**
     * @brief   Deleted copy constructor
     */
    DimensionTranslator(const DimensionTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    DimensionTranslator(DimensionTranslator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    DimensionTranslator &operator=(const DimensionTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    DimensionTranslator &operator=(DimensionTranslator &&) = delete;

public:
    /**
     * @brief   Translate SEDS dimension to ASN.1 SequenceOf size constriant
     *
     * @param   dimension       Dimension to translate
     * @param   asn1Type        Sequence to constraint
     */
    auto translate(const seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1Type) const -> void;

private:
    auto translateSizeDimension(const seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1Type) const
            -> void;
    auto translateIndexDimension(
            const seds::model::DimensionSize &dimension, Asn1Acn::Types::SequenceOf *asn1Type) const -> void;

    auto translateEnumDimensionIndex(
            const seds::model::EnumeratedDataType &indexType, Asn1Acn::Types::SequenceOf *asn1Type) const -> void;

private:
    Context &m_context;

    std::optional<uint64_t> m_threshold;
};

} // namespace conversion::asn1::translator
