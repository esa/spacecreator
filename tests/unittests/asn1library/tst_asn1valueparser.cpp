/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "asn1const.h"
#include "asn1valueparser.h"
#include "asnsequencecomponent.h"
#include "constraints/rangeconstraint.h"
#include "constraints/sizeconstraint.h"
#include "sourcelocation.h"
#include "typeassignment.h"
#include "types/bitstring.h"
#include "types/boolean.h"
#include "types/choice.h"
#include "types/enumerated.h"
#include "types/ia5string.h"
#include "types/integer.h"
#include "types/octetstring.h"
#include "types/real.h"
#include "types/sequence.h"
#include "types/sequenceof.h"
#include "types/type.h"
#include "types/userdefinedtype.h"

#include <QSignalSpy>
#include <QtTest>
#include <memory>

using namespace Asn1Acn;

class tst_Asn1ValueParser : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();

    void testIntValues();
    void testRealValues();
    void testStringValues();
    void testIntValuesFormatError();
    void testRealValuesFormatError();
    void testIntValuesWithRange();
    void testRealValuesWithRange();
    void testIntValuesWithRangeError();
    void testRealValuesWithRangeError();
    void testBoolValues();
    void testIA5StringValues();
    void testBitStringValues();
    void testOctetStringValues();
    void testEnumValues();
    void testBoolValuesError();
    void testEnumValuesError();
    void testChoiceValue();
    void testChoiceValueError();
    void testSequenceValueWithString();
    void testSequenceValue();
    void testSequenceValueError();
    void testSequenceOfValue();
    void testUserType();
    void testComplexSequence();

private:
    Asn1ValueParser *valueParser = nullptr;
};

void tst_Asn1ValueParser::init()
{
    valueParser = new Asn1ValueParser;
}

void tst_Asn1ValueParser::cleanup()
{
    delete valueParser;
    valueParser = nullptr;
}

void tst_Asn1ValueParser::testIntValues()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Integer>();
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyInt", "MyInt", location, std::move(type));

    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "3107", &convOk);
    QVERIFY(convOk);
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyInt"));
    QCOMPARE(valueMap["value"].toInt(), 3107);
}

void tst_Asn1ValueParser::testStringValues()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::IA5String>();
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyString", "MyString", location, std::move(type));

    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "teststringforcheck", &convOk);
    QVERIFY(convOk);
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString::fromLatin1("MyString"));
    QCOMPARE(valueMap["value"].toString(), QString::fromLatin1("teststringforcheck"));
}

void tst_Asn1ValueParser::testRealValues()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Real>();
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyDouble", "MyDouble", location, std::move(type));

    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "31.07", &convOk);
    QVERIFY(convOk);
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyDouble"));
    QCOMPARE(valueMap["value"].toDouble(), 31.07);
}

void tst_Asn1ValueParser::testIntValuesFormatError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Integer>();
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyInt", "MyInt", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "31o7", &convOk);
    QVERIFY(!convOk);
    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyInt"));
}

void tst_Asn1ValueParser::testRealValuesFormatError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Real>();
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyDouble", "MyDouble", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "31.o7", &convOk);
    QVERIFY(!convOk);
    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyDouble"));
}

void tst_Asn1ValueParser::testIntValuesWithRange()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Integer>();
    auto range = Range<typename IntegerValue::Type>(5, 15);
    type->constraints().append(range);
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyInt", "MyInt", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "13", &convOk);
    QVERIFY(convOk);
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyInt"));
    QCOMPARE(valueMap["value"].toInt(), 13);
}

void tst_Asn1ValueParser::testRealValuesWithRange()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Real>();
    auto range = Range<typename RealValue::Type>(10.0, 50.0);
    type->constraints().append(range);
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyDouble", "MyDouble", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "31.07", &convOk);
    QVERIFY(convOk);
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyDouble"));
    QCOMPARE(valueMap["value"].toDouble(), 31.07);
}

