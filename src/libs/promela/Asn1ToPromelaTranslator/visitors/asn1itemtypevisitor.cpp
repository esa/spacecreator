/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
 *
 * Thips library is free software; you can redistribute it and/or
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

#include "asn1itemtypevisitor.h"

#include "asn1sequencecomponentvisitor.h"
#include "enumeratedgenerator.h"
#include "enumvalueconstraintvisitor.h"
#include "integerrangeconstraintvisitor.h"
#include "proctypemaker.h"
#include "realrangeconstraintvisitor.h"
#include "sizeconstraintvisitor.h"

#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/labeltype.h>
#include <asn1library/asn1/types/null.h>
#include <asn1library/asn1/types/numericstring.h>
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <iostream>
#include <promela/PromelaModel/proctypeelement.h>

using Asn1Acn::BitStringValue;
using Asn1Acn::IntegerValue;
using Asn1Acn::OctetStringValue;
using Asn1Acn::StringValue;
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
using promela::model::ArrayType;
using promela::model::AssertCall;
using promela::model::Assignment;
using promela::model::BasicType;
using promela::model::BinaryExpression;
using promela::model::Constant;
using promela::model::DataType;
using promela::model::Declaration;
using promela::model::Expression;
using promela::model::ForLoop;
using promela::model::InlineCall;
using promela::model::InlineDef;
using promela::model::ProctypeElement;
using promela::model::PromelaModel;
using promela::model::Skip;
using promela::model::TypeAlias;
using promela::model::Utype;
using promela::model::UtypeRef;
using promela::model::ValueDefinition;
using promela::model::VariableRef;

namespace promela::translator {
Asn1ItemTypeVisitor::Asn1ItemTypeVisitor(PromelaModel &promelaModel, std::optional<QString> baseTypeName, QString name,
        bool generateInits, bool enhancedSpinSupport, int nestedIndexCounter)
    : m_promelaModel(promelaModel)
    , m_baseTypeName(std::move(baseTypeName))
    , m_name(std::move(name))
    , m_generateInits(generateInits)
    , m_enhancedSpinSupport(enhancedSpinSupport)
    , m_nestedIndexCounter(nestedIndexCounter)
{
}

const std::optional<model::DataType> &Asn1ItemTypeVisitor::getResultDataType() const noexcept
{
    return m_resultDataType;
}

void Asn1ItemTypeVisitor::visit(const Boolean &type)
{
    Q_UNUSED(type);
    const QString typeName = constructTypeName(m_name);
    m_promelaModel.addTypeAlias(TypeAlias(typeName, BasicType::BOOLEAN));

    addSimpleValueAssignmentInline(typeName);
    addBoolRangeCheckInline(type, typeName);

    if (m_generateInits) {
        addSimpleValueInitializationInline(typeName, Constant(0));
    }

    m_resultDataType = DataType(UtypeRef(typeName));
}

void Asn1ItemTypeVisitor::visit(const Null &type)
{
    Q_UNUSED(type);
    const QString typeName = constructTypeName(m_name);
    m_promelaModel.addTypeAlias(TypeAlias(typeName, BasicType::BIT));

    model::Sequence sequence(model::Sequence::Type::NORMAL);

    sequence.appendElement(Skip());

    addAssignValueInline(typeName, std::move(sequence));

    if (m_generateInits) {
        addEmptyValueInitializationInline(typeName);
    }

    m_resultDataType = DataType(UtypeRef(typeName));
}

void Asn1ItemTypeVisitor::visit(const BitString &type)
{
    SizeConstraintVisitor<BitStringValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);
    const auto isConstSize = constraintVisitor.getMinSize() == constraintVisitor.getMaxSize();

    const QString utypeName = constructTypeName(m_name);
    Utype utype = Utype(utypeName);

    utype.addField(Declaration(DataType(ArrayType(constraintVisitor.getMaxSize(), BasicType::BIT)), "data"));

    if (!isConstSize) {
        utype.addField(Declaration(DataType(BasicType::INT), "length"));
    }

    m_promelaModel.addUtype(utype);

    addSimpleArrayAssignInlineValue(utypeName, constraintVisitor.getMaxSize(), isConstSize);

    if (!isConstSize) {
        addSizeCheckInline(constraintVisitor.getMinSize(), constraintVisitor.getMaxSize(), utypeName);
    }

    if (m_generateInits) {
        addEmptyValueInitializationInline(utypeName);
    }

    m_resultDataType = DataType(UtypeRef(utypeName));
}

