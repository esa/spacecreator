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
        const DataTypesDependencyResolver::DataTypes *dataTypes,
        const DataTypesDependencyResolver::DataTypes *globalDataTypes)
{
    m_marks.clear();
    m_result.clear();

    m_dataTypes = dataTypes;
    m_globalDataTypes = globalDataTypes;

    if (m_globalDataTypes) {
        for (const auto *dataType : *m_globalDataTypes) {
            markPermanent(dataType);
        }
    }

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
        throw NotDagException();
    }

    markTemporary(dataType);

    if (const auto *arrayDataType = std::get_if<seds::model::ArrayDataType>(dataType)) {
        visitArray(*arrayDataType);
    } else if (const auto *containerDataType = std::get_if<seds::model::ContainerDataType>(dataType)) {
        visitContainer(*containerDataType);
    }

    markPermanent(dataType);
    m_result.push_back(dataType);
}

void DataTypesDependencyResolver::visitArray(const seds::model::ArrayDataType &arrayDataType)
{
    const auto &itemDataTypeRef = arrayDataType.type();
    const auto *itemDataType = findDataType(itemDataTypeRef);

    visit(itemDataType);

    for (const auto &dimension : arrayDataType.dimensions()) {
        if (dimension.indexTypeRef()) {
            const auto *indexType = findDataType(*dimension.indexTypeRef());
            visit(indexType);
        }
    }
}

void DataTypesDependencyResolver::visitContainer(const seds::model::ContainerDataType &containerDataType)
{
    const auto visitor = [this](auto &&entry) {
        using T = std::decay_t<decltype(entry)>;
        if constexpr (std::is_same_v<T, seds::model::PaddingEntry>) {
            return;
        } else {
            const auto &entryDataTypeRef = entry.type();
            const auto *entryDataType = findDataType(entryDataTypeRef);

            visit(entryDataType);
        }
    };

    for (const auto &containerEntry : containerDataType.entries()) {
        std::visit(visitor, containerEntry);
    }
    for (const auto &containerTrailerEntry : containerDataType.trailerEntries()) {
        std::visit(visitor, containerTrailerEntry);
    }

    const auto &baseTypeRef = containerDataType.baseType();
    if (baseTypeRef) {
        const auto *baseType = findDataType(*baseTypeRef);

        visit(baseType);
    }
}

const seds::model::DataType *DataTypesDependencyResolver::findDataType(const seds::model::DataTypeRef &dataTypeRef)
{
    auto result = std::find_if(m_dataTypes->begin(), m_dataTypes->end(),
            [&dataTypeRef](const auto *dataType) { return dataTypeNameStr(*dataType) == dataTypeRef.nameStr(); });

    if (result != m_dataTypes->end()) {
        return *result;
    }

    if (m_globalDataTypes) {
        result = std::find_if(m_globalDataTypes->begin(), m_globalDataTypes->end(),
                [&dataTypeRef](const auto *dataType) { return dataTypeNameStr(*dataType) == dataTypeRef.nameStr(); });

        if (result != m_globalDataTypes->end()) {
            return *result;
        }
    }

    throw UndeclaredDataTypeException(dataTypeRef.nameStr());
    return nullptr;
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

NotDagException::NotDagException()
    : ConversionException("Data types doesn't make a DAG")
{
}

UndeclaredDataTypeException::UndeclaredDataTypeException(const QString &dataTypeName)
    : ConversionException(QString("Undeclared data type '%1'").arg(dataTypeName))
{
}

} // namespace conversion::asn1::translator
