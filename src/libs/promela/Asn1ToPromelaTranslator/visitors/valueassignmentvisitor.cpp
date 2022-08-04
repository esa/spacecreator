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

#include "valueassignmentvisitor.h"

#include "sequencecomponentvaluevisitor.h"
#include "sizeconstraintvisitor.h"

#include <asn1library/asn1/choicevalue.h>
#include <asn1library/asn1/multiplevalue.h>
#include <asn1library/asn1/namedvalue.h>
#include <asn1library/asn1/singlevalue.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/converter/exceptions.h>
#include <iostream>
#include <promela/PromelaModel/proctypeelement.h>

using Asn1Acn::ChoiceValue;
using Asn1Acn::IntegerValue;
using Asn1Acn::MultipleValue;
using Asn1Acn::NamedValue;
using Asn1Acn::SingleValue;
using Asn1Acn::Value;
using Asn1Acn::ValuePtr;
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
using conversion::ConverterException;
using conversion::Escaper;
using promela::model::Assignment;
using promela::model::Constant;
using promela::model::Expression;
using promela::model::InlineCall;
using promela::model::ProctypeElement;
using promela::model::VariableRef;

namespace promela::translator {
ValueAssignmentVisitor::ValueAssignmentVisitor(
        Value *value, model::Sequence &sequence, const VariableRef &target, QString typeName)
    : m_value(value)
    , m_sequence(sequence)
    , m_target(target)
    , m_typeName(std::move(typeName))
{
}

void ValueAssignmentVisitor::visit(const Boolean &type)
{
    Q_UNUSED(type);
    if (m_value->typeEnum() != Value::SINGLE_VALUE) {
        throw ConverterException("Invalid value for BOOLEAN datatype");
    }
    const SingleValue *singleValue = dynamic_cast<const SingleValue *>(m_value);

    int value = singleValue->value().compare("true", Qt::CaseInsensitive) == 0 ? 1 : 0;

    const QString inlineCallName = QString("%1_assign_value").arg(Escaper::escapePromelaName(m_typeName));

    QList<InlineCall::Argument> inlineArguments;
    inlineArguments.append(m_target);
    inlineArguments.append(Constant(value));
    m_sequence.appendElement(InlineCall(inlineCallName, inlineArguments));
}

void ValueAssignmentVisitor::visit(const Null &type)
{
    Q_UNUSED(type);
    throw ConverterException("Value generation is not implemented for NULL datatype");
}

void ValueAssignmentVisitor::visit(const BitString &type)
{
    Q_UNUSED(type);
    throw ConverterException("Value generation is not implemented for BIT STRING datatype");
}

void ValueAssignmentVisitor::visit(const OctetString &type)
{
    Q_UNUSED(type);
    throw ConverterException("Value generation is not implemented for OCTET STRING datatype");
}

void ValueAssignmentVisitor::visit(const IA5String &type)
{
    Q_UNUSED(type);
    throw ConverterException("Value generation is not implemented for IA5String datatype");
}

void ValueAssignmentVisitor::visit(const NumericString &type)
{
    Q_UNUSED(type);
    throw ConverterException("Value generation is not implemented for NUMERIC STRING datatype");
}

void ValueAssignmentVisitor::visit(const Enumerated &type)
{
    if (m_value->typeEnum() != Value::SINGLE_VALUE) {
        throw ConverterException("Invalid value for ENUMERATED datatype");
    }
    const SingleValue *singleValue = dynamic_cast<const SingleValue *>(m_value);
    Q_UNUSED(type);

    const QString value = QString("%1_%2")
                                  .arg(Escaper::escapePromelaName(m_typeName))
                                  .arg(Escaper::escapePromelaName(singleValue->value()));

    const QString inlineCallName = QString("%1_assign_value").arg(Escaper::escapePromelaName(m_typeName));

    QList<InlineCall::Argument> inlineArguments;
    inlineArguments.append(m_target);
    inlineArguments.append(VariableRef(value));
    m_sequence.appendElement(InlineCall(inlineCallName, inlineArguments));
}

void ValueAssignmentVisitor::visit(const Choice &type)
{
    if (m_value->typeEnum() != Value::CHOICE_VALUE) {
        throw ConverterException("Invalid value for Choice datatype");
    }
    const ChoiceValue *choiceValue = dynamic_cast<const ChoiceValue *>(m_value);

    const auto escapedTypeName = Escaper::escapePromelaName(m_typeName);
    const auto escapedSelectionName = Escaper::escapePromelaName(choiceValue->name());

    auto selectionMember = m_target;
    selectionMember.appendElement("selection");

    auto dataMember = m_target;
    dataMember.appendElement("data");
    dataMember.appendElement(escapedSelectionName);

    const auto selectionValue = Expression(QString("%1_%2_PRESENT").arg(escapedTypeName).arg(escapedSelectionName));

    m_sequence.appendElement(Assignment(selectionMember, selectionValue));

    auto component = type.component(choiceValue->name());
    ValueAssignmentVisitor visitor(choiceValue->value().get(), m_sequence, dataMember,
            QString("%1_%2").arg(escapedTypeName).arg(escapedSelectionName));
    component->type()->accept(visitor);
}

void ValueAssignmentVisitor::visit(const Sequence &type)
{
    if (m_value->typeEnum() != Value::NAMED_VALUE) {
        throw ConverterException("Invalid value for SEQUENCE datatype");
    }
    const NamedValue *namedValue = dynamic_cast<const NamedValue *>(m_value);
    for (const std::unique_ptr<Asn1Acn::SequenceComponent> &component : type.components()) {
        SequenceComponentValueVisitor visitor(namedValue, m_sequence, m_target, m_typeName);
        component->accept(visitor);
    }
}

void ValueAssignmentVisitor::visit(const SequenceOf &type)
{
    if (m_value->typeEnum() != Value::MULTIPLE_VALUE) {
        throw ConverterException("Invalid value for SEQUENCEOF datatype");
    }

    SizeConstraintVisitor<IntegerValue> sizeConstraintVisitor;
    type.constraints().accept(sizeConstraintVisitor);

    const auto multipleValue = dynamic_cast<const MultipleValue *>(m_value);
    const auto &values = multipleValue->values();
    const auto valuesCount = values.size();

    for (std::size_t i = 0; i < valuesCount; ++i) {
        const auto &value = values[i];
        auto target = m_target;
        target.appendElement("data", std::make_unique<Expression>(Constant(i)));

        ValueAssignmentVisitor visitor(value.get(), m_sequence, target, QString("%1_%2").arg(m_typeName).arg("elem"));
        type.itemsType()->accept(visitor);
    }

    if (sizeConstraintVisitor.getMaxSize() != sizeConstraintVisitor.getMinSize()) {
        auto target = m_target;
        target.appendElement("length");

        auto assignment = Assignment(target, Expression(Constant(valuesCount)));
        m_sequence.appendElement(std::move(assignment));
    }
}

void ValueAssignmentVisitor::visit(const Real &type)
{
    Q_UNUSED(type);
    throw ConverterException("Value generation is not implemented for REAL datatype");
}

void ValueAssignmentVisitor::visit(const LabelType &type)
{
    Q_UNUSED(type);
    throw ConverterException("Value generation is not implemented for LabelType");
}

void ValueAssignmentVisitor::visit(const Integer &type)
{
    Q_UNUSED(type);
    if (m_value->typeEnum() != Value::SINGLE_VALUE) {
        throw ConverterException("Invalid value for INTEGER datatype");
    }
    const SingleValue *singleValue = dynamic_cast<const SingleValue *>(m_value);
    int value = singleValue->value().toInt();

    const QString inlineCallName = QString("%1_assign_value").arg(Escaper::escapePromelaName(m_typeName));

    QList<InlineCall::Argument> inlineArguments;
    inlineArguments.append(m_target);
    inlineArguments.append(Constant(value));
    m_sequence.appendElement(InlineCall(inlineCallName, inlineArguments));
}

void ValueAssignmentVisitor::visit(const UserdefinedType &type)
{
    ValueAssignmentVisitor visitor(m_value, m_sequence, m_target, type.typeName());

    type.type()->accept(visitor);
}
}
