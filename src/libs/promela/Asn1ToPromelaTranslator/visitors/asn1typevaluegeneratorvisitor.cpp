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

#include "asnsequencecomponent.h"
#include "assignment.h"
#include "enumeratedgenerator.h"
#include "inlinecall.h"
#include "inlinedef.h"
#include "integerconstraintvisitor.h"
#include "integergenerator.h"
#include "integersubset.h"
#include "sequence.h"
#include "sequencecomponent.h"
#include "types/type.h"
#include "types/typereadingvisitor.h"
#include "values.h"
#include "variableref.h"

#include <algorithm>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/sequence.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <list>
#include <memory>
#include <optional>
#include <promela/Asn1ToPromelaTranslator/visitors/asn1sequencecomponentvisitor.h>
#include <promela/PromelaModel/constant.h>
#include <qglobal.h>
#include <qlist.h>
#include <stdexcept>
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
using promela::model::Expression;
using promela::model::InlineDef;
using promela::model::ProctypeElement;
using promela::model::PromelaModel;
using promela::model::UtypeRef;
using promela::model::VariableRef;

namespace promela::translator {

static auto makeInlineCall(Asn1Acn::AsnSequenceComponent *envGeneratorInline, const QString &callArgumentName,
        const QString &generatorInlineName) -> std::unique_ptr<ProctypeElement>;
static auto makeNormalSequence() -> std::unique_ptr<promela::model::Sequence>;
static auto makeTrueExpressionProctypeElement() -> std::unique_ptr<promela::model::ProctypeElement>;
static auto makeAssignmentProctypeElement(const QString &valueName, int32_t value)
        -> std::unique_ptr<promela::model::ProctypeElement>;
static auto getSequenceComponentTypeName(const Asn1Acn::AsnSequenceComponent &asnComponent, const QString &sequenceName)
        -> QString;

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

    std::for_each(possibleValues.begin(), possibleValues.end(), [&conditional](const bool &value) {
        auto nestedSequence = std::make_unique<::promela::model::Sequence>(::promela::model::Sequence::Type::NORMAL);

        nestedSequence->appendElement(std::make_unique<ProctypeElement>(Expression(VariableRef("true"))));
        nestedSequence->appendElement(
                std::make_unique<ProctypeElement>(Assignment(VariableRef("value"), Expression(Constant(value)))));

        conditional.appendAlternative(std::move(nestedSequence));
    });

    ::promela::model::Sequence sequence(::promela::model::Sequence::Type::NORMAL);
    sequence.appendElement(std::make_unique<ProctypeElement>(std::move(conditional)));

    createValueGenerationInline(std::move(sequence));
}

void Asn1TypeValueGeneratorVisitor::visit(const Null &type)
{
    Q_UNUSED(type);
}

void Asn1TypeValueGeneratorVisitor::visit(const BitString &type)
{
    Q_UNUSED(type);
}

void Asn1TypeValueGeneratorVisitor::visit(const OctetString &type)
{
    Q_UNUSED(type);
}

void Asn1TypeValueGeneratorVisitor::visit(const IA5String &type)
{
    Q_UNUSED(type);
}

void Asn1TypeValueGeneratorVisitor::visit(const NumericString &type)
{
    Q_UNUSED(type);
}

void Asn1TypeValueGeneratorVisitor::visit(const Enumerated &type)
{
    Conditional conditional;

    EnumeratedGenerator generator(Escaper::escapePromelaName(m_name), type);

    while (generator.has_next()) {
        auto element = generator.next();

        std::unique_ptr<::promela::model::Sequence> nestedSequence =
                std::make_unique<::promela::model::Sequence>(::promela::model::Sequence::Type::NORMAL);

        nestedSequence->appendElement(std::make_unique<ProctypeElement>(Expression(VariableRef("true"))));
        nestedSequence->appendElement(std::make_unique<ProctypeElement>(
                Assignment(VariableRef("value"), Expression(VariableRef(element.first)))));

        conditional.appendAlternative(std::move(nestedSequence));
    }

    ::promela::model::Sequence sequence(::promela::model::Sequence::Type::NORMAL);
    sequence.appendElement(std::make_unique<ProctypeElement>(std::move(conditional)));

    createValueGenerationInline(std::move(sequence));
}

void Asn1TypeValueGeneratorVisitor::visit(const Choice &type)
{
    Q_UNUSED(type);
}

void Asn1TypeValueGeneratorVisitor::visit(const Sequence &type)
{
    const QString argumentName = "value";

    const QString inlineSeqGeneratorName = QString("%1_generate_value").arg(type.identifier());
    const QStringList inlineArguments = { argumentName };
    promela::model::Sequence sequence(promela::model::Sequence::Type::NORMAL);
    {
        for (auto &sequenceComponent : type.components()) {
            auto *const asnSequenceComponent = static_cast<Asn1Acn::AsnSequenceComponent *>(sequenceComponent.get());
            if (asnSequenceComponent != nullptr) {
                sequence.appendElement(generateAsnSequenceComponentInline(asnSequenceComponent, argumentName));
            }
        }
    }

    auto inlineDef = std::make_unique<InlineDef>(inlineSeqGeneratorName, inlineArguments, std::move(sequence));

    m_promelaModel.addInlineDef(std::move(inlineDef));
}

void Asn1TypeValueGeneratorVisitor::visit(const SequenceOf &type)
{
    Q_UNUSED(type);
}

void Asn1TypeValueGeneratorVisitor::visit(const Real &type)
{
    Q_UNUSED(type);
}

void Asn1TypeValueGeneratorVisitor::visit(const LabelType &type)
{
    Q_UNUSED(type);
}

