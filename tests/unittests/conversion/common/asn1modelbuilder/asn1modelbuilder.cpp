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

#include "asn1modelbuilder.h"

#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/integer.h>

using namespace Asn1Acn;
using namespace Asn1Acn::Types;

namespace tests::conversion::common {

Asn1ModelBuilder::Asn1ModelBuilder(QString name)
    : m_file(std::move(name))
{
    const auto definitionsName = name.toUpper();

    auto definitions = std::make_unique<Asn1Acn::Definitions>(definitionsName, Asn1Acn::SourceLocation());
    m_file.add(std::move(definitions));

    m_definitions = m_file.definitions(definitionsName);
}

std::unique_ptr<Asn1Model> Asn1ModelBuilder::build()
{
    std::vector<File> files;
    files.push_back(std::move(m_file));

    return std::make_unique<Asn1Model>(std::move(files));
}

Asn1ModelBuilder &Asn1ModelBuilder::withIntegerDataType(QString name)
{
    auto type = std::make_unique<Asn1Acn::Types::Integer>(name);
    type->setEncoding(IntegerEncoding::pos_int);
    type->setEndianness(Endianness::little);
    type->setSize(64);

    auto typeAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(name, name, Asn1Acn::SourceLocation(), std::move(type));

    m_definitions->addType(std::move(typeAssignment));

    return *this;
}

} // namespace tests::conversion::common