void tst_Asn1ValueParser::testIntValuesWithRangeError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Integer>();
    auto range = Range<typename IntegerValue::Type>(1, 10);
    type->constraints().append(range);
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyInt", "MyInt", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "13", &convOk);
    QVERIFY(!convOk);
    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyInt"));
}

void tst_Asn1ValueParser::testRealValuesWithRangeError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Real>();
    auto range = Range<typename RealValue::Type>(10.0, 30.0);
    type->constraints().append(range);
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyDouble", "MyDouble", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "31.07", &convOk);
    QVERIFY(!convOk);
    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyDouble"));
}

void tst_Asn1ValueParser::testBoolValues()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Boolean>();
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyBool", "MyBool", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "TRUE", &convOk);
    QVERIFY(convOk);
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyBool"));
    QVERIFY(valueMap["value"].toBool() == true);
}

void tst_Asn1ValueParser::testIA5StringValues()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::IA5String>();
    auto range = Asn1Acn::Range<int64_t>(3, 10);
    auto rangeConstraint = std::make_unique<Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>>(range);
    auto sizeConstraint =
            std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::StringValue>>(std::move(rangeConstraint));
    type->constraints().append(std::move(sizeConstraint));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyString", "MyString", location, std::move(type));

    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "TestString", &convOk);
    QVERIFY(convOk);
    QCOMPARE(valueMap.size(), 2);
    QCOMPARE(valueMap["name"].toString(), QString("MyString"));
    QVERIFY(valueMap["value"].toString() == "TestString");

    // Test range
    bool ok;
    valueMap = valueParser->parseAsn1Value(assignment.get(), "a", &ok);
    QVERIFY(!ok);
    valueMap = valueParser->parseAsn1Value(assignment.get(), "abcdefghijklm", &ok);
    QVERIFY(!ok);
}

void tst_Asn1ValueParser::testBitStringValues()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::BitString>();
    auto range = Asn1Acn::Range<int64_t>(4, 16);
    auto rangeConstraint = std::make_unique<Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>>(range);
    auto sizeConstraint =
            std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::BitStringValue>>(std::move(rangeConstraint));
    type->constraints().append(std::move(sizeConstraint));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyString", "MyString", location, std::move(type));

    bool convOk = false;
    QVariantMap valueMap = valueParser->parseAsn1Value(assignment.get(), "'AA'H", &convOk);
    QVERIFY(convOk);
    QCOMPARE(valueMap.size(), 2);
    QCOMPARE(valueMap["name"].toString(), QString("MyString"));
    QCOMPARE(valueMap["value"].toString(), "'AA'H");

    valueMap = valueParser->parseAsn1Value(assignment.get(), "'1001'B", &convOk);
    QVERIFY(convOk);
    QCOMPARE(valueMap.size(), 2);
    QCOMPARE(valueMap["name"].toString(), QString("MyString"));
    QCOMPARE(valueMap["value"].toString(), "'1001'B");

    bool ok;
    valueMap = valueParser->parseAsn1Value(assignment.get(), "nope", &ok);
    QVERIFY(!ok);
    valueMap = valueParser->parseAsn1Value(assignment.get(), "'nope'H", &ok);
    QVERIFY(!ok);
    valueMap = valueParser->parseAsn1Value(assignment.get(), "'nope'B", &ok);
    QVERIFY(!ok);
    valueMap = valueParser->parseAsn1Value(assignment.get(), "'ABAB'B", &ok);
    QVERIFY(!ok);

    // Test range
    valueMap = valueParser->parseAsn1Value(assignment.get(), "'01'B", &ok);
    QVERIFY(!ok);
    valueMap = valueParser->parseAsn1Value(assignment.get(), "'01010101010101010101010101010101'B");
    QVERIFY(!ok);
    valueMap = valueParser->parseAsn1Value(assignment.get(), "'AB01CDEF'H");
    QVERIFY(!ok);
}

