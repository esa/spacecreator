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
/**
 * @brief  Visitor for exporting @link{::tmc::promelamodel::DataType}
 *
 * This visitor creates prefix for data type.
 * This should be used together with DataTypeSuffixVisitor.
 */
class DataTypePrefixVisitor
{
public:
    /**
     * @brief  Constructor.
     *
     * @param stream  stream to append data type prefix
     */
    DataTypePrefixVisitor(QTextStream &stream);

    /**
     * @brief  Visit DataType
     *
     * @param dataType DataType to visit
     */
    void visit(const ::tmc::promela::model::DataType &dataType);

    /**
     * @brief Visit value of DataType, which is std::variant
     *
     * @param value std::variant to visit
     */
    void visit(const ::tmc::promela::model::DataType::Type &type);
    /**
     * @brief Visit value of ArrayType, which is std::variant
     *
     * @param value std::variant to visit
     */
    void visit(const ::tmc::promela::model::ArrayType::Type &type);

    /**
     * @brief Handle UnsignedDatatype
     *
     * This function appends prefix for UnsignedDataType to stream.
     *
     * @param type type to translate.
     */
    void operator()(const ::tmc::promela::model::UnsignedDataType &type);
    /**
     * @brief Handle BasicType
     *
     * This function appends prefix for BasicType to stream.
     *
     * @param type type to translate.
     */
    void operator()(const ::tmc::promela::model::BasicType &type);
    /**
     * @brief Handle UtypeRef
     *
     * This function appends prefix for UtypeRef to stream.
     *
     * @param type type to translate.
     */
    void operator()(const ::tmc::promela::model::UtypeRef &type);
    /**
     * @brief Handle MtypeRef
     *
     * This function appends prefix for MtypeRef to stream.
     *
     * @param type type to translate.
     */
    void operator()(const ::tmc::promela::model::MtypeRef &type);
    /**
     * @brief Handle ArrayType
     *
     * This function appends prefix for ArrayType to stream.
     *
     * @param type type to translate.
     */
    void operator()(const ::tmc::promela::model::ArrayType &type);

private:
    QTextStream &m_stream;
};
}
