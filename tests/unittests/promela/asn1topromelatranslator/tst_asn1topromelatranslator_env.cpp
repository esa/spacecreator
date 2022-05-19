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
#include "visitors/asn1constraintvisitor.h"

#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/values.h>
#include <common/textcheckerandconsumer/textcheckerandconsumer.h>
#include <exception>
#include <memory>
#include <modelloader.h>
#include <promela/Asn1ToPromelaTranslator/visitors/asn1nodevaluegeneratorvisitor.h>
#include <promela/PromelaModel/constant.h>
#include <promela/PromelaModel/promelamodel.h>
#include <promela/PromelaModel/sequence.h>
#include <qdir.h>
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
using tests::common::TextCheckerAndConsumer;

namespace tmc::test {

static auto exportPromelaModel(const PromelaModel &model, const QString &filename) -> void;
static auto translateAsnToPromela(const QString &inputAsnFilename, const QStringList &asnTypesToTranslate,
        const QString &actualOutputFilename) -> void;
static auto compareTextFiles(const QString &actualOutputFilename, const QString &expectedOutputFilename) -> void;

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

void tst_Asn1ToPromelaTranslator_Env::testSequence() const
{
    const QString inputAsnFilename = "sequence.asn";
    const QStringList asnTypesToTranslate = { "EnvIntParam", "EnvParamSeq" };
    const QString actualOutputFilename = "sequence.pml";
    const QString expectedOutputFilename = QString("%1.out").arg(actualOutputFilename);

    translateAsnToPromela(inputAsnFilename, asnTypesToTranslate, actualOutputFilename);
    compareTextFiles(actualOutputFilename, expectedOutputFilename);
}

void tst_Asn1ToPromelaTranslator_Env::testSequenceEmbeddedType() const
{
    const QString inputAsnFilename = "sequence-embedded.asn";
    const QStringList asnTypesToTranslate = { "EnvParamSeq" };
    const QString actualOutputFilename = "sequence-embedded.pml";
    const QString expectedOutputFilename = QString("%1.out").arg(actualOutputFilename);

    translateAsnToPromela(inputAsnFilename, asnTypesToTranslate, actualOutputFilename);
    compareTextFiles(actualOutputFilename, expectedOutputFilename);
}

void tst_Asn1ToPromelaTranslator_Env::testSequenceOf() const
{
    const QString inputAsnFilename = "sequenceof.asn";
    const QStringList asnTypesToTranslate = { "SimpleFixedSizeSequenceOf" };
    const QString actualOutputFilename = "sequenceof.pml";
    const QString expectedOutputFilename = QString("%1.out").arg(actualOutputFilename);

    translateAsnToPromela(inputAsnFilename, asnTypesToTranslate, actualOutputFilename);
    compareTextFiles(actualOutputFilename, expectedOutputFilename);
}

void tst_Asn1ToPromelaTranslator_Env::testSequenceOfVariableSize() const
{
    const QString inputAsnFilename = "sequenceof-variable-size.asn";
    const QStringList asnTypesToTranslate = { "SimpleVariableSizeSequenceOf" };
    const QString actualOutputFilename = "sequenceof-variable-size.pml";
    const QString expectedOutputFilename = QString("%1.out").arg(actualOutputFilename);

    translateAsnToPromela(inputAsnFilename, asnTypesToTranslate, actualOutputFilename);
    compareTextFiles(actualOutputFilename, expectedOutputFilename);
}

void tst_Asn1ToPromelaTranslator_Env::testChoice() const
{
    const QString inputAsnFilename = "choice.asn";
    const QStringList asnTypesToTranslate = { "SimpleChoice" };
    const QString actualOutputFilename = "choice.pml";
    const QString expectedOutputFilename = QString("%1.out").arg(actualOutputFilename);

    translateAsnToPromela(inputAsnFilename, asnTypesToTranslate, actualOutputFilename);
    compareTextFiles(actualOutputFilename, expectedOutputFilename);
}

void tst_Asn1ToPromelaTranslator_Env::testChoiceAnonymous() const
{
    const QString inputAsnFilename = "choice-anonymous.asn";
    const QStringList asnTypesToTranslate = {
        "SimpleChoiceWithAnonymousTypes",
    };
    const QString actualOutputFilename = "choice-anonymous.pml";
    const QString expectedOutputFilename = QString("%1.out").arg(actualOutputFilename);

    translateAsnToPromela(inputAsnFilename, asnTypesToTranslate, actualOutputFilename);
    compareTextFiles(actualOutputFilename, expectedOutputFilename);
}

std::unique_ptr<Definitions> tst_Asn1ToPromelaTranslator_Env::createModel() const
{
    return std::make_unique<Definitions>("myModule", SourceLocation());
}

void exportPromelaModel(const PromelaModel &model, const QString &filename)
{
    conversion::Options options;
    options.add(conversion::promela::PromelaOptions::outputFilepath, filename);
    promela::exporter::PromelaExporter exporter;
    exporter.exportModel(&model, options);
}

void translateAsnToPromela(
        const QString &inputAsnFilename, const QStringList &asnTypesToTranslate, const QString &actualOutputFilename)
{
    auto asnModel = plugincommon::ModelLoader::loadAsn1Model(
            QString("resources%1%2").arg(QDir::separator()).arg(inputAsnFilename));
    QVERIFY(asnModel != nullptr);
    QVERIFY(!asnModel->data().empty());

    PromelaModel promelaModel;
    Asn1NodeValueGeneratorVisitor visitor(promelaModel, asnTypesToTranslate);
    const auto &asnFile = *asnModel->data().front();
    visitor.visit(asnFile);

    exportPromelaModel(promelaModel, actualOutputFilename);
}

void compareTextFiles(const QString &actualOutputFilename, const QString &expectedOutputFilename)
{
    QFile outputFile(actualOutputFilename);
    if (!outputFile.open(QIODevice::ReadOnly)) {
        QFAIL(QString("requested file (%1) cannot be found").arg(actualOutputFilename).toStdString().c_str());
    }
    QTextStream consumableOutput(&outputFile);

    const QString expectedFilepath = QString("resources%1%2").arg(QDir::separator()).arg(expectedOutputFilename);
    try {
        const std::vector<QString> expectedOutput = TextCheckerAndConsumer::readLinesFromFile(expectedFilepath);
        TextCheckerAndConsumer::checkSequenceAndConsume(expectedOutput, consumableOutput);
    } catch (const std::exception &e) {
        QFAIL(e.what());
    }
}

} // namespace tmc::test
