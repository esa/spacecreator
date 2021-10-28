/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/typefactory.h>
#include <conversion/tmc/Asn1ToPromelaTranslator/translator.h>
#include <conversion/tmc/Asn1ToPromelaTranslator/visitors/asn1nodevisitor.h>
#include <tmc/PromelaModel/promelamodel.h>

using Asn1Acn::AsnSequenceComponent;
using Asn1Acn::BitStringValue;
using Asn1Acn::Definitions;
using Asn1Acn::IntegerValue;
using Asn1Acn::OctetStringValue;
using Asn1Acn::Range;
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
using Asn1Acn::Types::OctetString;
using Asn1Acn::Types::Sequence;
using Asn1Acn::Types::SequenceOf;
using Asn1Acn::Types::Type;
using Asn1Acn::Types::TypeFactory;
using conversion::tmc::translator::Asn1NodeVisitor;
using conversion::tmc::translator::Asn1ToPromelaTranslator;
using tmc::promela::model::BasicType;
using tmc::promela::model::Declaration;
using tmc::promela::model::PromelaModel;
using tmc::promela::model::TypeAlias;
using tmc::promela::model::Utype;
using tmc::promela::model::UtypeRef;
using tmc::promela::model::ValueDefinition;

namespace tmc::test {

class tst_Asn1ToPromelaTranslator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testBasicTypes();
    void testEnumerated();

    void testVariableBitString();
    void testFixedBitString();
    void testVariableOctetString();
    void testFixedOctetString();
    void testVariableIA5String();
    void testFixedIA5String();

    void testChoice();
    void testSequence();
    void testSequenceWithOptional();
    void testNestedSequence();

    void testVariableSequenceOf();
    void testFixedSequenceOf();

private:
    std::unique_ptr<Definitions> createModel();
};

void tst_Asn1ToPromelaTranslator::initTestCase() {}

void tst_Asn1ToPromelaTranslator::cleanupTestCase() {}

void tst_Asn1ToPromelaTranslator::testBasicTypes()
{
    auto model = createModel();

    {
        auto integerType = TypeFactory::createBuiltinType("INTEGER");
        auto myIntegerAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("myInteger"), QStringLiteral("myIntegerT"), SourceLocation(), std::move(integerType));
        model->addType(std::move(myIntegerAssignment));
    }

    {
        auto realType = TypeFactory::createBuiltinType("REAL");
        auto myRealAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("myReal"), QStringLiteral("myRealT"), SourceLocation(), std::move(realType));
        model->addType(std::move(myRealAssignment));
    }

    {
        auto booleanType = TypeFactory::createBuiltinType("BOOLEAN");
        auto myBooleanAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("myBoolean"), QStringLiteral("myBooleanT"), SourceLocation(), std::move(booleanType));
        model->addType(std::move(myBooleanAssignment));
    }

    {
        auto nullType = TypeFactory::createBuiltinType("NULL");
        auto myNullAssignment = std::make_unique<TypeAssignment>(
                QStringLiteral("myNull"), QStringLiteral("myNullT"), SourceLocation(), std::move(nullType));
        model->addType(std::move(myNullAssignment));
    }

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 4);

    const QList<TypeAlias> aliases = promelaModel.getTypeAliases();

    const TypeAlias &expectedInteger = aliases.at(0);
    QCOMPARE(expectedInteger.getName(), "myInteger");
    QVERIFY(std::holds_alternative<BasicType>(expectedInteger.getType()));
    QCOMPARE(std::get<BasicType>(expectedInteger.getType()), BasicType::INT);

    const TypeAlias &expectedReal = aliases.at(1);
    QCOMPARE(expectedReal.getName(), "myReal");
    QVERIFY(std::holds_alternative<BasicType>(expectedReal.getType()));
    QCOMPARE(std::get<BasicType>(expectedReal.getType()), BasicType::FLOAT);

    const TypeAlias &expectedBoolean = aliases.at(2);
    QCOMPARE(expectedBoolean.getName(), "myBoolean");
    QVERIFY(std::holds_alternative<BasicType>(expectedBoolean.getType()));
    QCOMPARE(std::get<BasicType>(expectedBoolean.getType()), BasicType::BOOLEAN);

    const TypeAlias &expectedNull = aliases.at(3);
    QCOMPARE(expectedNull.getName(), "myNull");
    QVERIFY(std::holds_alternative<BasicType>(expectedNull.getType()));
    QCOMPARE(std::get<BasicType>(expectedNull.getType()), BasicType::BIT);
}