void tst_Asn1ValueParser::testOctetStringValues()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::OctetString>();
    auto range = Asn1Acn::Range<int64_t>(2, 3);
    auto rangeConstraint = std::make_unique<Asn1Acn::Constraints::RangeConstraint<Asn1Acn::IntegerValue>>(range);
    auto sizeConstraint = std::make_unique<Asn1Acn::Constraints::SizeConstraint<Asn1Acn::OctetStringValue>>(
            std::move(rangeConstraint));
    type->constraints().append(std::move(sizeConstraint));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyString", "MyString", location, std::move(type));
    bool convOk = false;
    QVariantMap valueMap = valueParser->parseAsn1Value(assignment.get(), "'ABCD'H", &convOk);
    QVERIFY(convOk);
    QCOMPARE(valueMap.size(), 2);
    QCOMPARE(valueMap["name"].toString(), QString("MyString"));
    QCOMPARE(valueMap["value"].toString(), "'ABCD'H");

    valueMap = valueParser->parseAsn1Value(assignment.get(), "'0110011001100110'B", &convOk); // 2 byte/octet
    QVERIFY(convOk);
    QCOMPARE(valueMap.size(), 2);
    QCOMPARE(valueMap["name"].toString(), QString("MyString"));
    QCOMPARE(valueMap["value"].toString(), "'0110011001100110'B");

    bool ok;
    valueMap = valueParser->parseAsn1Value(assignment.get(), "nope", &ok);
    QVERIFY(!ok);
    QVERIFY(!ok);
    valueMap = valueParser->parseAsn1Value(assignment.get(), "'nope'H", &ok);
    QVERIFY(!ok);
    valueMap = valueParser->parseAsn1Value(assignment.get(), "'nope'B", &ok);
    QVERIFY(!ok);
    valueMap = valueParser->parseAsn1Value(assignment.get(), "'ABAB'B", &ok);
    QVERIFY(!ok);

    // Test range
    valueMap = valueParser->parseAsn1Value(assignment.get(), "'01'B", &ok);
    QVERIFY(!ok);
    valueMap = valueParser->parseAsn1Value(
            assignment.get(), "'011001100110011001100110011001100110011001100110'B"); // 6 byte/octet
    QVERIFY(!ok);
    valueMap = valueParser->parseAsn1Value(assignment.get(), "'0123ABCDEF'H"); // 8 byte/octet
    QVERIFY(!ok);
}

void tst_Asn1ValueParser::testEnumValues()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Enumerated>();
    type->addItem(Asn1Acn::Types::EnumeratedItem(0, "enum1", 0));
    type->addItem(Asn1Acn::Types::EnumeratedItem(1, "enum2", 1));
    type->addItem(Asn1Acn::Types::EnumeratedItem(2, "enum3", 2));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyEnum", "MyEnum", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "enum2", &convOk);
    QVERIFY(convOk);

    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyEnum"));
    QCOMPARE(valueMap["value"].toString(), QString("enum2"));
}

void tst_Asn1ValueParser::testBoolValuesError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Boolean>();
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyBool", "MyBool", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "true", &convOk);
    QVERIFY(!convOk);
    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyBool"));
}

void tst_Asn1ValueParser::testEnumValuesError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Enumerated>();
    type->addItem(Asn1Acn::Types::EnumeratedItem(0, "enum1", 0));
    type->addItem(Asn1Acn::Types::EnumeratedItem(1, "enum2", 1));
    type->addItem(Asn1Acn::Types::EnumeratedItem(2, "enum3", 2));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyEnum", "MyEnum", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "enum", &convOk);
    QVERIFY(!convOk);

    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyEnum"));
}

