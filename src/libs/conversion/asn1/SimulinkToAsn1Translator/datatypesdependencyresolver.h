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

#include <list>
#include <simulink/SimulinkModel/simulinkmodel.h>
#include <unordered_map>

namespace conversion::asn1::translator {

/**
 * @brief   Utility used for resolving dependencies in the SIMULINK types
 *
 * ASN.1 model requires Type object to be passed while creating a UserdefinedType.
 * Simulink Exporter doesn't sort topologically data types so we have to sort them here.
 */
class DataTypesDependencyResolver final
{
private:
    /// @brief  Type of the mark for the soring
    enum class MarkType
    {
        Temporary,
        Permanent
    };

    using SimulinkDataTypes = simulink::model::SimulinkModel::DataTypes;
    using MarksMap = std::unordered_map<const simulink::model::DataType *, MarkType>;
    using ResultList = std::list<const simulink::model::DataType *>;

public:
    /**
     * @brief   Constructor
     *
     * @param   dataTypes       Data types to sort
     */
    DataTypesDependencyResolver(const SimulinkDataTypes &simulinkDataTypes);
    /**
     * @brief   Resolve dependencies
     *
     * @return  Sorted list of data types
     */
    auto resolve() -> const ResultList &;

private:
    auto visit(const simulink::model::DataType &dataType) -> void;
    auto visitAlias(const simulink::model::AliasDataType &aliasDataType) -> void;
    auto visitBus(const simulink::model::BusDataType &busDataType) -> void;

    auto findDataType(const QString &dataTypeName) -> const simulink::model::DataType &;

    auto markTemporary(const simulink::model::DataType &dataType) -> void;
    auto markPermanent(const simulink::model::DataType &dataType) -> void;

    auto isTemporarilyMarked(const simulink::model::DataType &dataType) -> bool;
    auto isPermanentlyMarked(const simulink::model::DataType &dataType) -> bool;

private:
    const SimulinkDataTypes &m_simulinkDataTypes;

    MarksMap m_marks;
    ResultList m_result;
};

} // namespace conversion::asn1::translator
