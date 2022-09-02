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

#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/boolean.h>

using namespace Asn1Acn;
using namespace Asn1Acn::Constraints;
using namespace Asn1Acn::Types;

namespace tests::conversion::common {

Asn1ModelBuilder::Asn1ModelBuilder(QString name)
    : m_file(std::make_unique<File>(std::move(name)))
{
    const auto definitionsName = name.toUpper();

    auto definitions = std::make_unique<Asn1Acn::Definitions>(definitionsName, Asn1Acn::SourceLocation());
    m_file->add(std::move(definitions));

    m_definitions = m_file->definitions(definitionsName);
}

std::unique_ptr<Asn1Model> Asn1ModelBuilder::build()
{
    std::vector<std::unique_ptr<File>> files;
    files.push_back(std::move(m_file));

    return std::make_unique<Asn1Model>(std::move(files));
}

Asn1ModelBuilder &Asn1ModelBuilder::withBooleanDataType(QString name, QString trueValue, QString falseValue)
{
    auto type = std::make_unique<Boolean>(name);

    type->setTrueValue(trueValue);
    type->setFalseValue(falseValue);

    auto typeAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(name, name, Asn1Acn::SourceLocation(), std::move(type));

    m_definitions->addType(std::move(typeAssignment));

    return *this;
}

Asn1ModelBuilder &Asn1ModelBuilder::withRealDataType(
        QString name, Endianness endianness, RealEncoding realEncoding, double min, double max)
{
    auto type = std::make_unique<Real>(name);

    type->setEndianness(endianness);
    type->setEncoding(realEncoding);

    auto rangeConstraint = RangeConstraint<RealValue>::create({ min, max });
    type->constraints().append(std::move(rangeConstraint));

    auto typeAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(name, name, Asn1Acn::SourceLocation(), std::move(type));

    m_definitions->addType(std::move(typeAssignment));

    return *this;
}

Asn1ModelBuilder &Asn1ModelBuilder::withIntegerDataType(QString name, int size, int acnMinSize, int acnMaxSize,
        Endianness endianness, IntegerEncoding integerEncoding, int64_t min, int64_t max)
{
    auto type = std::make_unique<Integer>(name);

    type->setSize(size);
    type->setEndianness(endianness);
    type->setEncoding(integerEncoding);

    if (acnMinSize != 0) {
        type->setAcnMinSizeInBits(acnMinSize);
    }

    if (acnMaxSize != 0) {
        type->setAcnMaxSizeInBits(acnMaxSize);
    }

    if (min != 0 || max != 0) {
        auto rangeConstraint = RangeConstraint<IntegerValue>::create({ min, max });
        type->constraints().append(std::move(rangeConstraint));
    }

    auto typeAssignment =
            std::make_unique<Asn1Acn::TypeAssignment>(name, name, Asn1Acn::SourceLocation(), std::move(type));

    m_definitions->addType(std::move(typeAssignment));

    return *this;
}

} // namespace tests::conversion::common