void tst_Asn1ValueParser::testChoiceValue()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Choice>();
    auto choice1 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>("choiceInt", "choiceInt", "choiceInt",
            "choiceInt", "", Asn1Acn::SourceLocation(), std::make_unique<Asn1Acn::Types::Integer>("choiceInt"));
    type->addComponent(std::move(choice1));
    auto choice2 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>("choiceReal", "choiceReal", "choiceReal",
            "choiceReal", "", Asn1Acn::SourceLocation(), std::make_unique<Asn1Acn::Types::Real>("choiceReal"));
    type->addComponent(std::move(choice2));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyChoice", "MyChoice", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "choiceReal : 31.07", &convOk);
    QVERIFY(convOk);
    QCOMPARE(valueMap.size(), 2);
    QCOMPARE(valueMap["name"].toString(), QString("MyChoice"));

    auto choiceValue = valueMap["choice"].toMap();
    QCOMPARE(choiceValue.size(), 2);

    QCOMPARE(choiceValue["name"].toString(), QString("choiceReal"));
    QCOMPARE(choiceValue["value"].toDouble(), 31.07);
}

void tst_Asn1ValueParser::testChoiceValueError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Choice>();
    auto choice1 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>("choiceInt", "choiceInt", "choiceInt",
            "choiceInt", "", Asn1Acn::SourceLocation(), std::make_unique<Asn1Acn::Types::Integer>("choiceInt"));
    type->addComponent(std::move(choice1));
    auto choice2 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>("choiceReal", "choiceReal", "choiceReal",
            "choiceReal", "", Asn1Acn::SourceLocation(), std::make_unique<Asn1Acn::Types::Real>("choiceReal"));
    type->addComponent(std::move(choice2));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyChoice", "MyChoice", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "choice : TRUE", &convOk);
    QVERIFY(!convOk);
    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyChoice"));
}

void tst_Asn1ValueParser::testSequenceValueWithString()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Sequence>();
    auto sequence1 = std::make_unique<Asn1Acn::AsnSequenceComponent>("strVal", "strVal", false, std::nullopt, "",
            Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::IA5String>("strVal"));
    type->addComponent(std::move(sequence1));

    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MySequence", "MySequence", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "{ strVal potatoes }", &convOk);
    QVERIFY(convOk);

    QCOMPARE(valueMap.size(), 2);
    QCOMPARE(valueMap["name"].toString(), QString("MySequence"));

    auto childrenValue = valueMap["children"].toList();
    QCOMPARE(childrenValue.size(), 1);

    auto childValue = childrenValue.at(0).toMap();
    QCOMPARE(childValue.size(), 2);
    QCOMPARE(childValue["name"].toString(), QString("strVal"));
    QCOMPARE(childValue["value"].toString(), QString("potatoes"));
}

void tst_Asn1ValueParser::testSequenceValue()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Sequence>();
    auto sequence1 = std::make_unique<Asn1Acn::AsnSequenceComponent>("intVal", "intVal", false, std::nullopt, "",
            Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Integer>("intVal"));
    type->addComponent(std::move(sequence1));
    auto sequence2 = std::make_unique<Asn1Acn::AsnSequenceComponent>("realVal", "realVal", false, std::nullopt, "",
            Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Real>("realVal"));
    type->addComponent(std::move(sequence2));
    auto sequence3 = std::make_unique<Asn1Acn::AsnSequenceComponent>("boolVal", "boolVal", false, std::nullopt, "",
            Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Boolean>("boolVal"));
    type->addComponent(std::move(sequence3));
    auto choice = std::make_unique<Asn1Acn::Types::Choice>("choiceVal");
    auto choice1 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>("choice1", "choice1", "choice1", "choice1", "",
            Asn1Acn::SourceLocation(), std::make_unique<Asn1Acn::Types::Boolean>("choice1"));
    choice->addComponent(std::move(choice1));
    auto choice2 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>("choice2", "choice2", "choice2", "choice2", "",
            Asn1Acn::SourceLocation(), std::make_unique<Asn1Acn::Types::Integer>("choice2"));
    choice->addComponent(std::move(choice2));
    auto sequence4 = std::make_unique<Asn1Acn::AsnSequenceComponent>("choiceVal", "choiceVal", false, std::nullopt, "",
            Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(), std::move(choice));
    type->addComponent(std::move(sequence4));

    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MySequence", "MySequence", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(
            assignment.get(), "{ intVal 3107, realVal 31.07, boolVal TRUE, choiceVal choice1 : FALSE }", &convOk);
    QVERIFY(convOk);

    QCOMPARE(valueMap.size(), 2);
    QCOMPARE(valueMap["name"].toString(), QString("MySequence"));

    auto childrenValue = valueMap["children"].toList();
    QCOMPARE(childrenValue.size(), 4);

    auto childValue = childrenValue.at(0).toMap();
    QCOMPARE(childValue.size(), 2);
    QCOMPARE(childValue["name"].toString(), QString("intVal"));
    QCOMPARE(childValue["value"].toInt(), 3107);

    childValue = childrenValue.at(1).toMap();
    QCOMPARE(childValue.size(), 2);
    QCOMPARE(childValue["name"].toString(), QString("realVal"));
    QCOMPARE(childValue["value"].toDouble(), 31.07);

    childValue = childrenValue.at(2).toMap();
    QCOMPARE(childValue.size(), 2);
    QCOMPARE(childValue["name"].toString(), QString("boolVal"));
    QCOMPARE(childValue["value"].toBool(), true);

    childValue = childrenValue.at(3).toMap();
    QCOMPARE(childValue.size(), 2);
    QCOMPARE(childValue["name"].toString(), QString("choiceVal"));
    QVariantMap choiceMap = childValue["choice"].toMap();
    QCOMPARE(choiceMap.size(), 2);
    QCOMPARE(choiceMap["name"].toString(), QString("choice1"));
    QCOMPARE(choiceMap["value"].toBool(), false);
}

