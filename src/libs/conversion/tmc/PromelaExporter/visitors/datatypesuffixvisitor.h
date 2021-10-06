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

#include "tmc/PromelaModel/datatype.h"

#include <QTextStream>

namespace conversion::tmc::exporter {
using ::tmc::promelamodel::ArrayType;
using ::tmc::promelamodel::BasicType;
using ::tmc::promelamodel::DataType;
using ::tmc::promelamodel::UnsignedDataType;
using ::tmc::promelamodel::UtypeRef;

/**
 * @brief  Visitor for exporting @link{::tmc::promelamodel::DataType}
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
    void visit(const DataType &dataType);

    /**
     * @brief Visit value of DataType, whih is std::variant
     *
     * @param value std::variant to visit
     */
    void visit(const DataType::Value &value);
    /**
     * @brief Visit value of ArrayType, whih is std::variant
     *
     * @param value std::variant to visit
     */
    void visit(const ArrayType::Value &value);

    /**
     * @brief Handle UnsignedDatatype
     *
     * This function appends prefix for UnsignedDataType to stream.
     *
     * @param type type to translate.
     */
    void operator()(const UnsignedDataType &type);
    /**
     * @brief Handle BasicType
     *
     * This function appends prefix for BasicType to stream.
     *
     * @param type type to translate.
     */
    void operator()(const BasicType &type);
    /**
     * @brief Handle UtypeRef
     *
     * This function appends prefix for UtypeRef to stream.
     *
     * @param type type to translate.
     */
    void operator()(const UtypeRef &type);
    /**
     * @brief Handle ArrayType
     *
     * This function appends prefix for ArrayType to stream.
     *
     * @param type type to translate.
     */
    void operator()(const ArrayType &type);

private:
    QTextStream &m_stream;
};
}