void tst_Asn1ToPromelaTranslator::testEnumerated()
{
    auto model = createModel();
    std::unique_ptr<Enumerated> type = std::make_unique<Enumerated>();

    type->addItem(EnumeratedItem(0, "zero", 0));
    type->addItem(EnumeratedItem(1, "one", 1));
    type->addItem(EnumeratedItem(2, "infinity", 100));

    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 1);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 3);

    const TypeAlias &expectedTypeAlias = promelaModel.getTypeAliases().at(0);
    QCOMPARE(expectedTypeAlias.getName(), "myType");
    QVERIFY(std::holds_alternative<BasicType>(expectedTypeAlias.getType()));
    QCOMPARE(std::get<BasicType>(expectedTypeAlias.getType()), BasicType::INT);

    const QList<ValueDefinition> &valueDefs = promelaModel.getValueDefinitions();

    const ValueDefinition &expectedZero = valueDefs.at(0);
    QCOMPARE(expectedZero.getName(), "zero");
    QCOMPARE(expectedZero.getValue(), 0);

    const ValueDefinition &expectedOne = valueDefs.at(1);
    QCOMPARE(expectedOne.getName(), "one");
    QCOMPARE(expectedOne.getValue(), 1);

    const ValueDefinition &expectedInfinity = valueDefs.at(2);
    QCOMPARE(expectedInfinity.getName(), "infinity");
    QCOMPARE(expectedInfinity.getValue(), 100);
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
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);
    QCOMPARE(promelaModel.getTypeAliases().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);

    QCOMPARE(expectedUtype.getName(), "myType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 2);

    const Declaration &expectedArray = expectedUtype.getFields().at(0);

    QCOMPARE(expectedArray.getName(), "data");
    QVERIFY(expectedArray.getType().isArrayType());
    QCOMPARE(expectedArray.getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<BasicType>(expectedArray.getType().getArrayType().getType()));
    QCOMPARE(std::get<BasicType>(expectedArray.getType().getArrayType().getType()), BasicType::BIT);

    const Declaration expectedLength = expectedUtype.getFields().at(1);
    QCOMPARE(expectedLength.getName(), "length");
    QVERIFY(expectedLength.getType().isBasicType());
    QCOMPARE(expectedLength.getType().getBasicType(), BasicType::INT);
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
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);
    QCOMPARE(promelaModel.getTypeAliases().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);

    QCOMPARE(expectedUtype.getName(), "myType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 1);

    const Declaration &expectedArray = expectedUtype.getFields().at(0);

    QCOMPARE(expectedArray.getName(), "data");
    QVERIFY(expectedArray.getType().isArrayType());
    QCOMPARE(expectedArray.getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<BasicType>(expectedArray.getType().getArrayType().getType()));
    QCOMPARE(std::get<BasicType>(expectedArray.getType().getArrayType().getType()), BasicType::BIT);
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
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);
    QCOMPARE(promelaModel.getTypeAliases().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);

    QCOMPARE(expectedUtype.getName(), "myType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 2);

    const Declaration &expectedArray = expectedUtype.getFields().at(0);

    QCOMPARE(expectedArray.getName(), "data");
    QVERIFY(expectedArray.getType().isArrayType());
    QCOMPARE(expectedArray.getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<BasicType>(expectedArray.getType().getArrayType().getType()));
    QCOMPARE(std::get<BasicType>(expectedArray.getType().getArrayType().getType()), BasicType::BYTE);

    const Declaration expectedLength = expectedUtype.getFields().at(1);
    QCOMPARE(expectedLength.getName(), "length");
    QVERIFY(expectedLength.getType().isBasicType());
    QCOMPARE(expectedLength.getType().getBasicType(), BasicType::INT);
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
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);
    QCOMPARE(promelaModel.getTypeAliases().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);

    QCOMPARE(expectedUtype.getName(), "myType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 1);

    const Declaration &expectedArray = expectedUtype.getFields().at(0);

    QCOMPARE(expectedArray.getName(), "data");
    QVERIFY(expectedArray.getType().isArrayType());
    QCOMPARE(expectedArray.getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<BasicType>(expectedArray.getType().getArrayType().getType()));
    QCOMPARE(std::get<BasicType>(expectedArray.getType().getArrayType().getType()), BasicType::BYTE);
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
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);
    QCOMPARE(promelaModel.getTypeAliases().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);

    QCOMPARE(expectedUtype.getName(), "myType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 2);

    const Declaration &expectedArray = expectedUtype.getFields().at(0);

    QCOMPARE(expectedArray.getName(), "data");
    QVERIFY(expectedArray.getType().isArrayType());
    QCOMPARE(expectedArray.getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<BasicType>(expectedArray.getType().getArrayType().getType()));
    QCOMPARE(std::get<BasicType>(expectedArray.getType().getArrayType().getType()), BasicType::BYTE);

    const Declaration expectedLength = expectedUtype.getFields().at(1);
    QCOMPARE(expectedLength.getName(), "length");
    QVERIFY(expectedLength.getType().isBasicType());
    QCOMPARE(expectedLength.getType().getBasicType(), BasicType::INT);
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
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);
    QCOMPARE(promelaModel.getTypeAliases().size(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);

    QCOMPARE(expectedUtype.getName(), "myType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 1);

    const Declaration &expectedArray = expectedUtype.getFields().at(0);

    QCOMPARE(expectedArray.getName(), "data");
    QVERIFY(expectedArray.getType().isArrayType());
    QCOMPARE(expectedArray.getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<BasicType>(expectedArray.getType().getArrayType().getType()));
    QCOMPARE(std::get<BasicType>(expectedArray.getType().getArrayType().getType()), BasicType::BYTE);
}

void tst_Asn1ToPromelaTranslator::testChoice()
{
    auto model = createModel();
    auto type = std::make_unique<Choice>();
    type->addComponent(std::make_unique<ChoiceAlternative>(QStringLiteral("ch1"), QStringLiteral(""),
            QStringLiteral(""), QStringLiteral(""), QStringLiteral(""), SourceLocation(),
            TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));

    type->addComponent(std::make_unique<ChoiceAlternative>(QStringLiteral("ch2"), QStringLiteral(""),
            QStringLiteral(""), QStringLiteral(""), QStringLiteral(""), SourceLocation(),
            TypeFactory::createBuiltinType(QStringLiteral("REAL"))));

    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 2);
    QCOMPARE(promelaModel.getTypeAliases().size(), 2);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 3);

    const Utype &expectedDataUtype = promelaModel.getUtypes().at(0);
    QCOMPARE(expectedDataUtype.getName(), "myType_data");
    QVERIFY(expectedDataUtype.isUnionType());

    QCOMPARE(expectedDataUtype.getFields().size(), 2);
    QCOMPARE(expectedDataUtype.getFields().at(0).getName(), "ch1");
    QVERIFY(expectedDataUtype.getFields().at(0).getType().isUtypeReference());
    QCOMPARE(expectedDataUtype.getFields().at(0).getType().getUtypeReference().getName(), "myType_ch1");
    QCOMPARE(expectedDataUtype.getFields().at(1).getName(), "ch2");
    QVERIFY(expectedDataUtype.getFields().at(1).getType().isUtypeReference());
    QCOMPARE(expectedDataUtype.getFields().at(1).getType().getUtypeReference().getName(), "myType_ch2");

    const Utype &expectedUtype = promelaModel.getUtypes().at(1);
    QCOMPARE(expectedUtype.getName(), "myType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 2);

    QCOMPARE(expectedUtype.getFields().at(0).getName(), "data");
    QVERIFY(expectedUtype.getFields().at(0).getType().isUtypeReference());
    QCOMPARE(expectedUtype.getFields().at(0).getType().getUtypeReference().getName(), "myType_data");

    QCOMPARE(expectedUtype.getFields().at(1).getName(), "selection");
    QVERIFY(expectedUtype.getFields().at(1).getType().isBasicType());
    QCOMPARE(expectedUtype.getFields().at(1).getType().getBasicType(), BasicType::INT);

    QCOMPARE(promelaModel.getValueDefinitions().at(0).getName(), "myType_NONE");
    QCOMPARE(promelaModel.getValueDefinitions().at(0).getValue(), 0);
    QCOMPARE(promelaModel.getValueDefinitions().at(1).getName(), "myType_ch1_PRESENT");
    QCOMPARE(promelaModel.getValueDefinitions().at(1).getValue(), 1);
    QCOMPARE(promelaModel.getValueDefinitions().at(2).getName(), "myType_ch2_PRESENT");
    QCOMPARE(promelaModel.getValueDefinitions().at(2).getValue(), 2);

    QCOMPARE(promelaModel.getTypeAliases().at(0).getName(), "myType_ch1");
    QCOMPARE(promelaModel.getTypeAliases().at(1).getName(), "myType_ch2");
}