void Asn1ItemTypeVisitor::visit(const OctetString &type)
{
    SizeConstraintVisitor<OctetStringValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);
    const auto isConstSize = constraintVisitor.getMinSize() == constraintVisitor.getMaxSize();

    const QString utypeName = constructTypeName(m_name);
    Utype utype = Utype(utypeName);

    utype.addField(Declaration(DataType(ArrayType(constraintVisitor.getMaxSize(), BasicType::BYTE)), "data"));

    if (!isConstSize) {
        utype.addField(Declaration(DataType(BasicType::INT), "length"));
    }

    m_promelaModel.addUtype(utype);

    addSimpleArrayAssignInlineValue(utypeName, constraintVisitor.getMaxSize(), isConstSize);

    if (!isConstSize) {
        addSizeCheckInline(constraintVisitor.getMinSize(), constraintVisitor.getMaxSize(), utypeName);
    }

    if (m_generateInits) {
        addEmptyValueInitializationInline(utypeName);
    }

    m_resultDataType = DataType(UtypeRef(utypeName));
}

void Asn1ItemTypeVisitor::visit(const IA5String &type)
{
    SizeConstraintVisitor<StringValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);
    const auto isConstSize = constraintVisitor.getMinSize() == constraintVisitor.getMaxSize();

    const QString utypeName = constructTypeName(m_name);
    Utype utype = Utype(utypeName);

    utype.addField(Declaration(DataType(ArrayType(constraintVisitor.getMaxSize(), BasicType::BYTE)), "data"));

    if (!isConstSize) {
        utype.addField(Declaration(DataType(BasicType::INT), "length"));
    }

    m_promelaModel.addUtype(utype);

    addSimpleArrayAssignInlineValue(utypeName, constraintVisitor.getMaxSize(), isConstSize);

    if (!isConstSize) {
        addSizeCheckInline(constraintVisitor.getMinSize(), constraintVisitor.getMaxSize(), utypeName);
    }

    if (m_generateInits) {
        addEmptyValueInitializationInline(utypeName);
    }

    m_resultDataType = DataType(UtypeRef(utypeName));
}

void Asn1ItemTypeVisitor::visit(const NumericString &type)
{
    Q_UNUSED(type);
}

void Asn1ItemTypeVisitor::visit(const Enumerated &type)
{
    const QString typeName = constructTypeName(m_name);

    EnumeratedGenerator generator(typeName, type);

    while (generator.has_next()) {
        auto element = generator.next();
        m_promelaModel.addValueDefinition(ValueDefinition(element.first, element.second));
    }

    EnumValueConstraintVisitor visitor;
    type.constraints().accept(visitor);

    const auto &allowedValues = visitor.allowedValues();

    if (allowedValues.empty()) {
        auto errorMessage = QString("Unable to create enum range check for type %1, no value is allowed").arg(typeName);
        throw TranslationException(std::move(errorMessage));
    }

    m_promelaModel.addTypeAlias(TypeAlias(typeName, BasicType::INT));

    addSimpleValueAssignmentInline(typeName);
    addEnumRangeCheckInline(typeName, allowedValues);

    if (m_generateInits) {
        addSimpleValueInitializationInline(typeName, VariableRef(allowedValues.front()));
    }

    m_resultDataType = DataType(UtypeRef(typeName));
}

