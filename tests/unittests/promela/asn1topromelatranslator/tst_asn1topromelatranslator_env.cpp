/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 - 2022 N7 Space Sp. z o.o.
 *`
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

#include "tst_asn1topromelatranslator_env.h"

#include "asn1model.h"
#include "assignment.h"
#include "binaryexpression.h"
#include "conditional.h"
#include "conversion/asn1/Asn1Exporter/exporter.h"
#include "conversion/asn1/Asn1Importer/importer.h"
#include "conversion/asn1/Asn1Options/options.h"
#include "declaration.h"
#include "definitions.h"
#include "expression.h"
#include "file.h"
#include "inlinecall.h"
#include "options.h"
#include "promela/PromelaExporter/promelaexporter.h"
#include "promela/PromelaOptions/options.h"
#include "sequencecomponent.h"
#include "typeassignment.h"
#include "types/type.h"
#include "types/typereadingvisitor.h"
#include "types/userdefinedtype.h"
#include "variableref.h"

#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/values.h>
#include <memory>
#include <modelloader.h>
#include <promela/Asn1ToPromelaTranslator/visitors/asn1nodevaluegeneratorvisitor.h>
#include <promela/PromelaModel/constant.h>
#include <promela/PromelaModel/promelamodel.h>
#include <promela/PromelaModel/sequence.h>
#include <qtestcase.h>
#include <qvariant.h>
#include <utility>
#include <variant>

using Asn1Acn::AsnSequenceComponent;
using Asn1Acn::IntegerValue;
using Asn1Acn::SourceLocation;
using Asn1Acn::TypeAssignment;
using Asn1Acn::Constraints::RangeConstraint;
using Asn1Acn::Types::Boolean;
using Asn1Acn::Types::Enumerated;
using Asn1Acn::Types::EnumeratedItem;
using Asn1Acn::Types::Integer;
using promela::model::Assignment;
using promela::model::BinaryExpression;
using promela::model::Conditional;
using promela::model::Constant;
using promela::model::Declaration;
using promela::model::Expression;
using promela::model::InlineCall;
using promela::model::InlineDef;
using promela::model::PromelaModel;
using promela::model::Sequence;
using promela::model::VariableRef;
using promela::translator::Asn1NodeValueGeneratorVisitor;

namespace tmc::test {

void tst_Asn1ToPromelaTranslator_Env::initTestCase() {}

void tst_Asn1ToPromelaTranslator_Env::cleanupTestCase() {}

void tst_Asn1ToPromelaTranslator_Env::testBoolean() const
{
    auto asn1Model = createModel();

    {
        auto booleanType = std::make_unique<Boolean>();
        auto myBooleanAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyBoolean"), QStringLiteral("MyBoolean"), SourceLocation(), std::move(booleanType));

        asn1Model->addType(std::move(myBooleanAssignment));
    }

    PromelaModel promelaModel;
    const QStringList typesToTranslate = { "MyBoolean" };
    Asn1NodeValueGeneratorVisitor visitor(promelaModel, typesToTranslate);
    visitor.visit(*asn1Model);

    QCOMPARE(promelaModel.getInlineDefs().size(), 1);

    const std::unique_ptr<InlineDef> &inlineDef = promelaModel.getInlineDefs().front();

    QCOMPARE(inlineDef->getName(), "MyBoolean_generate_value");
    QCOMPARE(inlineDef->getArguments().size(), 1);
    const QString &argName = inlineDef->getArguments().front();

    const Sequence &mainSequence = inlineDef->getSequence();
    QCOMPARE(mainSequence.getContent().size(), 1);

    QVERIFY(std::holds_alternative<Conditional>(mainSequence.getContent().front()->getValue()));

    const Conditional &ifStatement = std::get<Conditional>(mainSequence.getContent().front()->getValue());

    const std::list<bool> possibleValues = { true, false };

    std::list<bool> valuesFound;
    for (const auto &sequence : ifStatement.getAlternatives()) {
        QCOMPARE(sequence->getContent().size(), 2);

        for (const auto &content : sequence->getContent()) {
            const auto &contentValue = content->getValue();
            if (!std::holds_alternative<Assignment>(contentValue)) {
                continue;
            }

            const Assignment &assignment = std::get<Assignment>(contentValue);
            const std::list<VariableRef::Element> &assignmentElements = assignment.getVariableRef().getElements();
            QCOMPARE(assignmentElements.size(), 1);
            QCOMPARE(assignmentElements.front().m_name, argName);
            QVERIFY(assignmentElements.front().m_index.get() == nullptr);

            const Constant &constant = std::get<Constant>(assignment.getExpression().getContent());
            valuesFound.push_back(static_cast<bool>(constant.getValue()));
        }
    }

    std::for_each(possibleValues.begin(), possibleValues.end(), [&valuesFound](const bool &value) {
        if (std::find(valuesFound.begin(), valuesFound.end(), value) == valuesFound.end()) {
            QFAIL(QString("Value '%1' not found").arg(value).toStdString().c_str());
        }
    });
}

void tst_Asn1ToPromelaTranslator_Env::testInteger() const
{
    auto model = createModel();

    {
        auto integerType = std::make_unique<Integer>();
        auto rangeConstraint = RangeConstraint<IntegerValue>::create({ 0, 3 });
        integerType->constraints().append(std::move(rangeConstraint));

        auto myIntegerAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyInteger"), QStringLiteral("MyInteger"), SourceLocation(), std::move(integerType));
        model->addType(std::move(myIntegerAssignment));
    }

    PromelaModel promelaModel;
    QStringList typesToTranslate;
    typesToTranslate.append(QString("MyInteger"));
    Asn1NodeValueGeneratorVisitor visitor(promelaModel, typesToTranslate);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getInlineDefs().size(), 1);

    const std::unique_ptr<InlineDef> &inlineDef = promelaModel.getInlineDefs().front();

    QCOMPARE(inlineDef->getName(), "MyInteger_generate_value");
    QCOMPARE(inlineDef->getArguments().size(), 1);
    const QString &argName = inlineDef->getArguments().front();

    const Sequence &mainSequence = inlineDef->getSequence();
    QCOMPARE(mainSequence.getContent().size(), 1);

    QVERIFY(std::holds_alternative<Conditional>(mainSequence.getContent().front()->getValue()));

    const Conditional &ifStatement = std::get<Conditional>(mainSequence.getContent().front()->getValue());

    auto iter = ifStatement.getAlternatives().begin();
    for (int i = 0; i < 4; ++i) {
        QVERIFY(iter != ifStatement.getAlternatives().end());
        const std::unique_ptr<Sequence> &nestedSequence = *iter;
        ++iter;
        QCOMPARE(nestedSequence->getContent().size(), 2);

        QVERIFY(std::holds_alternative<Assignment>(nestedSequence->getContent().back()->getValue()));
        const Assignment &assignment = std::get<Assignment>(nestedSequence->getContent().back()->getValue());
        const VariableRef &variableRef = assignment.getVariableRef();

        const std::list<VariableRef::Element> &variableRefElements = variableRef.getElements();
        QCOMPARE(variableRefElements.size(), 1);
        QCOMPARE(variableRefElements.front().m_name, argName);
        QVERIFY(variableRefElements.front().m_index.get() == nullptr);

        QVERIFY(std::holds_alternative<Constant>(assignment.getExpression().getContent()));
        const Constant &constant = std::get<Constant>(assignment.getExpression().getContent());
        QCOMPARE(constant.getValue(), i);
    }
}

void tst_Asn1ToPromelaTranslator_Env::testEnumerated() const
{
    auto model = createModel();
    std::unique_ptr<Enumerated> type = std::make_unique<Enumerated>();

    type->addItem(EnumeratedItem(0, "zero", 0));
    type->addItem(EnumeratedItem(1, "one", 1));
    type->addItem(EnumeratedItem(2, "infinity", 100));

    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    QStringList typesToTranslate;
    typesToTranslate.append(QString("MyType"));
    Asn1NodeValueGeneratorVisitor visitor(promelaModel, typesToTranslate);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getInlineDefs().size(), 1);

    const std::unique_ptr<InlineDef> &inlineDef = promelaModel.getInlineDefs().front();

    QCOMPARE(inlineDef->getName(), "MyType_generate_value");
    QCOMPARE(inlineDef->getArguments().size(), 1);
    const QString &argName = inlineDef->getArguments().front();

    const Sequence &mainSequence = inlineDef->getSequence();
    QCOMPARE(mainSequence.getContent().size(), 1);

    QVERIFY(std::holds_alternative<Conditional>(mainSequence.getContent().front()->getValue()));

    const Conditional &ifStatement = std::get<Conditional>(mainSequence.getContent().front()->getValue());

    auto iter = ifStatement.getAlternatives().begin();
    QStringList expectedValues;
    expectedValues.append("MyType_zero");
    expectedValues.append("MyType_one");
    expectedValues.append("MyType_infinity");
    for (const auto &element : expectedValues) {
        QVERIFY(iter != ifStatement.getAlternatives().end());
        const std::unique_ptr<Sequence> &nestedSequence = *iter;
        ++iter;
        QCOMPARE(nestedSequence->getContent().size(), 2);

        QVERIFY(std::holds_alternative<Assignment>(nestedSequence->getContent().back()->getValue()));
        const Assignment &assignment = std::get<Assignment>(nestedSequence->getContent().back()->getValue());
        QCOMPARE(assignment.getVariableRef().getElements().size(), 1);
        QCOMPARE(assignment.getVariableRef().getElements().front().m_name, argName);
        QVERIFY(assignment.getVariableRef().getElements().front().m_index.get() == nullptr);
        QVERIFY(std::holds_alternative<VariableRef>(assignment.getExpression().getContent()));
        const VariableRef &ref = std::get<VariableRef>(assignment.getExpression().getContent());
        QCOMPARE(ref.getElements().size(), 1);
        QCOMPARE(ref.getElements().front().m_name, element);
        QVERIFY(ref.getElements().front().m_index.get() == nullptr);
    }
}

void exportPromelaModel(const PromelaModel &model, const QString &filename)
{
    conversion::Options options;
    options.add(conversion::promela::PromelaOptions::outputFilepath, filename);
    promela::exporter::PromelaExporter exporter;
    exporter.exportModel(&model, options);
}

std::unique_ptr<Asn1Acn::Asn1Model> makeModelFromDefinitions(
        std::unique_ptr<Asn1Acn::Definitions> asnModel, const QString &name)
{
    Asn1Acn::File file(name);
    file.add(std::move(asnModel));

    std::vector<std::unique_ptr<Asn1Acn::File>> files;
    auto filePtr = std::make_unique<Asn1Acn::File>(file);
    files.push_back(std::move(filePtr));

    return std::make_unique<Asn1Acn::Asn1Model>(std::move(files));
}

void exportAsnModel(const Asn1Acn::Asn1Model *const model)
{
    conversion::Options options;
    options.add(conversion::asn1::Asn1Options::asn1FilepathPrefix, "asn_");
    conversion::asn1::exporter::Asn1Exporter exporter;
    exporter.exportModel(model, options);
}

void checkAlternativeContent(const promela::model::ProctypeElement::Value &contentValue)
{
    if (std::holds_alternative<Expression>(contentValue)) {
        const auto &expression = std::get<Expression>(contentValue);
        const auto &declaration = expression.getContent();
        const auto &varRef = std::get<VariableRef>(declaration);
        for (const auto &el : varRef.getElements()) {
            // printing expressions is recursive
            if (el.m_index != nullptr) {
                const auto &content = el.m_index->getContent();
                if (std::holds_alternative<VariableRef>(content)) {
                    // auto &elContent = std::get<VariableRef>(content);
                    // (void)elContent;
                    // TODO
                } else if (std::holds_alternative<Constant>(content)) {
                    // TODO
                } else if (std::holds_alternative<BinaryExpression>(content)) {
                    // TODO
                } else if (std::holds_alternative<InlineCall>(content)) {
                    // TODO
                }
            }
        }
    }
}

void checkConditional(const Conditional &conditional)
{
    for (const auto &alternative : conditional.getAlternatives()) {
        for (const auto &content : alternative->getContent()) {
            checkAlternativeContent(content->getValue());
        }
    }
}

void checkSequence(const Sequence &sequence, const int expectedContentSize = 1)
{
    QCOMPARE(sequence.getType(), Sequence::Type::NORMAL);
    const auto &sequenceContent = sequence.getContent();
    QCOMPARE(sequenceContent.size(), expectedContentSize);

    for (const auto &content : sequenceContent) {
        const auto &contentValue = content->getValue();

        const bool contentIsConditional = std::holds_alternative<Conditional>(contentValue);
        const bool contentIsSequence = std::holds_alternative<Sequence>(contentValue);
        const bool contentIsInlineCall = std::holds_alternative<InlineCall>(contentValue);

        if (!contentIsConditional && !contentIsSequence && !contentIsInlineCall) {
            QFAIL("Unknown content type in sequence");
        }

        if (contentIsConditional) {
            const auto &conditional = std::get<Conditional>(contentValue);
            checkConditional(conditional);
        } else if (contentIsSequence) {
            const auto &nestedSequence = std::get<Sequence>(contentValue);
            // checkSequence(sequence);
            (void)nestedSequence;
        } else if (contentIsInlineCall) {
            const auto &inlineCall = std::get<InlineCall>(contentValue);
            // checkInlineCall()
            (void)inlineCall;
        }
    }
}

void checkInlineDefinition(InlineDef *const definition, Asn1Acn::TypeAssignment *const typeAssignment,
        const QString &expectedDefinitionName)
{
    QVERIFY(definition != nullptr);
    QVERIFY(typeAssignment != nullptr);

    QCOMPARE(definition->getName(), expectedDefinitionName);

    const auto &definitionArguments = definition->getArguments();
    QCOMPARE(definitionArguments.size(), 1);
    const auto &argument = definitionArguments.front();
    QCOMPARE(argument, "value");

    const Sequence &mainSequence = definition->getSequence();
    if (typeAssignment->typeEnum() == Asn1Acn::Types::Type::SEQUENCE) {
        auto *const parentNode = static_cast<Asn1Acn::Definitions *>(typeAssignment->parent());
        QVERIFY(parentNode);
        checkSequence(mainSequence, parentNode->types().size());
    } else {
        checkSequence(mainSequence);
    }
}

void tst_Asn1ToPromelaTranslator_Env::testSequence() const
{
    auto asnModel = plugincommon::ModelLoader::loadAsn1Model("resources/myModule.asn");
    QVERIFY(asnModel != nullptr);
    QVERIFY(!asnModel->data().empty());

    PromelaModel promelaModel;
    const QStringList typesToTranslate = { "EnvIntParam", "EnvBoolParam", "EnvParamSeq" };
    Asn1NodeValueGeneratorVisitor visitor(promelaModel, typesToTranslate);
    QVERIFY(!asnModel->data().empty());
    const auto &asnFile = *asnModel->data().front();
    visitor.visit(asnFile);
    exportPromelaModel(promelaModel, "sequence.pr"); // TODO: this line shall be removed

    const auto &asnDefinitionsList = asnFile.definitionsList();
    const auto &promelaInlineDefs = promelaModel.getInlineDefs();
    QCOMPARE(promelaInlineDefs.size(), asnDefinitionsList.front()->types().size());
    // QCOMPARE(promelaInlineDefs.size(), asnDefinitionsList.front()->typeAssignmentNames().size());
    const int defsSize = promelaInlineDefs.size();

    const QStringList expectedDefNames = {
        "EnvIntParam_generate_value",
        "EnvBoolParam_generate_value",
        "EnvParamSeq_generate_value",
    };
    for (int i = 0; i < defsSize; i++) {
        if (std::next(promelaInlineDefs.begin(), i) == promelaInlineDefs.end() //
                || asnDefinitionsList.at(0) == nullptr) {
            break;
        }
        auto *const inlineDefPtr = (*std::next(promelaInlineDefs.begin(), i)).get();
        auto *const asnTypeAssignmentPtr = asnDefinitionsList.at(0)->types().at(i).get();

        checkInlineDefinition(inlineDefPtr, asnTypeAssignmentPtr, expectedDefNames.at(i));
    }
}

unsigned int countNestedDefinitionsInAsnSequences(const Asn1Acn::File &asnFile)
{
    unsigned int nestedDefinitions = 0;

    for (const auto &definition : asnFile.definitionsList()) {
        for (const auto &type : definition->types()) {
            const auto &typeType = type->type();
            if (typeType->typeEnum() == Asn1Acn::Types::Type::SEQUENCE) {
                const auto &typeTypeSeq = reinterpret_cast<Asn1Acn::Types::Sequence *>(typeType);
                nestedDefinitions += typeTypeSeq->components().size();
            }
        }
    }

    return nestedDefinitions;
}

void tst_Asn1ToPromelaTranslator_Env::testSequenceEmbeddedType() const
{
    auto asnModel = plugincommon::ModelLoader::loadAsn1Model("resources/myModule_typesInSeq.asn");
    QVERIFY(asnModel != nullptr);
    QVERIFY(!asnModel->data().empty());

    PromelaModel promelaModel;
    const QStringList typesToTranslate = { "EnvParamSeq" };
    Asn1NodeValueGeneratorVisitor visitor(promelaModel, typesToTranslate);
    QVERIFY(!asnModel->data().empty());
    const auto &asnFile = *asnModel->data().front();
    visitor.visit(asnFile);
    exportPromelaModel(promelaModel, "sequence-embedded.pr"); // TODO: this line shall be removed

    const auto &asnDefinitionsList = asnFile.definitionsList();
    const auto &promelaInlineDefs = promelaModel.getInlineDefs();
    const unsigned int promelaInlineDefinitionsNumber = promelaInlineDefs.size();
    const unsigned int asnDefinitionsNumber =
            asnDefinitionsList.front()->typeAssignmentNames().size() + countNestedDefinitionsInAsnSequences(asnFile);
    QCOMPARE(promelaInlineDefinitionsNumber, asnDefinitionsNumber);
    const int defsSize = promelaInlineDefs.size();

    const QStringList expectedDefNames = {
        "EnvParamSeq_x_generate_value",
        "EnvParamSeq_y_generate_value",
        "EnvParamSeq_z_generate_value",
        "EnvParamSeq_generate_value",
    };

    auto *const inlineDefPtr = promelaInlineDefs.begin()->get();

    const auto &asnDefinition = asnDefinitionsList.at(0);
    const auto &types = asnDefinition->types();
    const auto &type = types.at(0);
    auto *const asnTypeAssignmentPtr = type.get();

    // todo: ta funkcja powinna wyluskiwac skladowe z asn-owego typu zlozonego
    checkInlineDefinition(inlineDefPtr, asnTypeAssignmentPtr, expectedDefNames.at(0));
}

std::unique_ptr<Definitions> tst_Asn1ToPromelaTranslator_Env::createModel() const
{
    return std::make_unique<Definitions>("myModule", SourceLocation());
}

} // namespace tmc::test
