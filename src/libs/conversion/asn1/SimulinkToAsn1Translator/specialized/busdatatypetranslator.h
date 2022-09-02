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
#include "dimensiontranslator.h"

#include <simulink/SimulinkModel/datatypes/busdatatype.h>

namespace conversion::asn1::translator {

/**
 * @brief   Translator for SIMULINK bus member
 */
class BusDataTypeTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context     Current translation context
     */
    explicit BusDataTypeTranslator(Context &context);
    /**
     * @brief   Deleted copy constructor
     */
    BusDataTypeTranslator(const BusDataTypeTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    BusDataTypeTranslator(BusDataTypeTranslator &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    BusDataTypeTranslator &operator=(const BusDataTypeTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    BusDataTypeTranslator &operator=(BusDataTypeTranslator &&) = delete;

public:
    /**
     * @brief   Translate bus data type
     *
     * @param   busDataType    Type to translation
     */
    auto translate(const simulink::model::BusDataType &busDataType) -> void;

private:
    auto appendAsn1TypeToSequence(Asn1Acn::Types::Sequence *sequence, const QString &sequenceComponentName,
            std::unique_ptr<Asn1Acn::Types::Type> asn1Type) -> void;

private:
    Context &m_context;
    DimensionTranslator m_dimensionTranslator;
};

} // namespace conversion::asn1::translator
