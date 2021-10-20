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

#include "asn1nodecomponentvisitor.h"

#include "asn1constraintvisitor.h"

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
#include <asn1library/asn1/values.h>

using Asn1Acn::Types::BitString;
using Asn1Acn::Types::Boolean;
using Asn1Acn::Types::Choice;
using Asn1Acn::Types::Enumerated;
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
using tmc::promela::model::ArrayType;
using tmc::promela::model::BasicType;
using tmc::promela::model::DataType;
using tmc::promela::model::Declaration;
using tmc::promela::model::Utype;

namespace conversion::tmc::translator {
Asn1NodeComponentVisitor::Asn1NodeComponentVisitor(::tmc::promela::model::Utype &utype, QString name)
    : m_utype(utype)
    , m_name(std::move(name))
{
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::Boolean &type)
{
    Q_UNUSED(type);
    m_utype.addField(Declaration(DataType(BasicType::BOOLEAN), m_name));
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::Null &type)
{
    Q_UNUSED(type);
    m_utype.addField(Declaration(DataType(BasicType::BIT), m_name));
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::BitString &type)
{
    Q_UNUSED(type);
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::OctetString &type)
{
    Q_UNUSED(type);
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::IA5String &type)
{
    Q_UNUSED(type);
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::NumericString &type)
{
    Q_UNUSED(type);
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::Enumerated &type)
{
    Q_UNUSED(type);
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::Choice &type)
{
    Q_UNUSED(type);
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::Sequence &type)
{
    Q_UNUSED(type);
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::SequenceOf &type)
{
    Q_UNUSED(type);
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::Real &type)
{
    Q_UNUSED(type);
    m_utype.addField(Declaration(DataType(BasicType::FLOAT), m_name));
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::LabelType &type)
{
    Q_UNUSED(type);
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::Integer &type)
{
    Q_UNUSED(type);
    m_utype.addField(Declaration(DataType(BasicType::INT), m_name));
}

void Asn1NodeComponentVisitor::visit(const ::Asn1Acn::Types::UserdefinedType &type)
{
    Q_UNUSED(type);
}
}
