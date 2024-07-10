/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 - 2022 N7 Space Sp. z o.o.
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

#include "tst_asn1topromelatranslator.h"

#include <QObject>
#include <QtTest>
#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/constraints/constraintlist.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/sourcelocation.h>
#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/typefactory.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <modelloader.h>
#include <promela/Asn1ToPromelaTranslator/promelatypesorter.h>
#include <promela/Asn1ToPromelaTranslator/translator.h>
#include <promela/Asn1ToPromelaTranslator/visitors/asn1nodevisitor.h>
#include <promela/PromelaModel/promelamodel.h>

using Asn1Acn::AsnSequenceComponent;
using Asn1Acn::BitStringValue;
using Asn1Acn::Definitions;
using Asn1Acn::EnumValue;
using Asn1Acn::IntegerValue;
using Asn1Acn::OctetStringValue;
using Asn1Acn::Range;
using Asn1Acn::RealValue;
using Asn1Acn::SourceLocation;
using Asn1Acn::StringValue;
using Asn1Acn::TypeAssignment;
using Asn1Acn::Constraints::RangeConstraint;
using Asn1Acn::Constraints::SizeConstraint;
using Asn1Acn::Types::BitString;
using Asn1Acn::Types::Choice;
using Asn1Acn::Types::ChoiceAlternative;
using Asn1Acn::Types::Enumerated;
using Asn1Acn::Types::EnumeratedItem;
using Asn1Acn::Types::IA5String;
using Asn1Acn::Types::Integer;
using Asn1Acn::Types::OctetString;
using Asn1Acn::Types::Real;
using Asn1Acn::Types::Sequence;
using Asn1Acn::Types::SequenceOf;
using Asn1Acn::Types::Type;
using Asn1Acn::Types::TypeFactory;
using Asn1Acn::Types::UserdefinedType;
using promela::model::AssertCall;
using promela::model::Assignment;
using promela::model::BasicType;
using promela::model::BinaryExpression;
using promela::model::Conditional;
using promela::model::Constant;
using promela::model::Declaration;
using promela::model::ForLoop;
using promela::model::InlineCall;
using promela::model::InlineDef;
using promela::model::PromelaModel;
using promela::model::Skip;
using promela::model::TypeAlias;
using promela::model::Utype;
using promela::model::UtypeRef;
using promela::model::ValueDefinition;
using promela::model::VariableRef;
using promela::translator::Asn1NodeVisitor;
using promela::translator::PromelaTypeSorter;

namespace tmc::test {

void tst_Asn1ToPromelaTranslator::initTestCase() { }

void tst_Asn1ToPromelaTranslator::cleanupTestCase() { }

void tst_Asn1ToPromelaTranslator::testBasicTypes()
{
    auto model = createModel();

    {
        auto integerType = TypeFactory::createBuiltinType("INTEGER");
        dynamic_cast<Integer *>(integerType.get())
                ->constraints()
                .append(RangeConstraint<IntegerValue>::create({ 1, 5 }));
        auto myIntegerAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyInteger"), QStringLiteral("MyIntegerT"), SourceLocation(), std::move(integerType));
        model->addType(std::move(myIntegerAssignment));
    }

    {
        auto realType = TypeFactory::createBuiltinType("REAL");
        dynamic_cast<Real *>(realType.get())->constraints().append(RangeConstraint<RealValue>::create({ 1, 5 }));
        auto myRealAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyReal"), QStringLiteral("MyRealT"), SourceLocation(), std::move(realType));
        model->addType(std::move(myRealAssignment));
    }

    {
        auto booleanType = TypeFactory::createBuiltinType("BOOLEAN");
        auto myBooleanAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyBoolean"), QStringLiteral("MyBooleanT"), SourceLocation(), std::move(booleanType));
        model->addType(std::move(myBooleanAssignment));
    }

    {
        auto nullType = TypeFactory::createBuiltinType("NULL");
        auto myNullAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyNull"), QStringLiteral("MyNullT"), SourceLocation(), std::move(nullType));
        model->addType(std::move(myNullAssignment));
    }

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 4);

    const QList<TypeAlias> aliases = promelaModel.getTypeAliases();

    const TypeAlias &expectedInteger = aliases.at(0);
    QCOMPARE(expectedInteger.getName(), "MyInteger");
    QVERIFY(std::holds_alternative<BasicType>(expectedInteger.getType()));
    QCOMPARE(std::get<BasicType>(expectedInteger.getType()), BasicType::INT);

    const TypeAlias &expectedReal = aliases.at(1);
    QCOMPARE(expectedReal.getName(), "MyReal");
    QVERIFY(std::holds_alternative<BasicType>(expectedReal.getType()));
    QCOMPARE(std::get<BasicType>(expectedReal.getType()), BasicType::FLOAT);

    const TypeAlias &expectedBoolean = aliases.at(2);
    QCOMPARE(expectedBoolean.getName(), "MyBoolean");
    QVERIFY(std::holds_alternative<BasicType>(expectedBoolean.getType()));
    QCOMPARE(std::get<BasicType>(expectedBoolean.getType()), BasicType::BOOLEAN);

    const TypeAlias &expectedNull = aliases.at(3);
    QCOMPARE(expectedNull.getName(), "MyNull");
    QVERIFY(std::holds_alternative<BasicType>(expectedNull.getType()));
    QCOMPARE(std::get<BasicType>(expectedNull.getType()), BasicType::BIT);

    QCOMPARE(promelaModel.getInlineDefs().size(), 7);

    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "MyInteger_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);

        const auto &sequence = inlineDef->getSequence();
        QCOMPARE(sequence.getContent().size(), 2);

        const auto assignment = findProctypeElement<Assignment>(sequence, 0);
        QVERIFY(assignment != nullptr);

        const auto rangeCheckCall = findProctypeElement<InlineCall>(sequence, 1);
        QVERIFY(rangeCheckCall != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyReal_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);

        const auto &sequence = inlineDef->getSequence();
        QCOMPARE(sequence.getContent().size(), 2);

        const auto assignment = findProctypeElement<Assignment>(sequence, 0);
        QVERIFY(assignment != nullptr);

        const auto rangeCheckCall = findProctypeElement<InlineCall>(sequence, 1);
        QVERIFY(rangeCheckCall != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyBoolean_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);

        const auto &sequence = inlineDef->getSequence();
        QCOMPARE(sequence.getContent().size(), 2);

        const auto assignment = findProctypeElement<Assignment>(sequence, 0);
        QVERIFY(assignment != nullptr);

        const auto rangeCheckCall = findProctypeElement<InlineCall>(sequence, 1);
        QVERIFY(rangeCheckCall != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyNull_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);

        const auto &sequence = inlineDef->getSequence();
        QCOMPARE(sequence.getContent().size(), 1);

        const auto skip = findProctypeElement<Skip>(sequence, 0);
        QVERIFY(skip != nullptr);
    }

    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "MyInteger_range_check");
        QVERIFY(inlineDef != nullptr);
    }
    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "MyReal_range_check");
        QVERIFY(inlineDef != nullptr);
    }
    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "MyBoolean_range_check");
        QVERIFY(inlineDef != nullptr);
    }
}

void tst_Asn1ToPromelaTranslator::testEnumerated()
{
    auto model = createModel();
    std::unique_ptr<Enumerated> type = std::make_unique<Enumerated>();

    type->addItem(EnumeratedItem(0, "zero", 0));
    type->addItem(EnumeratedItem(1, "one", 1));
    type->addItem(EnumeratedItem(2, "infinity", 100));

    type->constraints().append(RangeConstraint<EnumValue>::create({ "zero" }));

    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 1);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 3);

    const TypeAlias &expectedTypeAlias = promelaModel.getTypeAliases().at(0);
    QCOMPARE(expectedTypeAlias.getName(), "MyType");
    QVERIFY(std::holds_alternative<BasicType>(expectedTypeAlias.getType()));
    QCOMPARE(std::get<BasicType>(expectedTypeAlias.getType()), BasicType::INT);

    const QList<ValueDefinition> &valueDefs = promelaModel.getValueDefinitions();

    const ValueDefinition &expectedZero = valueDefs.at(0);
    QCOMPARE(expectedZero.getName(), "MyType_zero");
    QCOMPARE(expectedZero.getValue(), 0);

    const ValueDefinition &expectedOne = valueDefs.at(1);
    QCOMPARE(expectedOne.getName(), "MyType_one");
    QCOMPARE(expectedOne.getValue(), 1);

    const ValueDefinition &expectedInfinity = valueDefs.at(2);
    QCOMPARE(expectedInfinity.getName(), "MyType_infinity");
    QCOMPARE(expectedInfinity.getValue(), 100);

    QCOMPARE(promelaModel.getInlineDefs().size(), 2);
    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "MyType_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);

        const auto &sequence = inlineDef->getSequence();
        QCOMPARE(sequence.getContent().size(), 2);

        const auto assignment = findProctypeElement<Assignment>(sequence, 0);
        QVERIFY(assignment != nullptr);

        const auto rangeCheckCall = findProctypeElement<InlineCall>(sequence, 1);
        QVERIFY(rangeCheckCall != nullptr);
    }
}

