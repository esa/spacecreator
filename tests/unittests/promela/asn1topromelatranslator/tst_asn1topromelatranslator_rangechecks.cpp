/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "tst_asn1topromelatranslator_rangechecks.h"

#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/sourcelocation.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/values.h>
#include <memory>
#include <promela/Asn1ToPromelaTranslator/visitors/asn1nodevisitor.h>
#include <promela/PromelaModel/promelamodel.h>

using Asn1Acn::Definitions;
using Asn1Acn::IntegerValue;
using Asn1Acn::SourceLocation;
using Asn1Acn::TypeAssignment;
using Asn1Acn::Constraints::RangeConstraint;
using Asn1Acn::Constraints::SizeConstraint;
using Asn1Acn::Types::Enumerated;
using Asn1Acn::Types::Integer;
using Asn1Acn::Types::SequenceOf;
using promela::model::AssertCall;
using promela::model::BinaryExpression;
using promela::model::Constant;
using promela::model::InlineDef;
using promela::model::PromelaModel;
using promela::model::VariableRef;
using promela::translator::Asn1NodeVisitor;

namespace tmc::test {

void tst_Asn1ToPromelaTranslator_RangeChecks::initTestCase() { }

void tst_Asn1ToPromelaTranslator_RangeChecks::cleanupTestCase() { }

void tst_Asn1ToPromelaTranslator_RangeChecks::testInteger() const
{
    auto asn1Model = std::make_unique<Definitions>("TmcTestModule", SourceLocation());

    {
        auto integerType = std::make_unique<Integer>();
        integerType->constraints().append({ 1, 10 });
        auto myIntegerAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyInteger"), QStringLiteral("MyInteger"), SourceLocation(), std::move(integerType));

        asn1Model->addType(std::move(myIntegerAssignment));
    }

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*asn1Model);

    QCOMPARE(promelaModel.getInlineDefs().size(), 2);
    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "MyInteger_range_check");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 1);

        const auto assertCall = findAssertCall(inlineDef->getSequence());
        QVERIFY(assertCall != nullptr);

        const auto assertExpr = std::get_if<BinaryExpression>(&assertCall->expression().getContent());
        QVERIFY(assertExpr != nullptr);

        QVERIFY(assertExpr->getOperator() == BinaryExpression::Operator::AND);

        const auto minValueExpr = std::get_if<BinaryExpression>(&assertExpr->getLeft()->getContent());
        QVERIFY(minValueExpr != nullptr);

        QVERIFY(minValueExpr->getOperator() == BinaryExpression::Operator::GEQUAL);
        const auto minValueVar = std::get_if<VariableRef>(&minValueExpr->getLeft()->getContent());
        QVERIFY(minValueVar != nullptr);
        QCOMPARE(minValueVar->getElements().begin()->m_name, "MyInteger_vc");
        const auto minValueConst = std::get_if<Constant>(&minValueExpr->getRight()->getContent());
        QVERIFY(minValueConst != nullptr);
        QCOMPARE(minValueConst->getValue(), 1);

        const auto maxValueExpr = std::get_if<BinaryExpression>(&assertExpr->getRight()->getContent());
        QVERIFY(maxValueExpr != nullptr);

        QVERIFY(maxValueExpr->getOperator() == BinaryExpression::Operator::LEQUAL);
        const auto maxValueVar = std::get_if<VariableRef>(&maxValueExpr->getLeft()->getContent());
        QVERIFY(maxValueVar != nullptr);
        QCOMPARE(maxValueVar->getElements().begin()->m_name, "MyInteger_vc");
        const auto maxValueConst = std::get_if<Constant>(&maxValueExpr->getRight()->getContent());
        QVERIFY(maxValueConst != nullptr);
        QCOMPARE(maxValueConst->getValue(), 10);
    }
}

void tst_Asn1ToPromelaTranslator_RangeChecks::testIntegerMinValue() const
{
    // test for special case, then check for minimum value shall not be generated
    auto asn1Model = std::make_unique<Definitions>("TmcTestModule", SourceLocation());

    {
        auto integerType = std::make_unique<Integer>();
        integerType->constraints().append({ std::numeric_limits<int32_t>::min(), 10 });
        auto myIntegerAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyInteger"), QStringLiteral("MyInteger"), SourceLocation(), std::move(integerType));

        asn1Model->addType(std::move(myIntegerAssignment));
    }

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*asn1Model);

    QCOMPARE(promelaModel.getInlineDefs().size(), 2);
    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "MyInteger_range_check");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 1);

        const auto assertCall = findAssertCall(inlineDef->getSequence());
        QVERIFY(assertCall != nullptr);

        const auto assertExpr = std::get_if<BinaryExpression>(&assertCall->expression().getContent());
        QVERIFY(assertExpr != nullptr);

        QVERIFY(assertExpr->getOperator() == BinaryExpression::Operator::LEQUAL);

        const auto maxValueVar = std::get_if<VariableRef>(&assertExpr->getLeft()->getContent());
        QVERIFY(maxValueVar != nullptr);
        QCOMPARE(maxValueVar->getElements().begin()->m_name, "MyInteger_vc");
        const auto maxValueConst = std::get_if<Constant>(&assertExpr->getRight()->getContent());
        QVERIFY(maxValueConst != nullptr);
        QCOMPARE(maxValueConst->getValue(), 10);
    }
}

