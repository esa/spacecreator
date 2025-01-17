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

#include "asn1typevaluegeneratorvisitor.h"

#include "visitors/integerrangeconstraintvisitor.h"
#include "visitors/realrangeconstraintvisitor.h"
#include "visitors/sizeconstraintvisitor.h"

#include <QList>
#include <algorithm>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/sequencecomponent.h>
#include <asn1library/asn1/sequencecomponentvisitor.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/type.h>
#include <asn1library/asn1/types/typereadingvisitor.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <asn1library/asn1/values.h>
#include <conversion/asn1/Asn1Exporter/visitors/asn1typecomponentreconstructingvisitor.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <list>
#include <memory>
#include <optional>
#include <promela/Asn1ToPromelaTranslator/enumeratedgenerator.h>
#include <promela/Asn1ToPromelaTranslator/integergenerator.h>
#include <promela/Asn1ToPromelaTranslator/integersubset.h>
#include <promela/Asn1ToPromelaTranslator/proctypemaker.h>
#include <promela/Asn1ToPromelaTranslator/realgenerator.h>
#include <promela/PromelaCommon/constants.h>
#include <promela/PromelaCommon/namehelper.h>
#include <promela/PromelaModel/basictypes.h>
#include <promela/PromelaModel/binaryexpression.h>
#include <promela/PromelaModel/conditional.h>
#include <promela/PromelaModel/constant.h>
#include <promela/PromelaModel/inlinecall.h>
#include <promela/PromelaModel/inlinedef.h>
#include <promela/PromelaModel/proctypeelement.h>
#include <promela/PromelaModel/realconstant.h>
#include <promela/PromelaModel/sequence.h>
#include <promela/PromelaModel/valuedefinition.h>
#include <promela/PromelaModel/variableref.h>
#include <qglobal.h>
#include <stdexcept>
#include <type_traits>
#include <utility>

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
using conversion::Escaper;
using conversion::translator::TranslationException;
using promela::common::PromelaConstants;
using promela::common::PromelaNameHelper;
using promela::model::Assignment;
using promela::model::BinaryExpression;
using promela::model::BooleanConstant;
using promela::model::Conditional;
using promela::model::Constant;
using promela::model::DoLoop;
using promela::model::ExitLoop;
using promela::model::Expression;
using promela::model::ForLoop;
using promela::model::InlineCall;
using promela::model::InlineDef;
using promela::model::ProctypeElement;
using promela::model::PromelaModel;
using promela::model::RealConstant;
using promela::model::Select;
using promela::model::Skip;
using promela::model::UtypeRef;
using promela::model::ValueDefinition;
using promela::model::VariableRef;

namespace promela::translator {

const QString Asn1TypeValueGeneratorVisitor::InlineDefAdder::lengthMemberName = "length";
const QString Asn1TypeValueGeneratorVisitor::InlineDefAdder::octetGeneratorName = "OctetStringElement_generate_value";
const QString Asn1TypeValueGeneratorVisitor::InlineDefAdder::ia5StringGeneratorName = "IA5StringElement_generate_value";

Asn1TypeValueGeneratorVisitor::Asn1TypeValueGeneratorVisitor(PromelaModel &promelaModel, QString name,
        const Asn1Acn::Types::Type *overridenType, const std::optional<float> delta = std::nullopt)
    : m_promelaModel(promelaModel)
    , m_name(std::move(name))
    , m_overridenType(overridenType)
    , m_delta(delta)
{
}

void Asn1TypeValueGeneratorVisitor::visit(const Boolean &type)
{
    Q_UNUSED(type);

    Conditional conditional;
    const std::list<bool> possibleValues = { true, false };
    const auto valueVariableName = getInlineArgumentName();

    std::for_each(possibleValues.begin(), possibleValues.end(), [&conditional, &valueVariableName](const bool &value) {
        auto nestedSequence = std::make_unique<model::Sequence>(model::Sequence::Type::NORMAL);

        nestedSequence->appendElement(Expression(BooleanConstant(true)));
        nestedSequence->appendElement(Assignment(VariableRef(valueVariableName), Expression(Constant(value))));

        conditional.appendAlternative(std::move(nestedSequence));
    });

    model::Sequence sequence(model::Sequence::Type::NORMAL);
    sequence.appendElement(std::move(conditional));

    createValueGenerationInline(std::move(sequence));
}

void Asn1TypeValueGeneratorVisitor::visit(const Null &type)
{
    Q_UNUSED(type);
    const QString message = QString("Null ASN.1 type's translation to Promela is not implemented yet (%1, %2)")
                                    .arg(__FILE__)
                                    .arg(__LINE__);
    throw std::logic_error(message.toStdString().c_str());
}

void Asn1TypeValueGeneratorVisitor::visit(const BitString &type)
{
    Q_UNUSED(type);
    const QString message = QString("BitString ASN.1 type's translation to Promela is not implemented yet (%1, %2)")
                                    .arg(__FILE__)
                                    .arg(__LINE__);
    throw std::logic_error(message.toStdString().c_str());
}

void Asn1TypeValueGeneratorVisitor::visit(const OctetString &type)
{
    SizeConstraintVisitor<Asn1Acn::OctetStringValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);