void tst_Asn1ToPromelaTranslator::testVariableBitString()
{
    const int EXPECTED_SIZE = 16;

    auto model = createModel();
    auto type = std::make_unique<BitString>();
    auto sizeConstraint = std::make_unique<SizeConstraint<BitStringValue>>(
            RangeConstraint<IntegerValue>::create({ 1, EXPECTED_SIZE }));
    type->constraints().append(std::move(sizeConstraint));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);
    QCOMPARE(promelaModel.getTypeAliases().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);

    QCOMPARE(expectedUtype.getName(), "MyType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 2);

    const Declaration *expectedArray = findUtypeField(expectedUtype, 0);

    QVERIFY(expectedArray);
    QCOMPARE(expectedArray->getName(), "data");
    QVERIFY(expectedArray->getType().isArrayType());
    QCOMPARE(expectedArray->getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<BasicType>(expectedArray->getType().getArrayType().getType()));
    QCOMPARE(std::get<BasicType>(expectedArray->getType().getArrayType().getType()), BasicType::BIT);

    const Declaration *expectedLength = findUtypeField(expectedUtype, 1);
    QVERIFY(expectedLength);
    QCOMPARE(expectedLength->getName(), "length");
    QVERIFY(expectedLength->getType().isBasicType());
    QCOMPARE(expectedLength->getType().getBasicType(), BasicType::INT);

    QCOMPARE(promelaModel.getInlineDefs().size(), 2);
    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "MyType_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);

        const auto &sequence = inlineDef->getSequence();
        QCOMPARE(sequence.getContent().size(), 5);

        const auto sizeCheckCall = findProctypeElement<InlineCall>(sequence, 0);
        QVERIFY(sizeCheckCall != nullptr);

        const auto decl = findProctypeElement<Declaration>(sequence, 1);
        QVERIFY(decl != nullptr);

        const auto dataLoop = findProctypeElement<ForLoop>(sequence, 2);
        QVERIFY(dataLoop != nullptr);
        QCOMPARE(dataLoop->getType(), ForLoop::Type::RANGE);
        QCOMPARE(dataLoop->getFirstIntValue(), 0);
        const auto dataLoopLastExpression = std::get_if<BinaryExpression>(&dataLoop->getLastExpression().getContent());
        QVERIFY(dataLoopLastExpression != nullptr);
        QCOMPARE(dataLoopLastExpression->getOperator(), BinaryExpression::Operator::SUBTRACT);
        QVERIFY(dataLoopLastExpression->getLeft() != nullptr);
        const auto dataLoopLastExpressionLeft =
                std::get_if<VariableRef>(&dataLoopLastExpression->getLeft()->getContent());
        QVERIFY(dataLoopLastExpressionLeft != nullptr);
        auto iter = dataLoopLastExpressionLeft->getElements().begin();
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "src");
        ++iter;
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "length");
        QVERIFY(dataLoopLastExpression->getRight() != nullptr);
        const auto dataLoopLastExpressionRight =
                std::get_if<Constant>(&dataLoopLastExpression->getRight()->getContent());
        QVERIFY(dataLoopLastExpressionRight != nullptr);
        QCOMPARE(dataLoopLastExpressionRight->getValue(), 1);

        const auto zeroLoop = findProctypeElement<ForLoop>(sequence, 3);
        QVERIFY(zeroLoop != nullptr);
        QCOMPARE(zeroLoop->getType(), ForLoop::Type::RANGE);
        const auto zeroLoopFirstExpression = std::get_if<VariableRef>(&zeroLoop->getFirstExpression().getContent());
        QVERIFY(zeroLoopFirstExpression != nullptr);
        iter = zeroLoopFirstExpression->getElements().begin();
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "src");
        ++iter;
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "length");
        QCOMPARE(zeroLoop->getLastIntValue(), EXPECTED_SIZE - 1);

        const auto assignment = findProctypeElement<Assignment>(sequence, 4);
        QVERIFY(assignment != nullptr);
    }
}

void tst_Asn1ToPromelaTranslator::testFixedBitString()
{
    const int EXPECTED_SIZE = 16;

    auto model = createModel();
    auto type = std::make_unique<BitString>();
    auto sizeConstraint = std::make_unique<SizeConstraint<BitStringValue>>(
            RangeConstraint<IntegerValue>::create(Range<int64_t>(EXPECTED_SIZE)));
    type->constraints().append(std::move(sizeConstraint));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);
    QCOMPARE(promelaModel.getTypeAliases().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);

    QCOMPARE(expectedUtype.getName(), "MyType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 1);

    const Declaration *expectedArray = findUtypeField(expectedUtype, 0);

    QVERIFY(expectedArray);
    QCOMPARE(expectedArray->getName(), "data");
    QVERIFY(expectedArray->getType().isArrayType());
    QCOMPARE(expectedArray->getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<BasicType>(expectedArray->getType().getArrayType().getType()));
    QCOMPARE(std::get<BasicType>(expectedArray->getType().getArrayType().getType()), BasicType::BIT);

    QCOMPARE(promelaModel.getInlineDefs().size(), 1);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const Declaration *decl = findProctypeElement<Declaration>(inlineDef->getSequence(), 0);
        QVERIFY(decl != nullptr);
        const ForLoop *loop = findProctypeElement<ForLoop>(inlineDef->getSequence(), 1);
        QVERIFY(loop != nullptr);
        QCOMPARE(loop->getType(), ForLoop::Type::RANGE);
        QCOMPARE(loop->getFirstIntValue(), 0);
        QCOMPARE(loop->getLastIntValue(), EXPECTED_SIZE - 1);
    }
}

void tst_Asn1ToPromelaTranslator::testVariableOctetString()
{
    const int EXPECTED_SIZE = 16;

    auto model = createModel();
    auto type = std::make_unique<OctetString>();
    auto sizeConstraint = std::make_unique<SizeConstraint<OctetStringValue>>(
            RangeConstraint<IntegerValue>::create({ 1, EXPECTED_SIZE }));
    type->constraints().append(std::move(sizeConstraint));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);
    QCOMPARE(promelaModel.getTypeAliases().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);

    QCOMPARE(expectedUtype.getName(), "MyType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 2);

    const Declaration *expectedArray = findUtypeField(expectedUtype, 0);

    QVERIFY(expectedArray);
    QCOMPARE(expectedArray->getName(), "data");
    QVERIFY(expectedArray->getType().isArrayType());
    QCOMPARE(expectedArray->getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<BasicType>(expectedArray->getType().getArrayType().getType()));
    QCOMPARE(std::get<BasicType>(expectedArray->getType().getArrayType().getType()), BasicType::BYTE);

    const Declaration *expectedLength = findUtypeField(expectedUtype, 1);
    QVERIFY(expectedLength);
    QCOMPARE(expectedLength->getName(), "length");
    QVERIFY(expectedLength->getType().isBasicType());
    QCOMPARE(expectedLength->getType().getBasicType(), BasicType::INT);
    QCOMPARE(promelaModel.getInlineDefs().size(), 2);
    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "MyType_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);

        const auto &sequence = inlineDef->getSequence();
        QCOMPARE(sequence.getContent().size(), 5);

        const auto sizeCheckCall = findProctypeElement<InlineCall>(sequence, 0);
        QVERIFY(sizeCheckCall != nullptr);

        const auto decl = findProctypeElement<Declaration>(sequence, 1);
        QVERIFY(decl != nullptr);

        const auto dataLoop = findProctypeElement<ForLoop>(sequence, 2);
        QVERIFY(dataLoop != nullptr);
        QCOMPARE(dataLoop->getType(), ForLoop::Type::RANGE);
        QCOMPARE(dataLoop->getFirstIntValue(), 0);
        const auto dataLoopLastExpression = std::get_if<BinaryExpression>(&dataLoop->getLastExpression().getContent());
        QVERIFY(dataLoopLastExpression != nullptr);
        QCOMPARE(dataLoopLastExpression->getOperator(), BinaryExpression::Operator::SUBTRACT);
        QVERIFY(dataLoopLastExpression->getLeft() != nullptr);
        const auto dataLoopLastExpressionLeft =
                std::get_if<VariableRef>(&dataLoopLastExpression->getLeft()->getContent());
        QVERIFY(dataLoopLastExpressionLeft != nullptr);
        auto iter = dataLoopLastExpressionLeft->getElements().begin();
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "src");
        ++iter;
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "length");
        QVERIFY(dataLoopLastExpression->getRight() != nullptr);
        const auto dataLoopLastExpressionRight =
                std::get_if<Constant>(&dataLoopLastExpression->getRight()->getContent());
        QVERIFY(dataLoopLastExpressionRight != nullptr);
        QCOMPARE(dataLoopLastExpressionRight->getValue(), 1);

        const auto zeroLoop = findProctypeElement<ForLoop>(sequence, 3);
        QVERIFY(zeroLoop != nullptr);
        QCOMPARE(zeroLoop->getType(), ForLoop::Type::RANGE);
        const auto zeroLoopFirstExpression = std::get_if<VariableRef>(&zeroLoop->getFirstExpression().getContent());
        QVERIFY(zeroLoopFirstExpression != nullptr);
        iter = zeroLoopFirstExpression->getElements().begin();
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "src");
        ++iter;
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "length");
        QCOMPARE(zeroLoop->getLastIntValue(), EXPECTED_SIZE - 1);

        const auto assignment = findProctypeElement<Assignment>(sequence, 4);
        QVERIFY(assignment != nullptr);
    }
}

