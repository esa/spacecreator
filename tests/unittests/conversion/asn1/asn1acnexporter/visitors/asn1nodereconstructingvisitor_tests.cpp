/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Asn1Acn - Tool for generating test cases
** based on ASN.1/ACN models and simulating malformed or malicious asn1library/asn1.
**
** Tool was developed under a programme and funded by
** European Space Agency.
**
** This Tool is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Tool is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "asn1nodereconstructingvisitor_tests.h"

#include <QtTest>
#include <asn1library/asn1/acnsequencecomponent.h>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/choicevalue.h>
#include <asn1library/asn1/constraints/constraintlist.h>
#include <asn1library/asn1/constraints/rangeconstraint.h>
#include <asn1library/asn1/constraints/sizeconstraint.h>
#include <asn1library/asn1/multiplevalue.h>
#include <asn1library/asn1/namedvalue.h>
#include <asn1library/asn1/singlevalue.h>
#include <asn1library/asn1/sourcelocation.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/typefactory.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/asn1/Asn1AcnExporter/visitors/asn1nodereconstructingvisitor.h>
// #include <reconstructor.h>

using namespace Asn1Acn;
using namespace Asn1Acn::Tests;

Asn1NodeReconstructingVisitorTests::Asn1NodeReconstructingVisitorTests(QObject *parent)
    : QObject(parent)
{
}