    if (!constraintVisitor.isSizeConstraintVisited()) {
        const QString msg("Specified OctetString has no size constraint");
        throw std::logic_error(msg.toStdString().c_str());
    }

    const QString &octetGeneratorName = InlineDefAdder::octetGeneratorName;
    if (!modelContainsInlineGenerator(octetGeneratorName)) {
        const long maxOctetValue = 255;
        InlineDefAdder::addRangedIntegerGeneratorToModel("OctetStringElement", m_promelaModel, 0, maxOctetValue);
    }

    const QString typeIdentifier = Escaper::escapePromelaName(m_name);
    const QString argumentName = QString("%1_gv").arg(typeIdentifier);
    const QString iteratorName = QString("%1_it").arg(typeIdentifier);

    auto sequence = ProctypeMaker::makeNormalSequence();
    sequence->appendElement(ProctypeMaker::makeVariableDeclaration(model::BasicType::INT, iteratorName));
    const size_t minSize = constraintVisitor.getMinSize();
    const size_t maxSize = constraintVisitor.getMaxSize();

    const auto isConstSize = minSize == maxSize;

    if (isConstSize) {
        sequence->appendElement(ProctypeMaker::makeCallForEachValue(
                octetGeneratorName, VariableRef(argumentName), Expression(maxSize - 1), iteratorName));
    } else { // OctetString has variable length
        const QString lengthGeneratorTypeName =
                QString("%1_%2").arg(typeIdentifier).arg(InlineDefAdder::lengthMemberName);
        InlineDefAdder::addRangedIntegerGeneratorToModel(
                lengthGeneratorTypeName, m_promelaModel, static_cast<long>(minSize), static_cast<long>(maxSize));

        sequence->appendElement(ProctypeMaker::makeInlineCall(QString("%1_generate_value").arg(lengthGeneratorTypeName),
                argumentName, VariableRef(InlineDefAdder::lengthMemberName)));

        Expression end = InlineDefAdder::getValueLengthMinusConstAsExpression(argumentName, 1);
        sequence->appendElement(
                ProctypeMaker::makeCallForEachValue(octetGeneratorName, argumentName, end, iteratorName));
    }

    if (m_overridenType != nullptr) {
        const auto overridenOctetString = dynamic_cast<const OctetString *>(m_overridenType);
        if (overridenOctetString == nullptr) {
            auto errorMessage = QString("Trying to subtype %1 with %2 which is not a OCTET STRING")
                                        .arg(m_overridenType->identifier())
                                        .arg(m_name);
            throw TranslationException(std::move(errorMessage));
        }

        handleOverridenType(type.identifier(), overridenOctetString->constraints(), "", argumentName, iteratorName,
                minSize, maxSize, sequence.get());
    }

    const QString inlineSeqGeneratorName = QString("%1_generate_value").arg(typeIdentifier);
    const QStringList inlineArguments = { argumentName };
    auto inlineDef = std::make_unique<InlineDef>(inlineSeqGeneratorName, inlineArguments, std::move(*sequence));

    if (!modelContainsInlineGenerator(inlineSeqGeneratorName)) {
        m_promelaModel.addInlineDef(std::move(inlineDef));
    }
}

