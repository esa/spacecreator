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
#include <asn1library/asn1/sourcelocation.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/typefactory.h>
#include <conversion/tmc/Asn1ToPromelaTranslator/translator.h>
#include <conversion/tmc/Asn1ToPromelaTranslator/visitors/asn1nodevisitor.h>
#include <tmc/PromelaModel/promelamodel.h>

using Asn1Acn::AsnSequenceComponent;
using Asn1Acn::Definitions;
using Asn1Acn::SourceLocation;
using Asn1Acn::TypeAssignment;
using Asn1Acn::Types::Sequence;
using Asn1Acn::Types::Type;
using Asn1Acn::Types::TypeFactory;
using conversion::tmc::translator::Asn1NodeVisitor;
using conversion::tmc::translator::Asn1ToPromelaTranslator;
using tmc::promela::model::BasicType;
using tmc::promela::model::Declaration;
using tmc::promela::model::PromelaModel;
using tmc::promela::model::TypeAlias;
using tmc::promela::model::Utype;

namespace tmc::test {

class tst_Asn1ToPromelaTranslator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testBasicTypes();
    void testEmptySequence();
    void testIntegerField();
    void testFloatField();
    void testBooleanField();

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

    QCOMPARE(0, promelaModel.getMtypeValues().size());
    QCOMPARE(0, promelaModel.getNamedMtypeValues().size());
    QCOMPARE(0, promelaModel.getUtypes().size());
    QCOMPARE(4, promelaModel.getTypeAliases().size());

    const QList<TypeAlias> aliases = promelaModel.getTypeAliases();

    const TypeAlias &expectedInteger = aliases.at(0);
    QCOMPARE("myInteger", expectedInteger.getName());
    QVERIFY(std::holds_alternative<BasicType>(expectedInteger.getType()));
    QCOMPARE(BasicType::INT, std::get<BasicType>(expectedInteger.getType()));

    const TypeAlias &expectedReal = aliases.at(1);
    QCOMPARE("myReal", expectedReal.getName());
    QVERIFY(std::holds_alternative<BasicType>(expectedReal.getType()));
    QCOMPARE(BasicType::FLOAT, std::get<BasicType>(expectedReal.getType()));

    const TypeAlias &expectedBoolean = aliases.at(2);
    QCOMPARE("myBoolean", expectedBoolean.getName());
    QVERIFY(std::holds_alternative<BasicType>(expectedBoolean.getType()));
    QCOMPARE(BasicType::BOOLEAN, std::get<BasicType>(expectedBoolean.getType()));

    const TypeAlias &expectedNull = aliases.at(3);
    QCOMPARE("myNull", expectedNull.getName());
    QVERIFY(std::holds_alternative<BasicType>(expectedNull.getType()));
    QCOMPARE(BasicType::BIT, std::get<BasicType>(expectedNull.getType()));
}

void tst_Asn1ToPromelaTranslator::testEmptySequence()
{
    auto model = createModel();
    auto type = Asn1Acn::Types::TypeFactory::createBuiltinType("SEQUENCE");
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(0, promelaModel.getMtypeValues().size());
    QCOMPARE(0, promelaModel.getNamedMtypeValues().size());
    QCOMPARE(1, promelaModel.getUtypes().size());
    QCOMPARE(0, promelaModel.getTypeAliases().size());
}

void tst_Asn1ToPromelaTranslator::testIntegerField()
{
    auto model = createModel();
    auto type = std::make_unique<Sequence>();
    auto component = std::make_unique<AsnSequenceComponent>(QStringLiteral("field1"), QStringLiteral("field1"), false,
            std::nullopt, QStringLiteral(""), SourceLocation(),
            TypeFactory::createBuiltinType(QStringLiteral("INTEGER")));
    type->addComponent(std::move(component));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(0, promelaModel.getMtypeValues().size());
    QCOMPARE(0, promelaModel.getNamedMtypeValues().size());
    QCOMPARE(0, promelaModel.getTypeAliases().size());

    QCOMPARE(1, promelaModel.getUtypes().size());
    const Utype &t = promelaModel.getUtypes().front();
    QCOMPARE(false, t.isUnionType());
    QCOMPARE("myType", t.getName());
    QCOMPARE(1, t.getFields().size());
    const Declaration &decl = t.getFields().front();
    QCOMPARE("field1", decl.getName());
    QVERIFY(decl.getType().isBasicType());
    QCOMPARE(BasicType::INT, decl.getType().getBasicType());
}

void tst_Asn1ToPromelaTranslator::testFloatField()
{
    auto model = createModel();
    auto type = std::make_unique<Sequence>();
    auto component = std::make_unique<AsnSequenceComponent>(QStringLiteral("field1"), QStringLiteral("field1"), false,
            std::nullopt, QStringLiteral(""), SourceLocation(), TypeFactory::createBuiltinType(QStringLiteral("REAL")));
    type->addComponent(std::move(component));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(0, promelaModel.getMtypeValues().size());
    QCOMPARE(0, promelaModel.getNamedMtypeValues().size());
    QCOMPARE(0, promelaModel.getTypeAliases().size());

    QCOMPARE(1, promelaModel.getUtypes().size());
    const Utype &t = promelaModel.getUtypes().front();
    QCOMPARE(false, t.isUnionType());
    QCOMPARE("myType", t.getName());
    QCOMPARE(1, t.getFields().size());
    const Declaration &decl = t.getFields().front();
    QCOMPARE("field1", decl.getName());
    QVERIFY(decl.getType().isBasicType());
    QCOMPARE(BasicType::FLOAT, decl.getType().getBasicType());
}

void tst_Asn1ToPromelaTranslator::testBooleanField()
{
    auto model = createModel();
    auto type = std::make_unique<Sequence>();
    auto component = std::make_unique<AsnSequenceComponent>(QStringLiteral("field1"), QStringLiteral("field1"), false,
            std::nullopt, QStringLiteral(""), SourceLocation(),
            TypeFactory::createBuiltinType(QStringLiteral("BOOLEAN")));
    type->addComponent(std::move(component));
    auto typeAssignment = std::make_unique<TypeAssignment>(
            QStringLiteral("myType"), QStringLiteral("myTypeT"), SourceLocation(), std::move(type));
    model->addType(std::move(typeAssignment));

    PromelaModel promelaModel;
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*model);

    QCOMPARE(0, promelaModel.getMtypeValues().size());
    QCOMPARE(0, promelaModel.getNamedMtypeValues().size());
    QCOMPARE(0, promelaModel.getTypeAliases().size());

    QCOMPARE(1, promelaModel.getUtypes().size());
    const Utype &t = promelaModel.getUtypes().front();
    QCOMPARE(false, t.isUnionType());
    QCOMPARE("myType", t.getName());
    QCOMPARE(1, t.getFields().size());
    const Declaration &decl = t.getFields().front();
    QCOMPARE("field1", decl.getName());
    QVERIFY(decl.getType().isBasicType());
    QCOMPARE(BasicType::BOOLEAN, decl.getType().getBasicType());
}

std::unique_ptr<Definitions> tst_Asn1ToPromelaTranslator::createModel()
{
    return std::make_unique<Definitions>("myModule", SourceLocation());
}
}

QTEST_MAIN(tmc::test::tst_Asn1ToPromelaTranslator)

#include "tst_asn1topromelatranslator.moc"