void Asn1ItemTypeVisitor::visit(const Choice &type)
{
    const QString utypeName = constructTypeName(m_name);
    Utype utype(utypeName);

    const QString nestedUtypeName = constructTypeName(QString("%1_data").arg(m_name));
    Utype nestedUtype(nestedUtypeName, m_enhancedSpinSupport);

    const QString none = QString("%1_NONE").arg(utypeName);
    m_promelaModel.addValueDefinition(ValueDefinition(none, 0));

    QString selectionTypeName = utypeName.toLower();
    selectionTypeName[0] = selectionTypeName[0].toUpper();

    model::Sequence sequence(model::Sequence::Type::NORMAL);

    utype.addField(Declaration(DataType(UtypeRef(nestedUtypeName)), "data"));
    utype.addField(Declaration(DataType(BasicType::INT), "selection"));
    m_promelaModel.addUtype(utype);

    {
        VariableRef dst("dst");
        dst.appendElement("selection");
        VariableRef src("src");
        src.appendElement("selection");
        sequence.appendElement(Assignment(dst, Expression(src)));
    }

    model::Conditional assignConditional;
    int32_t index = 1;
    for (const std::unique_ptr<Asn1Acn::Types::ChoiceAlternative> &component : type.components()) {
        Asn1ItemTypeVisitor nestedVisitor(
                m_promelaModel, utypeName, component->name(), false, m_enhancedSpinSupport, m_nestedIndexCounter + 1);
        component->type()->accept(nestedVisitor);
        std::optional<DataType> nestedDataType = nestedVisitor.getResultDataType();

        const QString componentName = Escaper::escapePromelaName(component->name());

        const QString fieldPresent = QString("%1_%2_PRESENT").arg(utypeName).arg(componentName);
        m_promelaModel.addValueDefinition(ValueDefinition(fieldPresent, index));
        const QString selectionPresent = QString("%1_selection_%2_PRESENT").arg(selectionTypeName).arg(componentName);
        m_promelaModel.addValueDefinition(ValueDefinition(selectionPresent, index));
        ++index;
        nestedUtype.addField(Declaration(nestedDataType.value(), componentName));

        auto assignSequence = std::make_unique<model::Sequence>(model::Sequence::Type::NORMAL);

        VariableRef currentSelection("dst");
        currentSelection.appendElement("selection");
        VariableRef componentSelection(fieldPresent);
        model::BinaryExpression assignCheckSelection(model::BinaryExpression::Operator::EQUAL,
                std::make_unique<Expression>(std::move(currentSelection)),
                std::make_unique<Expression>(std::move(componentSelection)));
        model::Expression assignCheckSelectionExpr(std::move(assignCheckSelection));

        assignSequence->appendElement(std::move(assignCheckSelectionExpr));

        VariableRef dst("dst");
        dst.appendElement("data");
        dst.appendElement(componentName);
        VariableRef src("src");
        src.appendElement("data");
        src.appendElement(componentName);
        const QString inlineName = getAssignValueInlineNameForNestedType(utypeName, componentName);
        QList<InlineCall::Argument> inlineArguments;
        inlineArguments.append(dst);
        inlineArguments.append(src);
        assignSequence->appendElement(InlineCall(inlineName, inlineArguments));

        assignConditional.appendAlternative(std::move(assignSequence));
    }

    auto assignSequenceElse = std::make_unique<model::Sequence>(model::Sequence::Type::NORMAL);
    model::Expression assignCheckElseExpr(VariableRef("else"));
    assignSequenceElse->appendElement(std::move(assignCheckElseExpr));
    assignSequenceElse->appendElement(Skip());
    assignConditional.appendAlternative(std::move(assignSequenceElse));

    sequence.appendElement(std::move(assignConditional));

    m_promelaModel.addUtype(nestedUtype);

    addAssignValueInline(utypeName, std::move(sequence));

    if (m_generateInits) {
        addEmptyValueInitializationInline(utypeName);
    }

    {
        model::Sequence seq(model::Sequence::Type::NORMAL);

        Assignment assignment(VariableRef("dst"), Expression(VariableRef("src")));
        seq.appendElement(std::move(assignment));

        addAssignValueInline(QString("%1_selection").arg(selectionTypeName), std::move(seq));
    }

    m_resultDataType = DataType(UtypeRef(utypeName));
}

void Asn1ItemTypeVisitor::visit(const Sequence &type)
{
    const QString nestedUtypeName = constructTypeName(m_name);
    Utype nestedUtype(nestedUtypeName);

    QList<QString> optionalFields;
    model::Sequence sequence(model::Sequence::Type::NORMAL);

    for (const std::unique_ptr<Asn1Acn::SequenceComponent> &component : type.components()) {
        Asn1SequenceComponentVisitor componentVisitor(
                m_promelaModel, nestedUtypeName, m_enhancedSpinSupport, m_nestedIndexCounter + 1);
        component->accept(componentVisitor);

        if (componentVisitor.wasComponentVisited()) {
            nestedUtype.addField(Declaration(componentVisitor.getComponentType(), componentVisitor.getComponentName()));

            if (componentVisitor.isComponentOptional()) {
                optionalFields.append(componentVisitor.getComponentName());
            }

            VariableRef dst("dst");
            dst.appendElement(componentVisitor.getComponentName());
            VariableRef src("src");
            src.appendElement(componentVisitor.getComponentName());

            const QString inlineName = getAssignValueInlineNameForNestedType(
                    nestedUtypeName, Escaper::escapePromelaName(componentVisitor.getComponentName()));
            QList<InlineCall::Argument> arguments;
            arguments.append(dst);
            arguments.append(src);
            sequence.appendElement(InlineCall(inlineName, arguments));
        }
    }

    if (!optionalFields.isEmpty()) {
        const QString existUtypeName = QString("%1_exist").arg(nestedUtypeName);
        Utype existUtype(existUtypeName);
        for (const QString &field : optionalFields) {
            existUtype.addField(Declaration(DataType(BasicType::BOOLEAN), field));
        }
        m_promelaModel.addUtype(existUtype);
        const QString existFieldName = "exist";
        nestedUtype.addField(Declaration(DataType(UtypeRef(existUtypeName)), existFieldName));

        for (const QString &field : optionalFields) {
            VariableRef dst("dst");
            dst.appendElement(existFieldName);
            dst.appendElement(field);
            VariableRef src("src");
            src.appendElement(existFieldName);
            src.appendElement(field);
            sequence.appendElement(Assignment(dst, Expression(src)));
        }
    }

    if (nestedUtype.getFields().isEmpty()) {
        nestedUtype.addField(Declaration(DataType(BasicType::BIT), "dummy"));
        sequence.appendElement(Skip());
    }

    m_promelaModel.addUtype(nestedUtype);

    addAssignValueInline(nestedUtypeName, std::move(sequence));

    if (m_generateInits) {
        addEmptyValueInitializationInline(nestedUtypeName);
    }

    m_resultDataType = DataType(UtypeRef(nestedUtypeName));
}