void Asn1TypeValueGeneratorVisitor::visit(const IA5String &type)
{
    SizeConstraintVisitor<Asn1Acn::StringValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);

    if (!constraintVisitor.isSizeConstraintVisited()) {
        const QString msg("Specified IA5String has no size constraint");
        throw std::logic_error(msg.toStdString().c_str());
    }

    const QString &ia5StringGeneratorName = InlineDefAdder::ia5StringGeneratorName;
    if (!modelContainsInlineGenerator(ia5StringGeneratorName)) {
        const long maxStringValue = 127;
        InlineDefAdder::addRangedIntegerGeneratorToModel("IA5StringElement", m_promelaModel, 0, maxStringValue);
    }

    const QString typeIdentifier = Escaper::escapePromelaName(m_name);
    const QString argumentName = QString("%1_gv").arg(typeIdentifier);
    const QString iteratorName = QString("%1_it").arg(typeIdentifier);

    auto sequence = ProctypeMaker::makeNormalSequence();
    sequence->appendElement(ProctypeMaker::makeVariableDeclaration(model::BasicType::INT, iteratorName));
    const size_t minSize = constraintVisitor.getMinSize();
    const size_t maxSize = constraintVisitor.getMaxSize();

    const auto isConstSize = minSize == maxSize;

    if (isConstSize) {
        sequence->appendElement(ProctypeMaker::makeCallForEachValue(
                ia5StringGeneratorName, argumentName, Expression(maxSize - 1), iteratorName));
    } else { // IA5String has variable length
        const QString lengthGeneratorTypeName =
                QString("%1_%2").arg(typeIdentifier).arg(InlineDefAdder::lengthMemberName);
        InlineDefAdder::addRangedIntegerGeneratorToModel(
                lengthGeneratorTypeName, m_promelaModel, static_cast<long>(minSize), static_cast<long>(maxSize));

        sequence->appendElement(ProctypeMaker::makeInlineCall(QString("%1_generate_value").arg(lengthGeneratorTypeName),
                argumentName, VariableRef(InlineDefAdder::lengthMemberName)));

        Expression end = InlineDefAdder::getValueLengthMinusConstAsExpression(argumentName, 1);
        sequence->appendElement(
                ProctypeMaker::makeCallForEachValue(ia5StringGeneratorName, argumentName, end, iteratorName));
    }

    if (m_overridenType != nullptr) {
        const auto overridenIA5String = dynamic_cast<const IA5String *>(m_overridenType);
        if (overridenIA5String == nullptr) {
            auto errorMessage = QString("Trying to subtype %1 with %2 which is not a IA5String")
                                        .arg(m_overridenType->identifier())
                                        .arg(m_name);
            throw TranslationException(std::move(errorMessage));
        }

        handleOverridenType(type.identifier(), overridenIA5String->constraints(), "", argumentName, iteratorName,
                minSize, maxSize, sequence.get());
    }

    const QString inlineSeqGeneratorName = QString("%1_generate_value").arg(typeIdentifier);
    const QStringList inlineArguments = { argumentName };
    auto inlineDef = std::make_unique<InlineDef>(inlineSeqGeneratorName, inlineArguments, std::move(*sequence));

    if (!modelContainsInlineGenerator(inlineSeqGeneratorName)) {
        m_promelaModel.addInlineDef(std::move(inlineDef));
    }
}

void Asn1TypeValueGeneratorVisitor::visit(const NumericString &type)
{
    Q_UNUSED(type);
    const QString message = QString("NumericString ASN.1 type's translation to Promela is not implemented yet (%1, %2)")
                                    .arg(__FILE__)
                                    .arg(__LINE__);
    throw std::logic_error(message.toStdString().c_str());
}

void Asn1TypeValueGeneratorVisitor::visit(const Enumerated &type)
{
    Conditional conditional;

    const auto valueVariableName = getInlineArgumentName();
    EnumeratedGenerator generator(Escaper::escapePromelaName(m_name), type);

    while (generator.has_next()) {
        auto element = generator.next();

        std::unique_ptr<model::Sequence> nestedSequence =
                std::make_unique<model::Sequence>(model::Sequence::Type::NORMAL);

        nestedSequence->appendElement(Expression(BooleanConstant(true)));
        nestedSequence->appendElement(
                Assignment(VariableRef(valueVariableName), Expression(VariableRef(element.first))));

        conditional.appendAlternative(std::move(nestedSequence));
    }

    model::Sequence sequence(model::Sequence::Type::NORMAL);
    sequence.appendElement(std::move(conditional));

    createValueGenerationInline(std::move(sequence));
}