void tst_Asn1ToPromelaTranslator::testSequence()
{
    auto model = createModel();
    auto type = std::make_unique<Sequence>();
    auto component1 = std::make_unique<AsnSequenceComponent>(QStringLiteral("field1"), QStringLiteral("field1"), false,
            std::nullopt, QStringLiteral(""), SourceLocation(),
            TypeFactory::createBuiltinType(QStringLiteral("INTEGER")));
    type->addComponent(std::move(component1));
    auto component2 = std::make_unique<AsnSequenceComponent>(QStringLiteral("field2"), QStringLiteral("field2"), false,
            std::nullopt, QStringLiteral(""), SourceLocation(), TypeFactory::createBuiltinType(QStringLiteral("REAL")));
    type->addComponent(std::move(component2));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 2);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);

    const Utype &expectedUtype = promelaModel.getUtypes().front();
    QCOMPARE(expectedUtype.isUnionType(), false);
    QCOMPARE(expectedUtype.getName(), "myType");
    QCOMPARE(expectedUtype.getFields().size(), 2);

    const Declaration &decl1 = expectedUtype.getFields().at(0);
    QCOMPARE("field1", decl1.getName());
    QVERIFY(decl1.getType().isUtypeReference());
    QCOMPARE(decl1.getType().getUtypeReference().getName(), "myType_field1");

    const Declaration &decl2 = expectedUtype.getFields().at(1);
    QCOMPARE("field2", decl2.getName());
    QVERIFY(decl2.getType().isUtypeReference());
    QCOMPARE(decl2.getType().getUtypeReference().getName(), "myType_field2");

    const TypeAlias &expectedAlias1 = promelaModel.getTypeAliases().at(0);
    QCOMPARE(expectedAlias1.getName(), "myType_field1");
    QVERIFY(std::holds_alternative<BasicType>(expectedAlias1.getType()));
    QCOMPARE(std::get<BasicType>(expectedAlias1.getType()), BasicType::INT);

    const TypeAlias &expectedAlias2 = promelaModel.getTypeAliases().at(1);
    QCOMPARE(expectedAlias2.getName(), "myType_field2");
    QVERIFY(std::holds_alternative<BasicType>(expectedAlias2.getType()));
    QCOMPARE(std::get<BasicType>(expectedAlias2.getType()), BasicType::FLOAT);
}