void tst_Asn1ToPromelaTranslator::testFixedOctetString()
{
    const int EXPECTED_SIZE = 16;

    auto model = createModel();
    auto type = std::make_unique<OctetString>();
    auto sizeConstraint = std::make_unique<SizeConstraint<OctetStringValue>>(
            RangeConstraint<IntegerValue>::create(Range<int64_t>(EXPECTED_SIZE)));
    type->constraints().append(std::move(sizeConstraint));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);
    QCOMPARE(promelaModel.getTypeAliases().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);

    QCOMPARE(expectedUtype.getName(), "MyType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 1);

    const Declaration *expectedArray = findUtypeField(expectedUtype, 0);

    QVERIFY(expectedArray);
    QCOMPARE(expectedArray->getName(), "data");
    QVERIFY(expectedArray->getType().isArrayType());
    QCOMPARE(expectedArray->getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<BasicType>(expectedArray->getType().getArrayType().getType()));
    QCOMPARE(std::get<BasicType>(expectedArray->getType().getArrayType().getType()), BasicType::BYTE);

    QCOMPARE(promelaModel.getInlineDefs().size(), 1);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const Declaration *decl = findProctypeElement<Declaration>(inlineDef->getSequence(), 0);
        QVERIFY(decl != nullptr);
        const ForLoop *loop = findProctypeElement<ForLoop>(inlineDef->getSequence(), 1);
        QVERIFY(loop != nullptr);
        QCOMPARE(loop->getType(), ForLoop::Type::RANGE);
        QCOMPARE(loop->getFirstIntValue(), 0);
        QCOMPARE(loop->getLastIntValue(), EXPECTED_SIZE - 1);
    }
}

void tst_Asn1ToPromelaTranslator::testVariableIA5String()
{
    const int EXPECTED_SIZE = 16;

    auto model = createModel();
    auto type = std::make_unique<IA5String>();
    auto sizeConstraint =
            std::make_unique<SizeConstraint<StringValue>>(RangeConstraint<IntegerValue>::create({ 1, EXPECTED_SIZE }));
    type->constraints().append(std::move(sizeConstraint));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);
    QCOMPARE(promelaModel.getTypeAliases().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);

    QCOMPARE(expectedUtype.getName(), "MyType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 2);

    const Declaration *expectedArray = findUtypeField(expectedUtype, 0);

    QVERIFY(expectedArray);
    QVERIFY(expectedArray);
    QCOMPARE(expectedArray->getName(), "data");
    QVERIFY(expectedArray->getType().isArrayType());
    QCOMPARE(expectedArray->getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<BasicType>(expectedArray->getType().getArrayType().getType()));
    QCOMPARE(std::get<BasicType>(expectedArray->getType().getArrayType().getType()), BasicType::BYTE);

    const Declaration *expectedLength = findUtypeField(expectedUtype, 1);
    QVERIFY(expectedLength);
    QCOMPARE(expectedLength->getName(), "length");
    QVERIFY(expectedLength->getType().isBasicType());
    QCOMPARE(expectedLength->getType().getBasicType(), BasicType::INT);

    QCOMPARE(promelaModel.getInlineDefs().size(), 2);
    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "MyType_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);

        const auto &sequence = inlineDef->getSequence();
        QCOMPARE(sequence.getContent().size(), 5);

        const auto sizeCheckCall = findProctypeElement<InlineCall>(sequence, 0);
        QVERIFY(sizeCheckCall != nullptr);

        const auto decl = findProctypeElement<Declaration>(sequence, 1);
        QVERIFY(decl != nullptr);

        const auto dataLoop = findProctypeElement<ForLoop>(sequence, 2);
        QVERIFY(dataLoop != nullptr);
        QCOMPARE(dataLoop->getType(), ForLoop::Type::RANGE);
        QCOMPARE(dataLoop->getFirstIntValue(), 0);
        const auto dataLoopLastExpression = std::get_if<BinaryExpression>(&dataLoop->getLastExpression().getContent());
        QVERIFY(dataLoopLastExpression != nullptr);
        QCOMPARE(dataLoopLastExpression->getOperator(), BinaryExpression::Operator::SUBTRACT);
        QVERIFY(dataLoopLastExpression->getLeft() != nullptr);
        const auto dataLoopLastExpressionLeft =
                std::get_if<VariableRef>(&dataLoopLastExpression->getLeft()->getContent());
        QVERIFY(dataLoopLastExpressionLeft != nullptr);
        auto iter = dataLoopLastExpressionLeft->getElements().begin();
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "src");
        ++iter;
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "length");
        QVERIFY(dataLoopLastExpression->getRight() != nullptr);
        const auto dataLoopLastExpressionRight =
                std::get_if<Constant>(&dataLoopLastExpression->getRight()->getContent());
        QVERIFY(dataLoopLastExpressionRight != nullptr);
        QCOMPARE(dataLoopLastExpressionRight->getValue(), 1);

        const auto zeroLoop = findProctypeElement<ForLoop>(sequence, 3);
        QVERIFY(zeroLoop != nullptr);
        QCOMPARE(zeroLoop->getType(), ForLoop::Type::RANGE);
        const auto zeroLoopFirstExpression = std::get_if<VariableRef>(&zeroLoop->getFirstExpression().getContent());
        QVERIFY(zeroLoopFirstExpression != nullptr);
        iter = zeroLoopFirstExpression->getElements().begin();
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "src");
        ++iter;
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "length");
        QCOMPARE(zeroLoop->getLastIntValue(), EXPECTED_SIZE - 1);

        const auto assignment = findProctypeElement<Assignment>(sequence, 4);
        QVERIFY(assignment != nullptr);
    }
}

void tst_Asn1ToPromelaTranslator::testFixedIA5String()
{
    const int EXPECTED_SIZE = 16;

    auto model = createModel();
    auto type = std::make_unique<IA5String>();
    auto sizeConstraint = std::make_unique<SizeConstraint<StringValue>>(
            RangeConstraint<IntegerValue>::create(Range<int64_t>(EXPECTED_SIZE)));
    type->constraints().append(std::move(sizeConstraint));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);
    QCOMPARE(promelaModel.getTypeAliases().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);

    QCOMPARE(expectedUtype.getName(), "MyType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 1);

    const Declaration *expectedArray = findUtypeField(expectedUtype, 0);

    QVERIFY(expectedArray);
    QCOMPARE(expectedArray->getName(), "data");
    QVERIFY(expectedArray->getType().isArrayType());
    QCOMPARE(expectedArray->getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<BasicType>(expectedArray->getType().getArrayType().getType()));
    QCOMPARE(std::get<BasicType>(expectedArray->getType().getArrayType().getType()), BasicType::BYTE);

    QCOMPARE(promelaModel.getInlineDefs().size(), 1);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const Declaration *decl = findProctypeElement<Declaration>(inlineDef->getSequence(), 0);
        QVERIFY(decl != nullptr);
        const ForLoop *loop = findProctypeElement<ForLoop>(inlineDef->getSequence(), 1);
        QVERIFY(loop != nullptr);
        QCOMPARE(loop->getType(), ForLoop::Type::RANGE);
        QCOMPARE(loop->getFirstIntValue(), 0);
        QCOMPARE(loop->getLastIntValue(), EXPECTED_SIZE - 1);
    }
}

