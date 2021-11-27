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

#include "typevisitor.h"

#include "constraintvisitor.h"

#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/labeltype.h>
#include <asn1library/asn1/types/null.h>
#include <asn1library/asn1/types/numericstring.h>
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <iostream>
#include <seds/SedsModel/types/arraydatatype.h>
#include <seds/SedsModel/types/binarydatatype.h>
#include <seds/SedsModel/types/booleandatatype.h>
#include <seds/SedsModel/types/containerdatatype.h>
#include <seds/SedsModel/types/datatype.h>
#include <seds/SedsModel/types/encodings/coreintegerencoding.h>
#include <seds/SedsModel/types/encodings/falsevalue.h>
#include <seds/SedsModel/types/enumerateddatatype.h>
#include <seds/SedsModel/types/floatdatatype.h>
#include <seds/SedsModel/types/integerdatatype.h>
#include <seds/SedsModel/types/ranges/minmaxrange.h>
#include <seds/SedsModel/types/stringdatatype.h>
#include <seds/SedsModel/types/subrangedatatype.h>

using Asn1Acn::BitStringValue;
using Asn1Acn::IntegerValue;
using Asn1Acn::OctetStringValue;
using Asn1Acn::StringValue;
using Asn1Acn::Types::BitString;
using Asn1Acn::Types::Boolean;
using Asn1Acn::Types::Choice;
using Asn1Acn::Types::Enumerated;
using Asn1Acn::Types::EnumeratedItem;
using Asn1Acn::Types::IA5String;
using Asn1Acn::Types::Integer;
using Asn1Acn::Types::LabelType;
using Asn1Acn::Types::Null;
using Asn1Acn::Types::NumericString;
using Asn1Acn::Types::OctetString;
using Asn1Acn::Types::Real;
using Asn1Acn::Types::Sequence;
using Asn1Acn::Types::SequenceOf;
using Asn1Acn::Types::UserdefinedType;

namespace conversion::seds::translator {

TypeVisitor::Context::Context(const Asn1Acn::Asn1Model *asn1Model, const Asn1Acn::Definitions *definitions,
        const QString name, ::seds::model::Package *sedsPackage)
    : m_asn1Model(asn1Model)
    , m_definitions(definitions)
    , m_name(name)
    , m_sedsPackage(sedsPackage)
{
}

auto TypeVisitor::Context::model() -> const Asn1Acn::Asn1Model *
{
    return m_asn1Model;
}
auto TypeVisitor::Context::definitions() -> const Asn1Acn::Definitions *
{
    return m_definitions;
}

auto TypeVisitor::Context::name() -> const QString
{
    return m_name;
}

auto TypeVisitor::Context::package() -> ::seds::model::Package *
{
    return m_sedsPackage;
}

TypeVisitor::TypeVisitor(Context &context)
    : m_context(context)
{
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Boolean &type)
{
    Q_UNUSED(type);
    ::seds::model::BooleanDataType sedsType;
    sedsType.setName(m_context.name());
    m_context.package()->addDataType(std::move(sedsType));
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Null &type)
{
    Q_UNUSED(type);
}

void TypeVisitor::visit(const ::Asn1Acn::Types::BitString &type)
{
    Q_UNUSED(type);
}

void TypeVisitor::visit(const ::Asn1Acn::Types::OctetString &type)
{
    Q_UNUSED(type);
}

void TypeVisitor::visit(const ::Asn1Acn::Types::IA5String &type)
{
    Q_UNUSED(type);
}

void TypeVisitor::visit(const ::Asn1Acn::Types::NumericString &type)
{
    Q_UNUSED(type);
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Enumerated &type)
{
    Q_UNUSED(type);
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Choice &type)
{
    Q_UNUSED(type);
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Sequence &type)
{
    Q_UNUSED(type);
}

void TypeVisitor::visit(const ::Asn1Acn::Types::SequenceOf &type)
{
    Q_UNUSED(type);
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Real &type)
{
    Q_UNUSED(type);
}

void TypeVisitor::visit(const ::Asn1Acn::Types::LabelType &type)
{
    Q_UNUSED(type);
}

void TypeVisitor::visit(const ::Asn1Acn::Types::Integer &type)
{
    Q_UNUSED(type);
    ConstraintVisitor<IntegerValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);
    std::cout << "Constraints size " << type.constraints().constraints().size() << " " << std::endl;

    ::seds::model::IntegerDataType sedsType;

    if (constraintVisitor.isRangeConstraintVisited()) {
        std::cout << "Constraint do" << std::endl;
        ::seds::model::MinMaxRange range;
        range.setType(::seds::model::RangeType::InclusiveMinInclusiveMax);
        range.setMax(IntegerValue::asString(constraintVisitor.getRange().end()));
        range.setMin(IntegerValue::asString(constraintVisitor.getRange().begin()));
        sedsType.setRange(std::move(range));
    }

    /*::seds::model::IntegerDataEncoding encoding;
    encoding.setByteOrder(::seds::model::ByteOrder::BigEndian);
    encoding.setEncoding(::seds::model::CoreIntegerEncoding::TwosComplement);
    encoding.setBits(16);*/

    sedsType.setName(m_context.name());

    m_context.package()->addDataType(std::move(sedsType));
}

void TypeVisitor::visit(const ::Asn1Acn::Types::UserdefinedType &type)
{
    Q_UNUSED(type);
}

} // namespace conversion::seds::translator
