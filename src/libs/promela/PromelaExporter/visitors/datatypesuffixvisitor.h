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

#include <QTextStream>
#include <promela/PromelaModel/datatype.h>

namespace promela::exporter {
/**
 * @brief  Visitor for exporting @link{model::DataType}
 *
 * This translator creates suffix for data type.
 * This should be used together with DataTypePrefixVisitor.
 */
class DataTypeSuffixVisitor
{
public:
    /**
     * @brief  Constructor.
     *
     * @param stream  steam to append data type suffix
     */
    DataTypeSuffixVisitor(QTextStream &stream);

    /**
     * @brief  Visit DataType
     *
     * @param dataType DataType to visit
     */
    void visit(const model::DataType &dataType);

    /**
     * @brief Visit value of DataType, which is std::variant
     *
     * @param type std::variant to visit
     */
    void visit(const model::DataType::Type &type);
    /**
     * @brief Visit value of ArrayType, which is std::variant
     *
     * @param type std::variant to visit
     */
    void visit(const model::ArrayType::Type &type);

    /**
     * @brief Handle UnsignedDatatype
     *
     * This function appends prefix for UnsignedDataType to stream.
     *
     * @param type type to translate.
     */
    void operator()(const model::UnsignedDataType &type);
    /**
     * @brief Handle BasicType
     *
     * This function appends prefix for BasicType to stream.
     *
     * @param type type to translate.
     */
    void operator()(const model::BasicType &type);
    /**
     * @brief Handle UtypeRef
     *
     * This function appends prefix for UtypeRef to stream.
     *
     * @param type type to translate.
     */
    void operator()(const model::UtypeRef &type);
    /**
     * @brief Handle MtypeRef
     *
     * This function appends prefix for MtypeRef to stream.
     *
     * @param type type to translate.
     */
    void operator()(const model::MtypeRef &type);
    /**
     * @brief Handle ArrayType
     *
     * This function appends prefix for ArrayType to stream.
     *
     * @param type type to translate.
     */
    void operator()(const model::ArrayType &type);

private:
    QTextStream &m_stream;
};
}