void Asn1TypeValueGeneratorVisitor::visit(const Choice &type)
{
    const auto valueVariableName = getInlineArgumentName();
    const QStringList inlineArguments = { valueVariableName };

    auto sequence = ProctypeMaker::makeNormalSequence();

    const auto &withComponentConstraints = type.withComponentConstraints();
    const auto &withComponentAbsentConstraints = type.withComponentAbsentConstraints();
    const auto hasConstraints = !withComponentConstraints.empty();
    const auto hasAbsentConstraints = !withComponentAbsentConstraints.empty();

    auto conditional = std::make_unique<Conditional>();
    int selectorVal = 1;

    for (auto &component : type.components()) {
        const QString &componentName = component->name();
        if (hasConstraints && withComponentConstraints.count(componentName) == 0) {
            continue;
        }

        const QString thisComponentSelected =
                Escaper::escapePromelaName(QString("%1_%2_PRESENT").arg(m_name).arg(componentName));
        m_promelaModel.addValueDefinition(ValueDefinition(thisComponentSelected, selectorVal++));

        if (hasAbsentConstraints && withComponentAbsentConstraints.count(componentName) != 0) {
            continue;
        }

        auto *const choiceComponent = component.get();
        const QString componentTypeName =
                Escaper::escapePromelaName(getChoiceComponentTypeName(*choiceComponent, m_name));
        const QString inlineTypeGeneratorName = Escaper::escapePromelaName(getInlineGeneratorName(componentTypeName));

        if (!modelContainsInlineGenerator(inlineTypeGeneratorName)) {
            auto *const choiceComponentType = getChoiceComponentType(choiceComponent);
            Asn1TypeValueGeneratorVisitor visitor(m_promelaModel, componentTypeName, nullptr);
            choiceComponentType->accept(visitor);
        }

        VariableRef componentRef = VariableRef("data");
        componentRef.appendElement(Escaper::escapePromelaName(componentName));

        auto alternative = ProctypeMaker::makeNormalSequence();
        alternative->appendElement(ProctypeMaker::makeTrueExpressionProctypeElement());
        alternative->appendElement(
                ProctypeMaker::makeInlineCall(inlineTypeGeneratorName, valueVariableName, std::move(componentRef)));
        alternative->appendElement(ProctypeMaker::makeAssignmentProctypeElement(
                QString("%1.selection").arg(valueVariableName), VariableRef(thisComponentSelected)));
        conditional->appendAlternative(std::move(alternative));
    }
    sequence->appendElement(std::move(*conditional));

    const QString choiceGeneratorInlineName = Escaper::escapePromelaName(QString("%1_generate_value").arg(m_name));
    auto inlineDef = std::make_unique<InlineDef>(choiceGeneratorInlineName, inlineArguments, std::move(*sequence));

    if (!modelContainsInlineGenerator(choiceGeneratorInlineName)) {
        m_promelaModel.addInlineDef(std::move(inlineDef));
    }
}

void Asn1TypeValueGeneratorVisitor::visit(const Sequence &type)
{
    const auto valueVariableName = getInlineArgumentName();

    const QString inlineSeqGeneratorName = getInlineGeneratorName(m_name);

    const QStringList inlineArguments = { valueVariableName };
    promela::model::Sequence sequence(promela::model::Sequence::Type::NORMAL);
    for (auto &sequenceComponent : type.components()) {
        auto *const asnSequenceComponent = dynamic_cast<Asn1Acn::AsnSequenceComponent *>(sequenceComponent.get());
        if (asnSequenceComponent != nullptr) {
            const QString componentTypeName = getSequenceComponentTypeName(*asnSequenceComponent, m_name);
            const QString inlineTypeGeneratorName = getInlineGeneratorName(componentTypeName);

            if (asnSequenceComponent->presence() != Asn1Acn::AsnSequenceComponent::Presence::NotSpecified) {
                // presence is either absent or present, need to generate exist flag
                VariableRef existFieldRef = VariableRef(valueVariableName);
                existFieldRef.appendElement("exist");
                existFieldRef.appendElement(Escaper::escapePromelaName(asnSequenceComponent->name()));
                auto presenceFlag =
                        asnSequenceComponent->presence() == Asn1Acn::AsnSequenceComponent::Presence::AlwaysAbsent
                        ? PresenceFlag::Absent
                        : PresenceFlag::Present;
                auto assignment = Assignment(std::move(existFieldRef), Expression(presenceFlag));
                sequence.appendElement(std::move(assignment));
            }

            if (asnSequenceComponent->presence() != Asn1Acn::AsnSequenceComponent::Presence::AlwaysAbsent) {
                // presence is either not specified or present, need to generate value
                if (!modelContainsInlineGenerator(inlineTypeGeneratorName)) {
                    auto *const asnSequenceComponentType = getAsnSequenceComponentType(asnSequenceComponent);
                    Asn1TypeValueGeneratorVisitor visitor(m_promelaModel, componentTypeName, nullptr);
                    asnSequenceComponentType->accept(visitor);
                }

                auto asnSequenceComponentInlineCall =
                        generateAsnSequenceComponentInlineCall(asnSequenceComponent, valueVariableName);
                sequence.appendElement(std::move(asnSequenceComponentInlineCall));
            }
        }
    }

    if (sequence.isEmpty()) {
        // the sequence is empty when the ASN.1 definiton is SEQUENCE { }
        // the _assign_value inline does nothing
        sequence.appendElement(Skip());
    }

    auto inlineDef = std::make_unique<InlineDef>(inlineSeqGeneratorName, inlineArguments, std::move(sequence));

    if (!modelContainsInlineGenerator(inlineSeqGeneratorName)) {
        m_promelaModel.addInlineDef(std::move(inlineDef));
    }
}