void tst_Asn1ToPromelaTranslator::testChoice()
{
    auto model = createModel();
    auto type = std::make_unique<Choice>();

    auto ch1Type = TypeFactory::createBuiltinType(QStringLiteral("INTEGER"));
    dynamic_cast<Integer *>(ch1Type.get())->constraints().append(RangeConstraint<IntegerValue>::create({ 1, 5 }));
    type->addComponent(std::make_unique<ChoiceAlternative>(QStringLiteral("ch1"), QStringLiteral(""),
            QStringLiteral(""), QStringLiteral(""), QStringLiteral(""), SourceLocation(), std::move(ch1Type)));

    auto ch2Type = TypeFactory::createBuiltinType(QStringLiteral("REAL"));
    dynamic_cast<Real *>(ch2Type.get())->constraints().append(RangeConstraint<RealValue>::create({ 1, 5 }));
    type->addComponent(std::make_unique<ChoiceAlternative>(QStringLiteral("ch2"), QStringLiteral(""),
            QStringLiteral(""), QStringLiteral(""), QStringLiteral(""), SourceLocation(), std::move(ch2Type)));

    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 2);
    QCOMPARE(promelaModel.getTypeAliases().size(), 2);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 5);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);
    QCOMPARE(expectedUtype.getName(), "MyType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 2);

    const Declaration *expectedData = findUtypeField(expectedUtype, 0);
    QVERIFY(expectedData);
    QCOMPARE(expectedData->getName(), "data");
    QVERIFY(expectedData->getType().isUtypeReference());
    QCOMPARE(expectedData->getType().getUtypeReference().getName(), "MyType_data");

    const Declaration *expectedSelection = findUtypeField(expectedUtype, 1);
    QVERIFY(expectedSelection);
    QCOMPARE(expectedSelection->getName(), "selection");
    QVERIFY(expectedSelection->getType().isBasicType());
    QCOMPARE(expectedSelection->getType().getBasicType(), BasicType::INT);

    const Utype &expectedDataUtype = promelaModel.getUtypes().at(1);
    QCOMPARE(expectedDataUtype.getName(), "MyType_data");
    QVERIFY(expectedDataUtype.isUnionType());

    QCOMPARE(expectedDataUtype.getFields().size(), 2);

    const Declaration *expectedField1 = findUtypeField(expectedDataUtype, 0);
    QVERIFY(expectedField1);
    QCOMPARE(expectedField1->getName(), "ch1");
    QVERIFY(expectedField1->getType().isUtypeReference());
    QCOMPARE(expectedField1->getType().getUtypeReference().getName(), "MyType__ch1");

    const Declaration *expectedField2 = findUtypeField(expectedDataUtype, 1);
    QVERIFY(expectedField2);
    QCOMPARE(expectedField2->getName(), "ch2");
    QVERIFY(expectedField2->getType().isUtypeReference());
    QCOMPARE(expectedField2->getType().getUtypeReference().getName(), "MyType__ch2");

    QCOMPARE(promelaModel.getValueDefinitions().at(0).getName(), "MyType_NONE");
    QCOMPARE(promelaModel.getValueDefinitions().at(0).getValue(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().at(1).getName(), "MyType_ch1_PRESENT");
    QCOMPARE(promelaModel.getValueDefinitions().at(1).getValue(), 1);
    QCOMPARE(promelaModel.getValueDefinitions().at(2).getName(), "Mytype_selection_ch1_PRESENT");
    QCOMPARE(promelaModel.getValueDefinitions().at(2).getValue(), 1);
    QCOMPARE(promelaModel.getValueDefinitions().at(3).getName(), "MyType_ch2_PRESENT");
    QCOMPARE(promelaModel.getValueDefinitions().at(3).getValue(), 2);
    QCOMPARE(promelaModel.getValueDefinitions().at(4).getName(), "Mytype_selection_ch2_PRESENT");
    QCOMPARE(promelaModel.getValueDefinitions().at(4).getValue(), 2);

    QCOMPARE(promelaModel.getTypeAliases().at(0).getName(), "MyType__ch1");
    QCOMPARE(promelaModel.getTypeAliases().at(1).getName(), "MyType__ch2");

    QCOMPARE(promelaModel.getInlineDefs().size(), 6);
    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "MyType_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);

        const auto selectionAssignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 0);
        QVERIFY(selectionAssignment != nullptr);

        const auto assignConditional = findProctypeElement<Conditional>(inlineDef->getSequence(), 1);
        QVERIFY(assignConditional != nullptr);
        QCOMPARE(assignConditional->getAlternatives().size(), 3);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__ch1_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 0);
        QVERIFY(assignment != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__ch2_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 0);
        QVERIFY(assignment != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__ch1_range_check");
        QVERIFY(inlineDef != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__ch2_range_check");
        QVERIFY(inlineDef != nullptr);
    }
    {
        const auto inlineDef = findInline(promelaModel.getInlineDefs(), "Mytype_selection_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 1);
        const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 0);
        QVERIFY(assignment != nullptr);
    }
}

void tst_Asn1ToPromelaTranslator::testSequence()
{
    auto model = createModel();
    auto type = std::make_unique<Sequence>();
    auto field1Type = TypeFactory::createBuiltinType(QStringLiteral("INTEGER"));
    dynamic_cast<Integer *>(field1Type.get())->constraints().append(RangeConstraint<IntegerValue>::create({ 1, 5 }));
    auto component1 = std::make_unique<AsnSequenceComponent>(QStringLiteral("field1"), QStringLiteral("field1"), false,
            std::nullopt, QStringLiteral(""), AsnSequenceComponent::Presence::NotSpecified, SourceLocation(),
            std::move(field1Type));
    type->addComponent(std::move(component1));

    auto field2Type = TypeFactory::createBuiltinType(QStringLiteral("REAL"));
    dynamic_cast<Real *>(field2Type.get())->constraints().append(RangeConstraint<RealValue>::create({ 1, 5 }));
    auto component2 = std::make_unique<AsnSequenceComponent>(QStringLiteral("field2"), QStringLiteral("field2"), false,
            std::nullopt, QStringLiteral(""), AsnSequenceComponent::Presence::NotSpecified, SourceLocation(),
            std::move(field2Type));
    type->addComponent(std::move(component2));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 2);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);

    const Utype &expectedUtype = promelaModel.getUtypes().front();
    QCOMPARE(expectedUtype.isUnionType(), false);
    QCOMPARE(expectedUtype.getName(), "MyType");
    QCOMPARE(expectedUtype.getFields().size(), 2);

    const Declaration *decl1 = findUtypeField(expectedUtype, 0);
    QVERIFY(decl1);
    QCOMPARE("field1", decl1->getName());
    QVERIFY(decl1->getType().isUtypeReference());
    QCOMPARE(decl1->getType().getUtypeReference().getName(), "MyType__field1");

    const Declaration *decl2 = findUtypeField(expectedUtype, 1);
    QVERIFY(decl2);
    QCOMPARE("field2", decl2->getName());
    QVERIFY(decl2->getType().isUtypeReference());
    QCOMPARE(decl2->getType().getUtypeReference().getName(), "MyType__field2");

    const TypeAlias &expectedAlias1 = promelaModel.getTypeAliases().at(0);
    QCOMPARE(expectedAlias1.getName(), "MyType__field1");
    QVERIFY(std::holds_alternative<BasicType>(expectedAlias1.getType()));
    QCOMPARE(std::get<BasicType>(expectedAlias1.getType()), BasicType::INT);

    const TypeAlias &expectedAlias2 = promelaModel.getTypeAliases().at(1);
    QCOMPARE(expectedAlias2.getName(), "MyType__field2");
    QVERIFY(std::holds_alternative<BasicType>(expectedAlias2.getType()));
    QCOMPARE(std::get<BasicType>(expectedAlias2.getType()), BasicType::FLOAT);

    QCOMPARE(promelaModel.getInlineDefs().size(), 5);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const InlineCall *inlineCall = findProctypeElement<InlineCall>(inlineDef->getSequence(), 0);
        QVERIFY(inlineCall != nullptr);
        inlineCall = findProctypeElement<InlineCall>(inlineDef->getSequence(), 1);
        QVERIFY(inlineCall != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__field1_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 0);
        QVERIFY(assignment != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__field2_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 0);
        QVERIFY(assignment != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__field1_range_check");
        QVERIFY(inlineDef != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__field2_range_check");
        QVERIFY(inlineDef != nullptr);
    }
}

