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

#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/values.h>
#include <promela/Asn1ToPromelaTranslator/visitors/asn1nodevaluegeneratorvisitor.h>
#include <promela/PromelaModel/constant.h>
#include <promela/PromelaModel/promelamodel.h>
#include <variant>

using Asn1Acn::IntegerValue;
using Asn1Acn::SourceLocation;
using Asn1Acn::TypeAssignment;
using Asn1Acn::Constraints::RangeConstraint;
using Asn1Acn::Types::Boolean;
using Asn1Acn::Types::Enumerated;
using Asn1Acn::Types::EnumeratedItem;
using Asn1Acn::Types::Integer;
using promela::model::Assignment;
using promela::model::Conditional;
using promela::model::Constant;
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
            if (!std::holds_alternative<Assignment>(content->getValue())) {
                continue;
            }

            const Assignment &assignment = std::get<Assignment>(content->getValue());
            QCOMPARE(assignment.getVariableRef().getElements().size(), 1);
            QCOMPARE(assignment.getVariableRef().getElements().front().m_name, argName);
            QVERIFY(assignment.getVariableRef().getElements().front().m_index.get() == nullptr);

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
        QCOMPARE(assignment.getVariableRef().getElements().size(), 1);
        QCOMPARE(assignment.getVariableRef().getElements().front().m_name, argName);
        QVERIFY(assignment.getVariableRef().getElements().front().m_index.get() == nullptr);
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

std::unique_ptr<Definitions> tst_Asn1ToPromelaTranslator_Env::createModel() const
{
    return std::make_unique<Definitions>("myModule", SourceLocation());
}
}
