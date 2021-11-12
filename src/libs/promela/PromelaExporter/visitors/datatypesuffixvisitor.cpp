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

#include "datatypesuffixvisitor.h"

using promela::model::ArrayType;
using promela::model::BasicType;
using promela::model::DataType;
using promela::model::MtypeRef;
using promela::model::UnsignedDataType;
using promela::model::UtypeRef;

namespace promela::exporter {

DataTypeSuffixVisitor::DataTypeSuffixVisitor(QTextStream &stream)
    : m_stream(stream)
{
}

void DataTypeSuffixVisitor::visit(const DataType &dataType)
{
    visit(dataType.getType());
}

void DataTypeSuffixVisitor::visit(const DataType::Type &type)
{
    std::visit(*this, type);
}

void DataTypeSuffixVisitor::visit(const ArrayType::Type &type)
{
    std::visit(*this, type);
}

void DataTypeSuffixVisitor::operator()(const UnsignedDataType &type)
{
    m_stream << ":" << type.size();
}

void DataTypeSuffixVisitor::operator()(const BasicType &type)
{
    Q_UNUSED(type);
}

void DataTypeSuffixVisitor::operator()(const UtypeRef &type)
{
    Q_UNUSED(type);
}

void DataTypeSuffixVisitor::operator()(const MtypeRef &type)
{
    Q_UNUSED(type);
}

void DataTypeSuffixVisitor::operator()(const ArrayType &type)
{
    m_stream << "[" << type.getSize() << "]";
}
}