void Asn1ItemTypeVisitor::visit(const SequenceOf &type)
{
    SizeConstraintVisitor<IntegerValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);
    const auto isConstSize = constraintVisitor.getMinSize() == constraintVisitor.getMaxSize();

    const QString utypeName = constructTypeName(m_name);
    Utype utype = Utype(utypeName);

    Asn1ItemTypeVisitor itemTypeVisitor(
            m_promelaModel, utypeName, "elem", true, m_enhancedSpinSupport, m_nestedIndexCounter + 1);
    type.itemsType()->accept(itemTypeVisitor);
    DataType dataType = itemTypeVisitor.getResultDataType().value();

    if (dataType.isBasicType()) {
        utype.addField(
                Declaration(DataType(ArrayType(constraintVisitor.getMaxSize(), dataType.getBasicType())), "data"));
    } else if (dataType.isUtypeReference()) {
        utype.addField(
                Declaration(DataType(ArrayType(constraintVisitor.getMaxSize(), dataType.getUtypeReference())), "data"));
    }

    if (!isConstSize) {
        utype.addField(Declaration(DataType(BasicType::INT), "length"));
    }

    m_promelaModel.addUtype(utype);

    model::Sequence sequence(model::Sequence::Type::NORMAL);

    sequence.appendElement(Declaration(DataType(BasicType::INT), getIndexVariableName()));

    if (isConstSize) {
        auto loopRangeEnd = Constant(constraintVisitor.getMaxSize() - 1);

        auto dataLoop = createSequenceOfDataLoop(utypeName, Expression(std::move(loopRangeEnd)));
        sequence.appendElement(std::move(dataLoop));
    } else {
        auto srcLength = VariableRef("src");
        srcLength.appendElement("length");

        auto loopRangeEnd = BinaryExpression(BinaryExpression::Operator::SUBTRACT,
                std::make_unique<Expression>(srcLength), std::make_unique<Expression>(1));

        auto dataLoop = createSequenceOfDataLoop(utypeName, Expression(std::move(loopRangeEnd)));
        sequence.appendElement(std::move(dataLoop));

        auto zeroingLoop = createSequenceOfInitLoop(utypeName, constraintVisitor.getMaxSize() - 1);
        sequence.appendElement(std::move(zeroingLoop));
    }

    if (!isConstSize) {
        VariableRef dst_length = VariableRef("dst");
        dst_length.appendElement("length");
        VariableRef src_length = VariableRef("src");
        src_length.appendElement("length");
        sequence.appendElement(Assignment(dst_length, Expression(src_length)));
    }

    addAssignValueInline(utypeName, std::move(sequence));

    if (!isConstSize) {
        addSizeCheckInline(constraintVisitor.getMinSize(), constraintVisitor.getMaxSize(), utypeName);
    }

    if (m_generateInits) {
        addEmptyValueInitializationInline(utypeName);
    }

    m_resultDataType = DataType(UtypeRef(utypeName));
}

void Asn1ItemTypeVisitor::visit(const Real &type)
{
    const QString typeName = constructTypeName(m_name);

    RealRangeConstraintVisitor visitor;
    type.constraints().accept(visitor);

    const auto &rangeSubsets = visitor.getResultSubset();

    if (!rangeSubsets.has_value() || rangeSubsets->isEmpty()) {
        auto errorMessage = QString("Real type %1 allows for no value").arg(m_name);
        throw TranslationException(std::move(errorMessage));
    }

    if (m_enhancedSpinSupport) {
        m_promelaModel.addTypeAlias(TypeAlias(typeName, BasicType::FLOAT));
    } else {
        m_promelaModel.addTypeAlias(TypeAlias(typeName, BasicType::INT));
    }

    addSimpleValueAssignmentInline(typeName);
    addRealRangeCheckInline(typeName, *rangeSubsets);

    if (m_generateInits) {
        // TODO: Implement when floating points will be implemented
        addEmptyValueInitializationInline(typeName);
    }

    m_resultDataType = DataType(UtypeRef(typeName));
}