void tst_Asn1ToPromelaTranslator::testSequenceWithOptional()
{
    auto model = createModel();
    auto type = std::make_unique<Sequence>();

    auto field1Type = TypeFactory::createBuiltinType(QStringLiteral("INTEGER"));
    dynamic_cast<Integer *>(field1Type.get())->constraints().append(RangeConstraint<IntegerValue>::create({ 1, 5 }));
    auto component1 = std::make_unique<AsnSequenceComponent>(QStringLiteral("field1"), QStringLiteral("field1"), true,
            std::nullopt, QStringLiteral(""), AsnSequenceComponent::Presence::NotSpecified, SourceLocation(),
            std::move(field1Type));
    type->addComponent(std::move(component1));

    auto field2Type = TypeFactory::createBuiltinType(QStringLiteral("REAL"));
    dynamic_cast<Real *>(field2Type.get())->constraints().append(RangeConstraint<RealValue>::create({ 1, 5 }));
    auto component2 = std::make_unique<AsnSequenceComponent>(QStringLiteral("field2"), QStringLiteral("field2"), true,
            std::nullopt, QStringLiteral(""), AsnSequenceComponent::Presence::NotSpecified, SourceLocation(),
            std::move(field2Type));
    type->addComponent(std::move(component2));

    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 2);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 2);

    const Utype &expectedExistUtype = promelaModel.getUtypes().at(0);
    QCOMPARE(expectedExistUtype.getName(), "MyType_exist");
    QCOMPARE(expectedExistUtype.isUnionType(), false);
    QCOMPARE(expectedExistUtype.getFields().size(), 2);

    const Declaration *expectedField1 = findUtypeField(expectedExistUtype, 0);
    QVERIFY(expectedField1);
    QCOMPARE(expectedField1->getName(), "field1");
    QVERIFY(expectedField1->getType().isBasicType());
    QCOMPARE(expectedField1->getType().getBasicType(), BasicType::BOOLEAN);

    const Declaration *expectedField2 = findUtypeField(expectedExistUtype, 1);
    QVERIFY(expectedField2);

    QCOMPARE(expectedField2->getName(), "field2");
    QVERIFY(expectedField2->getType().isBasicType());
    QCOMPARE(expectedField2->getType().getBasicType(), BasicType::BOOLEAN);

    const Utype &expectedUtype = promelaModel.getUtypes().at(1);
    QCOMPARE(expectedUtype.isUnionType(), false);
    QCOMPARE(expectedUtype.getName(), "MyType");
    QCOMPARE(expectedUtype.getFields().size(), 3);

    const Declaration *decl1 = findUtypeField(expectedUtype, 0);
    QVERIFY(decl1);
    QCOMPARE("field1", decl1->getName());
    QVERIFY(decl1->getType().isUtypeReference());
    QCOMPARE(decl1->getType().getUtypeReference().getName(), "MyType__field1");

    const Declaration *decl2 = findUtypeField(expectedUtype, 1);
    QVERIFY(decl2);
    QCOMPARE("field2", decl2->getName());
    QVERIFY(decl2->getType().isUtypeReference());
    QCOMPARE(decl2->getType().getUtypeReference().getName(), "MyType__field2");

    const Declaration *decl3 = findUtypeField(expectedUtype, 2);
    QVERIFY(decl3);
    QCOMPARE("exist", decl3->getName());
    QVERIFY(decl3->getType().isUtypeReference());
    QCOMPARE(decl3->getType().getUtypeReference().getName(), "MyType_exist");

    const TypeAlias &expectedAlias1 = promelaModel.getTypeAliases().at(0);
    QCOMPARE(expectedAlias1.getName(), "MyType__field1");
    QVERIFY(std::holds_alternative<BasicType>(expectedAlias1.getType()));
    QCOMPARE(std::get<BasicType>(expectedAlias1.getType()), BasicType::INT);

    const TypeAlias &expectedAlias2 = promelaModel.getTypeAliases().at(1);
    QCOMPARE(expectedAlias2.getName(), "MyType__field2");
    QVERIFY(std::holds_alternative<BasicType>(expectedAlias2.getType()));
    QCOMPARE(std::get<BasicType>(expectedAlias2.getType()), BasicType::FLOAT);

    QCOMPARE(promelaModel.getInlineDefs().size(), 5);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 4);
        const InlineCall *inlineCall = findProctypeElement<InlineCall>(inlineDef->getSequence(), 0);
        QVERIFY(inlineCall != nullptr);
        inlineCall = findProctypeElement<InlineCall>(inlineDef->getSequence(), 1);
        QVERIFY(inlineCall != nullptr);
        const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 2);
        QVERIFY(assignment != nullptr);
        {
            QCOMPARE(assignment->getVariableRef().getElements().size(), 3);
            auto iter = assignment->getVariableRef().getElements().begin();
            QVERIFY(iter->m_index.get() == nullptr);
            QCOMPARE(iter->m_name, "dst");
            ++iter;

            QVERIFY(iter->m_index.get() == nullptr);
            QCOMPARE(iter->m_name, "exist");
            ++iter;

            QVERIFY(iter->m_index.get() == nullptr);
            QCOMPARE(iter->m_name, "field1");

            QVERIFY(std::holds_alternative<VariableRef>(assignment->getExpression().getContent()));
            const VariableRef &rhs = std::get<VariableRef>(assignment->getExpression().getContent());
            QCOMPARE(rhs.getElements().size(), 3);
            iter = rhs.getElements().begin();
            QVERIFY(iter->m_index.get() == nullptr);
            QCOMPARE(iter->m_name, "src");
            ++iter;

            QVERIFY(iter->m_index.get() == nullptr);
            QCOMPARE(iter->m_name, "exist");
            ++iter;

            QVERIFY(iter->m_index.get() == nullptr);
            QCOMPARE(iter->m_name, "field1");
        }

        assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 3);
        QVERIFY(assignment != nullptr);
        {
            QCOMPARE(assignment->getVariableRef().getElements().size(), 3);
            auto iter = assignment->getVariableRef().getElements().begin();
            QVERIFY(iter->m_index.get() == nullptr);
            QCOMPARE(iter->m_name, "dst");
            ++iter;

            QVERIFY(iter->m_index.get() == nullptr);
            QCOMPARE(iter->m_name, "exist");
            ++iter;

            QVERIFY(iter->m_index.get() == nullptr);
            QCOMPARE(iter->m_name, "field2");

            QVERIFY(std::holds_alternative<VariableRef>(assignment->getExpression().getContent()));
            const VariableRef &rhs = std::get<VariableRef>(assignment->getExpression().getContent());
            QCOMPARE(rhs.getElements().size(), 3);
            iter = rhs.getElements().begin();
            QVERIFY(iter->m_index.get() == nullptr);
            QCOMPARE(iter->m_name, "src");
            ++iter;

            QVERIFY(iter->m_index.get() == nullptr);
            QCOMPARE(iter->m_name, "exist");
            ++iter;

            QVERIFY(iter->m_index.get() == nullptr);
            QCOMPARE(iter->m_name, "field2");
        }
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__field1_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 0);
        QVERIFY(assignment != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__field2_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 0);
        QVERIFY(assignment != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__field1_range_check");
        QVERIFY(inlineDef != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__field2_range_check");
        QVERIFY(inlineDef != nullptr);
    }
}

