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

#include "tst_acnnodereconstructingvisitor.h"

#include <QtTest>
#include <asn1library/asn1/acnsequencecomponent.h>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/choicevalue.h>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/multiplevalue.h>
#include <asn1library/asn1/namedvalue.h>
#include <asn1library/asn1/singlevalue.h>
#include <asn1library/asn1/sourcelocation.h>
#include <asn1library/asn1/typeassignment.h>
#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/null.h>
#include <asn1library/asn1/types/numericstring.h>
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/typefactory.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/asn1/Asn1Exporter/visitors/acnnodereconstructingvisitor.h>

using namespace Asn1Acn;

namespace conversion::asn1::test {

void tst_AcnNodeReconstructingVisitor::testEmptyModule()
{
    auto defs = std::make_unique<Definitions>(QStringLiteral("MyModule"), SourceLocation());

    auto actual = restoreNode(*defs);
    auto expected = QStringLiteral("MyModule DEFINITIONS ::= BEGIN\n"
                                   "END\n"
                                   "\n");

    QCOMPARE(actual, expected);
}

void tst_AcnNodeReconstructingVisitor::testBoolean()
{
    auto type = std::make_unique<Types::Boolean>();

    type->setTrueValue(QStringLiteral("1010"));
    type->setFalseValue(QStringLiteral("0101"));
    type->setAlignToNext(Types::Type::mapAlignToNext(QString("word")));

    auto expectedResult = QStringLiteral("MyBool [align-to-next word, true-value '1010'B, false-value '0101'B]\n");

    performTest(QStringLiteral("MyBool"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testNull()
{
    auto type = std::make_unique<Types::Null>();

    type->setPattern(QStringLiteral("0011"));
    type->setAlignToNext(Types::Type::mapAlignToNext(QString("word")));

    auto expectedResult = QStringLiteral("MyNull [align-to-next word, pattern '0011'B]\n");

    performTest(QStringLiteral("MyNull"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testInteger()
{
    auto type = std::make_unique<Types::Integer>();

    type->setAlignToNext(Types::Type::mapAlignToNext(QString("word")));
    type->setSize(10); // NOLINT
    type->setEncoding(Types::IntegerAcnParameters::mapEncoding(QStringLiteral("pos-int")));
    type->setEndianness(Types::Type::mapEndianess(QStringLiteral("big")));

    auto expectedResult = QStringLiteral("MyInteger [align-to-next word, size 10, encoding pos-int, endianness big]\n");

    performTest(QStringLiteral("MyInteger"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testReal()
{
    auto type = std::make_unique<Types::Real>();

    type->setAlignToNext(Types::Type::mapAlignToNext(QString("dword")));
    type->setEncoding(Types::Real::mapEncoding(QStringLiteral("IEEE754-1985-32")));
    type->setEndianness(Types::Type::mapEndianess(QStringLiteral("big")));

    auto expectedResult = QStringLiteral("MyReal [align-to-next dword, encoding IEEE754-1985-32, endianness big]\n");

    performTest(QStringLiteral("MyReal"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testBitString()
{
    auto type = std::make_unique<Types::BitString>();

    type->setAcnSize(QStringLiteral("10"));
    type->setAlignToNext(Types::Type::mapAlignToNext(QString("dword")));

    auto expectedResult = QStringLiteral("MyBitString [align-to-next dword, size 10]\n");

    performTest(QStringLiteral("MyBitString"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testOctetString()
{
    auto type = std::make_unique<Types::OctetString>();

    type->setAcnSize(QStringLiteral("10"));
    type->setAlignToNext(Types::Type::mapAlignToNext(QString("dword")));

    auto expectedResult = QStringLiteral("MyOctetString [align-to-next dword, size 10]\n");

    performTest(QStringLiteral("MyOctetString"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testIA5String()
{
    auto type = std::make_unique<Types::IA5String>();

    type->setAcnSize(QStringLiteral("10"));
    type->setAlignToNext(Types::Type::mapAlignToNext(QString("dword")));
    type->setTerminationPattern(QStringLiteral("10101010"));
    type->setEncoding(Types::AsciiStringAcnParameters::mapEncoding(QStringLiteral("ASCII")));

    auto expectedResult = QStringLiteral(
            "MyIA5String [align-to-next dword, size 10, encoding ASCII, termination-pattern '10101010'H]\n");

    performTest(QStringLiteral("MyIA5String"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testNumericString()
{
    auto type = std::make_unique<Types::NumericString>();

    type->setAcnSize(QStringLiteral("10"));
    type->setAlignToNext(Types::Type::mapAlignToNext(QString("dword")));
    type->setTerminationPattern(QStringLiteral("10101010"));
    type->setEncoding(Types::AsciiStringAcnParameters::mapEncoding(QStringLiteral("ASCII")));

    auto expectedResult = QStringLiteral(
            "MyNumericString [align-to-next dword, size 10, encoding ASCII, termination-pattern '10101010'H]\n");

    performTest(QStringLiteral("MyNumericString"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testEnumerated()
{
    auto type = std::make_unique<Types::Enumerated>();

    type->setAlignToNext(Types::Type::mapAlignToNext(QString("word")));
    type->setSize(10); // NOLINT
    type->setEncoding(Types::IntegerAcnParameters::mapEncoding(QStringLiteral("pos-int")));
    type->setEndianness(Types::Type::mapEndianess(QStringLiteral("big")));
    type->setEncodeValues(true);

    auto expectedResult =
            QStringLiteral("MyEnumerated [align-to-next word, size 10, encoding pos-int, endianness big]\n");

    performTest(QStringLiteral("MyEnumerated"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testSequenceOf()
{
    auto type = std::make_unique<Types::SequenceOf>();

    type->setAlignToNext(Types::Type::mapAlignToNext(QString("dword")));
    type->setAcnSize(QStringLiteral("10"));

    auto expectedResult = QStringLiteral("MySequenceOf [align-to-next dword, size 10]\n");

    performTest(QStringLiteral("MySequenceOf"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testChoice()
{
    auto type = std::make_unique<Types::Choice>();

    type->setAlignToNext(Types::Type::mapAlignToNext(QString("word")));

    auto expectedResult = QStringLiteral("MyChoice [align-to-next word]\n");

    performTest(QStringLiteral("MyChoice"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testChoiceNested()
{
    auto choiceType = std::make_unique<Types::Choice>();
    auto sequenceType = std::make_unique<Types::Sequence>();

    sequenceType->addComponent(std::make_unique<AcnSequenceComponent>(QStringLiteral("id1"), QStringLiteral("seqItem1"),
            Types::TypeFactory::createBuiltinType(QStringLiteral("BOOLEAN"))));

    sequenceType->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("seqItem2"),
            QStringLiteral("seqItem2"), true, std::nullopt, QStringLiteral("seqItem1"), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));

    choiceType->addComponent(std::make_unique<Types::ChoiceAlternative>(QStringLiteral("choiceAlt1"),
            QStringLiteral(""), QStringLiteral("ada-name-attr"), QStringLiteral("c-name-attr"),
            QStringLiteral("presWhen1"), SourceLocation(), std::move(sequenceType)));

    choiceType->addComponent(std::make_unique<Types::ChoiceAlternative>(QStringLiteral("choiceAlt2"),
            QStringLiteral(""), QStringLiteral("ada-name-attr"), QStringLiteral("c-name-attr"), QStringLiteral(""),
            SourceLocation(), Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));

    auto expectedResult = QStringLiteral("MyChoice []\n"
                                         "{\n"
                                         "    choiceAlt1 [present-when presWhen1]\n"
                                         "    {\n"
                                         "        seqItem1 BOOLEAN [],\n"
                                         "        seqItem2 [present-when seqItem1]\n"
                                         "    },\n"
                                         "    choiceAlt2 []\n"
                                         "}\n");

    performTest(QStringLiteral("MyChoice"), expectedResult, std::move(choiceType));
}

void tst_AcnNodeReconstructingVisitor::testChoiceParametrized()
{
    auto choiceType = std::make_unique<Types::Choice>();

    choiceType->addComponent(std::make_unique<Types::ChoiceAlternative>(QStringLiteral("choiceItem1"),
            QStringLiteral(""), QStringLiteral("ada-name-attr"), QStringLiteral("c-name-attr"),
            QStringLiteral("type1 == 1 type2 == 2"), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));
    choiceType->addComponent(std::make_unique<Types::ChoiceAlternative>(QStringLiteral("choiceItem2"),
            QStringLiteral(""), QStringLiteral("ada-name-attr"), QStringLiteral("c-name-attr"),
            QStringLiteral("type1 == 3 type2 == 4"), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));
    choiceType->addComponent(std::make_unique<Types::ChoiceAlternative>(QStringLiteral("choiceItem3"),
            QStringLiteral(""), QStringLiteral("ada-name-attr"), QStringLiteral("c-name-attr"),
            QStringLiteral("type1 == 5 type2 == 6"), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));

    choiceType->addParameter(std::make_unique<AcnParameter>(
            QStringLiteral("type1"), QStringLiteral("type1"), QStringLiteral("INTEGER")));

    choiceType->addParameter(std::make_unique<AcnParameter>(
            QStringLiteral("type2"), QStringLiteral("type2"), QStringLiteral("INTEGER")));

    auto expectedResult = QStringLiteral("MyChoice <INTEGER:type1, INTEGER:type2> []\n"
                                         "{\n"
                                         "    choiceItem1 [present-when type1 == 1 type2 == 2],\n"
                                         "    choiceItem2 [present-when type1 == 3 type2 == 4],\n"
                                         "    choiceItem3 [present-when type1 == 5 type2 == 6]\n"
                                         "}\n");

    performTest(QStringLiteral("MyChoice"), expectedResult, std::move(choiceType));
}

void tst_AcnNodeReconstructingVisitor::testSequence()
{
    auto type = std::make_unique<Types::Sequence>();

    type->setAlignToNext(Types::Type::mapAlignToNext(QString("word")));

    auto expectedResult = QStringLiteral("MySequence [align-to-next word]\n");

    performTest(QStringLiteral("MySequence"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testSequenceNested()
{
    auto sequenceType = std::make_unique<Types::Sequence>();
    auto choiceType = std::make_unique<Types::Choice>();

    choiceType->addComponent(std::make_unique<Types::ChoiceAlternative>(QStringLiteral("choiceAlt1"),
            QStringLiteral(""), QStringLiteral("ada-name-attr"), QStringLiteral("c-name-attr"), QStringLiteral(""),
            SourceLocation(), Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));

    choiceType->addComponent(std::make_unique<Types::ChoiceAlternative>(QStringLiteral("choiceAlt2"),
            QStringLiteral(""), QStringLiteral("ada-name-attr"), QStringLiteral("c-name-attr"), QStringLiteral(""),
            SourceLocation(), Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));

    sequenceType->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("seq1"), QStringLiteral("seq1"),
            true, std::nullopt, QStringLiteral("preWhenValue"), SourceLocation(),
            Types::TypeFactory::createBuiltinType(QStringLiteral("INTEGER"))));

    sequenceType->addComponent(std::make_unique<AsnSequenceComponent>(QStringLiteral("seq2"), QStringLiteral("seq2"),
            false, std::nullopt, QStringLiteral(""), SourceLocation(), std::move(choiceType)));

    auto expectedResult = QStringLiteral("MySequence []\n"
                                         "{\n"
                                         "    seq1 [present-when preWhenValue],\n"
                                         "    seq2 []\n"
                                         "    {\n"
                                         "        choiceAlt1 [],\n"
                                         "        choiceAlt2 []\n"
                                         "    }\n"
                                         "}\n");

    performTest(QStringLiteral("MySequence"), expectedResult, std::move(sequenceType));
}

void tst_AcnNodeReconstructingVisitor::testAcnParameter()
{
    auto type = std::make_unique<Types::Sequence>();
    type->addParameter(std::make_unique<AcnParameter>(
            QStringLiteral("id"), QStringLiteral("ParamName"), QStringLiteral("ParamType")));

    auto expectedResult = QStringLiteral("MySequence <ParamType:ParamName> []\n");

    performTest(QStringLiteral("MySequence"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testAcnArgument()
{
    auto type = std::make_unique<Types::UserdefinedType>(QStringLiteral("MyType"), QStringLiteral("MyModule"));
    type->addArgument(std::make_unique<AcnArgument>(QStringLiteral("ArgumentName")));

    auto expectedResult = QStringLiteral("MySequence <ArgumentName> []\n");

    performTest(QStringLiteral("MySequence"), expectedResult, std::move(type));
}

void tst_AcnNodeReconstructingVisitor::testIntegerEncodingToString()
{
    using namespace Types;
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::pos_int), QStringLiteral("pos-int"));
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::twos_complement),
            QStringLiteral("twos-complement"));
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::ASCII), QStringLiteral("ASCII"));
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::BCD), QStringLiteral("BCD"));
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::unspecified), QStringLiteral(""));
}

void tst_AcnNodeReconstructingVisitor::testRealEncodingToString()
{
    using namespace Types;
    QCOMPARE(Real::encodingToString(RealEncoding::IEEE754_1985_32), QStringLiteral("IEEE754-1985-32"));
    QCOMPARE(Real::encodingToString(RealEncoding::IEEE754_1985_64), QStringLiteral("IEEE754-1985-64"));
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::unspecified), QStringLiteral(""));
}

void tst_AcnNodeReconstructingVisitor::testAsciiStringEncodingToString()
{
    using namespace Types;
    QCOMPARE(AsciiStringAcnParameters::encodingToString(AsciiStringEncoding::ASCII), QStringLiteral("ASCII"));
    QCOMPARE(IntegerAcnParameters::encodingToString(IntegerEncoding::unspecified), QStringLiteral(""));
}

void tst_AcnNodeReconstructingVisitor::performTest(
        const QString &name, const QString &expectedResult, std::unique_ptr<Types::Type> type) const
{
    auto assignment = std::make_unique<TypeAssignment>(name, name + "T", SourceLocation(), std::move(type));

    auto actualResult = restoreNode(*assignment);
    QCOMPARE(actualResult, expectedResult);
}

QString tst_AcnNodeReconstructingVisitor::restoreNode(const Node &node) const
{
    QString ret;
    QTextStream outStream(&ret);

    AcnNodeReconstructingVisitor visitor(outStream);
    node.accept(visitor);

    return ret;
}

} // namespace conversion::asn1::test
