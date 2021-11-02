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

#include "basictypegenerator.h"

using tmc::promela::model::BasicType;

namespace tmc::exporter {

BasicTypeGenerator::BasicTypeGenerator(QTextStream &stream)
    : m_stream(stream)
{
}

void BasicTypeGenerator::generate(::tmc::promela::model::BasicType type)
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
}