void Asn1TypeValueGeneratorVisitor::visit(const SequenceOf &type)
{
    const auto valueVariableName = getInlineArgumentName();
    const QString iteratorName = Escaper::escapePromelaName(QString("%1_it").arg(m_name));
    const QString componentTypeName = Escaper::escapePromelaName(type.itemsType()->typeName());
    const QString inlineTypeGeneratorName = Escaper::escapePromelaName(getInlineGeneratorName(componentTypeName));
    if (!modelContainsInlineGenerator(inlineTypeGeneratorName)) {
        auto *const asnSequenceComponentType = type.itemsType();
        Asn1TypeValueGeneratorVisitor visitor(m_promelaModel, componentTypeName, nullptr);
        asnSequenceComponentType->accept(visitor);
    }

    SizeConstraintVisitor<Asn1Acn::IntegerValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);

    const QString typeIdentifier = Escaper::escapePromelaName(m_name);

    auto sequence = ProctypeMaker::makeNormalSequence();
    sequence->appendElement(ProctypeMaker::makeVariableDeclaration(model::BasicType::INT, iteratorName));
    const size_t minSize = constraintVisitor.getMinSize();
    const size_t maxSize = constraintVisitor.getMaxSize();

    const auto isConstSize = minSize == maxSize;

    if (isConstSize) {
        sequence->appendElement(ProctypeMaker::makeCallForEachValue(
                inlineTypeGeneratorName, valueVariableName, Expression(maxSize - 1), iteratorName));
    } else { // sequenceOf has variable length
        const QString lengthGeneratorTypeName =
                QString("%1_%2").arg(typeIdentifier).arg(InlineDefAdder::lengthMemberName);
        InlineDefAdder::addRangedIntegerGeneratorToModel(
                lengthGeneratorTypeName, m_promelaModel, static_cast<long>(minSize), static_cast<long>(maxSize));

        sequence->appendElement(ProctypeMaker::makeInlineCall(QString("%1_generate_value").arg(lengthGeneratorTypeName),
                valueVariableName, VariableRef(InlineDefAdder::lengthMemberName)));

        Expression end = InlineDefAdder::getValueLengthMinusConstAsExpression(valueVariableName, 1);
        sequence->appendElement(
                ProctypeMaker::makeCallForEachValue(inlineTypeGeneratorName, valueVariableName, end, iteratorName));
    }

    if (m_overridenType != nullptr) {
        if (const auto overridenSequenceOf = dynamic_cast<const SequenceOf *>(m_overridenType);
                overridenSequenceOf != nullptr) {
            const auto initCallName = QString("%1_init_value")
                                              .arg(PromelaNameHelper::createChildTypeName(
                                                      Escaper::escapePromelaName(m_overridenType->identifier()),
                                                      PromelaConstants::sequenceOfElementTypeNameSuffix));
            handleOverridenType(type.identifier(), overridenSequenceOf->constraints(), initCallName, valueVariableName,
                    iteratorName, minSize, maxSize, sequence.get());
        } else if (const auto overridenOctetString = dynamic_cast<const OctetString *>(m_overridenType);
                   overridenOctetString != nullptr) {
            handleOverridenType(type.identifier(), overridenOctetString->constraints(), "", valueVariableName,
                    iteratorName, minSize, maxSize, sequence.get());
        } else if (const auto overridenIA5String = dynamic_cast<const IA5String *>(m_overridenType);
                   overridenIA5String != nullptr) {
            handleOverridenType(type.identifier(), overridenIA5String->constraints(), "", valueVariableName,
                    iteratorName, minSize, maxSize, sequence.get());
        } else {
            auto errorMessage = QString("Trying to subtype %1 with %2 which is not a SEQUENCE OF")
                                        .arg(m_overridenType->identifier())
                                        .arg(m_name);
            throw TranslationException(std::move(errorMessage));
        }
    }

    const QString inlineSeqGeneratorName = QString("%1_generate_value").arg(typeIdentifier);
    const QStringList inlineArguments = { valueVariableName };
    auto inlineDef = std::make_unique<InlineDef>(inlineSeqGeneratorName, inlineArguments, std::move(*sequence));

    if (!modelContainsInlineGenerator(inlineSeqGeneratorName)) {
        m_promelaModel.addInlineDef(std::move(inlineDef));
    }
}

