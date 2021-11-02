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

#include "typealiasvisitor.h"

#include "basictypegenerator.h"

using tmc::promela::model::BasicType;
using tmc::promela::model::TypeAlias;
using tmc::promela::model::UtypeRef;

namespace tmc::exporter {
TypeAliasVisitor::TypeAliasVisitor(QTextStream &stream)
    : m_stream(stream)
{
}

void TypeAliasVisitor::operator()(const TypeAlias &typeAlias)
{
    m_stream << "#define " << typeAlias.getName() << " ";
    std::visit(*this, typeAlias.getType());
    m_stream << "\n";
}

void TypeAliasVisitor::operator()(const BasicType &basicType)
{
    BasicTypeGenerator generator(m_stream);
    generator.generate(basicType);
}

void TypeAliasVisitor::operator()(const UtypeRef &utypeRef)
{
    m_stream << utypeRef.getName();
}
}
