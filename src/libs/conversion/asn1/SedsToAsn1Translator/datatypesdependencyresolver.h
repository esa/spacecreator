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

#pragma once

#include <conversion/common/exceptions.h>
#include <list>
#include <seds/SedsModel/types/datatype.h>
#include <unordered_map>
#include <vector>

namespace seds::model {
class DataTypeRef;
} // namespace seds::model

namespace conversion::asn1::translator {

/**
 * @brief   Utility used for resolving dependencies in the SEDS types
 *
 * SEDS allows to reference a type (in an array or a container) before it's defined.
 * ASN.1 model on the other hand requires Type object to be passed while creating a UserdefinedType
 * Because of that we have to sort data types topologically so that we are sure that references in the
 * compound types are already translated.
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

    using DataTypes = std::vector<const seds::model::DataType *>;
    using MarksMap = std::unordered_map<const seds::model::DataType *, MarkType>;
    using ResultList = std::list<const seds::model::DataType *>;

public:
    /**
     * @brief   Resolve dependencies in given data type set
     *
     * @param   dataTypes       Data types to sort
     *
     * @return  Sorted list of data types
     */
    auto resolve(const DataTypes *dataTypes) -> ResultList;

private:
    auto visit(const seds::model::DataType *dataType) -> void;
    auto visitArray(const seds::model::ArrayDataType &arrayDataType) -> void;
    auto visitContainer(const seds::model::ContainerDataType &containerDataType) -> void;

    auto findDataType(const seds::model::DataTypeRef &dataTypeRef) -> const seds::model::DataType *;

    auto markTemporary(const seds::model::DataType *dataType) -> void;
    auto markPermanent(const seds::model::DataType *dataType) -> void;
    auto isTemporarilyMarked(const seds::model::DataType *dataType) -> bool;
    auto isPermanentlyMarked(const seds::model::DataType *dataType) -> bool;

private:
    const DataTypes *m_dataTypes;
    MarksMap m_marks;
    ResultList m_result;
};

/**
 * @brief   Exception thrown when data types to sort doesn't create a DAG
 *
 * Exception for cyclic dependency
 */
class NotDagException final : public ConversionException
{
public:
    /**
     * @brief   Constructor
     */
    NotDagException();
};

class UndeclaredDataTypeException final : public ConversionException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   dataTypeName    Name of the referenced undeclared type
     */
    UndeclaredDataTypeException(const QString &dataTypeName);
};

} // namespace conversion::asn1::translator
