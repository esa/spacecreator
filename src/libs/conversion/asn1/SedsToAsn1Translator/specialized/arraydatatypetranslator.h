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
#include "specialized/dimensiontranslator.h"

#include <asn1library/asn1/types/sequenceof.h>
#include <seds/SedsModel/types/arraydatatype.h>

namespace conversion::asn1::translator {

/**
 * @brief   Translator for SEDS array data type
 */
class ArrayDataTypeTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context     Current translation context
     */
    explicit ArrayDataTypeTranslator(Context &context);
    /**
     * @brief   Deleted copy constructor
     */
    ArrayDataTypeTranslator(const ArrayDataTypeTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    ArrayDataTypeTranslator(ArrayDataTypeTranslator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    ArrayDataTypeTranslator &operator=(const ArrayDataTypeTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    ArrayDataTypeTranslator &operator=(ArrayDataTypeTranslator &&) = delete;

public:
    /**
     * @brief   Translate passed data type
     *
     * @param   sedsType    Type to translation
     */
    auto translate(const seds::model::ArrayDataType &sedsType) -> void;

private:
    auto translateArrayType(const seds::model::DataTypeRef &sedsTypeRef, Asn1Acn::Types::SequenceOf *asn1Type) const
            -> void;

private:
    Context &m_context;

    DimensionTranslator m_dimensionTranslator;
};

} // namespace conversion::asn1::translator