void tst_Asn1ToPromelaTranslator::testSequenceWithOptional()
{
    auto model = createModel();
    auto type = std::make_unique<Sequence>();
    auto component1 = std::make_unique<AsnSequenceComponent>(QStringLiteral("field1"), QStringLiteral("field1"), true,
            std::nullopt, QStringLiteral(""), SourceLocation(),
            TypeFactory::createBuiltinType(QStringLiteral("INTEGER")));
    type->addComponent(std::move(component1));
    auto component2 = std::make_unique<AsnSequenceComponent>(QStringLiteral("field2"), QStringLiteral("field2"), true,
            std::nullopt, QStringLiteral(""), SourceLocation(), TypeFactory::createBuiltinType(QStringLiteral("REAL")));
    type->addComponent(std::move(component2));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 2);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 2);

    const Utype &expectedExistUtype = promelaModel.getUtypes().at(0);
    QCOMPARE(expectedExistUtype.getName(), "myType_exist");
    QCOMPARE(expectedExistUtype.isUnionType(), false);
    QCOMPARE(expectedExistUtype.getFields().size(), 2);
    QCOMPARE(expectedExistUtype.getFields().at(0).getName(), "field1");
    QVERIFY(expectedExistUtype.getFields().at(0).getType().isBasicType());
    QCOMPARE(expectedExistUtype.getFields().at(0).getType().getBasicType(), BasicType::BOOLEAN);
    QCOMPARE(expectedExistUtype.getFields().at(1).getName(), "field2");
    QVERIFY(expectedExistUtype.getFields().at(1).getType().isBasicType());
    QCOMPARE(expectedExistUtype.getFields().at(1).getType().getBasicType(), BasicType::BOOLEAN);

    const Utype &expectedUtype = promelaModel.getUtypes().at(1);
    QCOMPARE(expectedUtype.isUnionType(), false);
    QCOMPARE(expectedUtype.getName(), "myType");
    QCOMPARE(expectedUtype.getFields().size(), 3);

    const Declaration &decl1 = expectedUtype.getFields().at(0);
    QCOMPARE("field1", decl1.getName());
    QVERIFY(decl1.getType().isUtypeReference());
    QCOMPARE(decl1.getType().getUtypeReference().getName(), "myType_field1");

    const Declaration &decl2 = expectedUtype.getFields().at(1);
    QCOMPARE("field2", decl2.getName());
    QVERIFY(decl2.getType().isUtypeReference());
    QCOMPARE(decl2.getType().getUtypeReference().getName(), "myType_field2");

    const TypeAlias &expectedAlias1 = promelaModel.getTypeAliases().at(0);
    QCOMPARE(expectedAlias1.getName(), "myType_field1");
    QVERIFY(std::holds_alternative<BasicType>(expectedAlias1.getType()));
    QCOMPARE(std::get<BasicType>(expectedAlias1.getType()), BasicType::INT);

    const TypeAlias &expectedAlias2 = promelaModel.getTypeAliases().at(1);
    QCOMPARE(expectedAlias2.getName(), "myType_field2");
    QVERIFY(std::holds_alternative<BasicType>(expectedAlias2.getType()));
    QCOMPARE(std::get<BasicType>(expectedAlias2.getType()), BasicType::FLOAT);
}