void tst_Asn1ToPromelaTranslator::testNestedSequence()
{
    auto model = createModel();
    auto level0 = std::make_unique<Sequence>();
    auto level1 = std::make_unique<Sequence>();
    auto level2 = std::make_unique<Sequence>();

    auto fieldType = TypeFactory::createBuiltinType(QStringLiteral("INTEGER"));
    dynamic_cast<Integer *>(fieldType.get())->constraints().append(RangeConstraint<IntegerValue>::create({ 1, 5 }));
    auto level2Component = std::make_unique<AsnSequenceComponent>(QStringLiteral("field"), QStringLiteral("field"),
            false, std::nullopt, QStringLiteral(""), AsnSequenceComponent::Presence::NotSpecified, SourceLocation(),
            std::move(fieldType));

    level2->addComponent(std::move(level2Component));

    auto level1Component = std::make_unique<AsnSequenceComponent>(QStringLiteral("level2"), QStringLiteral("level2"),
            false, std::nullopt, QStringLiteral(""), AsnSequenceComponent::Presence::NotSpecified, SourceLocation(),
            std::move(level2));

    level1->addComponent(std::move(level1Component));

    auto level0Component = std::make_unique<AsnSequenceComponent>(QStringLiteral("level1"), QStringLiteral("level1"),
            false, std::nullopt, QStringLiteral(""), AsnSequenceComponent::Presence::NotSpecified, SourceLocation(),
            std::move(level1));

    level0->addComponent(std::move(level0Component));

    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("Level0"), QStringLiteral("Level0T"), SourceLocation(), std::move(level0));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 1);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    QCOMPARE(promelaModel.getUtypes().size(), 3);

    {
        const Utype &expectedLevel2 = promelaModel.getUtypes().at(0);
        QCOMPARE(expectedLevel2.isUnionType(), false);
        QCOMPARE(expectedLevel2.getName(), "Level0__level1__level2");
        QCOMPARE(expectedLevel2.getFields().size(), 1);
        const Declaration *expectedLevel2Field = findUtypeField(expectedLevel2, 0);
        QVERIFY(expectedLevel2Field);
        QCOMPARE(expectedLevel2Field->getName(), "field");
        QVERIFY(expectedLevel2Field->getType().isUtypeReference());
        QCOMPARE(expectedLevel2Field->getType().getUtypeReference().getName(), "Level0__level1__level2__field");
    }

    {
        const Utype &expectedLevel1 = promelaModel.getUtypes().at(1);
        QCOMPARE(expectedLevel1.isUnionType(), false);
        QCOMPARE(expectedLevel1.getName(), "Level0__level1");
        QCOMPARE(expectedLevel1.getFields().size(), 1);
        const Declaration *expectedLevel1Field = findUtypeField(expectedLevel1, 0);
        QVERIFY(expectedLevel1Field);
        QCOMPARE(expectedLevel1Field->getName(), "level2");
        QVERIFY(expectedLevel1Field->getType().isUtypeReference());
        QCOMPARE(expectedLevel1Field->getType().getUtypeReference().getName(), "Level0__level1__level2");
    }

    {
        const Utype &expectedLevel0 = promelaModel.getUtypes().at(2);
        QCOMPARE(expectedLevel0.isUnionType(), false);
        QCOMPARE(expectedLevel0.getName(), "Level0");
        QCOMPARE(expectedLevel0.getFields().size(), 1);
        const Declaration *expectedLevel0Field = findUtypeField(expectedLevel0, 0);
        QVERIFY(expectedLevel0Field);
        QCOMPARE(expectedLevel0Field->getName(), "level1");
        QVERIFY(expectedLevel0Field->getType().isUtypeReference());
        QCOMPARE(expectedLevel0Field->getType().getUtypeReference().getName(), "Level0__level1");
    }

    const TypeAlias &expectedAlias = promelaModel.getTypeAliases().at(0);
    QCOMPARE(expectedAlias.getName(), "Level0__level1__level2__field");
    QVERIFY(std::holds_alternative<BasicType>(expectedAlias.getType()));
    QCOMPARE(std::get<BasicType>(expectedAlias.getType()), BasicType::INT);

    QCOMPARE(promelaModel.getInlineDefs().size(), 5);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "Level0_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 1);
        const InlineCall *inlineCall = findProctypeElement<InlineCall>(inlineDef->getSequence(), 0);
        QVERIFY(inlineCall != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "Level0__level1_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 1);
        const InlineCall *inlineCall = findProctypeElement<InlineCall>(inlineDef->getSequence(), 0);
        QVERIFY(inlineCall != nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "Level0__level1__level2_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 1);
        const InlineCall *inlineCall = findProctypeElement<InlineCall>(inlineDef->getSequence(), 0);
        QVERIFY(inlineCall != nullptr);
    }
    {
        const InlineDef *inlineDef =
                findInline(promelaModel.getInlineDefs(), "Level0__level1__level2__field_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 0);
        QVERIFY(assignment != nullptr);
    }
}

void tst_Asn1ToPromelaTranslator::testVariableSequenceOf()
{
    const int EXPECTED_SIZE = 16;

    auto model = createModel();
    auto type = std::make_unique<SequenceOf>();
    auto itemsType = TypeFactory::createBuiltinType(QStringLiteral("INTEGER"));
    dynamic_cast<Integer *>(itemsType.get())->constraints().append(RangeConstraint<IntegerValue>::create({ 1, 5 }));
    type->setItemsType(std::move(itemsType));
    auto sizeConstraint = std::make_unique<SizeConstraint<IntegerValue>>(
            RangeConstraint<IntegerValue>::create(Range<int64_t>({ 1, EXPECTED_SIZE })));
    type->constraints().append(std::move(sizeConstraint));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 1);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);
    QCOMPARE(expectedUtype.getName(), "MyType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 2);

    const Declaration *expectedData = findUtypeField(expectedUtype, 0);
    QVERIFY(expectedData);
    QCOMPARE(expectedData->getName(), "data");
    QVERIFY(expectedData->getType().isArrayType());
    QCOMPARE(expectedData->getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<UtypeRef>(expectedData->getType().getArrayType().getType()));
    const UtypeRef &expectedRef = std::get<UtypeRef>(expectedData->getType().getArrayType().getType());
    QCOMPARE(expectedRef.getName(), "MyType__elem");

    const Declaration *expectedLength = findUtypeField(expectedUtype, 1);
    QVERIFY(expectedLength);

    QCOMPARE(expectedLength->getName(), "length");
    QVERIFY(expectedLength->getType().isBasicType());
    QCOMPARE(expectedLength->getType().getBasicType(), BasicType::INT);

    QCOMPARE(promelaModel.getTypeAliases().at(0).getName(), "MyType__elem");
    QVERIFY(std::holds_alternative<BasicType>(promelaModel.getTypeAliases().at(0).getType()));
    QCOMPARE(std::get<BasicType>(promelaModel.getTypeAliases().at(0).getType()), BasicType::INT);

    QCOMPARE(promelaModel.getInlineDefs().size(), 5);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 4);
        const Declaration *decl = findProctypeElement<Declaration>(inlineDef->getSequence(), 0);
        QVERIFY(decl != nullptr);
        const ForLoop *dataLoop = findProctypeElement<ForLoop>(inlineDef->getSequence(), 1);
        QVERIFY(dataLoop != nullptr);
        QCOMPARE(dataLoop->getType(), ForLoop::Type::RANGE);
        QCOMPARE(dataLoop->getFirstIntValue(), 0);
        const auto dataLoopLastExpression = std::get_if<BinaryExpression>(&dataLoop->getLastExpression().getContent());
        QVERIFY(dataLoopLastExpression != nullptr);
        QCOMPARE(dataLoopLastExpression->getOperator(), BinaryExpression::Operator::SUBTRACT);
        QVERIFY(dataLoopLastExpression->getLeft() != nullptr);
        const auto dataLoopLastExpressionLeft =
                std::get_if<VariableRef>(&dataLoopLastExpression->getLeft()->getContent());
        QVERIFY(dataLoopLastExpressionLeft != nullptr);
        auto iter = dataLoopLastExpressionLeft->getElements().begin();
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "src");
        ++iter;
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "length");
        QVERIFY(dataLoopLastExpression->getRight() != nullptr);
        const auto dataLoopLastExpressionRight =
                std::get_if<Constant>(&dataLoopLastExpression->getRight()->getContent());
        QVERIFY(dataLoopLastExpressionRight != nullptr);
        QCOMPARE(dataLoopLastExpressionRight->getValue(), 1);
        const ForLoop *zeroLoop = findProctypeElement<ForLoop>(inlineDef->getSequence(), 2);
        QVERIFY(zeroLoop != nullptr);
        QCOMPARE(zeroLoop->getType(), ForLoop::Type::RANGE);
        const auto zeroLoopFirstExpression = std::get_if<VariableRef>(&zeroLoop->getFirstExpression().getContent());
        QVERIFY(zeroLoopFirstExpression != nullptr);
        iter = zeroLoopFirstExpression->getElements().begin();
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "src");
        ++iter;
        QVERIFY(iter->m_index.get() == nullptr);
        QCOMPARE(iter->m_name, "length");
        QCOMPARE(zeroLoop->getLastIntValue(), EXPECTED_SIZE - 1);
        const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 3);
        QVERIFY(assignment != nullptr);
    }

    QCOMPARE(promelaModel.getInlineDefs().size(), 5);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__elem_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 0);
        QVERIFY(assignment != nullptr);
    }
}