void Asn1TypeValueGeneratorVisitor::visit(const Real &type)
{
    RealRangeConstraintVisitor constraintVisitor;
    type.constraints().accept(constraintVisitor);

    const auto valueVariableName = getInlineArgumentName();
    const auto tempName = Escaper::escapePromelaName(QString("%1_tmp").arg(m_name));
    std::optional<RealSubset> realSubset = constraintVisitor.getResultSubset();

    if (!realSubset.has_value()) {
        auto message =
                QString("Unable to generate values for type %1: unable to determine available subset").arg(m_name);
        throw TranslationException(message);
    }

    Conditional conditional;
    float delta = m_delta.value_or(1.f);

    for (const auto &range : realSubset.value().getRanges()) {
        const auto rangeMin = range.first;
        const auto rangeMax = range.second;

        float diff = rangeMax - rangeMin;
        int steps = diff / delta;

        auto basePtr = std::make_unique<Expression>(Constant(range.first));
        auto multiplicationPtr = std::make_unique<Expression>(BinaryExpression(BinaryExpression::Operator::MULTIPLY,
                std::make_unique<Expression>(VariableRef(tempName)),
                std::make_unique<Expression>(RealConstant(delta))));
        auto additionPtr = std::make_unique<Expression>(BinaryExpression(BinaryExpression::Operator::ADD,
                std::make_unique<Expression>(Constant(range.first)), std::move(multiplicationPtr)));

        std::unique_ptr<model::Sequence> nestedSequence =
                std::make_unique<model::Sequence>(model::Sequence::Type::ATOMIC);
        nestedSequence->appendElement(
                Select(VariableRef(tempName), Expression(Constant(0)), Expression(Constant(steps))));
        nestedSequence->appendElement(Assignment(VariableRef(valueVariableName), *additionPtr));
        conditional.appendAlternative(std::move(nestedSequence));
    }

    model::Sequence sequence(model::Sequence::Type::NORMAL);
    sequence.appendElement(ProctypeMaker::makeVariableDeclaration(model::BasicType::INT, tempName));
    sequence.appendElement(std::move(conditional));

    createValueGenerationInline(std::move(sequence));
}

void Asn1TypeValueGeneratorVisitor::visit(const LabelType &type)
{
    Q_UNUSED(type);
    const QString message = QString("LabelType ASN.1 type's translation to Promela is not implemented yet (%1, %2)")
                                    .arg(__FILE__)
                                    .arg(__LINE__);
    throw std::logic_error(message.toStdString().c_str());
}

void Asn1TypeValueGeneratorVisitor::visit(const Integer &type)
{
    IntegerRangeConstraintVisitor constraintVisitor;
    type.constraints().accept(constraintVisitor);

    const auto valueVariableName = getInlineArgumentName();
    const auto tempName = Escaper::escapePromelaName(QString("%1_tmp").arg(m_name));
    std::optional<IntegerSubset> integerSubset = constraintVisitor.getResultSubset();

    if (!integerSubset.has_value()) {
        auto message =
                QString("Unable to generate values for type %1: unable to determine available subset").arg(m_name);
        throw TranslationException(message);
    }

    Conditional conditional;

    for (const auto &range : integerSubset.value().getRanges()) {
        const auto rangeMin = range.first;
        const auto rangeMax = range.second;

        std::unique_ptr<model::Sequence> nestedSequence =
                std::make_unique<model::Sequence>(model::Sequence::Type::ATOMIC);
        nestedSequence->appendElement(
                Select(VariableRef(tempName), Expression(Constant(rangeMin)), Expression(Constant(rangeMax))));
        conditional.appendAlternative(std::move(nestedSequence));
    }

    model::Sequence sequence(model::Sequence::Type::NORMAL);
    sequence.appendElement(ProctypeMaker::makeVariableDeclaration(model::BasicType::INT, tempName));
    sequence.appendElement(std::move(conditional));
    sequence.appendElement(Assignment(VariableRef(valueVariableName), Expression(VariableRef(tempName))));

    createValueGenerationInline(std::move(sequence));
}

void Asn1TypeValueGeneratorVisitor::visit(const UserdefinedType &type)
{
    auto *const typeType = type.type();
    if (typeType != nullptr) {
        const auto name = m_name.isEmpty() ? type.typeName() : m_name;
        Asn1TypeValueGeneratorVisitor visitor(m_promelaModel, name, m_overridenType);
        typeType->accept(visitor);
    }
}

void Asn1TypeValueGeneratorVisitor::createValueGenerationInline(model::Sequence sequence)
{
    const auto valueVariableName = getInlineArgumentName();
    const QString inlineName = getInlineGeneratorName(m_name);

    if (!modelContainsInlineGenerator(inlineName)) {
        const QList<QString> inlineArguments = { valueVariableName };
        std::unique_ptr<InlineDef> inlineDef =
                std::make_unique<InlineDef>(inlineName, inlineArguments, std::move(sequence));

        m_promelaModel.addInlineDef(std::move(inlineDef));
    }
}

Asn1Acn::Types::Type *Asn1TypeValueGeneratorVisitor::getAsnSequenceComponentType(
        Asn1Acn::AsnSequenceComponent *const component)
{
    if (component == nullptr) {
        throw std::runtime_error("Component cannot be null");
    }

    Asn1Acn::Types::Type *const componentType = component->type();

    if (componentType == nullptr) {
        throw std::runtime_error("Type not specified in Component");
    }

    return componentType;
}