void tst_Asn1ValueParser::testSequenceValueError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Sequence>();
    auto sequence1 = std::make_unique<Asn1Acn::AsnSequenceComponent>("intVal", "intVal", false, std::nullopt, "",
            Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Integer>("intVal"));
    type->addComponent(std::move(sequence1));
    auto sequence2 = std::make_unique<Asn1Acn::AsnSequenceComponent>("realVal", "realVal", false, std::nullopt, "",
            Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Real>("realVal"));
    type->addComponent(std::move(sequence2));
    auto sequence3 = std::make_unique<Asn1Acn::AsnSequenceComponent>("boolVal", "boolVal", false, std::nullopt, "",
            Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Boolean>("boolVal"));
    type->addComponent(std::move(sequence3));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MySequence", "MySequence", location, std::move(type));
    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "{ intVal 31o7, realVal 31.07 }", &convOk);
    QVERIFY(!convOk);

    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 2);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for intVal"));

    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MySequence"));
}

void tst_Asn1ValueParser::testSequenceOfValue()
{
    Asn1Acn::SourceLocation location;
    auto ofType = std::make_unique<Asn1Acn::Types::Enumerated>();
    ofType->addItem(Asn1Acn::Types::EnumeratedItem(0, "red", 0));
    ofType->addItem(Asn1Acn::Types::EnumeratedItem(1, "green", 1));
    ofType->addItem(Asn1Acn::Types::EnumeratedItem(2, "blue", 2));

    auto type = std::make_unique<Asn1Acn::Types::SequenceOf>();
    auto range = Range<typename IntegerValue::Type>(2, 2);
    type->constraints().append(range);
    type->setItemsType(std::move(ofType));

    auto assignment =
            std::make_unique<Asn1Acn::TypeAssignment>("MySequenceOf", "MySequenceOf", location, std::move(type));

    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "{ blue, green }", &convOk);
    QVERIFY(convOk);

    QCOMPARE(valueMap.size(), 2);
    QCOMPARE(valueMap["name"].toString(), QString("MySequenceOf"));

    auto childrenValue = valueMap["seqofvalue"].toList();
    QCOMPARE(childrenValue.size(), 2);

    auto childValue = childrenValue.at(0).toMap();
    QCOMPARE(childValue.size(), 2);
    QCOMPARE(childValue["value"].toString(), QString("blue"));

    childValue = childrenValue.at(1).toMap();
    QCOMPARE(childValue.size(), 2);
    QCOMPARE(childValue["value"].toString(), QString("green"));
}

