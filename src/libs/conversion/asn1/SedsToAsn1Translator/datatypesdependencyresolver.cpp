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
#include <conversion/common/translation/exceptions.h>
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

using conversion::translator::NotDagException;
using conversion::translator::TranslationException;
using conversion::translator::UndeclaredDataTypeException;

namespace conversion::asn1::translator::seds {

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
            markDataTypeAs(dataType, MarkType::Permanent);
        }
    }

    for (const auto *dataType : *m_dataTypes) {
        visit(dataType);
    }

    return m_result;
}

void DataTypesDependencyResolver::visit(const ::seds::model::DataType *dataType)
{
    if (isDataTypeMarkedAs(dataType, MarkType::Permanent)) {
        return;
    }

    if (isDataTypeMarkedAs(dataType, MarkType::Temporary)) {
        throw NotDagException(dataTypeNameStr(*dataType));
    }

    markDataTypeAs(dataType, MarkType::Temporary);

    if (const auto *arrayDataType = std::get_if<::seds::model::ArrayDataType>(dataType)) {
        visitArray(*arrayDataType);
    } else if (const auto *containerDataType = std::get_if<::seds::model::ContainerDataType>(dataType)) {
        visitContainer(*containerDataType);
    }

    markDataTypeAs(dataType, MarkType::Permanent);
    m_result.push_back(dataType);
}

void DataTypesDependencyResolver::visitArray(const ::seds::model::ArrayDataType &arrayDataType)
{
    const auto &itemDataTypeRef = arrayDataType.type();

    if (itemDataTypeRef.packageStr().has_value()) {
        return;
    }

    const auto *itemDataType = findDataType(itemDataTypeRef.nameStr());
    visit(itemDataType);

    for (const auto &dimension : arrayDataType.dimensions()) {
        const auto &indexTypeRef = dimension.indexTypeRef();
        if (indexTypeRef) {
            if (indexTypeRef->packageStr()) {
                return;
            }

            const auto *indexType = findDataType(indexTypeRef->nameStr());
            visit(indexType);
        }
    }
}

void DataTypesDependencyResolver::visitContainer(const ::seds::model::ContainerDataType &containerDataType)
{
    const auto &baseTypeRef = containerDataType.baseType();
    if (baseTypeRef) {
        if (baseTypeRef->packageStr().has_value()) {
            auto errorMessage = QString("Cross-package reference inheritance in type \"%1\" is not supported because "
                                        "of the ASN.1 limitations")
                                        .arg(containerDataType.nameStr());
            throw TranslationException(std::move(errorMessage));
        }

        const auto *baseType = findDataType(baseTypeRef->nameStr());
        visit(baseType);
    }

    const auto visitor = [this](auto &&entry) {
        using T = std::decay_t<decltype(entry)>;
        if constexpr (std::is_same_v<T, ::seds::model::PaddingEntry>) {
            return;
        } else {
            const auto &entryTypeRef = entry.type();

            if (entryTypeRef.packageStr().has_value()) {
                return;
            }

            const auto *entryDataType = findDataType(entryTypeRef.nameStr());
            visit(entryDataType);
        }
    };

    for (const auto &containerEntry : containerDataType.entries()) {
        std::visit(visitor, containerEntry);
    }
    for (const auto &containerTrailerEntry : containerDataType.trailerEntries()) {
        std::visit(visitor, containerTrailerEntry);
    }
}

const ::seds::model::DataType *DataTypesDependencyResolver::findDataType(const QString &dataTypeName)
{
    auto result = std::find_if(m_dataTypes->begin(), m_dataTypes->end(),
            [&dataTypeName](const auto *dataType) { return dataTypeNameStr(*dataType) == dataTypeName; });

    if (result != m_dataTypes->end()) {
        return *result;
    }

    if (m_globalDataTypes) {
        result = std::find_if(m_globalDataTypes->begin(), m_globalDataTypes->end(),
                [&dataTypeName](const auto *dataType) { return dataTypeNameStr(*dataType) == dataTypeName; });

        if (result != m_globalDataTypes->end()) {
            return *result;
        }
    }

    throw UndeclaredDataTypeException(dataTypeName);
    return nullptr;
}

void DataTypesDependencyResolver::markDataTypeAs(const ::seds::model::DataType *dataType, MarkType markType)
{
    m_marks[dataType] = markType;
}

bool DataTypesDependencyResolver::isDataTypeMarkedAs(const ::seds::model::DataType *dataType, MarkType markType) const
{
    if (m_marks.count(dataType) == 0) {
        return false;
    }

    return m_marks.at(dataType) == markType;
}

} // namespace conversion::asn1::translator::seds
