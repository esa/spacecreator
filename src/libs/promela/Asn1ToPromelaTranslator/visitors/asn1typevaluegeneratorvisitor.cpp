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
#include "enumeratedgenerator.h"
#include "integerconstraintvisitor.h"
#include "integergenerator.h"
#include "integersubset.h"
#include "sequencecomponent.h"
#include "types/type.h"

#include <QDebug>
#include <algorithm>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/sequence.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <list>
#include <optional>
#include <promela/Asn1ToPromelaTranslator/visitors/asn1sequencecomponentvisitor.h>
#include <promela/PromelaModel/constant.h>
#include <qdebug.h>
#include <qglobal.h>
#include <stdexcept>

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

void Asn1TypeValueGeneratorVisitor::visit(const Sequence &type)
{
    if (type.typeName().compare("SEQUENCE") != 0 || type.typeEnum() != Asn1Acn::Types::Type::SEQUENCE) {
        throw std::runtime_error("Invalid type");
    }

    // TODO:
    // type.acnParameters()
    // type.identifier()
    // type.label()

    // Asn1SequenceComponentVisitor visitor(m_promelaModel, "", false);

    for (const auto &component : type.components()) {
        if (component == nullptr || component->type() == nullptr) {
            throw std::runtime_error("Component type not specified");
        }
        const auto &componentTypeEnum = component->type()->typeEnum();
        if (componentTypeEnum == Asn1Acn::Types::Type::ASN1Type::BOOLEAN) {
            qDebug() << "type is bool";
        } else if (componentTypeEnum == Asn1Acn::Types::Type::ASN1Type::INTEGER) {
            qDebug() << "type is int";
        } else {
            throw std::runtime_error("Unknown component type");
        }
    }
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

} // namespace promela::translator