void Asn1NodeReconstructingVisitorTests::test_singleImportedType()
{
    auto defs = createDefinitions(QStringLiteral("MyModule_1"));
    defs->addImportedType(ImportedType(QStringLiteral("MyModule_1"), QStringLiteral("ImportedType_1")));

    auto actual = restoreNode(*defs);

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "IMPORTS\n"
        "ImportedType_1 FROM MyModule_1\n"
        ";\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_multipleImportedTypes()
{
    auto defs = createDefinitions(QStringLiteral("MyModule_1"));

    defs->addImportedType(ImportedType(QStringLiteral("MyModule_1"), QStringLiteral("ImportedType_1")));
    defs->addImportedType(ImportedType(QStringLiteral("MyModule_2"), QStringLiteral("ImportedType_2")));

    auto actual = restoreNode(*defs);

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "IMPORTS\n"
        "ImportedType_1 FROM MyModule_1\n"
        "ImportedType_2 FROM MyModule_2\n"
        ";\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_singleImportedValue()
{
    auto defs = createDefinitions(QStringLiteral("MyModule_1"));
    defs->addImportedValue(ImportedValue(QStringLiteral("MyModule_1"), QStringLiteral("ImportedValue_1")));

    auto actual = restoreNode(*defs);

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "IMPORTS\n"
        "ImportedValue_1 FROM MyModule_1\n"
        ";\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_multipleImportedValues()
{
    auto defs = createDefinitions(QStringLiteral("MyModule_1"));

    defs->addImportedValue(ImportedValue(QStringLiteral("MyModule_1"), QStringLiteral("ImportedValue_1")));
    defs->addImportedValue(ImportedValue(QStringLiteral("MyModule_2"), QStringLiteral("ImportedValue_2")));

    auto actual = restoreNode(*defs);

    // clang-format off
    QString expected =
        "MyModule_1 DEFINITIONS ::= BEGIN\n"
        "IMPORTS\n"
        "ImportedValue_1 FROM MyModule_1\n"
        "ImportedValue_2 FROM MyModule_2\n"
        ";\n"
        "END\n"
        "\n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_valueAssignmentSimpleValue()
{
    auto actual = createSingleValueValueAssignmentValue(QStringLiteral("INTEGER"), QStringLiteral("10"));
    QString expected = "myValue INTEGER ::= 10\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_valueAssignmentBitString()
{
    auto actual = createSingleValueValueAssignmentValue(
            QStringLiteral("BIT_STRING"), QStringLiteral("1010"), BitStringValue::asString);
    QString expected = "myValue BIT STRING ::= '1010'B\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_valueAssignmentBoolean()
{
    auto actual = createSingleValueValueAssignmentValue(
            QStringLiteral("BOOLEAN"), QStringLiteral("true"), BooleanValue::reformatString);
    QString expected = "myValue BOOLEAN ::= TRUE\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_valueAssignmentOctetString()
{
    auto actual = createSingleValueValueAssignmentValue(
            QStringLiteral("OCTET_STRING"), QStringLiteral("1010"), OctetStringValue::asString);
    QString expected = "myValue OCTET STRING ::= '1010'H\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_valueAssignmentIA5String()
{
    auto actual = createSingleValueValueAssignmentValue(
            QStringLiteral("IA5String"), QStringLiteral("MyValue"), StringValue::asString);
    QString expected = "myValue IA5String ::= \"MyValue\"\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_valueAssignmentChoice()
{
    auto actual = createChoiceValueAssignmentValue();

    QString expected = "myChoice CHOICE ::= MyChoice:10\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_valueAssignmentMultipleValues()
{
    auto actual = createMultipleValueValueAssignmentValue();

    QString expected = "mySequence SEQUENCE OF ::= {1, 2, 3}\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_valueAssignmentNamedValue()
{
    auto actual = createNamedValueAssignmentValue();

    QString expected = "mySequence SEQUENCE ::= {v1 1, v2 TRUE, v3 3.0}\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentBooleanAcnParams()
{
    //
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentBoolean()
{
    testSimpleTypeAssignment(QStringLiteral("BOOLEAN"), QStringLiteral("BOOLEAN"));
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentNull()
{
    testSimpleTypeAssignment(QStringLiteral("NULL"), QStringLiteral("NULL"));
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentBitString()
{
    testSimpleTypeAssignment(QStringLiteral("BIT_STRING"), QStringLiteral("BIT STRING"));
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentBitStringWithValue()
{
    auto assignment = createTypeAssignmentWithConstraint(
            QStringLiteral("BIT_STRING"), new Constraints::RangeConstraint<BitStringValue>({ "1111", "1111" }));

    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= BIT STRING('1111'B)\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentOctetString()
{
    testSimpleTypeAssignment(QStringLiteral("OCTET_STRING"), QStringLiteral("OCTET STRING"));
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentOctetStringWithValue()
{
    auto assignment = createTypeAssignmentWithConstraint(
            QStringLiteral("OCTET_STRING"), new Constraints::RangeConstraint<OctetStringValue>({ "1010", "1010" }));

    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= OCTET STRING('1010'H)\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentIA5String()
{
    testSimpleTypeAssignment(QStringLiteral("IA5String"), QStringLiteral("IA5String"));
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentIA5StringWithValue()
{
    auto assignment = createTypeAssignmentWithConstraint(
            QStringLiteral("IA5String"), new Constraints::RangeConstraint<StringValue>({ "TextString", "TextString" }));

    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= IA5String(\"TextString\")\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentNumericString()
{
    testSimpleTypeAssignment(QStringLiteral("NumericString"), QStringLiteral("NumericString"));
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentNumericStringWithValue()
{
    auto assignment = createTypeAssignmentWithConstraint(
            QStringLiteral("NumericString"), new Constraints::RangeConstraint<StringValue>({ "12345678", "12345678" }));

    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= NumericString(\"12345678\")\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentEnumerated()
{
    auto type = std::make_unique<Types::Enumerated>();
    type->addItem(Types::EnumeratedItem(1, QStringLiteral("e1"), 10, SourceLocation()));
    type->addItem(Types::EnumeratedItem(2, QStringLiteral("e2"), 20, SourceLocation()));
    type->addItem(Types::EnumeratedItem(0, QStringLiteral("e3"), 30, SourceLocation()));

    const auto actual = createComponentialTypeAssignmentValue(std::move(type));

    const QString expected = "MyType ::= ENUMERATED\n"
                             "{\n"
                             "    e3(30),\n"
                             "    e1(10),\n"
                             "    e2(20)\n"
                             "}\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentChoice()
{
    auto type = std::make_unique<Types::Choice>();
    type->addComponent(std::make_unique<Types::ChoiceAlternative>(QStringLiteral("ch1"), QStringLiteral(""),
            QStringLiteral(""), QStringLiteral(""), QStringLiteral(""), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));

    type->addComponent(std::make_unique<Types::ChoiceAlternative>(QStringLiteral("ch2"), QStringLiteral(""),
            QStringLiteral(""), QStringLiteral(""), QStringLiteral(""), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("REAL"))));

    auto actual = createComponentialTypeAssignmentValue(std::move(type));

    // clang-format off
    QString expected =
        "MyType ::= CHOICE \n"
        "{ \n"
        "    ch1 INTEGER, \n"
        "    ch2 REAL\n"
        "} \n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentNestedChoice()
{
    auto sequenceType = std::make_unique<Types::Sequence>();
    auto type = std::make_unique<Types::Choice>();

    sequenceType->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("seq1"), QStringLiteral("seq1"),
            true, QStringLiteral(""), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));

    sequenceType->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("seq2"), QStringLiteral("seq2"),
            true, QStringLiteral(""), SourceLocation(), Types::TypeFactory::createBuiltinType(QStringLiteral("REAL"))));

    type->addComponent(std::make_unique<Types::ChoiceAlternative>(QStringLiteral("ch1"), QStringLiteral(""),
            QStringLiteral(""), QStringLiteral(""), QStringLiteral(""), SourceLocation(), std::move(sequenceType)));
    auto actual = createComponentialTypeAssignmentValue(std::move(type));

    // clang-format off
    QString expected =
        "MyType ::= CHOICE \n"
        "{ \n"
        "    ch1 SEQUENCE \n"
        "    { \n"
        "        seq1 INTEGER, \n"
        "        seq2 REAL\n"
        "    } \n"
        "} \n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentSequence()
{
    auto type = std::make_unique<Types::Sequence>();
    type->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("seq1"), QStringLiteral("seq1"), true,
            QStringLiteral(""), SourceLocation(), Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));

    type->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("seq2"), QStringLiteral("seq2"), true,
            QStringLiteral(""), SourceLocation(), Types::TypeFactory::createBuiltinType(QStringLiteral("REAL"))));

    auto actual = createComponentialTypeAssignmentValue(std::move(type));

    // clang-format off
    QString expected =
        "MyType ::= SEQUENCE \n"
        "{ \n"
        "    seq1 INTEGER, \n"
        "    seq2 REAL\n"
        "} \n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentNestedSequence()
{
    auto type = std::make_unique<Types::Sequence>();
    auto internalType = std::make_unique<Types::Sequence>();

    internalType->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("n1"), QStringLiteral("n1"), true,
            QStringLiteral(""), SourceLocation(), Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));
    internalType->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("n2"), QStringLiteral("n2"), true,
            QStringLiteral(""), SourceLocation(), Types::TypeFactory::createBuiltinType(QStringLiteral("REAL"))));

    type->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("seq1"), QStringLiteral("seq1"), true,
            QStringLiteral(""), SourceLocation(), std::move(internalType)));

    auto actual = createComponentialTypeAssignmentValue(std::move(type));

    // clang-format off
    QString expected =
        "MyType ::= SEQUENCE \n"
        "{ \n"
        "    seq1 SEQUENCE \n"
        "    { \n"
        "        n1 INTEGER, \n"
        "        n2 REAL\n"
        "    } \n"
        "} \n";
    // clang-format on

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentSequenceOf()
{
    auto type = std::make_unique<Types::SequenceOf>();
    type->setItemsType(std::make_unique<Types::Integer>());

    auto actual = createComponentialTypeAssignmentValue(std::move(type));

    QString expected = "MyType ::= SEQUENCE  OF INTEGER\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentSequenceOfWithValue()
{
    auto internalType = std::make_unique<Types::Integer>();

    internalType->constraints().append(Constraints::RangeConstraint<IntegerValue>::create({ 5, 10 }));

    auto assignment = createTypeAssignmentWithConstraint(QStringLiteral("SEQUENCE_OF"),
            new Constraints::SizeConstraint<IntegerValue>(Constraints::RangeConstraint<IntegerValue>::create({ 10 })));

    auto &sequenceType = dynamic_cast<Types::SequenceOf &>(*(assignment->type()));
    sequenceType.setItemsType(std::move(internalType));

    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= SEQUENCE (SIZE(10)) OF INTEGER(5 .. 10)\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentReal()
{
    testSimpleTypeAssignment(QStringLiteral("REAL"), QStringLiteral("REAL"));
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentRealWithValue()
{
    auto assignment = createTypeAssignmentWithConstraint(
            QStringLiteral("REAL"), new Constraints::RangeConstraint<RealValue>(1.1));
    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= REAL(1.1)\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentInteger()
{
    testSimpleTypeAssignment(QStringLiteral("INTEGER"), QStringLiteral("INTEGER"));
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentIntegerWithValue()
{
    auto assignment = createTypeAssignmentWithConstraint(
            QStringLiteral("INTEGER"), new Constraints::RangeConstraint<IntegerValue>({ 1, 2 }));
    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= INTEGER(1 .. 2)\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentUserDefined()
{
    auto type = std::make_unique<Types::UserdefinedType>(
            QStringLiteral("ReferencedType"), QStringLiteral("MyModule"), nullptr);
    type->setType(Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER")));

    auto assignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= ReferencedType\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::test_typeAssignmentUserDefinedWithValue()
{
    auto referedType = Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"));
    auto &constrainedType = dynamic_cast<Constraints::WithConstraints<IntegerValue> &>(*referedType);
    constrainedType.constraints().append({ 1, 2 });

    auto type = std::make_unique<Types::UserdefinedType>(
            QStringLiteral("ReferencedType"), QStringLiteral("MyModule"), nullptr);
    type->setType(std::move(referedType));
    auto assignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));

    auto actual = restoreNode(*assignment);

    QString expected = "MyType ::= ReferencedType(1 .. 2)\n";

    QCOMPARE(actual, expected);
}

void Asn1NodeReconstructingVisitorTests::testSimpleTypeAssignment(
        const QString &astValue, const QString &asn1Value) const
{
    auto assignment = createSimpleTypeAssignment(astValue);
    auto actual = restoreNode(*assignment);

    QString expected = "MyType ::= " + asn1Value + "\n";

    QCOMPARE(actual, expected);
}

std::unique_ptr<TypeAssignment> Asn1NodeReconstructingVisitorTests::createSimpleTypeAssignment(
        const QString &astTypeName) const
{
    auto type = Types::TypeFactory::createBuiltinType(astTypeName);
    auto assignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));

    return assignment;
}

template<typename T>
std::unique_ptr<TypeAssignment> Asn1NodeReconstructingVisitorTests::createTypeAssignmentWithConstraint(
        const QString &astTypeName, Constraints::Constraint<T> *constraint) const
{
    auto assignment = createSimpleTypeAssignment(astTypeName);
    auto &constrainedType = dynamic_cast<Constraints::WithConstraints<T> &>(*assignment->type());
    constrainedType.constraints().append(std::unique_ptr<Constraints::Constraint<T>>(constraint));

    return assignment;
}

std::unique_ptr<Definitions> Asn1NodeReconstructingVisitorTests::createDefinitions(const QString &name) const
{
    return std::make_unique<Definitions>(name, SourceLocation());
}

QString Asn1NodeReconstructingVisitorTests::createSingleValueValueAssignmentValue(
        const QString &typeName, const QString &typeValue, std::function<QString(const QString &)> printer) const
{
    auto type = Types::TypeFactory::createBuiltinType(typeName);
    auto value = std::make_unique<SingleValue>(typeValue, printer);

    auto assignment = std::make_unique<ValueAssignment>(
            QStringLiteral("myValue"), SourceLocation(), std::move(type), std::move(value));
    return restoreNode(*assignment);
}

QString Asn1NodeReconstructingVisitorTests::createMultipleValueValueAssignmentValue() const
{
    auto multipleValue = std::make_unique<MultipleValue>();

    multipleValue->addValue(std::make_unique<SingleValue>(QStringLiteral("1")));
    multipleValue->addValue(std::make_unique<SingleValue>(QStringLiteral("2")));
    multipleValue->addValue(std::make_unique<SingleValue>(QStringLiteral("3")));

    auto type = Types::TypeFactory::createBuiltinType(QStringLiteral("SEQUENCE_OF"));
    auto assignment = std::make_unique<ValueAssignment>(
            QStringLiteral("mySequence"), SourceLocation(), std::move(type), std::move(multipleValue));
    return restoreNode(*assignment);
}

QString Asn1NodeReconstructingVisitorTests::createNamedValueAssignmentValue() const
{
    auto multipleValue = std::make_unique<NamedValue>();

    multipleValue->addValue(QStringLiteral("v1"), std::make_unique<SingleValue>("1"));
    multipleValue->addValue(QStringLiteral("v2"), std::make_unique<SingleValue>("TRUE"));
    multipleValue->addValue(QStringLiteral("v3"), std::make_unique<SingleValue>("3.0"));

    auto type = Types::TypeFactory::createBuiltinType(QStringLiteral("SEQUENCE"));
    auto assignment = std::make_unique<ValueAssignment>(
            QStringLiteral("mySequence"), SourceLocation(), std::move(type), std::move(multipleValue));
    return restoreNode(*assignment);
}

QString Asn1NodeReconstructingVisitorTests::createComponentialTypeAssignmentValue(
        std::unique_ptr<Types::Type> type) const
{
    auto assignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    return restoreNode(*assignment);
}

QString Asn1NodeReconstructingVisitorTests::createChoiceValueAssignmentValue() const
{
    auto internalValue = std::make_unique<SingleValue>(QStringLiteral("10"));
    auto choiceValue = std::make_unique<ChoiceValue>(QStringLiteral("MyChoice"), std::move(internalValue));

    auto type = Types::TypeFactory::createBuiltinType(QStringLiteral("CHOICE"));
    auto assignment = std::make_unique<ValueAssignment>(
            QStringLiteral("myChoice"), SourceLocation(), std::move(type), std::move(choiceValue));
    return restoreNode(*assignment);
}

QString Asn1NodeReconstructingVisitorTests::restoreNode(const Node &node) const
{
    QString ret;
    QTextStream outStream(&ret);

    Asn1NodeReconstructingVisitor visitor(outStream);
    node.accept(visitor);

    return ret;
}