void Asn1TypeValueGeneratorVisitor::visit(const Integer &type)
{
    IntegerConstraintVisitor constraintVisitor;
    type.constraints().accept(constraintVisitor);

    std::optional<IntegerSubset> integerSubset = constraintVisitor.getResultSubset();

    if (!integerSubset.has_value()) {
        auto message =
                QString("Unable to generate values for type %1: unable to determine available subset").arg(m_name);
        throw TranslationException(message);
    }

    Conditional conditional;
    IntegerGenerator generator(integerSubset.value());

    while (generator.has_next()) {
        int value = generator.next();

        std::unique_ptr<::promela::model::Sequence> nestedSequence =
                std::make_unique<::promela::model::Sequence>(::promela::model::Sequence::Type::NORMAL);

        nestedSequence->appendElement(std::make_unique<ProctypeElement>(Expression(VariableRef("true"))));
        nestedSequence->appendElement(
                std::make_unique<ProctypeElement>(Assignment(VariableRef("value"), Expression(Constant(value)))));

        conditional.appendAlternative(std::move(nestedSequence));
    }

    ::promela::model::Sequence sequence(::promela::model::Sequence::Type::NORMAL);
    sequence.appendElement(std::make_unique<ProctypeElement>(std::move(conditional)));

    createValueGenerationInline(std::move(sequence));
}

void Asn1TypeValueGeneratorVisitor::visit(const UserdefinedType &type)
{
    Q_UNUSED(type);
}

void Asn1TypeValueGeneratorVisitor::createValueGenerationInline(::promela::model::Sequence sequence)
{
    const QString inlineName = QString("%1_generate_value").arg(m_name);
    const QList<QString> inlineArguments = { QString("value") };
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

    const auto &componentTypeEnum = componentType->typeEnum();
    if (componentTypeEnum != Asn1Acn::Types::Type::ASN1Type::BOOLEAN
            && componentTypeEnum != Asn1Acn::Types::Type::ASN1Type::INTEGER
            && componentTypeEnum != Asn1Acn::Types::Type::ASN1Type::USERDEFINED) {
        throw std::runtime_error("Unknown component type");
    }

    return componentType;
}

std::unique_ptr<ProctypeElement> makeInlineCall(Asn1Acn::AsnSequenceComponent *const envGeneratorInline,
        const QString &callArgumentName, const QString &generatorInlineName)
{
    const QString inlineCallArgument = QString("%1.%2").arg(callArgumentName).arg(envGeneratorInline->name());
    QList<promela::model::InlineCall::Argument> args({ promela::model::InlineCall::Argument(inlineCallArgument) });

    auto inlineCall = promela::model::InlineCall(generatorInlineName, args);
    return std::make_unique<ProctypeElement>(std::move(inlineCall));
}

std::unique_ptr<promela::model::Sequence> makeNormalSequence()
{
    return std::make_unique<promela::model::Sequence>(promela::model::Sequence::Type::NORMAL);
}

std::unique_ptr<promela::model::ProctypeElement> makeTrueExpressionProctypeElement()
{
    return std::make_unique<ProctypeElement>(Expression(VariableRef("true")));
}

std::unique_ptr<promela::model::ProctypeElement> makeAssignmentProctypeElement(
        const QString &valueName, const int32_t value)
{
    Assignment valueExistAssignment((VariableRef(valueName)), Expression(Constant(value)));

    return std::make_unique<ProctypeElement>(std::move(valueExistAssignment));
}

QString getSequenceComponentTypeName(const Asn1Acn::AsnSequenceComponent &asnComponent, const QString &sequenceName)
{
    const auto &type = asnComponent.type();
    if (type->label().contains(".")) {
        return type->typeName();
    } else {
        return QString("%1_%2").arg(sequenceName).arg(asnComponent.name());
    }
}

std::unique_ptr<ProctypeElement> Asn1TypeValueGeneratorVisitor::generateAsnSequenceComponentInline(
        Asn1Acn::AsnSequenceComponent *asnSequenceComponent, const QString &argumentName)
{
    const QString typeToGenerateName = getSequenceComponentTypeName(*asnSequenceComponent, m_name);
    auto *const asnSequenceComponentType = getAsnSequenceComponentType(asnSequenceComponent);
    ChangeStringInScope changeName(&m_name, typeToGenerateName);
    asnSequenceComponentType->accept(*this);

    const QString typeGeneratorToCallName = QString("%1_generate_value").arg(typeToGenerateName);
    if (asnSequenceComponent->isOptional()) {
        const QString valueExistAssignmentName =
                QString("%1.exist.%2").arg(argumentName).arg(asnSequenceComponent->name());

        auto valueExistsSequence = makeNormalSequence();
        valueExistsSequence->appendElement(makeTrueExpressionProctypeElement());
        valueExistsSequence->appendElement(makeInlineCall(asnSequenceComponent, argumentName, typeGeneratorToCallName));
        valueExistsSequence->appendElement(makeAssignmentProctypeElement(valueExistAssignmentName, 1));

        auto valueNotExistSequence = makeNormalSequence();
        valueNotExistSequence->appendElement(makeTrueExpressionProctypeElement());
        valueNotExistSequence->appendElement(makeAssignmentProctypeElement(valueExistAssignmentName, 0));

        Conditional conditional;
        conditional.appendAlternative(std::move(valueExistsSequence));
        conditional.appendAlternative(std::move(valueNotExistSequence));

        return std::make_unique<ProctypeElement>(std::move(conditional));
    } else {
        return makeInlineCall(asnSequenceComponent, argumentName, typeGeneratorToCallName);
    }
}

} // namespace promela::translator
