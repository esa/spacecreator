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
#include "inlinedef.h"
#include "integerconstraintvisitor.h"
#include "integergenerator.h"
#include "integersubset.h"
#include "proctypeelement.h"
#include "sequence.h"
#include "sequencecomponent.h"
#include "types/type.h"
#include "types/typereadingvisitor.h"
#include "values.h"
#include "variableref.h"

#include <QDebug>
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
#include <qdebug.h>
#include <qglobal.h>
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

QString getName(Asn1Acn::AsnSequenceComponent *const envGeneratorInline, const QString &componentAsnTypeName)
{
    QString generatorInlineName;

    const QString &inlineTypeName = envGeneratorInline->type()->typeName();
    if (inlineTypeName.compare("INTEGER") == 0) {
        generatorInlineName = "INTEGER";
    } else if (inlineTypeName.compare("BOOLEAN") == 0) {
        generatorInlineName = "BOOLEAN";
    } else {
        generatorInlineName = componentAsnTypeName;
    }

    return QString("%1_generate_value").arg(generatorInlineName);
}

std::unique_ptr<ProctypeElement> makeInlineCall(
        Asn1Acn::AsnSequenceComponent *const envGeneratorInline, const QString &callArgumentName)
{
    const QString inlineCallArgument = QString("%1.%2").arg(callArgumentName).arg(envGeneratorInline->name());
    const QString componentTypeLabel = envGeneratorInline->type()->label();
    const QString componentAsnTypeName = componentTypeLabel.split(".").last().split(" ").last();
    const QString generatorInlineName = getName(envGeneratorInline, componentAsnTypeName);

    auto inlineCall = promela::model::InlineCall(generatorInlineName, QList<VariableRef>({ inlineCallArgument }));
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

void Asn1TypeValueGeneratorVisitor::visit(const Sequence &type)
{
    auto *const typeReadingVisitorPtr = static_cast<Asn1Acn::Types::TypeReadingVisitor *>(this);

    const QString inlineSeqGeneratorName = QString("%1_generate_value").arg(type.identifier());
    const QString argumentName = "value";
    const QList<QString> inlineArguments = { argumentName };
    promela::model::Sequence sequence(promela::model::Sequence::Type::NORMAL);
    for (auto &sequenceComponent : type.components()) {
        auto *const asnSequenceComponent = static_cast<Asn1Acn::AsnSequenceComponent *>(sequenceComponent.get());
        const auto asnTypeName = asnSequenceComponent->type()->typeName();
        if (asnSequenceComponent != nullptr) {
            const QString sequenceName = m_name;
            m_name = asnTypeName;
            auto *const asnSequenceComponentType = getAsnSequenceComponentType(asnSequenceComponent);
            asnSequenceComponentType->accept(*typeReadingVisitorPtr);
            m_name = sequenceName;

            // if (asnSequenceComponent->isOptional()) {
            //     const QString valueExistName =
            //     QString("%1.exist.%2").arg(argumentName).arg(sequenceComponent->name());

            //     auto sequenceWithInlineToGenerateValue = makeNormalSequence();
            //     sequenceWithInlineToGenerateValue->appendElement(makeTrueExpressionProctypeElement());
            //     sequenceWithInlineToGenerateValue->appendElement(makeInlineCall(asnSequenceComponent, argumentName));
            //     sequenceWithInlineToGenerateValue->appendElement(makeAssignmentProctypeElement(valueExistName, 1));

            //     auto emptySequenceOptionalIsOff = makeNormalSequence();
            //     emptySequenceOptionalIsOff->appendElement(makeTrueExpressionProctypeElement());
            //     emptySequenceOptionalIsOff->appendElement(makeAssignmentProctypeElement(valueExistName, 0));

            //     Conditional conditional;
            //     conditional.appendAlternative(std::move(sequenceWithInlineToGenerateValue));
            //     conditional.appendAlternative(std::move(emptySequenceOptionalIsOff));

            //     sequence.appendElement(std::make_unique<ProctypeElement>(std::move(conditional)));
            // } else {
            //     sequence.appendElement(makeInlineCall(asnSequenceComponent, argumentName));
            // }
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

Asn1TypeValueGeneratorVisitor::NameChanger::NameChanger(QString *nameToTemporarilyChange, const QString &temporaryName)
    : m_initialName(*nameToTemporarilyChange)
{
    *nameToTemporarilyChange = temporaryName;
}

Asn1TypeValueGeneratorVisitor::NameChanger::~NameChanger()
{
    *m_nameToChange = m_initialName;
}

} // namespace promela::translator
