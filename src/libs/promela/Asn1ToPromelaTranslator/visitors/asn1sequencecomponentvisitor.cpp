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

#include "asn1sequencecomponentvisitor.h"

#include "asn1itemtypevisitor.h"

#include <asn1library/asn1/acnsequencecomponent.h>
#include <asn1library/asn1/asnsequencecomponent.h>

using Asn1Acn::AcnSequenceComponent;
using Asn1Acn::AsnSequenceComponent;
using promela::model::BasicType;
using promela::model::DataType;
using promela::model::Declaration;
using promela::model::PromelaModel;
using promela::model::Utype;

namespace promela::translator {
Asn1SequenceComponentVisitor::Asn1SequenceComponentVisitor(
        PromelaModel &promelaModel, Utype &utype, QString baseTypeName, QList<QString> &optionalFields)
    : m_promelaModel(promelaModel)
    , m_utype(utype)
    , m_baseTypeName(std::move(baseTypeName))
    , m_optionalFields(optionalFields)
{
}

void Asn1SequenceComponentVisitor::visit(const AsnSequenceComponent &component)
{
    Asn1ItemTypeVisitor visitor(m_promelaModel, m_baseTypeName, component.name());
    component.type()->accept(visitor);
    m_utype.addField(Declaration(visitor.getResultDataType().value(), component.name()));
    if (component.isOptional()) {
        m_optionalFields.append(component.name());
    }
}

void Asn1SequenceComponentVisitor::visit(const AcnSequenceComponent &component)
{
    Q_UNUSED(component);
}
}
