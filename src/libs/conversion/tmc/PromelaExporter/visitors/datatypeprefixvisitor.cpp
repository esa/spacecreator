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

#include "visitors/datatypeprefixvisitor.h"

namespace conversion::tmc::exporter {
using ::tmc::promela::model::ArrayType;
using ::tmc::promela::model::BasicType;
using ::tmc::promela::model::DataType;
using ::tmc::promela::model::MtypeRef;
using ::tmc::promela::model::UnsignedDataType;
using ::tmc::promela::model::UtypeRef;

DataTypePrefixVisitor::DataTypePrefixVisitor(QTextStream &stream)
    : m_stream(stream)
{
}

void DataTypePrefixVisitor::visit(const DataType &dataType)
{
    visit(dataType.getType());
}

void DataTypePrefixVisitor::visit(const DataType::Type &type)
{
    std::visit(*this, type);
}

void DataTypePrefixVisitor::visit(const ArrayType::Type &type)
{
    std::visit(*this, type);
}

void DataTypePrefixVisitor::operator()(const UnsignedDataType &type)
{
    Q_UNUSED(type);
    m_stream << "unsigned";
}

void DataTypePrefixVisitor::operator()(const BasicType &type)
{
    switch (type) {
    case BasicType::BIT:
        m_stream << "bit";
        break;
    case BasicType::BOOLEAN:
        m_stream << "bool";
        break;
    case BasicType::BYTE:
        m_stream << "byte";
        break;
    case BasicType::PID:
        m_stream << "pid";
        break;
    case BasicType::SHORT:
        m_stream << "short";
        break;
    case BasicType::INT:
        m_stream << "int";
        break;
    case BasicType::FLOAT:
        m_stream << "float";
        break;
    case BasicType::CHAN:
        m_stream << "chan";
        break;
    }
}

void DataTypePrefixVisitor::operator()(const UtypeRef &type)
{
    m_stream << type.getName();
}

void DataTypePrefixVisitor::operator()(const MtypeRef &type)
{
    if (type.isNamed()) {
        m_stream << "mtype : " << type.getName();
    } else {
        m_stream << "mtype";
    }
}

void DataTypePrefixVisitor::operator()(const ArrayType &type)
{
    std::visit(*this, type.getType());
}
}
