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

#include "proctypemaker.h"
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
#include <promela/PromelaModel/basictypes.h>
#include <promela/PromelaModel/binaryexpression.h>
#include <promela/PromelaModel/conditional.h>
#include <promela/PromelaModel/constant.h>
#include <promela/PromelaModel/inlinecall.h>
#include <promela/PromelaModel/inlinedef.h>
#include <promela/PromelaModel/proctypeelement.h>
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
using promela::model::Assignment;
using promela::model::Conditional;
using promela::model::Constant;
using promela::model::DoLoop;
using promela::model::ExitLoop;
using promela::model::Expression;
using promela::model::InlineCall;
using promela::model::InlineDef;
using promela::model::ProctypeElement;
using promela::model::PromelaModel;
using promela::model::Select;
using promela::model::UtypeRef;
using promela::model::ValueDefinition;
using promela::model::VariableRef;

namespace promela::translator {

const QString Asn1TypeValueGeneratorVisitor::InlineDefAdder::lengthMemberName = "length";
const QString Asn1TypeValueGeneratorVisitor::InlineDefAdder::octetGeneratorName = "OctetStringElement_generate_value";

Asn1TypeValueGeneratorVisitor::Asn1TypeValueGeneratorVisitor(PromelaModel &promelaModel, QString name)
    : m_promelaModel(promelaModel)
    , m_name(std::move(name))
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

        nestedSequence->appendElement(std::make_unique<ProctypeElement>(Expression(VariableRef("true"))));
        nestedSequence->appendElement(std::make_unique<ProctypeElement>(
                Assignment(VariableRef(valueVariableName), Expression(Constant(value)))));

        conditional.appendAlternative(std::move(nestedSequence));
    });

    model::Sequence sequence(model::Sequence::Type::NORMAL);
    sequence.appendElement(std::make_unique<ProctypeElement>(std::move(conditional)));

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

    auto sequence = ProctypeMaker::makeNormalSequence();
    sequence->appendElement(ProctypeMaker::makeVariableDeclaration(model::BasicType::INT, "i"));
    const size_t minSize = constraintVisitor.getMinSize();
    const size_t maxSize = constraintVisitor.getMaxSize();
    if (minSize == maxSize) {
        sequence->appendElement(
                ProctypeMaker::makeCallForEachValue(octetGeneratorName, argumentName, Expression(maxSize - 1)));
    } else { // OctetString has variable length
        const QString lengthGeneratorTypeName =
                QString("%1_%2").arg(typeIdentifier).arg(InlineDefAdder::lengthMemberName);
        InlineDefAdder::addRangedIntegerGeneratorToModel(
                lengthGeneratorTypeName, m_promelaModel, static_cast<long>(minSize), static_cast<long>(maxSize));

        const QString valueLengthVariableName =
                QString("%1.%2").arg(argumentName).arg(InlineDefAdder::lengthMemberName);
        sequence->appendElement(ProctypeMaker::makeInlineCall(
                QString("%1_generate_value").arg(lengthGeneratorTypeName), valueLengthVariableName));

        Expression end = InlineDefAdder::getValueLenghtMinusConstAsExpression(argumentName, 1);
        sequence->appendElement(ProctypeMaker::makeCallForEachValue(octetGeneratorName, argumentName, end));
    }

    const QString inlineSeqGeneratorName = QString("%1_generate_value").arg(typeIdentifier);
    const QStringList inlineArguments = { argumentName };
    auto inlineDef = std::make_unique<InlineDef>(inlineSeqGeneratorName, inlineArguments, std::move(*sequence));

    m_promelaModel.addInlineDef(std::move(inlineDef));
}

void Asn1TypeValueGeneratorVisitor::visit(const IA5String &type)
{
    Q_UNUSED(type);
    const QString message = QString("IA5String ASN.1 type's translation to Promela is not implemented yet (%1, %2)")
                                    .arg(__FILE__)
                                    .arg(__LINE__);
    throw std::logic_error(message.toStdString().c_str());
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

        nestedSequence->appendElement(std::make_unique<ProctypeElement>(Expression(VariableRef("true"))));
        nestedSequence->appendElement(std::make_unique<ProctypeElement>(
                Assignment(VariableRef(valueVariableName), Expression(VariableRef(element.first)))));

        conditional.appendAlternative(std::move(nestedSequence));
    }

    model::Sequence sequence(model::Sequence::Type::NORMAL);
    sequence.appendElement(std::make_unique<ProctypeElement>(std::move(conditional)));

    createValueGenerationInline(std::move(sequence));
}

