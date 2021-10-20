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

#include "asn1typevisitor.h"

#include "asn1sequencecomponentvisitor.h"

#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/integeracnparams.h>
#include <asn1library/asn1/types/null.h>
#include <asn1library/asn1/types/numericstring.h>
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/typereadingvisitor.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <tmc/PromelaModel/utype.h>

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
using tmc::promela::model::BasicType;
using tmc::promela::model::PromelaModel;
using tmc::promela::model::TypeAlias;
using tmc::promela::model::Utype;

namespace conversion::tmc::translator {
Asn1TypeVisitor::Asn1TypeVisitor(PromelaModel &promelaModel, QString name)
    : m_promelaModel(promelaModel)
    , m_typeName(std::move(name))
{
}

void Asn1TypeVisitor::visit(const Boolean &type)
{
    Q_UNUSED(type);
    m_promelaModel.addTypeAlias(TypeAlias(m_typeName, BasicType::BOOLEAN));
}

void Asn1TypeVisitor::visit(const Null &type)
{
    Q_UNUSED(type);
    m_promelaModel.addTypeAlias(TypeAlias(m_typeName, BasicType::BIT));
}

void Asn1TypeVisitor::visit(const BitString &type)
{
    Q_UNUSED(type);
}

void Asn1TypeVisitor::visit(const OctetString &type)
{
    Q_UNUSED(type);
}

void Asn1TypeVisitor::visit(const IA5String &type)
{
    Q_UNUSED(type);
}

void Asn1TypeVisitor::visit(const NumericString &type)
{
    Q_UNUSED(type);
}

void Asn1TypeVisitor::visit(const Enumerated &type)
{
    Q_UNUSED(type);
}

void Asn1TypeVisitor::visit(const Choice &type)
{
    Q_UNUSED(type);
}

void Asn1TypeVisitor::visit(const Sequence &type)
{
    Utype utype(m_typeName);
    for (const std::unique_ptr<Asn1Acn::SequenceComponent> &component : type.components()) {
        Asn1SequenceComponentVisitor componentVisitor(m_promelaModel, utype);
        component->accept(componentVisitor);
    }
    m_promelaModel.addUtype(utype);
}

void Asn1TypeVisitor::visit(const SequenceOf &type)
{
    Q_UNUSED(type);
}

void Asn1TypeVisitor::visit(const Real &type)
{
    Q_UNUSED(type);
    m_promelaModel.addTypeAlias(TypeAlias(m_typeName, BasicType::FLOAT));
}

void Asn1TypeVisitor::visit(const LabelType &type)
{
    Q_UNUSED(type);
}

void Asn1TypeVisitor::visit(const Integer &type)
{
    Q_UNUSED(type);
    m_promelaModel.addTypeAlias(TypeAlias(m_typeName, BasicType::INT));
}

void Asn1TypeVisitor::visit(const UserdefinedType &type)
{
    m_promelaModel.addTypeAlias(TypeAlias(m_typeName, type.typeName()));
}
}
