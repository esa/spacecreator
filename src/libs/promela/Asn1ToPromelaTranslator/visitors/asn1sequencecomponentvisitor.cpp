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
#include <conversion/common/escaper/escaper.h>

using Asn1Acn::AcnSequenceComponent;
using Asn1Acn::AsnSequenceComponent;
using conversion::Escaper;
using promela::model::BasicType;
using promela::model::DataType;
using promela::model::Declaration;
using promela::model::PromelaModel;
using promela::model::Utype;

namespace promela::translator {
Asn1SequenceComponentVisitor::Asn1SequenceComponentVisitor(
        PromelaModel &promelaModel, QString baseTypeName, bool enhancedSpinSupport, int nestedIndexCounter)
    : m_promelaModel(promelaModel)
    , m_baseTypeName(std::move(baseTypeName))
    , m_enhancedSpinSupport(enhancedSpinSupport)
    , m_optionalComponent(false)
    , m_nestedIndexCounter(nestedIndexCounter)
{
}

void Asn1SequenceComponentVisitor::visit(const AsnSequenceComponent &component)
{
    Asn1ItemTypeVisitor visitor(
            m_promelaModel, m_baseTypeName, component.name(), false, m_enhancedSpinSupport, m_nestedIndexCounter);
    component.type()->accept(visitor);
    m_componentName = Escaper::escapePromelaName(component.name());
    m_componentType = visitor.getResultDataType();
    m_optionalComponent = component.isOptional();
}

void Asn1SequenceComponentVisitor::visit(const AcnSequenceComponent &component)
{
    Q_UNUSED(component);
}

bool Asn1SequenceComponentVisitor::wasComponentVisited() const
{
    return m_componentName.has_value();
}

QString Asn1SequenceComponentVisitor::getComponentName() const
{
    return m_componentName.value();
}

DataType Asn1SequenceComponentVisitor::getComponentType() const
{
    return m_componentType.value();
}

bool Asn1SequenceComponentVisitor::isComponentOptional() const
{
    return m_optionalComponent;
}
}