void Asn1ItemTypeVisitor::visit(const LabelType &type)
{
    Q_UNUSED(type);
}

void Asn1ItemTypeVisitor::visit(const Integer &type)
{
    const QString typeName = constructTypeName(m_name);

    IntegerRangeConstraintVisitor visitor;
    type.constraints().accept(visitor);

    const auto &rangeSubsets = visitor.getResultSubset();

    if (!rangeSubsets.has_value() || rangeSubsets->isEmpty()) {
        auto errorMessage = QString("Integer type %1 allows for no value").arg(m_name);
        throw TranslationException(std::move(errorMessage));
    }

    m_promelaModel.addTypeAlias(TypeAlias(typeName, BasicType::INT));

    addSimpleValueAssignmentInline(typeName);
    addIntegerRangeCheckInline(typeName, *rangeSubsets);

    if (m_generateInits) {
        addSimpleValueInitializationInline(typeName, Constant(*rangeSubsets->getMin()));
    }

    m_resultDataType = DataType(UtypeRef(typeName));
}

void Asn1ItemTypeVisitor::visit(const UserdefinedType &type)
{
    if (type.type()) {
        const auto name = m_name.isEmpty() ? type.typeName() : m_name;
        Asn1ItemTypeVisitor visitor(
                m_promelaModel, m_baseTypeName, name, m_generateInits, m_enhancedSpinSupport, m_nestedIndexCounter);
        type.type()->accept(visitor);

        m_resultDataType = visitor.getResultDataType();
    }
}

QString Asn1ItemTypeVisitor::constructTypeName(QString name)
{
    if (m_baseTypeName.has_value()) {
        return QString("%1_%2").arg(m_baseTypeName.value()).arg(Escaper::escapePromelaName(name));
    }
    return Escaper::escapePromelaName(std::move(name));
}

void Asn1ItemTypeVisitor::addSimpleValueAssignmentInline(const QString &typeName)
{
    model::Sequence sequence(model::Sequence::Type::NORMAL);

    Assignment assignment(VariableRef("dst"), Expression(VariableRef("src")));
    sequence.appendElement(std::move(assignment));

    QList<InlineCall::Argument> rangeCheckCallArguments;
    rangeCheckCallArguments.append(VariableRef("dst"));
    const auto rangeCheckInlineName =
            QString("%1%2").arg(Escaper::escapePromelaName(typeName)).arg(m_rangeCheckInlineSuffix);
    InlineCall rangeCheckCall(rangeCheckInlineName, std::move(rangeCheckCallArguments));
    sequence.appendElement(std::move(rangeCheckCall));

    addAssignValueInline(typeName, std::move(sequence));
}

void Asn1ItemTypeVisitor::addSimpleArrayAssignInlineValue(const QString &typeName, std::size_t length, bool isConstSize)
{
    model::Sequence sequence(model::Sequence::Type::NORMAL);

    if (!isConstSize) {
        QList<InlineCall::Argument> sizeCheckCallArguments;

        VariableRef src_length = VariableRef("src");
        src_length.appendElement("length");
        sizeCheckCallArguments.append(src_length);

        const auto sizeCheckInlineName =
                QString("%1%2").arg(Escaper::escapePromelaName(typeName)).arg(m_sizeCheckInlineSuffix);
        InlineCall sizeCheckCall(sizeCheckInlineName, std::move(sizeCheckCallArguments));
        sequence.appendElement(std::move(sizeCheckCall));
    }

    sequence.appendElement(Declaration(DataType(BasicType::INT), getIndexVariableName()));

    if (isConstSize) {
        auto loopRangeEnd = Constant(length - 1);

        auto dataLoop = createStringDataLoop(Expression(std::move(loopRangeEnd)));
        sequence.appendElement(std::move(dataLoop));
    } else {
        auto srcLength = VariableRef("src");
        srcLength.appendElement("length");

        auto loopRangeEnd = BinaryExpression(BinaryExpression::Operator::SUBTRACT,
                std::make_unique<Expression>(srcLength), std::make_unique<Expression>(1));

        auto dataLoop = createStringDataLoop(Expression(std::move(loopRangeEnd)));
        sequence.appendElement(std::move(dataLoop));

        auto zeroingLoop = createStringInitLoop(length - 1);
        sequence.appendElement(std::move(zeroingLoop));
    }

    if (!isConstSize) {
        VariableRef dst_length = VariableRef("dst");
        dst_length.appendElement("length");
        VariableRef src_length = VariableRef("src");
        src_length.appendElement("length");
        sequence.appendElement(Assignment(dst_length, Expression(src_length)));
    }

    addAssignValueInline(typeName, std::move(sequence));
}

