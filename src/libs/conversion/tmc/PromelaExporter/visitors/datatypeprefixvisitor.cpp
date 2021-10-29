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

#include "datatypeprefixvisitor.h"

#include "basictypegenerator.h"

using tmc::promela::model::ArrayType;
using tmc::promela::model::BasicType;
using tmc::promela::model::DataType;
using tmc::promela::model::MtypeRef;
using tmc::promela::model::UnsignedDataType;
using tmc::promela::model::UtypeRef;

namespace conversion::tmc::exporter {

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
    BasicTypeGenerator generator(m_stream);
    generator.generate(type);
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
