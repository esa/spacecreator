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

#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/sourcelocation.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/values.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <promela/Asn1ToPromelaTranslator/visitors/asn1nodevisitor.h>
#include <promela/PromelaModel/promelamodel.h>
#include <memory>

using Asn1Acn::Definitions;
using Asn1Acn::IntegerValue;
using Asn1Acn::TypeAssignment;
using Asn1Acn::SourceLocation;
using Asn1Acn::Constraints::RangeConstraint;
using Asn1Acn::Types::Integer;
using promela::model::AssertCall;
using promela::model::BinaryExpression;
using promela::model::Constant;
using promela::model::InlineDef;
using promela::model::PromelaModel;
using promela::model::VariableRef;
using promela::translator::Asn1NodeVisitor;

namespace tmc::test {

void tst_Asn1ToPromelaTranslator_RangeChecks::initTestCase() {}

void tst_Asn1ToPromelaTranslator_RangeChecks::cleanupTestCase() {}

void tst_Asn1ToPromelaTranslator_RangeChecks::testInteger() const
{
    auto asn1Model = std::make_unique<Definitions>("TmcTestModule", SourceLocation());

    {
        auto integerType = std::make_unique<Integer>();
        integerType->constraints().append({ 1, 10 });
        auto myIntegerAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyInteger"), QStringLiteral("MyInteger"), SourceLocation(), integerType->clone());

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
        const auto minValueConst = std::get_if<Constant>(&minValueExpr->getRight()->getContent());
        QVERIFY(minValueConst != nullptr);
        QCOMPARE(minValueConst->getValue(), 1);

        const auto maxValueExpr = std::get_if<BinaryExpression>(&assertExpr->getRight()->getContent());
        QVERIFY(maxValueExpr != nullptr);

        QVERIFY(maxValueExpr->getOperator() == BinaryExpression::Operator::LEQUAL);
        const auto maxValueVar = std::get_if<VariableRef>(&maxValueExpr->getLeft()->getContent());
        QVERIFY(maxValueVar != nullptr);
        const auto maxValueConst = std::get_if<Constant>(&maxValueExpr->getRight()->getContent());
        QVERIFY(maxValueConst != nullptr);
        QCOMPARE(maxValueConst->getValue(), 10);
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