void tst_Asn1ToPromelaTranslator::testNestedSequence()
{
    auto model = createModel();
    auto level0 = std::make_unique<Sequence>();
    auto level1 = std::make_unique<Sequence>();
    auto level2 = std::make_unique<Sequence>();

    auto level2Component = std::make_unique<AsnSequenceComponent>(QStringLiteral("field"), QStringLiteral("field"),
            false, std::nullopt, QStringLiteral(""), SourceLocation(),
            TypeFactory::createBuiltinType(QStringLiteral("INTEGER")));

    level2->addComponent(std::move(level2Component));

    auto level1Component = std::make_unique<AsnSequenceComponent>(QStringLiteral("level2"), QStringLiteral("level2"),
            false, std::nullopt, QStringLiteral(""), SourceLocation(), std::move(level2));

    level1->addComponent(std::move(level1Component));

    auto level0Component = std::make_unique<AsnSequenceComponent>(QStringLiteral("level1"), QStringLiteral("level1"),
            false, std::nullopt, QStringLiteral(""), SourceLocation(), std::move(level1));

    level0->addComponent(std::move(level0Component));

    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("level0"), QStringLiteral("level0T"), SourceLocation(), std::move(level0));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 1);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);

    QCOMPARE(promelaModel.getUtypes().size(), 3);

    {
        const Utype &expectedLevel2 = promelaModel.getUtypes().at(0);
        QCOMPARE(expectedLevel2.isUnionType(), false);
        QCOMPARE(expectedLevel2.getName(), "level0_level1_level2");
        QCOMPARE(expectedLevel2.getFields().size(), 1);
        const Declaration &expectedLevel2Field = expectedLevel2.getFields().front();
        QCOMPARE(expectedLevel2Field.getName(), "field");
        QVERIFY(expectedLevel2Field.getType().isUtypeReference());
        QCOMPARE(expectedLevel2Field.getType().getUtypeReference().getName(), "level0_level1_level2_field");
    }

    {
        const Utype &expectedLevel1 = promelaModel.getUtypes().at(1);
        QCOMPARE(expectedLevel1.isUnionType(), false);
        QCOMPARE(expectedLevel1.getName(), "level0_level1");
        QCOMPARE(expectedLevel1.getFields().size(), 1);
        const Declaration &expectedLevel1Field = expectedLevel1.getFields().front();
        QCOMPARE(expectedLevel1Field.getName(), "level2");
        QVERIFY(expectedLevel1Field.getType().isUtypeReference());
        QCOMPARE(expectedLevel1Field.getType().getUtypeReference().getName(), "level0_level1_level2");
    }

    {
        const Utype &expectedLevel0 = promelaModel.getUtypes().at(2);
        QCOMPARE(expectedLevel0.isUnionType(), false);
        QCOMPARE(expectedLevel0.getName(), "level0");
        QCOMPARE(expectedLevel0.getFields().size(), 1);
        const Declaration &expectedLevel0Field = expectedLevel0.getFields().front();
        QCOMPARE(expectedLevel0Field.getName(), "level1");
        ;
        QVERIFY(expectedLevel0Field.getType().isUtypeReference());
        QCOMPARE(expectedLevel0Field.getType().getUtypeReference().getName(), "level0_level1");
    }

    const TypeAlias &expectedAlias = promelaModel.getTypeAliases().at(0);
    QCOMPARE(expectedAlias.getName(), "level0_level1_level2_field");
    QVERIFY(std::holds_alternative<BasicType>(expectedAlias.getType()));
    QCOMPARE(std::get<BasicType>(expectedAlias.getType()), BasicType::INT);
}