void Asn1TypeValueGeneratorVisitor::visit(const Choice &type)
{
    const auto valueVariableName = getInlineArgumentName();
    const QStringList inlineArguments = { valueVariableName };

    auto sequence = ProctypeMaker::makeNormalSequence();

    auto conditional = std::make_unique<Conditional>();
    int selectorVal = 1;
    for (auto &component : type.components()) {
        const QString &componentName = component->name();
        const QString thisComponentSelected =
                Escaper::escapePromelaName(QString("%1_%2_PRESENT").arg(m_name).arg(componentName));
        m_promelaModel.addValueDefinition(ValueDefinition(thisComponentSelected, selectorVal++));

        auto *const choiceComponent = component.get();
        const QString componentTypeName =
                Escaper::escapePromelaName(getChoiceComponentTypeName(*choiceComponent, m_name));
        const QString inlineTypeGeneratorName = Escaper::escapePromelaName(getInlineGeneratorName(componentTypeName));

        if (!modelContainsInlineGenerator(inlineTypeGeneratorName)) {
            auto *const choiceComponentType = getChoiceComponentType(choiceComponent);
            Asn1TypeValueGeneratorVisitor visitor(m_promelaModel, componentTypeName);
            choiceComponentType->accept(visitor);
        }

        auto alternative = ProctypeMaker::makeNormalSequence();
        alternative->appendElement(ProctypeMaker::makeTrueExpressionProctypeElement());
        alternative->appendElement(ProctypeMaker::makeInlineCall(inlineTypeGeneratorName,
                QString("%1.data.%2").arg(valueVariableName).arg(Escaper::escapePromelaName(componentName))));
        alternative->appendElement(ProctypeMaker::makeAssignmentProctypeElement(
                QString("%1.selection").arg(valueVariableName), thisComponentSelected));

        conditional->appendAlternative(std::move(alternative));
    }
    sequence->appendElement(std::make_unique<ProctypeElement>(std::move(*conditional)));

    const QString choiceGeneratorInlineName = Escaper::escapePromelaName(QString("%1_generate_value").arg(m_name));
    auto inlineDef = std::make_unique<InlineDef>(choiceGeneratorInlineName, inlineArguments, std::move(*sequence));

    m_promelaModel.addInlineDef(std::move(inlineDef));
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
            if (!modelContainsInlineGenerator(inlineTypeGeneratorName)) {
                auto *const asnSequenceComponentType = getAsnSequenceComponentType(asnSequenceComponent);
                Asn1TypeValueGeneratorVisitor visitor(m_promelaModel, componentTypeName);
                asnSequenceComponentType->accept(visitor);
            }

            auto asnSequenceComponentInlineCall =
                    generateAsnSequenceComponentInlineCall(asnSequenceComponent, valueVariableName);
            sequence.appendElement(std::move(asnSequenceComponentInlineCall));
        }
    }

    auto inlineDef = std::make_unique<InlineDef>(inlineSeqGeneratorName, inlineArguments, std::move(sequence));

    m_promelaModel.addInlineDef(std::move(inlineDef));
}

void Asn1TypeValueGeneratorVisitor::visit(const SequenceOf &type)
{
    const auto valueVariableName = getInlineArgumentName();
    const QString componentTypeName = Escaper::escapePromelaName(type.itemsType()->typeName());
    const QString inlineTypeGeneratorName = Escaper::escapePromelaName(getInlineGeneratorName(componentTypeName));
    if (!modelContainsInlineGenerator(inlineTypeGeneratorName)) {
        auto *const asnSequenceComponentType = type.itemsType();
        Asn1TypeValueGeneratorVisitor visitor(m_promelaModel, componentTypeName);
        asnSequenceComponentType->accept(visitor);
    }

    SizeConstraintVisitor<Asn1Acn::IntegerValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);

    const QString typeIdentifier = Escaper::escapePromelaName(m_name);

    auto sequence = ProctypeMaker::makeNormalSequence();
    sequence->appendElement(ProctypeMaker::makeVariableDeclaration(model::BasicType::INT, "i"));
    const size_t minSize = constraintVisitor.getMinSize();
    const size_t maxSize = constraintVisitor.getMaxSize();
    const QString typeGeneratorInline = QString("%1%2").arg(componentTypeName).arg("_generate_value");
    if (minSize == maxSize) {
        sequence->appendElement(
                ProctypeMaker::makeCallForEachValue(typeGeneratorInline, valueVariableName, Expression(maxSize - 1)));
    } else { // sequenceOf has variable length
        const QString lengthGeneratorTypeName =
                QString("%1_%2").arg(typeIdentifier).arg(InlineDefAdder::lengthMemberName);
        InlineDefAdder::addRangedIntegerGeneratorToModel(
                lengthGeneratorTypeName, m_promelaModel, static_cast<long>(minSize), static_cast<long>(maxSize));

        sequence->appendElement(ProctypeMaker::makeInlineCall(QString("%1_generate_value").arg(lengthGeneratorTypeName),
                QString("%1.%2").arg(valueVariableName).arg(InlineDefAdder::lengthMemberName)));

        Expression end = InlineDefAdder::getValueLenghtMinusConstAsExpression(valueVariableName, 1);
        sequence->appendElement(ProctypeMaker::makeCallForEachValue(typeGeneratorInline, valueVariableName, end));
    }

    const QString inlineSeqGeneratorName = QString("%1_generate_value").arg(typeIdentifier);
    const QStringList inlineArguments = { valueVariableName };
    auto inlineDef = std::make_unique<InlineDef>(inlineSeqGeneratorName, inlineArguments, std::move(*sequence));

    m_promelaModel.addInlineDef(std::move(inlineDef));
}

