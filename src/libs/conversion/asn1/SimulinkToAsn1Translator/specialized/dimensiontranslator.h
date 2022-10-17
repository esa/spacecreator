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

#pragma once

#include "context.h"

#include <asn1library/asn1/types/sequenceof.h>
#include <simulink/SimulinkModel/common/vectordimensions.h>

namespace conversion::asn1::translator::simulink {

/**
 * @brief   Translator for SIMULINK dimensions
 */
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
     * @brief   Default copy constructor
     */
    DimensionTranslator(const DimensionTranslator &) = delete;
    /**
     * @brief   Default move constructor
     */
    DimensionTranslator(DimensionTranslator &&) = delete;

    /**
     * @brief   Default copy assignment operator
     */
    DimensionTranslator &operator=(const DimensionTranslator &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    DimensionTranslator &operator=(DimensionTranslator &&) = delete;

public:
    /**
     * @brief   Translate SIMULINK scalar dimension
     *
     * @param   sequenceOfAsn1TypeName name for the constructed SequenceOf object
     * @param   itemAsn1Type           ASN.1 type corresponding to basetype for translated simulink datatype
     * @param   scalarDimensions       scalar dimension to translate
     *
     * @return  Constructed SequenceOf object
     */
    auto translate(const QString &sequenceOfAsn1TypeName, const Asn1Acn::Types::Type *itemAsn1Type,
            int64_t scalarDimensions) -> std::unique_ptr<Asn1Acn::Types::SequenceOf>;

    /**
     * @brief   Translate SIMULINK vector dimension
     *
     * @param   itemAsn1Type           ASN.1 type corresponding to basetype for translated simulink datatype
     * @param   vectorDimensions       vector dimension to translate
     *
     * @return  Constructed SequenceOf object
     */
    auto translate(const Asn1Acn::Types::Type *itemAsn1Type,
            const ::simulink::model::VectorDimensions::Vector &vectorDimensions)
            -> std::unique_ptr<Asn1Acn::Types::SequenceOf>;

private:
    auto addSizeConstraint(Asn1Acn::Types::SequenceOf *sequenceOf, const int64_t size) -> void;

private:
    Context &m_context;
};

} // namespace conversion::asn1::translator::simulink
