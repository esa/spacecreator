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

#include <list>
#include <seds/SedsModel/types/datatype.h>
#include <vector>

namespace seds::model {
class DataTypeRef;
} // namespace seds::model

namespace conversion::asn1::translator {

class DataTypesDependencyResolver final
{
private:
    enum class MarkType
    {
        Temporary,
        Permanent
    };

    using DataTypes = std::vector<const seds::model::DataType *>;
    using MarksMap = std::unordered_map<const seds::model::DataType *, MarkType>;
    using ResultList = std::list<const seds::model::DataType *>;

public:
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

class NotDAGException final : public std::exception
{
public:
    virtual auto what() const noexcept -> const char * override;
};

class UndeclaredDataTypeException final : public std::exception
{
public:
    UndeclaredDataTypeException(QString dataTypeName);
    virtual auto what() const noexcept -> const char * override;

private:
    std::string m_message;
};

} // namespace conversion::asn1::translator
