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

#include "datatypesdependencyresolver.h"

#include <algorithm>
#include <conversion/common/simulink/matlabstandarddatatypes.h>
#include <conversion/common/simulink/tastestandarddatatypes.h>
#include <conversion/common/translation/exceptions.h>
#include <simulink/SimulinkModel/datatypes/aliasdatatype.h>
#include <simulink/SimulinkModel/datatypes/busdatatype.h>

using conversion::simulink::MatLabStandardDataTypes;
using conversion::simulink::TasteStandardDataTypes;
using conversion::translator::NotDagException;
using conversion::translator::UndeclaredDataTypeException;
using simulink::model::AliasDataType;
using simulink::model::BusDataType;
using simulink::model::DataType;

namespace conversion::asn1::translator::simulink {

DataTypesDependencyResolver::DataTypesDependencyResolver(const SimulinkDataTypes &simulinkDataTypes)
    : m_simulinkDataTypes(simulinkDataTypes)
{
    m_marks.clear();
    m_result.clear();
}

const DataTypesDependencyResolver::ResultList &DataTypesDependencyResolver::resolve()
{
    for (const auto &dataType : m_simulinkDataTypes) {
        visit(dataType);
    }

    return m_result;
}

void DataTypesDependencyResolver::visit(const DataType &dataType)
{
    if (isDataTypeMarkedAs(dataType, MarkType::Permanent)) {
        return;
    }

    if (isDataTypeMarkedAs(dataType, MarkType::Temporary)) {
        throw NotDagException(dataTypeNameStr(dataType));
    }

    markDataTypeAs(dataType, MarkType::Temporary);

    if (const auto *aliasDataType = std::get_if<AliasDataType>(&dataType)) {
        visitAlias(*aliasDataType);
    } else if (const auto *busDataType = std::get_if<BusDataType>(&dataType)) {
        visitBus(*busDataType);
    }

    markDataTypeAs(dataType, MarkType::Permanent);

    m_result.push_back(&dataType);
}

void DataTypesDependencyResolver::visitAlias(const AliasDataType &aliasDataType)
{
    const auto &baseType = aliasDataType.baseType();

    if (MatLabStandardDataTypes::isMatLabType(baseType) || TasteStandardDataTypes::isTasteType(baseType)) {
        return;
    }

    const auto &itemDataType = findDataType(baseType);
    visit(itemDataType);
}

void DataTypesDependencyResolver::visitBus(const BusDataType &busDataType)
{
    for (const auto &busMember : busDataType.busMembers()) {
        const auto &busMemberDataType = busMember.dataType();

        if (MatLabStandardDataTypes::isMatLabType(busMemberDataType)
                || TasteStandardDataTypes::isTasteType(busMemberDataType)) {
            continue;
        }

        const auto &itemDataType = findDataType(busMemberDataType);
        visit(itemDataType);
    }
}

const DataType &DataTypesDependencyResolver::findDataType(const QString &dataTypeName)
{
    auto compareNamesOfDataTypes = [&dataTypeName](
                                           const auto &dataType) { return dataTypeNameStr(dataType) == dataTypeName; };

    auto result = std::find_if(m_simulinkDataTypes.begin(), m_simulinkDataTypes.end(), compareNamesOfDataTypes);

    if (result != m_simulinkDataTypes.end()) {
        return *result;
    }

    throw UndeclaredDataTypeException(dataTypeName);
}

void DataTypesDependencyResolver::markDataTypeAs(const DataType &dataType, MarkType markType)
{
    m_marks[&dataType] = markType;
}

bool DataTypesDependencyResolver::isDataTypeMarkedAs(const DataType &dataType, MarkType markType) const
{
    if (m_marks.count(&dataType) == 0) {
        return false;
    }

    return m_marks.at(&dataType) == markType;
}

} // namespace conversion::asn1::translator::simulink
