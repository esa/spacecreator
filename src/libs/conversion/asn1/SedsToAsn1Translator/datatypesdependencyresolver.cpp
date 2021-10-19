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

#include "datatypesdependencyresolver.h"

#include <algorithm>
#include <seds/SedsModel/types/arraydatatype.h>
#include <seds/SedsModel/types/binarydatatype.h>
#include <seds/SedsModel/types/booleandatatype.h>
#include <seds/SedsModel/types/containerdatatype.h>
#include <seds/SedsModel/types/enumerateddatatype.h>
#include <seds/SedsModel/types/floatdatatype.h>
#include <seds/SedsModel/types/integerdatatype.h>
#include <seds/SedsModel/types/stringdatatype.h>
#include <seds/SedsModel/types/subrangedatatype.h>
#include <unordered_map>

namespace conversion::asn1::translator {

DataTypesDependencyResolver::ResultList DataTypesDependencyResolver::resolve(
        const DataTypesDependencyResolver::DataTypes *dataTypes)
{
    m_dataTypes = dataTypes;

    m_marks.clear();
    m_result.clear();

    for (const auto *dataType : *m_dataTypes) {
        visit(dataType);
    }

    return m_result;
}

void DataTypesDependencyResolver::visit(const seds::model::DataType *dataType)
{
    if (isPermanentlyMarked(dataType)) {
        return;
    }

    if (isTemporarilyMarked(dataType)) {
        throw NotDAGException();
    }

    markTemporary(dataType);

    std::visit(
            [this](auto &&dataType) {
                using T = std::decay_t<decltype(dataType)>;
                if constexpr (std::is_same_v<T, seds::model::ArrayDataType>) {
                    visitArray(dataType);
                } else if constexpr (std::is_same_v<T, seds::model::ContainerDataType>) {
                    visitContainer(dataType);
                }
            },
            *dataType);

    markPermanent(dataType);
    m_result.push_back(dataType);
}

void DataTypesDependencyResolver::visitArray(const seds::model::ArrayDataType &arrayDataType)
{
    const auto &itemDataTypeRef = arrayDataType.type();
    const auto *itemDataType = findDataType(itemDataTypeRef);

    visit(itemDataType);
}

void DataTypesDependencyResolver::visitContainer(const seds::model::ContainerDataType &containerDataType)
{
    for (const auto &entry : containerDataType.entries()) {
        std::visit(
                [this](auto &&entry) {
                    using T = std::decay_t<decltype(entry)>;
                    if constexpr (std::is_same_v<T, seds::model::PaddingEntry>) {
                        return;
                    } else {
                        const auto &entryDataTypeRef = entry.type();
                        const auto *entryDataType = findDataType(entryDataTypeRef);

                        visit(entryDataType);
                    }
                },
                entry);
    }
}

const seds::model::DataType *DataTypesDependencyResolver::findDataType(const seds::model::DataTypeRef &dataTypeRef)
{
    const auto result = std::find_if(m_dataTypes->begin(), m_dataTypes->end(),
            [&dataTypeRef](const auto *dataType) { return dataTypeNameStr(*dataType) == dataTypeRef.nameStr(); });

    if (result == m_dataTypes->end()) {
        throw UndeclaredDataTypeException(dataTypeRef.nameStr());
        return nullptr;
    }

    return *result;
}

void DataTypesDependencyResolver::markTemporary(const seds::model::DataType *dataType)
{
    m_marks.insert({ dataType, MarkType::Temporary });
}

void DataTypesDependencyResolver::markPermanent(const seds::model::DataType *dataType)
{
    m_marks[dataType] = MarkType::Permanent;
}

bool DataTypesDependencyResolver::isTemporarilyMarked(const seds::model::DataType *dataType)
{
    if (m_marks.count(dataType) == 0) {
        return false;
    }

    return m_marks.at(dataType) == MarkType::Temporary;
}

bool DataTypesDependencyResolver::isPermanentlyMarked(const seds::model::DataType *dataType)
{
    if (m_marks.count(dataType) == 0) {
        return false;
    }

    return m_marks.at(dataType) == MarkType::Permanent;
}

const char *NotDAGException::what() const noexcept
{
    return "Data types doesn't make a DAG";
}

UndeclaredDataTypeException::UndeclaredDataTypeException(QString dataTypeName)
    : m_message(QString("Undeclared data type '%1'").arg(std::move(dataTypeName)).toStdString())
{
}

const char *UndeclaredDataTypeException::what() const noexcept
{
    return m_message.c_str();
}

} // namespace conversion::asn1::translator
