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

#include "visitors/datatypesuffixvisitor.h"

namespace conversion::tmc::exporter {
DataTypeSuffixVisitor::DataTypeSuffixVisitor(QTextStream &stream)
    : m_stream(stream)
{
}

void DataTypeSuffixVisitor::visit(const DataType &dataType)
{
    visit(dataType.getValue());
}

void DataTypeSuffixVisitor::visit(const DataType::Value &value)
{
    std::visit(*this, value);
}

void DataTypeSuffixVisitor::visit(const ArrayType::Value &value)
{
    std::visit(*this, value);
}

void DataTypeSuffixVisitor::operator()(const UnsignedDataType &type)
{
    m_stream << ":" << type.size();
}
void DataTypeSuffixVisitor::operator()(const BasicType &type) {}
void DataTypeSuffixVisitor::operator()(const UtypeRef &type) {}
void DataTypeSuffixVisitor::operator()(const ArrayType &type)
{
    m_stream << "[" << type.getSize() << "]";
}

}