void Asn1TypeValueGeneratorVisitor::visit(const Real &type)
{
    Q_UNUSED(type);
    const QString message = QString("Real ASN.1 type's translation to Promela is not implemented yet (%1, %2)")
                                    .arg(__FILE__)
                                    .arg(__LINE__);
    throw std::logic_error(message.toStdString().c_str());
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
        nestedSequence->appendElement(std::make_unique<ProctypeElement>(
                Select(VariableRef(tempName), Expression(Constant(rangeMin)), Expression(Constant(rangeMax)))));
        conditional.appendAlternative(std::move(nestedSequence));
    }

    model::Sequence sequence(model::Sequence::Type::NORMAL);
    sequence.appendElement(ProctypeMaker::makeVariableDeclaration(model::BasicType::INT, tempName));
    sequence.appendElement(std::make_unique<ProctypeElement>(std::move(conditional)));
    sequence.appendElement(std::make_unique<ProctypeElement>(
            Assignment(VariableRef(valueVariableName), Expression(VariableRef(tempName)))));

    createValueGenerationInline(std::move(sequence));
}

void Asn1TypeValueGeneratorVisitor::visit(const UserdefinedType &type)
{
    auto *const typeType = type.type();
    if (typeType != nullptr) {
        const auto name = m_name.isEmpty() ? type.typeName() : m_name;
        Asn1TypeValueGeneratorVisitor visitor(m_promelaModel, name);
        typeType->accept(visitor);
    }
}

void Asn1TypeValueGeneratorVisitor::createValueGenerationInline(model::Sequence sequence)
{
    const auto valueVariableName = getInlineArgumentName();
    const QString inlineName = getInlineGeneratorName(m_name);
    const QList<QString> inlineArguments = { valueVariableName };
    std::unique_ptr<InlineDef> inlineDef =
            std::make_unique<InlineDef>(inlineName, inlineArguments, std::move(sequence));

    m_promelaModel.addInlineDef(std::move(inlineDef));
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
        return QString("%1_%2").arg(sequenceName).arg(asnComponent.name());
    } else {
        return type.typeName();
    }
}

QString Asn1TypeValueGeneratorVisitor::getChoiceComponentTypeName(
        const Asn1Acn::Types::ChoiceAlternative &choiceComponent, const QString &choiceName)
{
    const auto &type = *choiceComponent.type();

    if (isEmbeddedType(type)) {
        return QString("%1_%2").arg(choiceName).arg(choiceComponent.name());
    } else {
        return type.typeName();
    }
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
    const QString &componentName = asnSequenceComponent->name();

    if (asnSequenceComponent->isOptional()) {
        const QString valueExistAssignmentName = QString("%1.exist.%2").arg(argumentName).arg(componentName);

        auto valueExistsSequence = ProctypeMaker::makeNormalSequence();
        valueExistsSequence->appendElement(ProctypeMaker::makeTrueExpressionProctypeElement());
        valueExistsSequence->appendElement(ProctypeMaker::makeInlineCall(typeGeneratorToCallName,
                Escaper::escapePromelaName(argumentName), Escaper::escapePromelaName(componentName)));
        valueExistsSequence->appendElement(ProctypeMaker::makeAssignmentProctypeElement(valueExistAssignmentName, 1));

        auto valueNotExistSequence = ProctypeMaker::makeNormalSequence();
        valueNotExistSequence->appendElement(ProctypeMaker::makeTrueExpressionProctypeElement());
        valueNotExistSequence->appendElement(ProctypeMaker::makeAssignmentProctypeElement(valueExistAssignmentName, 0));

        Conditional conditional;
        conditional.appendAlternative(std::move(valueExistsSequence));
        conditional.appendAlternative(std::move(valueNotExistSequence));

        return std::make_unique<ProctypeElement>(std::move(conditional));
    } else {
        return ProctypeMaker::makeInlineCall(
                typeGeneratorToCallName, argumentName, Escaper::escapePromelaName(componentName));
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

    Asn1TypeValueGeneratorVisitor lenVisitor(model, inlineName);
    lengthType.accept(lenVisitor);
}

Expression Asn1TypeValueGeneratorVisitor::InlineDefAdder::getValueLenghtMinusConstAsExpression(
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

} // namespace promela::translator