Asn1Acn::Types::Type *Asn1TypeValueGeneratorVisitor::getChoiceComponentType(
        Asn1Acn::Types::ChoiceAlternative *const component)
{
    if (component == nullptr) {
        throw std::runtime_error("Component cannot be null");
    }

    Asn1Acn::Types::Type *const componentType = component->type();

    if (componentType == nullptr) {
        throw std::runtime_error("Type not specified in Component");
    }

    return componentType;
}

QString Asn1TypeValueGeneratorVisitor::getSequenceComponentTypeName(
        const Asn1Acn::AsnSequenceComponent &asnComponent, const QString &sequenceName)
{
    const auto &type = *asnComponent.type();

    if (isEmbeddedType(type)) {
        return PromelaNameHelper::createChildTypeName(sequenceName, asnComponent.name());
    } else {
        return type.typeName();
    }
}

QString Asn1TypeValueGeneratorVisitor::getChoiceComponentTypeName(
        const Asn1Acn::Types::ChoiceAlternative &choiceComponent, const QString &choiceName)
{
    const auto &type = *choiceComponent.type();

    if (isEmbeddedType(type)) {
        return PromelaNameHelper::createChildTypeName(choiceName, choiceComponent.name());
    } else {
        return type.typeName();
    }
}

QString Asn1TypeValueGeneratorVisitor::getInlineInitName(const QString &typeName)
{
    return QString("%1_init_value").arg(Escaper::escapePromelaName(typeName));
}

QString Asn1TypeValueGeneratorVisitor::getInlineGeneratorName(const QString &typeName)
{
    return QString("%1_generate_value").arg(Escaper::escapePromelaName(typeName));
}

std::unique_ptr<ProctypeElement> Asn1TypeValueGeneratorVisitor::generateAsnSequenceComponentInlineCall(
        Asn1Acn::AsnSequenceComponent *const asnSequenceComponent, const QString &argumentName)
{
    const QString typeToGenerateName = getSequenceComponentTypeName(*asnSequenceComponent, m_name);
    const QString typeGeneratorToCallName = getInlineGeneratorName(typeToGenerateName);
    const QString &componentName = Escaper::escapePromelaName(asnSequenceComponent->name());

    if (asnSequenceComponent->isOptional()) {
        const QString valueExistAssignmentName = QString("%1.exist.%2").arg(argumentName).arg(componentName);

        auto valueExistsSequence = ProctypeMaker::makeNormalSequence();
        valueExistsSequence->appendElement(ProctypeMaker::makeTrueExpressionProctypeElement());
        valueExistsSequence->appendElement(ProctypeMaker::makeInlineCall(
                typeGeneratorToCallName, Escaper::escapePromelaName(argumentName), componentName));
        valueExistsSequence->appendElement(
                ProctypeMaker::makeAssignmentProctypeElement(valueExistAssignmentName, Constant(1)));

        auto valueNotExistSequence = ProctypeMaker::makeNormalSequence();
        valueNotExistSequence->appendElement(ProctypeMaker::makeTrueExpressionProctypeElement());
        valueNotExistSequence->appendElement(
                ProctypeMaker::makeAssignmentProctypeElement(valueExistAssignmentName, Constant(0)));

        Conditional conditional;
        conditional.appendAlternative(std::move(valueExistsSequence));
        conditional.appendAlternative(std::move(valueNotExistSequence));

        return std::make_unique<ProctypeElement>(std::move(conditional));
    } else {
        return ProctypeMaker::makeInlineCall(typeGeneratorToCallName, argumentName, componentName);
    }
}

bool Asn1TypeValueGeneratorVisitor::modelContainsInlineGenerator(const QString &inlineGeneratorName)
{
    for (const auto &inlineDef : m_promelaModel.getInlineDefs()) {
        if (inlineDef->getName().compare(inlineGeneratorName) == 0) {
            return true;
        }
    }
    return false;
}

bool Asn1TypeValueGeneratorVisitor::isEmbeddedType(const Asn1Acn::Types::Type &type)
{
    return !type.label().contains(".");
}

QString Asn1TypeValueGeneratorVisitor::getInlineArgumentName()
{
    // gv is generated value; it is short to reduce the chance of generating a too long identifier
    return Escaper::escapePromelaName(QString("%1_gv").arg(m_name));
}

void Asn1TypeValueGeneratorVisitor::InlineDefAdder::addRangedIntegerGeneratorToModel(
        const QString &inlineName, model::PromelaModel &model, const long minSize, const long maxSize)
{
    Asn1Acn::Types::Integer lengthType(lengthMemberName);
    const Asn1Acn::Range<Asn1Acn::IntegerValue::Type> range(minSize, maxSize);
    lengthType.constraints().append(range);

    Asn1TypeValueGeneratorVisitor lenVisitor(model, inlineName, nullptr);
    lengthType.accept(lenVisitor);
}