void tst_Asn1ValueParser::testUserType()
{
    Asn1Acn::SourceLocation location;

    auto colorType = std::make_unique<Asn1Acn::Types::Enumerated>();
    colorType->addItem(Asn1Acn::Types::EnumeratedItem(0, "red", 0));
    colorType->addItem(Asn1Acn::Types::EnumeratedItem(1, "green", 1));
    colorType->addItem(Asn1Acn::Types::EnumeratedItem(2, "blue", 2));

    auto userType = std::make_unique<Asn1Acn::Types::UserdefinedType>("Color", "");
    userType->setType(std::move(colorType));

    auto type = std::make_unique<Asn1Acn::Types::SequenceOf>();
    auto range = Range<typename IntegerValue::Type>(2, 2);
    type->constraints().append(range);
    type->setItemsType(std::move(userType));
    auto assignment =
            std::make_unique<Asn1Acn::TypeAssignment>("MySequenceOf", "MySequenceOf", location, std::move(type));

    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "{ blue, green }", &convOk);
    QVERIFY(convOk);

    QCOMPARE(valueMap.size(), 2);
    QCOMPARE(valueMap["name"].toString(), QString("MySequenceOf"));

    auto childrenValue = valueMap["seqofvalue"].toList();
    QCOMPARE(childrenValue.size(), 2);

    auto childValue = childrenValue.at(0).toMap();
    QCOMPARE(childValue.size(), 2);
    QCOMPARE(childValue["value"].toString(), QString("blue"));

    childValue = childrenValue.at(1).toMap();
    QCOMPARE(childValue.size(), 2);
    QCOMPARE(childValue["value"].toString(), QString("green"));
}

