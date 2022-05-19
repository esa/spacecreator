/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "sequencecomponentvaluevisitor.h"

#include "valueassignmentvisitor.h"

#include <QString>
#include <asn1library/asn1/acnsequencecomponent.h>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/converter/exceptions.h>
#include <promela/PromelaModel/assignment.h>
#include <promela/PromelaModel/expression.h>
#include <promela/PromelaModel/proctypeelement.h>
#include <promela/PromelaModel/variableref.h>

using Asn1Acn::AcnSequenceComponent;
using Asn1Acn::AsnSequenceComponent;
using Asn1Acn::NamedValue;
using conversion::ConverterException;
using conversion::Escaper;
using promela::model::Assignment;
using promela::model::Constant;
using promela::model::Expression;
using promela::model::ProctypeElement;
using promela::model::VariableRef;

namespace promela::translator {
SequenceComponentValueVisitor::SequenceComponentValueVisitor(const NamedValue *value,
        model::Sequence &sequence, const VariableRef &target, QString sequenceTypeName)
    : m_value(value)
    , m_sequence(sequence)
    , m_target(target)
    , m_sequenceTypeName(std::move(sequenceTypeName))
{
}

void SequenceComponentValueVisitor::visit(const AsnSequenceComponent &component)
{
    const QString &componentName = component.name();
    const NamedValue::Components &values = m_value->components();

    auto iter = std::find_if(
            values.begin(), values.end(), [componentName](const auto &data) { return data.first == componentName; });

    VariableRef existFieldRef = m_target;

    existFieldRef.appendElement("exist");
    existFieldRef.appendElement(Escaper::escapePromelaName(componentName));

    if (iter == values.end() && component.isOptional()) {
        m_sequence.appendElement(std::make_unique<ProctypeElement>(Assignment(existFieldRef, Expression(Constant(0)))));
        return;
    }

    if (iter == values.end()) {
        throw ConverterException(
                QString("Missing value for non optional field %1 in %2").arg(component.name(), m_sequenceTypeName));
    }

    if (component.isOptional()) {
        m_sequence.appendElement(std::make_unique<ProctypeElement>(Assignment(existFieldRef, Expression(Constant(1)))));
    }

    m_target.appendElement(Escaper::escapePromelaName(componentName));

    ValueAssignmentVisitor visitor(
            iter->second.get(), m_sequence, m_target, QString("%1_%2").arg(m_sequenceTypeName).arg(componentName));

    component.type()->accept(visitor);
}

void SequenceComponentValueVisitor::visit(const AcnSequenceComponent &component)
{
    Q_UNUSED(component);
}
}