void tst_Asn1ToPromelaTranslator::testFixedSequenceOf()
{
    const int EXPECTED_SIZE = 16;

    auto model = createModel();
    auto type = std::make_unique<SequenceOf>();
    auto itemsType = TypeFactory::createBuiltinType(QStringLiteral("INTEGER"));
    dynamic_cast<Integer *>(itemsType.get())->constraints().append(RangeConstraint<IntegerValue>::create({ 1, 5 }));
    type->setItemsType(std::move(itemsType));
    auto sizeConstraint = std::make_unique<SizeConstraint<IntegerValue>>(
            RangeConstraint<IntegerValue>::create(Range<int64_t>(EXPECTED_SIZE)));
    type->constraints().append(std::move(sizeConstraint));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 1);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);
    QCOMPARE(expectedUtype.getName(), "MyType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 1);

    const Declaration *expectedData = findUtypeField(expectedUtype, 0);
    QVERIFY(expectedData);

    QCOMPARE(expectedData->getName(), "data");
    QVERIFY(expectedData->getType().isArrayType());
    QCOMPARE(expectedData->getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<UtypeRef>(expectedData->getType().getArrayType().getType()));
    const UtypeRef &expectedRef = std::get<UtypeRef>(expectedData->getType().getArrayType().getType());
    QCOMPARE(expectedRef.getName(), "MyType__elem");

    QCOMPARE(promelaModel.getTypeAliases().at(0).getName(), "MyType__elem");
    QVERIFY(std::holds_alternative<BasicType>(promelaModel.getTypeAliases().at(0).getType()));
    QCOMPARE(std::get<BasicType>(promelaModel.getTypeAliases().at(0).getType()), BasicType::INT);

    QCOMPARE(promelaModel.getInlineDefs().size(), 4);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const Declaration *decl = findProctypeElement<Declaration>(inlineDef->getSequence(), 0);
        QVERIFY(decl != nullptr);
        const ForLoop *loop = findProctypeElement<ForLoop>(inlineDef->getSequence(), 1);
        QVERIFY(loop != nullptr);
        QCOMPARE(loop->getType(), ForLoop::Type::RANGE);
        QCOMPARE(loop->getFirstIntValue(), 0);
        QCOMPARE(loop->getLastIntValue(), EXPECTED_SIZE - 1);
    }

    QCOMPARE(promelaModel.getInlineDefs().size(), 4);
    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyType__elem_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);
        QCOMPARE(inlineDef->getSequence().getContent().size(), 2);
        const Assignment *assignment = findProctypeElement<Assignment>(inlineDef->getSequence(), 0);
        QVERIFY(assignment != nullptr);
    }
}

void tst_Asn1ToPromelaTranslator::testUserDefinedType()
{
    auto model = createModel();

    {
        auto integerType = TypeFactory::createBuiltinType("INTEGER");
        dynamic_cast<Integer *>(integerType.get())
                ->constraints()
                .append(RangeConstraint<IntegerValue>::create({ 1, 5 }));
        auto myIntegerAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("MyInteger"), QStringLiteral("MyIntegerT"), SourceLocation(), integerType->clone());
        model->addType(std::move(myIntegerAssignment));

        auto typeAlias = std::make_unique<UserdefinedType>("MyIntegerAlias", "");
        typeAlias->setType(integerType->clone());
        auto typeAliasAssignment = std::make_unique<TypeAssignment>(QStringLiteral("MyIntegerAlias"),
                QStringLiteral("MyIntegerAliasT"), SourceLocation(), std::move(typeAlias));
        model->addType(std::move(typeAliasAssignment));
    }

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 2);

    const QList<TypeAlias> aliases = promelaModel.getTypeAliases();

    const TypeAlias &expectedInteger = aliases.at(0);
    QCOMPARE(expectedInteger.getName(), "MyInteger");
    QVERIFY(std::holds_alternative<BasicType>(expectedInteger.getType()));
    QCOMPARE(std::get<BasicType>(expectedInteger.getType()), BasicType::INT);

    const TypeAlias &expectedAlias = aliases.at(1);
    QCOMPARE(expectedAlias.getName(), "MyIntegerAlias");
    QVERIFY(std::holds_alternative<BasicType>(expectedAlias.getType()));
    QCOMPARE(std::get<BasicType>(expectedAlias.getType()), BasicType::INT);

    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyInteger_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);

        const auto &sequence = inlineDef->getSequence();
        QCOMPARE(sequence.getContent().size(), 2);

        const auto assignment = findProctypeElement<Assignment>(sequence, 0);
        QVERIFY(assignment != nullptr);

        const auto rangeCheckCall = findProctypeElement<InlineCall>(sequence, 1);
        QVERIFY(rangeCheckCall != nullptr);
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel.getInlineDefs(), "MyIntegerAlias_assign_value");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 2);

        const auto &sequence = inlineDef->getSequence();
        QCOMPARE(sequence.getContent().size(), 2);

        const auto assignment = findProctypeElement<Assignment>(sequence, 0);
        QVERIFY(assignment != nullptr);

        const auto rangeCheckCall = findProctypeElement<InlineCall>(sequence, 1);
        QVERIFY(rangeCheckCall != nullptr);
    }
}

void tst_Asn1ToPromelaTranslator::testTypeSorting()
{
    auto model = createModel();

    auto secondType = std::make_unique<Sequence>("MyTypeSecond");

    auto secondTypeComponentType = TypeFactory::createBuiltinType("INTEGER");
    dynamic_cast<Integer *>(secondTypeComponentType.get())
            ->constraints()
            .append(RangeConstraint<IntegerValue>::create({ 1, 5 }));
    auto secondTypeComponentTypeAssignment = std::make_unique<TypeAssignment>(QStringLiteral("MyInteger"),
            QStringLiteral("MyIntegerT"), SourceLocation(), secondTypeComponentType->clone());
    model->addType(std::move(secondTypeComponentTypeAssignment));

    auto secondTypeComponentTypeReference = std::make_unique<UserdefinedType>("MyIntegerAlias", "");
    secondTypeComponentTypeReference->setType(secondTypeComponentType->clone());
    auto secondTypeComponentTypeReferenceAssignment = std::make_unique<TypeAssignment>(
            "MyIntegerAlias", "MyIntegerAliasT", SourceLocation(), secondTypeComponentTypeReference->clone());
    model->addType(std::move(secondTypeComponentTypeReferenceAssignment));

    auto secondTypeComponent = std::make_unique<AsnSequenceComponent>("field2", "field2", false, std::nullopt, "",
            AsnSequenceComponent::Presence::NotSpecified, SourceLocation(), secondTypeComponentTypeReference->clone());
    secondType->addComponent(std::move(secondTypeComponent));

    auto secondTypeAssignment =
            std::make_unique<TypeAssignment>("MyTypeSecond", "MyTypeSecondT", SourceLocation(), std::move(secondType));
    model->addType(std::move(secondTypeAssignment));

    auto firstType = std::make_unique<Sequence>("MyTypeFirst");

    auto firstTypeComponentType = TypeFactory::createBuiltinType("INTEGER");
    dynamic_cast<Integer *>(firstTypeComponentType.get())
            ->constraints()
            .append(RangeConstraint<IntegerValue>::create({ 1, 5 }));
    auto firstTypeComponent = std::make_unique<AsnSequenceComponent>("field1", "field1", false, std::nullopt, "",
            AsnSequenceComponent::Presence::NotSpecified, SourceLocation(), std::move(firstTypeComponentType));
    firstType->addComponent(std::move(firstTypeComponent));

    auto firstTypeAssignment =
            std::make_unique<TypeAssignment>("MyTypeFirst", "MyTypeSecondT", SourceLocation(), std::move(firstType));
    model->addType(std::move(firstTypeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model);

    PromelaTypeSorter typeSorter;
    typeSorter.sortTypeDefinitions(promelaModel);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 4);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 2);

    {
        const Utype &expectedUtype = promelaModel.getUtypes().front();
        QCOMPARE(expectedUtype.isUnionType(), false);
        QCOMPARE(expectedUtype.getName(), "MyTypeFirst");
        QCOMPARE(expectedUtype.getFields().size(), 1);
    }

    {
        const Utype &expectedUtype = promelaModel.getUtypes().back();
        QCOMPARE(expectedUtype.isUnionType(), false);
        QCOMPARE(expectedUtype.getName(), "MyTypeSecond");
        QCOMPARE(expectedUtype.getFields().size(), 1);
    }
}