void Asn1ItemTypeVisitor::addAssignValueInline(const QString &typeName, model::Sequence sequence)
{
    const QString assignValueInline =
            QString("%1%2").arg(Escaper::escapePromelaName(typeName)).arg(m_assignValueInlineSuffix);
    QList<QString> arguments;
    arguments.append("dst");
    arguments.append("src");

    m_promelaModel.addInlineDef(std::make_unique<InlineDef>(assignValueInline, arguments, std::move(sequence)));
}

void Asn1ItemTypeVisitor::addSimpleValueInitializationInline(const QString &typeName, InlineCall::Argument initValue)
{
    model::Sequence sequence(model::Sequence::Type::NORMAL);

    const QString assignValueInlineName =
            QString("%1%2").arg(Escaper::escapePromelaName(typeName)).arg(m_assignValueInlineSuffix);

    QList<InlineCall::Argument> assignValueInlineArguments;
    assignValueInlineArguments.append(VariableRef("dst"));
    assignValueInlineArguments.append(initValue);

    auto assignValueInlineCall = InlineCall(assignValueInlineName, assignValueInlineArguments);
    sequence.appendElement(std::move(assignValueInlineCall));

    addInitializeValueInline(typeName, std::move(sequence));
}

void Asn1ItemTypeVisitor::addEmptyValueInitializationInline(const QString &typeName)
{
    model::Sequence sequence(model::Sequence::Type::NORMAL);
    sequence.appendElement(Skip());

    addInitializeValueInline(typeName, std::move(sequence));
}

void Asn1ItemTypeVisitor::addInitializeValueInline(const QString &typeName, model::Sequence sequence)
{
    const auto initValueInlineName =
            QString("%1%2").arg(Escaper::escapePromelaName(typeName)).arg(m_initializeValueInlineSuffix);

    QList<QString> arguments;
    arguments.append("dst");

    auto initializeValueInlineDef = std::make_unique<InlineDef>(initValueInlineName, arguments, std::move(sequence));
    m_promelaModel.addInlineDef(std::move(initializeValueInlineDef));
}

void Asn1ItemTypeVisitor::addBoolRangeCheckInline(const Boolean &type, const QString &typeName)
{
    // TODO: Generating empty inline for now
    Q_UNUSED(type);
    addRangeCheckInline(Expression(VariableRef("true")), typeName);
}

void Asn1ItemTypeVisitor::addEnumRangeCheckInline(const QString &typeName, const std::vector<QString> &allowedValues)
{
    const auto argumentName = buildCheckArgumentName(typeName, "v");

    // Build one big expression for range check
    std::vector<BinaryExpression> valueCheckingExpressions;
    for (const auto &enumValue : allowedValues) {
        auto valueVar = std::make_unique<Expression>(VariableRef(argumentName));

        const auto enumValueName = QString("%1_%2").arg(typeName).arg(Escaper::escapePromelaName(enumValue));
        auto enumValueVar = std::make_unique<Expression>(VariableRef(enumValueName));

        auto equalExpr =
                BinaryExpression(BinaryExpression::Operator::EQUAL, std::move(valueVar), std::move(enumValueVar));
        valueCheckingExpressions.push_back(std::move(equalExpr));
    }

    auto valueCheckingExpression =
            std::accumulate(std::next(valueCheckingExpressions.begin()), valueCheckingExpressions.end(),
                    std::make_unique<Expression>(valueCheckingExpressions.front()), [&](auto &&acc, const auto &expr) {
                        return std::make_unique<Expression>(BinaryExpression(
                                BinaryExpression::Operator::OR, std::move(acc), std::make_unique<Expression>(expr)));
                    });

    addRangeCheckInline(*valueCheckingExpression, typeName);
}