void tst_Asn1ToPromelaTranslator::testVariableSequenceOf()
{
    const int EXPECTED_SIZE = 16;

    auto model = createModel();
    auto type = std::make_unique<SequenceOf>();
    type->setItemsType(TypeFactory::createBuiltinType("INTEGER"));
    auto sizeConstraint = std::make_unique<SizeConstraint<IntegerValue>>(
            RangeConstraint<IntegerValue>::create(Range<int64_t>({ 1, EXPECTED_SIZE })));
    type->constraints().append(std::move(sizeConstraint));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 1);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);
    QCOMPARE(expectedUtype.getName(), "myType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 2);

    const Declaration &expectedData = expectedUtype.getFields().at(0);
    QCOMPARE(expectedData.getName(), "data");
    QVERIFY(expectedData.getType().isArrayType());
    QCOMPARE(expectedData.getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<UtypeRef>(expectedData.getType().getArrayType().getType()));
    const UtypeRef &expectedRef = std::get<UtypeRef>(expectedData.getType().getArrayType().getType());
    QCOMPARE(expectedRef.getName(), "myType_item");

    const Declaration expectedLength = expectedUtype.getFields().at(1);
    QCOMPARE(expectedLength.getName(), "length");
    QVERIFY(expectedLength.getType().isBasicType());
    QCOMPARE(expectedLength.getType().getBasicType(), BasicType::INT);

    QCOMPARE(promelaModel.getTypeAliases().at(0).getName(), "myType_item");
    QVERIFY(std::holds_alternative<BasicType>(promelaModel.getTypeAliases().at(0).getType()));
    QCOMPARE(std::get<BasicType>(promelaModel.getTypeAliases().at(0).getType()), BasicType::INT);
}

void tst_Asn1ToPromelaTranslator::testFixedSequenceOf()
{
    const int EXPECTED_SIZE = 16;

    auto model = createModel();
    auto type = std::make_unique<SequenceOf>();
    type->setItemsType(TypeFactory::createBuiltinType("INTEGER"));
    auto sizeConstraint = std::make_unique<SizeConstraint<IntegerValue>>(
            RangeConstraint<IntegerValue>::create(Range<int64_t>(EXPECTED_SIZE)));
    type->constraints().append(std::move(sizeConstraint));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(promelaModel.getMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getNamedMtypeValues().size(), 0);
    QCOMPARE(promelaModel.getTypeAliases().size(), 1);
    QCOMPARE(promelaModel.getValueDefinitions().size(), 0);
    QCOMPARE(promelaModel.getUtypes().size(), 1);

    const Utype &expectedUtype = promelaModel.getUtypes().at(0);
    QCOMPARE(expectedUtype.getName(), "myType");
    QVERIFY(!expectedUtype.isUnionType());
    QCOMPARE(expectedUtype.getFields().size(), 1);

    const Declaration &expectedData = expectedUtype.getFields().at(0);
    QCOMPARE(expectedData.getName(), "data");
    QVERIFY(expectedData.getType().isArrayType());
    QCOMPARE(expectedData.getType().getArrayType().getSize(), EXPECTED_SIZE);
    QVERIFY(std::holds_alternative<UtypeRef>(expectedData.getType().getArrayType().getType()));
    const UtypeRef &expectedRef = std::get<UtypeRef>(expectedData.getType().getArrayType().getType());
    QCOMPARE(expectedRef.getName(), "myType_item");

    QCOMPARE(promelaModel.getTypeAliases().at(0).getName(), "myType_item");
    QVERIFY(std::holds_alternative<BasicType>(promelaModel.getTypeAliases().at(0).getType()));
    QCOMPARE(std::get<BasicType>(promelaModel.getTypeAliases().at(0).getType()), BasicType::INT);
}

std::unique_ptr<Definitions> tst_Asn1ToPromelaTranslator::createModel()
{
    return std::make_unique<Definitions>("myModule", SourceLocation());
}
}

QTEST_MAIN(tmc::test::tst_Asn1ToPromelaTranslator)

#include "tst_asn1topromelatranslator.moc"