void tst_Asn1ToPromelaTranslator::testSequenceSubtyping()
{
    const QString inputAsnFilename = "subtyping.asn";

    auto model = plugincommon::ModelLoader::loadAsn1Model(
            QString("resources%1%2").arg(QDir::separator()).arg(inputAsnFilename));
    QVERIFY(model != nullptr);
    QVERIFY(!model->data().empty());

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel, true);
    visitor.visit(*model->data().front());

    QCOMPARE(promelaModel.getUtypes().size(), 6);

    const Utype *base = findUtype(promelaModel, "MyBase");
    QVERIFY(base);
    QCOMPARE(base->getFields().size(), 3);
    {
        const Declaration *abc = findUtypeField(*base, 0);
        QVERIFY(abc);
        QCOMPARE(abc->getName(), "abc");
        const Declaration *test = findUtypeField(*base, 1);
        QVERIFY(test);
        QCOMPARE(test->getName(), "test");
        const Declaration *exist = findUtypeField(*base, 2);
        QVERIFY(exist);
        QCOMPARE(exist->getName(), "exist");
    }

    const Utype *baseExist = findUtype(promelaModel, "MyBase_exist");
    QVERIFY(baseExist);
    QCOMPARE(baseExist->getFields().size(), 1);
    {
        const Declaration *test = findUtypeField(*baseExist, 0);
        QVERIFY(test);
        QCOMPARE(test->getName(), "test");
    }

    const Utype *childA = findUtype(promelaModel, "MyChildA");
    QVERIFY(childA);
    QCOMPARE(childA->getFields().size(), 3);
    {
        const Declaration *abc = findUtypeField(*childA, 0);
        QVERIFY(abc);
        QCOMPARE(abc->getName(), "abc");
        const Declaration *test = findUtypeField(*childA, 1);
        QVERIFY(test);
        QCOMPARE(test->getName(), "test");
        const Declaration *exist = findUtypeField(*childA, 2);
        QVERIFY(exist);
        QCOMPARE(exist->getName(), "exist");
    }

    const Utype *childAExist = findUtype(promelaModel, "MyChildA_exist");
    QVERIFY(childAExist);
    QCOMPARE(childAExist->getFields().size(), 1);
    {
        const Declaration *test = findUtypeField(*childAExist, 0);
        QVERIFY(test);
        QCOMPARE(test->getName(), "test");
    }

    const Utype *childB = findUtype(promelaModel, "MyChildB");
    QVERIFY(childB);
    QCOMPARE(childB->getFields().size(), 3);
    {
        const Declaration *abc = findUtypeField(*childB, 0);
        QVERIFY(abc);
        QCOMPARE(abc->getName(), "abc");
        const Declaration *test = findUtypeField(*childB, 1);
        QVERIFY(test);
        QCOMPARE(test->getName(), "test");
        const Declaration *exist = findUtypeField(*childB, 2);
        QVERIFY(exist);
        QCOMPARE(exist->getName(), "exist");
    }

    const Utype *childBExist = findUtype(promelaModel, "MyChildB_exist");
    QVERIFY(childBExist);
    QCOMPARE(childBExist->getFields().size(), 1);
    {
        const Declaration *test = findUtypeField(*childBExist, 0);
        QVERIFY(test);
        QCOMPARE(test->getName(), "test");
    }

    {
        const InlineDef *childAAssignValue = findInline(promelaModel.getInlineDefs(), "MyChildA_assign_value");
        QVERIFY(childAAssignValue);
        QCOMPARE(childAAssignValue->getArguments().size(), 2);
        const auto &sequence = childAAssignValue->getSequence();
        QCOMPARE(sequence.getContent().size(), 4);

        const auto abcFieldAssignment = findProctypeElement<InlineCall>(sequence, 0);
        QVERIFY(abcFieldAssignment);
        const auto testFieldAssignment = findProctypeElement<InlineCall>(sequence, 1);
        QVERIFY(testFieldAssignment);
        const auto existAssignment = findProctypeElement<Assignment>(sequence, 2);
        QVERIFY(existAssignment);
        const auto check = findProctypeElement<AssertCall>(sequence, 3);
        QVERIFY(check);
        const BinaryExpression *expr = std::get_if<BinaryExpression>(&check->expression().getContent());
        QVERIFY(expr);
        QCOMPARE(expr->getOperator(), BinaryExpression::Operator::EQUAL);

        const VariableRef *expectedRef = std::get_if<VariableRef>(&expr->getLeft()->getContent());
        QVERIFY(expectedRef);

        const Constant *expectedOne = std::get_if<Constant>(&expr->getRight()->getContent());
        QVERIFY(expectedOne);
        QCOMPARE(expectedOne->getValue(), 1);
    }

    {
        const InlineDef *childBAssignValue = findInline(promelaModel.getInlineDefs(), "MyChildB_assign_value");
        QVERIFY(childBAssignValue);
        QCOMPARE(childBAssignValue->getArguments().size(), 2);
        const auto &sequence = childBAssignValue->getSequence();
        QCOMPARE(sequence.getContent().size(), 4);

        const auto abcFieldAssignment = findProctypeElement<InlineCall>(sequence, 0);
        QVERIFY(abcFieldAssignment);
        const auto testFieldAssignment = findProctypeElement<InlineCall>(sequence, 1);
        QVERIFY(testFieldAssignment);
        const auto existAssignment = findProctypeElement<Assignment>(sequence, 2);
        QVERIFY(existAssignment);
        const auto check = findProctypeElement<AssertCall>(sequence, 3);
        QVERIFY(check);
        const BinaryExpression *expr = std::get_if<BinaryExpression>(&check->expression().getContent());
        QVERIFY(expr);
        QCOMPARE(expr->getOperator(), BinaryExpression::Operator::EQUAL);

        const VariableRef *expectedRef = std::get_if<VariableRef>(&expr->getLeft()->getContent());
        QVERIFY(expectedRef);

        const Constant *expectedZero = std::get_if<Constant>(&expr->getRight()->getContent());
        QVERIFY(expectedZero);
        QCOMPARE(expectedZero->getValue(), 0);
    }
}

std::unique_ptr<Definitions> tst_Asn1ToPromelaTranslator::createModel()
{
    return std::make_unique<Definitions>("myModule", SourceLocation());
}

const InlineDef *tst_Asn1ToPromelaTranslator::findInline(
        const std::list<std::unique_ptr<InlineDef>> &list, const QString &name)
{
    auto iter = std::find_if(list.begin(), list.end(),
            [&](const std::unique_ptr<InlineDef> &proctype) { return proctype->getName() == name; });

    if (iter == list.end()) {
        return nullptr;
    }
    return iter->get();
}

const ::promela::model::Declaration *tst_Asn1ToPromelaTranslator::findUtypeField(
        const ::promela::model::Utype &utype, int index)
{
    int current = 0;
    for (const Utype::Element &field : utype.getFields()) {
        if (current == index) {
            if (std::holds_alternative<Declaration>(field)) {
                return &std::get<Declaration>(field);
            } else {
                return nullptr;
            }
        }
        ++current;
    }
    return nullptr;
}

const ::promela::model::Utype *tst_Asn1ToPromelaTranslator::findUtype(
        const ::promela::model::PromelaModel &model, const QString &name)
{
    const QList<Utype> &utypes = model.getUtypes();
    auto iter = std::find_if(utypes.begin(), utypes.end(), [&](const Utype &utype) { return utype.getName() == name; });
    if (iter != utypes.end()) {
        return &(*iter);
    }
    return nullptr;
}

template<typename T>
const T *tst_Asn1ToPromelaTranslator::findProctypeElement(const ::promela::model::Sequence &sequence, size_t index)
{
    auto iter = sequence.getContent().begin();
    std::advance(iter, index);
    if (iter == sequence.getContent().end()) {
        return nullptr;
    }
    if (std::holds_alternative<T>((*iter)->getValue())) {
        return &(std::get<T>((*iter)->getValue()));
    }
    return nullptr;
}

}