void Asn1ItemTypeVisitor::addIntegerRangeCheckInline(const QString &typeName, const IntegerSubset &rangeSubsets)
{
    const auto argumentName = buildCheckArgumentName(typeName, "v");

    // Build one big expression for range check
    std::vector<BinaryExpression> rangeCheckingExpressions;
    for (const auto &range : rangeSubsets.getRanges()) {
        auto minValueConst = std::make_unique<Expression>(Constant(range.first));
        auto minValueVar = std::make_unique<Expression>(VariableRef(argumentName));
        auto greaterThanExpr = std::make_unique<Expression>(
                BinaryExpression(BinaryExpression::Operator::GEQUAL, std::move(minValueVar), std::move(minValueConst)));

        auto maxValueConst = std::make_unique<Expression>(Constant(range.second));
        auto maxValueVar = std::make_unique<Expression>(VariableRef(argumentName));
        auto lessThanExpr = std::make_unique<Expression>(
                BinaryExpression(BinaryExpression::Operator::LEQUAL, std::move(maxValueVar), std::move(maxValueConst)));

        BinaryExpression combinedExpr(
                BinaryExpression::Operator::AND, std::move(greaterThanExpr), std::move(lessThanExpr));
        rangeCheckingExpressions.push_back(std::move(combinedExpr));
    }

    auto rangeCheckingExpression =
            std::accumulate(std::next(rangeCheckingExpressions.begin()), rangeCheckingExpressions.end(),
                    std::make_unique<Expression>(rangeCheckingExpressions.front()), [&](auto &&acc, const auto &expr) {
                        return std::make_unique<Expression>(BinaryExpression(
                                BinaryExpression::Operator::OR, std::move(acc), std::make_unique<Expression>(expr)));
                    });

    addRangeCheckInline(*rangeCheckingExpression, typeName);
}

void Asn1ItemTypeVisitor::addRealRangeCheckInline(const QString &typeName, const RealSubset &rangeSubsets)
{
    const auto argumentName = buildCheckArgumentName(typeName, "v");

    // Build one big expression for range check
    std::vector<BinaryExpression> rangeCheckingExpressions;
    for (const auto &range : rangeSubsets.getRanges()) {
        auto minValueConst = std::make_unique<Expression>(Constant(range.first));
        auto minValueVar = std::make_unique<Expression>(VariableRef(argumentName));
        auto greaterThanExpr = std::make_unique<Expression>(
                BinaryExpression(BinaryExpression::Operator::GEQUAL, std::move(minValueVar), std::move(minValueConst)));

        auto maxValueConst = std::make_unique<Expression>(Constant(range.second));
        auto maxValueVar = std::make_unique<Expression>(VariableRef(argumentName));
        auto lessThanExpr = std::make_unique<Expression>(
                BinaryExpression(BinaryExpression::Operator::LEQUAL, std::move(maxValueVar), std::move(maxValueConst)));

        BinaryExpression combinedExpr(
                BinaryExpression::Operator::AND, std::move(greaterThanExpr), std::move(lessThanExpr));
        rangeCheckingExpressions.push_back(std::move(combinedExpr));
    }

    auto rangeCheckingExpression =
            std::accumulate(std::next(rangeCheckingExpressions.begin()), rangeCheckingExpressions.end(),
                    std::make_unique<Expression>(rangeCheckingExpressions.front()), [&](auto &&acc, const auto &expr) {
                        return std::make_unique<Expression>(BinaryExpression(
                                BinaryExpression::Operator::OR, std::move(acc), std::make_unique<Expression>(expr)));
                    });

    addRangeCheckInline(*rangeCheckingExpression, typeName);
}

void Asn1ItemTypeVisitor::addRangeCheckInline(const Expression &expression, const QString &typeName)
{
    const auto inlineName = QString("%1%2").arg(Escaper::escapePromelaName(typeName)).arg(m_rangeCheckInlineSuffix);
    QList<QString> arguments;
    const auto argumentName = buildCheckArgumentName(typeName, "v");
    arguments.append(argumentName);

    model::Sequence sequence(model::Sequence::Type::NORMAL);

    AssertCall assertCall(expression);
    sequence.appendElement(std::move(assertCall));

    auto rangeCheckInline = std::make_unique<InlineDef>(inlineName, arguments, std::move(sequence));
    m_promelaModel.addInlineDef(std::move(rangeCheckInline));
}

void Asn1ItemTypeVisitor::addSizeCheckInline(
        const std::size_t minValue, const std::size_t maxValue, const QString &typeName)
{
    const auto inlineName = QString("%1%2").arg(Escaper::escapePromelaName(typeName)).arg(m_sizeCheckInlineSuffix);
    QList<QString> arguments;
    const auto argumentName = buildCheckArgumentName(typeName, "s");
    arguments.append(argumentName);

    model::Sequence sequence(model::Sequence::Type::NORMAL);

    auto minValueConst = std::make_unique<Expression>(Constant(minValue));
    auto minValueVar = std::make_unique<Expression>(VariableRef(argumentName));
    auto greaterThanExpr = std::make_unique<Expression>(
            BinaryExpression(BinaryExpression::Operator::GEQUAL, std::move(minValueVar), std::move(minValueConst)));

    auto maxValueConst = std::make_unique<Expression>(Constant(maxValue));
    auto maxValueVar = std::make_unique<Expression>(VariableRef(argumentName));
    auto lessThanExpr = std::make_unique<Expression>(
            BinaryExpression(BinaryExpression::Operator::LEQUAL, std::move(maxValueVar), std::move(maxValueConst)));

    BinaryExpression sizeCheckingExpression(
            BinaryExpression::Operator::AND, std::move(greaterThanExpr), std::move(lessThanExpr));

    AssertCall assertCall(Expression(std::move(sizeCheckingExpression)));
    sequence.appendElement(std::move(assertCall));

    auto rangeCheckInline = std::make_unique<InlineDef>(inlineName, arguments, std::move(sequence));
    m_promelaModel.addInlineDef(std::move(rangeCheckInline));
}