Expression Asn1TypeValueGeneratorVisitor::InlineDefAdder::getValueLengthAsExpression(const QString &valueVariableName)
{
    const VariableRef varRef(QString("%1.%2").arg(valueVariableName).arg(lengthMemberName));

    return Expression(varRef);
}

Expression Asn1TypeValueGeneratorVisitor::InlineDefAdder::getValueLengthMinusConstAsExpression(
        const QString &valueVariableName, const int x)
{
    const auto subtractOperator = model::BinaryExpression::Operator::SUBTRACT;

    const VariableRef endVarRef(QString("%1.%2").arg(valueVariableName).arg(lengthMemberName));
    auto endVarRefExpression = std::make_unique<Expression>(endVarRef);
    auto oneConstExpression = std::make_unique<Expression>(model::Constant(x));

    const model::BinaryExpression endBinaryExpression(
            subtractOperator, std::move(endVarRefExpression), std::move(oneConstExpression));

    return Expression(endBinaryExpression);
}

template<typename ValueType>
void Asn1TypeValueGeneratorVisitor::handleOverridenType(const QString &typeName,
        const Asn1Acn::Constraints::ConstraintList<ValueType> &constraints, const QString &initInlineName,
        const QString &valueName, const QString &iteratorName, const size_t minSize, const size_t maxSize,
        model::Sequence *sequence) const
{
    SizeConstraintVisitor<ValueType> constraintVisitor;
    constraints.accept(constraintVisitor);

    const size_t overridenMinSize = constraintVisitor.getMinSize();
    const size_t overridenMaxSize = constraintVisitor.getMaxSize();

    const auto isConstSize = minSize == maxSize;
    const auto isOverridenConstSize = overridenMinSize == overridenMaxSize;

    const auto areSameSize = (minSize == overridenMinSize && maxSize == overridenMaxSize);

    // If both types have the same size, then we don't need to do anything
    if (areSameSize) {
        return;
    }

    // We allow to subtype one const size with another const size
    if (!isConstSize && !isOverridenConstSize) {
        if (minSize < overridenMinSize) {
            auto errorMessage = QString("Trying to subtype %1 with %2 which has smaller minimum size")
                                        .arg(m_overridenType->identifier())
                                        .arg(typeName);
            throw TranslationException(std::move(errorMessage));
        } else if (maxSize > overridenMaxSize) {
            auto errorMessage = QString("Trying to subtype %1 with %2 which has bigger maximum size")
                                        .arg(m_overridenType->identifier())
                                        .arg(typeName);
            throw TranslationException(std::move(errorMessage));
        }
    }

    if (isConstSize) {
        if (isOverridenConstSize) {
            if (initInlineName.isEmpty()) {
                auto loopSequence = std::make_unique<model::Sequence>(model::Sequence::Type::NORMAL);

                VariableRef dst(valueName);
                dst.appendElement("data", std::make_unique<Expression>(VariableRef(iteratorName)));

                loopSequence->appendElement(Assignment(dst, Expression(Constant(0))));

                auto forLoop = ForLoop(VariableRef(iteratorName), Expression(maxSize), Expression(overridenMaxSize - 1),
                        std::move(loopSequence));
                sequence->appendElement(std::move(forLoop));
            } else {
                auto initCall = ProctypeMaker::makeCallForEachValue(
                        initInlineName, valueName, Expression(maxSize), Expression(overridenMaxSize - 1), iteratorName);
                sequence->appendElement(std::move(initCall));
            }
        } else {
            // If we are overriding a variable size type with const size type we just need to set length
            VariableRef dst(valueName);
            dst.appendElement("length");

            sequence->appendElement(Assignment(dst, Expression(Constant(maxSize))));
        }
    } else {
        if (isOverridenConstSize) {
            auto errorMessage = QString("Trying to subtype fixed-size %1 with variable-size %2")
                                        .arg(m_overridenType->identifier())
                                        .arg(typeName);
            throw TranslationException(std::move(errorMessage));
        }

        Expression start = InlineDefAdder::getValueLengthAsExpression(valueName);

        if (initInlineName.isEmpty()) {
            auto loopSequence = std::make_unique<model::Sequence>(model::Sequence::Type::NORMAL);

            VariableRef dst(valueName);
            dst.appendElement("data", std::make_unique<Expression>(VariableRef(iteratorName)));

            loopSequence->appendElement(Assignment(dst, Expression(Constant(0))));

            auto forLoop = ForLoop(
                    VariableRef(iteratorName), start, Expression(overridenMaxSize - 1), std::move(loopSequence));
            sequence->appendElement(std::move(forLoop));
        } else {
            auto initCall = ProctypeMaker::makeCallForEachValue(
                    initInlineName, valueName, start, Expression(overridenMaxSize - 1), iteratorName);
            sequence->appendElement(std::move(initCall));
        }
    }
}

} // namespace promela::translator