void tst_Asn1ValueParser::testComplexSequence()
{
    Asn1Acn::SourceLocation location;

    // MyInteger
    auto myIntegerType = std::make_unique<Asn1Acn::Types::Integer>();
    {
        auto range = Range<typename IntegerValue::Type>(0, 100);
        myIntegerType->constraints().append(range);
    }

    // MySmallSequence
    auto mySmallSequenceType = std::make_unique<Asn1Acn::Types::Sequence>();
    {
        auto myIntegerRef = std::make_unique<Asn1Acn::Types::UserdefinedType>("MyInteger", "");
        myIntegerRef->setType(myIntegerType->clone());
        myIntegerRef->setIdentifier("left");
        auto mySmallSequenceComponentLeft =
                std::make_unique<Asn1Acn::AsnSequenceComponent>("left", "left", false, std::nullopt, "",
                        Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, location, std::move(myIntegerRef));
        mySmallSequenceType->addComponent(std::move(mySmallSequenceComponentLeft));
    }
    {
        auto myIntegerRef = std::make_unique<Asn1Acn::Types::UserdefinedType>("MyInteger", "");
        myIntegerRef->setType(myIntegerType->clone());
        myIntegerRef->setIdentifier("right");
        auto mySmallSequenceComponentRight =
                std::make_unique<Asn1Acn::AsnSequenceComponent>("right", "right", false, std::nullopt, "",
                        Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, location, std::move(myIntegerRef));
        mySmallSequenceType->addComponent(std::move(mySmallSequenceComponentRight));
    }

    // MyChoice
    auto myChoiceType = std::make_unique<Asn1Acn::Types::Choice>();
    {
        auto mySmallSequenceRef = std::make_unique<Asn1Acn::Types::UserdefinedType>("MySmallSequence", "");
        mySmallSequenceRef->setType(mySmallSequenceType->clone());
        mySmallSequenceType->setIdentifier("red");
        auto myChoiceAlternativeRed = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
                "red", "red", "red", "red", "", location, std::move(mySmallSequenceRef));
        myChoiceType->addComponent(std::move(myChoiceAlternativeRed));
    }
    {
        auto myIntegerRef = std::make_unique<Asn1Acn::Types::UserdefinedType>("MyInteger", "");
        myIntegerRef->setType(myIntegerType->clone());
        myIntegerRef->setIdentifier("green");
        auto myChoiceAlternativeGreen = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
                "green", "green", "green", "green", "", location, std::move(myIntegerRef));
        myChoiceType->addComponent(std::move(myChoiceAlternativeGreen));
    }

    // MySequenceOf
    auto mySequenceOfType = std::make_unique<Asn1Acn::Types::SequenceOf>();
    {
        auto range = Range<typename IntegerValue::Type>(1, 2);
        mySequenceOfType->constraints().append(range);
        auto mySmallSequenceRef = std::make_unique<Asn1Acn::Types::UserdefinedType>("MySmallSequence", "");
        mySmallSequenceRef->setType(mySmallSequenceType->clone());
        mySequenceOfType->setItemsType(std::move(mySmallSequenceRef));
    }

    // MySequence
    auto mySequenceType = std::make_unique<Asn1Acn::Types::Sequence>();
    {
        auto myChoiceRef = std::make_unique<Asn1Acn::Types::UserdefinedType>("MyChoice", "");
        myChoiceRef->setType(myChoiceType->clone());
        myChoiceRef->setIdentifier("sun");
        auto mySequenceComponentSun = std::make_unique<Asn1Acn::AsnSequenceComponent>("sun", "sun", false, std::nullopt,
                "", Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, location, std::move(myChoiceRef));
        mySequenceType->addComponent(std::move(mySequenceComponentSun));
    }
    {
        auto mySequenceOfRef = std::make_unique<Asn1Acn::Types::UserdefinedType>("MySequenceOf", "");
        mySequenceOfRef->setType(mySequenceOfType->clone());
        mySequenceOfRef->setIdentifier("moon");
        auto mySequenceComponentMoon =
                std::make_unique<Asn1Acn::AsnSequenceComponent>("moon", "moon", false, std::nullopt, "",
                        Asn1Acn::AsnSequenceComponent::Presence::NotSpecified, location, std::move(mySequenceOfRef));
        mySequenceType->addComponent(std::move(mySequenceComponentMoon));
    }

    auto assignment =
            std::make_unique<Asn1Acn::TypeAssignment>("MySequence", "MySequence", location, std::move(mySequenceType));

    bool convOk = false;
    auto valueMap = valueParser->parseAsn1Value(assignment.get(),
            "{ sun red : { left 11, right 99}, moon { { left  0, right 100 }, { left 10, right 12 } } }", &convOk);
    QVERIFY(convOk);

    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].type(), QVariant::String);
    QCOMPARE(valueMap["name"].toString(), QString("MySequence"));

    QCOMPARE(valueMap["children"].type(), QVariant::List);
    auto childrenValue = valueMap["children"].toList();
    QCOMPARE(childrenValue.size(), 2);

    {
        QCOMPARE(childrenValue[0].type(), QVariant::Map);
        auto sunComponent = childrenValue[0].toMap();
        QCOMPARE(sunComponent.size(), 2);
        QVERIFY(sunComponent.contains("name"));
        QCOMPARE(sunComponent.value("name").type(), QVariant::String);

        QCOMPARE(sunComponent.value("name").toString(), "sun");
        QVERIFY(sunComponent.contains("choice"));
        QCOMPARE(sunComponent.value("choice").type(), QVariant::Map);
    }
    {
        QCOMPARE(childrenValue[1].type(), QVariant::Map);
        auto sunComponent = childrenValue[1].toMap();
        QCOMPARE(sunComponent.size(), 2);
        QVERIFY(sunComponent.contains("name"));
        QCOMPARE(sunComponent.value("name").type(), QVariant::String);
        QCOMPARE(sunComponent.value("name").toString(), "moon");
        QVERIFY(sunComponent.contains("seqofvalue"));
    }
}

QTEST_APPLESS_MAIN(tst_Asn1ValueParser)

#include "tst_asn1valueparser.moc"