QString Asn1ItemTypeVisitor::getAssignValueInlineNameForNestedType(const QString &utype, const QString &field) const
{
    return Escaper::escapePromelaName(utype) + "_" + Escaper::escapePromelaName(field) + m_assignValueInlineSuffix;
}

QString Asn1ItemTypeVisitor::buildCheckArgumentName(const QString &typeName, const QString &postfix) const
{
    return QString("%1_%2c").arg(typeName).arg(postfix);
}

QString Asn1ItemTypeVisitor::getIndexVariableName() const
{
    return QString("i_%1").arg(m_nestedIndexCounter);
}

ForLoop Asn1ItemTypeVisitor::createSequenceOfDataLoop(const QString &utypeName, Expression loopRangeEnd) const
{
    auto loopSequence = std::make_unique<model::Sequence>(model::Sequence::Type::NORMAL);

    VariableRef dst("dst");
    dst.appendElement("data", std::make_unique<Expression>(VariableRef(getIndexVariableName())));
    VariableRef src("src");
    src.appendElement("data", std::make_unique<Expression>(VariableRef(getIndexVariableName())));

    QList<InlineCall::Argument> inlineArguments;
    inlineArguments.append(dst);
    inlineArguments.append(src);

    const auto inlineName = QString("%1_elem%2").arg(utypeName).arg(m_assignValueInlineSuffix);
    auto inlineCall = InlineCall(inlineName, inlineArguments);
    loopSequence->appendElement(std::move(inlineCall));

    auto forLoop = ForLoop(VariableRef(getIndexVariableName()), 0, std::move(loopRangeEnd), std::move(loopSequence));

    return forLoop;
}

ForLoop Asn1ItemTypeVisitor::createStringDataLoop(Expression loopRangeEnd) const
{
    auto loopSequence = std::make_unique<model::Sequence>(model::Sequence::Type::NORMAL);

    VariableRef dst("dst");
    dst.appendElement("data", std::make_unique<Expression>(VariableRef(getIndexVariableName())));
    VariableRef src("src");
    src.appendElement("data", std::make_unique<Expression>(VariableRef(getIndexVariableName())));

    loopSequence->appendElement(Assignment(dst, Expression(src)));

    auto forLoop = ForLoop(VariableRef(getIndexVariableName()), 0, std::move(loopRangeEnd), std::move(loopSequence));

    return forLoop;
}

ForLoop Asn1ItemTypeVisitor::createSequenceOfInitLoop(const QString &utypeName, const std::size_t loopRangeEnd) const
{
    auto loopSequence = std::make_unique<model::Sequence>(model::Sequence::Type::NORMAL);

    VariableRef dst("dst");
    dst.appendElement("data", std::make_unique<Expression>(VariableRef(getIndexVariableName())));
    VariableRef srcLength = VariableRef("src");
    srcLength.appendElement("length");

    QList<InlineCall::Argument> inlineArguments;
    inlineArguments.append(dst);

    const auto inlineName = QString("%1_elem%2").arg(utypeName).arg(m_initializeValueInlineSuffix);
    auto inlineCall = InlineCall(inlineName, inlineArguments);
    loopSequence->appendElement(std::move(inlineCall));

    auto forLoop =
            ForLoop(VariableRef(getIndexVariableName()), Expression(srcLength), loopRangeEnd, std::move(loopSequence));

    return forLoop;
}

ForLoop Asn1ItemTypeVisitor::createStringInitLoop(const std::size_t loopRangeEnd) const
{
    auto loopSequence = std::make_unique<model::Sequence>(model::Sequence::Type::NORMAL);

    VariableRef dst("dst");
    dst.appendElement("data", std::make_unique<Expression>(VariableRef(getIndexVariableName())));
    VariableRef srcLength = VariableRef("src");
    srcLength.appendElement("length");

    loopSequence->appendElement(Assignment(dst, Expression(Constant(0))));

    auto forLoop =
            ForLoop(VariableRef(getIndexVariableName()), Expression(srcLength), loopRangeEnd, std::move(loopSequence));

    return forLoop;
}

}
