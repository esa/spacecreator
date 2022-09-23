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

#include "busdatatypetranslator.h"
#include "context.h"

#include <simulink/SimulinkModel/datatypes/aliasdatatype.h>
#include <simulink/SimulinkModel/datatypes/busdatatype.h>
#include <simulink/SimulinkModel/datatypes/enumdatatype.h>

namespace conversion::asn1::translator::simulink {

/**
 * @brief   Translator visitor for SIMULINK data type
 *
 * Translated data type will be added to the passed simulink model ASN.1 Definitions
 */
class DataTypeTranslatorVisitor final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   context                 Current translation context
     */
    explicit DataTypeTranslatorVisitor(Context &context);
    /**
     * @brief   Deleted copy constructor
     */
    DataTypeTranslatorVisitor(const DataTypeTranslatorVisitor &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    DataTypeTranslatorVisitor(DataTypeTranslatorVisitor &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    DataTypeTranslatorVisitor &operator=(const DataTypeTranslatorVisitor &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    DataTypeTranslatorVisitor &operator=(DataTypeTranslatorVisitor &&) = delete;

public:
    /**
     * @brief   Translate SIMULINK enum data type
     *
     * @param   simulinkEnumType    Type to translate
     */
    auto operator()(const ::simulink::model::EnumDataType &simulinkEnumType) -> void;
    /**
     * @brief   Translate SIMULINK alias data type
     *
     * @param   simulinkAliasType    Type to translate
     */
    auto operator()(const ::simulink::model::AliasDataType &simulinkAliasType) -> void;
    /**
     * @brief   Translate SIMULINK bus data type
     *
     * @param   simulinkBusType    Type to translate
     */
    auto operator()(const ::simulink::model::BusDataType &simulinkBusType) -> void;

private:
    Context &m_context;

    BusDataTypeTranslator m_busDataTypeTranslator;
};

} // namespace conversion::asn1::translator::simulink