void tst_Asn1ToPromelaTranslator_RangeChecks::testEnum() const
{
    auto asn1Model = std::make_unique<Definitions>("TmcTestModule", SourceLocation());

    {
        auto enumType = std::make_unique<Enumerated>();
        enumType->addItem({ 1, "value1", 1 });
        enumType->addItem({ 2, "value2", 2 });
        enumType->addItem({ 3, "value3", 3 });
        enumType->constraints().append({ "value2" });
        auto myEnumAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyEnum"), QStringLiteral("MyEnum"), SourceLocation(), std::move(enumType));

        asn1Model->addType(std::move(myEnumAssignment));
    }

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*asn1Model);

    QCOMPARE(promelaModel.getInlineDefs().size(), 2);
    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "MyEnum_range_check");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 1);

        const auto assertCall = findAssertCall(inlineDef->getSequence());
        QVERIFY(assertCall != nullptr);

        const auto assertExpr = std::get_if<BinaryExpression>(&assertCall->expression().getContent());
        QVERIFY(assertExpr != nullptr);

        QVERIFY(assertExpr->getOperator() == BinaryExpression::Operator::EQUAL);
        const auto valueVar = std::get_if<VariableRef>(&assertExpr->getLeft()->getContent());
        QVERIFY(valueVar != nullptr);
        QCOMPARE(valueVar->getElements().begin()->m_name, "MyEnum_vc");
        const auto enumValueVar = std::get_if<VariableRef>(&assertExpr->getRight()->getContent());
        QVERIFY(enumValueVar != nullptr);
        QCOMPARE(enumValueVar->getElements().begin()->m_name, "MyEnum_value2");
    }
}

void tst_Asn1ToPromelaTranslator_RangeChecks::testSequenceOf() const
{
    auto asn1Model = std::make_unique<Definitions>("TmcTestModule", SourceLocation());

    {
        auto integerType = std::make_unique<Integer>();
        integerType->constraints().append({ 1, 10 });

        auto sequenceOfType = std::make_unique<SequenceOf>();
        sequenceOfType->setItemsType(integerType->clone());
        auto sizeRangeConstraint = RangeConstraint<IntegerValue>::create({ 2, 5 });
        auto sizeConstraint = std::make_unique<SizeConstraint<IntegerValue>>(std::move(sizeRangeConstraint));
        sequenceOfType->constraints().append(std::move(sizeConstraint));

        auto myIntegerAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyInteger"), QStringLiteral("MyInteger"), SourceLocation(), std::move(integerType));
        asn1Model->addType(std::move(myIntegerAssignment));
        auto mySequenceOfAssignment = std::make_unique<TypeAssignment>(QStringLiteral("MySequenceOf"),
                QStringLiteral("MySequenceOf"), SourceLocation(), std::move(sequenceOfType));
        asn1Model->addType(std::move(mySequenceOfAssignment));
    }

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*asn1Model);

    QCOMPARE(promelaModel.getInlineDefs().size(), 7);
    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "MySequenceOf_size_check");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 1);

        const auto assertCall = findAssertCall(inlineDef->getSequence());
        QVERIFY(assertCall != nullptr);

        const auto assertExpr = std::get_if<BinaryExpression>(&assertCall->expression().getContent());
        QVERIFY(assertExpr != nullptr);

        QVERIFY(assertExpr->getOperator() == BinaryExpression::Operator::AND);

        const auto minSizeExpr = std::get_if<BinaryExpression>(&assertExpr->getLeft()->getContent());
        QVERIFY(minSizeExpr != nullptr);

        QVERIFY(minSizeExpr->getOperator() == BinaryExpression::Operator::GEQUAL);
        const auto minSizeVar = std::get_if<VariableRef>(&minSizeExpr->getLeft()->getContent());
        QVERIFY(minSizeVar != nullptr);
        QCOMPARE(minSizeVar->getElements().begin()->m_name, "MySequenceOf_sc");
        const auto minSizeConst = std::get_if<Constant>(&minSizeExpr->getRight()->getContent());
        QVERIFY(minSizeConst != nullptr);
        QCOMPARE(minSizeConst->getValue(), 2);

        const auto maxSizeExpr = std::get_if<BinaryExpression>(&assertExpr->getRight()->getContent());
        QVERIFY(maxSizeExpr != nullptr);

        QVERIFY(maxSizeExpr->getOperator() == BinaryExpression::Operator::LEQUAL);
        const auto maxSizeVar = std::get_if<VariableRef>(&maxSizeExpr->getLeft()->getContent());
        QVERIFY(maxSizeVar != nullptr);
        QCOMPARE(maxSizeVar->getElements().begin()->m_name, "MySequenceOf_sc");
        const auto maxSizeConst = std::get_if<Constant>(&maxSizeExpr->getRight()->getContent());
        QVERIFY(maxSizeConst != nullptr);
        QCOMPARE(maxSizeConst->getValue(), 5);
    }
}

const InlineDef *tst_Asn1ToPromelaTranslator_RangeChecks::findInline(
        const std::list<std::unique_ptr<InlineDef>> &list, const QString &name) const
{
    auto iter = std::find_if(list.begin(), list.end(),
            [&](const std::unique_ptr<InlineDef> &proctype) { return proctype->getName() == name; });

    if (iter == list.end()) {
        return nullptr;
    }
    return iter->get();
}

const AssertCall *tst_Asn1ToPromelaTranslator_RangeChecks::findAssertCall(
        const ::promela::model::Sequence &sequence) const
{
    auto iter = sequence.getContent().begin();
    if (iter == sequence.getContent().end()) {
        return nullptr;
    }
    if (std::holds_alternative<AssertCall>((*iter)->getValue())) {
        return &(std::get<AssertCall>((*iter)->getValue()));
    }
    return nullptr;
}

} // namespace tmc::test
