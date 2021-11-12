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

#include "tst_asn1nodereconstructingvisitor.h"

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
#include <conversion/asn1/Asn1Exporter/visitors/asn1nodereconstructingvisitor.h>

using namespace Asn1Acn;

namespace conversion::asn1::test {

void tst_Asn1NodeReconstructingVisitor::testSingleImportedType()
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

void tst_Asn1NodeReconstructingVisitor::testMultipleImportedTypes()
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

void tst_Asn1NodeReconstructingVisitor::testSingleImportedValue()
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

void tst_Asn1NodeReconstructingVisitor::testMultipleImportedValues()
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

void tst_Asn1NodeReconstructingVisitor::testValueAssignmentSimpleValue()
{
    auto actual = createSingleValueValueAssignmentValue(QStringLiteral("INTEGER"), QStringLiteral("10"));
    QString expected = "myValue INTEGER ::= 10\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testValueAssignmentBitString()
{
    auto actual = createSingleValueValueAssignmentValue(
            QStringLiteral("BIT_STRING"), QStringLiteral("1010"), BitStringValue::asString);
    QString expected = "myValue BIT STRING ::= '1010'B\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testValueAssignmentBoolean()
{
    auto actual = createSingleValueValueAssignmentValue(
            QStringLiteral("BOOLEAN"), QStringLiteral("true"), BooleanValue::reformatString);
    QString expected = "myValue BOOLEAN ::= TRUE\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testValueAssignmentOctetString()
{
    auto actual = createSingleValueValueAssignmentValue(
            QStringLiteral("OCTET_STRING"), QStringLiteral("1010"), OctetStringValue::asString);
    QString expected = "myValue OCTET STRING ::= '1010'H\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testValueAssignmentIA5String()
{
    auto actual = createSingleValueValueAssignmentValue(
            QStringLiteral("IA5String"), QStringLiteral("MyValue"), StringValue::asString);
    QString expected = "myValue IA5String ::= \"MyValue\"\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testValueAssignmentChoice()
{
    auto actual = createChoiceValueAssignmentValue();

    QString expected = "myChoice CHOICE ::= MyChoice:10\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testValueAssignmentMultipleValues()
{
    auto actual = createMultipleValueValueAssignmentValue();

    QString expected = "mySequence SEQUENCE OF ::= {1, 2, 3}\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testValueAssignmentNamedValue()
{
    auto actual = createNamedValueAssignmentValue();

    QString expected = "mySequence SEQUENCE ::= {v1 1, v2 TRUE, v3 3.0}\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentBooleanAcnParams()
{
    //
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentBoolean()
{
    testSimpleTypeAssignment(QStringLiteral("BOOLEAN"), QStringLiteral("BOOLEAN"));
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentNull()
{
    testSimpleTypeAssignment(QStringLiteral("NULL"), QStringLiteral("NULL"));
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentBitString()
{
    testSimpleTypeAssignment(QStringLiteral("BIT_STRING"), QStringLiteral("BIT STRING"));
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentBitStringWithValue()
{
    auto assignment = createTypeAssignmentWithConstraint(
            QStringLiteral("BIT_STRING"), new Constraints::RangeConstraint<BitStringValue>({ "1111", "1111" }));

    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= BIT STRING('1111'B)\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentOctetString()
{
    testSimpleTypeAssignment(QStringLiteral("OCTET_STRING"), QStringLiteral("OCTET STRING"));
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentOctetStringWithValue()
{
    auto assignment = createTypeAssignmentWithConstraint(
            QStringLiteral("OCTET_STRING"), new Constraints::RangeConstraint<OctetStringValue>({ "1010", "1010" }));

    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= OCTET STRING('1010'H)\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentIA5String()
{
    testSimpleTypeAssignment(QStringLiteral("IA5String"), QStringLiteral("IA5String"));
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentIA5StringWithValue()
{
    auto assignment = createTypeAssignmentWithConstraint(
            QStringLiteral("IA5String"), new Constraints::RangeConstraint<StringValue>({ "TextString", "TextString" }));

    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= IA5String(\"TextString\")\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentNumericString()
{
    testSimpleTypeAssignment(QStringLiteral("NumericString"), QStringLiteral("NumericString"));
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentNumericStringWithValue()
{
    auto assignment = createTypeAssignmentWithConstraint(
            QStringLiteral("NumericString"), new Constraints::RangeConstraint<StringValue>({ "12345678", "12345678" }));

    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= NumericString(\"12345678\")\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentEnumerated()
{
    auto type = std::make_unique<Types::Enumerated>();
    type->addItem(Types::EnumeratedItem(1, QStringLiteral("e1"), 10, SourceLocation())); // NOLINT
    type->addItem(Types::EnumeratedItem(2, QStringLiteral("e2"), 20, SourceLocation())); // NOLINT
    type->addItem(Types::EnumeratedItem(0, QStringLiteral("e3"), 30, SourceLocation())); // NOLINT

    const auto actual = createComponentialTypeAssignmentValue(std::move(type));

    const QString expected = "MyType ::= ENUMERATED\n"
                             "{\n"
                             "    e3(30),\n"
                             "    e1(10),\n"
                             "    e2(20)\n"
                             "}\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentChoice()
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

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentNestedChoice()
{
    auto sequenceType = std::make_unique<Types::Sequence>();
    auto type = std::make_unique<Types::Choice>();

    sequenceType->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("seq1"), QStringLiteral("seq1"),
            true, std::nullopt, QStringLiteral(""), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));

    sequenceType->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("seq2"), QStringLiteral("seq2"),
            true, std::nullopt, QStringLiteral(""), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("REAL"))));

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

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentSequence()
{
    auto type = std::make_unique<Types::Sequence>();
    type->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("seq1"), QStringLiteral("seq1"), true,
            std::nullopt, QStringLiteral(""), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));

    type->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("seq2"), QStringLiteral("seq2"), true,
            std::nullopt, QStringLiteral(""), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("REAL"))));

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

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentNestedSequence()
{
    auto type = std::make_unique<Types::Sequence>();
    auto internalType = std::make_unique<Types::Sequence>();

    internalType->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("n1"), QStringLiteral("n1"), true,
            std::nullopt, QStringLiteral(""), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));
    internalType->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("n2"), QStringLiteral("n2"), true,
            std::nullopt, QStringLiteral(""), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("REAL"))));

    type->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("seq1"), QStringLiteral("seq1"), true,
            std::nullopt, QStringLiteral(""), SourceLocation(), std::move(internalType)));

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

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentSequenceOf()
{
    auto type = std::make_unique<Types::SequenceOf>();
    type->setItemsType(std::make_unique<Types::Integer>());

    auto actual = createComponentialTypeAssignmentValue(std::move(type));

    QString expected = "MyType ::= SEQUENCE  OF INTEGER\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentSequenceOfWithValue()
{
    auto internalType = std::make_unique<Types::Integer>();

    internalType->constraints().append(Constraints::RangeConstraint<IntegerValue>::create({ 5, 10 })); // NOLINT

    auto assignment = createTypeAssignmentWithConstraint(QStringLiteral("SEQUENCE_OF"),
            new Constraints::SizeConstraint<IntegerValue>(
                    Constraints::RangeConstraint<IntegerValue>::create({ 10 }))); // NOLINT

    auto &sequenceType = dynamic_cast<Types::SequenceOf &>(*(assignment->type()));
    sequenceType.setItemsType(std::move(internalType));

    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= SEQUENCE (SIZE(10)) OF INTEGER(5 .. 10)\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentReal()
{
    testSimpleTypeAssignment(QStringLiteral("REAL"), QStringLiteral("REAL"));
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentRealWithValue()
{
    auto assignment = createTypeAssignmentWithConstraint(
            QStringLiteral("REAL"), new Constraints::RangeConstraint<RealValue>(1.1)); // NOLINT
    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= REAL(1.1)\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentInteger()
{
    testSimpleTypeAssignment(QStringLiteral("INTEGER"), QStringLiteral("INTEGER"));
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentIntegerWithValue()
{
    auto assignment = createTypeAssignmentWithConstraint(
            QStringLiteral("INTEGER"), new Constraints::RangeConstraint<IntegerValue>({ 1, 2 })); // NOLINT
    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= INTEGER(1 .. 2)\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentUserDefined()
{
    auto type = std::make_unique<Types::UserdefinedType>(QStringLiteral("ReferencedType"), QStringLiteral("MyModule"));
    type->setType(Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER")));

    auto assignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    auto actual = restoreNode(*assignment);
    QString expected = "MyType ::= ReferencedType\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testTypeAssignmentUserDefinedWithValue()
{
    auto referedType = Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"));
    auto &constrainedType = dynamic_cast<Constraints::WithConstraints<IntegerValue> &>(*referedType);
    constrainedType.constraints().append({ 1, 2 }); // NOLINT

    auto type = std::make_unique<Types::UserdefinedType>(QStringLiteral("ReferencedType"), QStringLiteral("MyModule"));
    type->setType(std::move(referedType));
    auto assignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));

    auto actual = restoreNode(*assignment);

    QString expected = "MyType ::= ReferencedType(1 .. 2)\n";

    QCOMPARE(actual, expected);
}

void tst_Asn1NodeReconstructingVisitor::testSimpleTypeAssignment(
        const QString &astValue, const QString &asn1Value) const
{
    auto assignment = createSimpleTypeAssignment(astValue);
    auto actual = restoreNode(*assignment);

    QString expected = "MyType ::= " + asn1Value + "\n";

    QCOMPARE(actual, expected);
}

std::unique_ptr<TypeAssignment> tst_Asn1NodeReconstructingVisitor::createSimpleTypeAssignment(
        const QString &astTypeName) const
{
    auto type = Types::TypeFactory::createBuiltinType(astTypeName);
    auto assignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));

    return assignment;
}

std::unique_ptr<Definitions> tst_Asn1NodeReconstructingVisitor::createDefinitions(const QString &name) const
{
    return std::make_unique<Definitions>(name, SourceLocation());
}

QString tst_Asn1NodeReconstructingVisitor::createSingleValueValueAssignmentValue(
        const QString &typeName, const QString &typeValue, std::function<QString(const QString &)> printer) const
{
    auto type = Types::TypeFactory::createBuiltinType(typeName);
    auto value = std::make_unique<SingleValue>(typeValue, std::move(printer));

    auto assignment = std::make_unique<ValueAssignment>(
            QStringLiteral("myValue"), SourceLocation(), std::move(type), std::move(value));
    return restoreNode(*assignment);
}

QString tst_Asn1NodeReconstructingVisitor::createMultipleValueValueAssignmentValue() const
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

QString tst_Asn1NodeReconstructingVisitor::createNamedValueAssignmentValue() const
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

QString tst_Asn1NodeReconstructingVisitor::createComponentialTypeAssignmentValue(
        std::unique_ptr<Types::Type> type) const
{
    auto assignment = std::make_unique<TypeAssignment>(
            QStringLiteral("MyType"), QStringLiteral("MyTypeT"), SourceLocation(), std::move(type));
    return restoreNode(*assignment);
}

QString tst_Asn1NodeReconstructingVisitor::createChoiceValueAssignmentValue() const
{
    auto internalValue = std::make_unique<SingleValue>(QStringLiteral("10"));
    auto choiceValue = std::make_unique<ChoiceValue>(QStringLiteral("MyChoice"), std::move(internalValue));

    auto type = Types::TypeFactory::createBuiltinType(QStringLiteral("CHOICE"));
    auto assignment = std::make_unique<ValueAssignment>(
            QStringLiteral("myChoice"), SourceLocation(), std::move(type), std::move(choiceValue));
    return restoreNode(*assignment);
}

QString tst_Asn1NodeReconstructingVisitor::restoreNode(const Node &node) const
{
    QString ret;
    QTextStream outStream(&ret);

    Asn1NodeReconstructingVisitor visitor(outStream);
    node.accept(visitor);

    return ret;
}

} // namespace conversion::asn1::test
