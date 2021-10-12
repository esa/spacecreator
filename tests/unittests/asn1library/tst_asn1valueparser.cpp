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
#include "sourcelocation.h"
#include "typeassignment.h"
#include "types/boolean.h"
#include "types/choice.h"
#include "types/enumerated.h"
#include "types/ia5string.h"
#include "types/integer.h"
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
    void testEnumValues();
    void testBoolValuesError();
    void testEnumValuesError();
    void testChoiceValue();
    void testChoiceValueError();
    void testSequenceValue();
    void testSequenceValueError();
    void testSequenceOfValue();
    void testUserType();

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

    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "3107");
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyInt"));
    QCOMPARE(valueMap["value"].toInt(), 3107);
}

void tst_Asn1ValueParser::testStringValues()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::IA5String>();
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyString", "MyString", location, std::move(type));

    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "teststringforcheck");
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString::fromLatin1("MyString"));
    QCOMPARE(valueMap["value"].toString(), QString::fromLatin1("teststringforcheck"));
}

void tst_Asn1ValueParser::testRealValues()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Real>();
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyDouble", "MyDouble", location, std::move(type));

    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "31.07");
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
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "31o7");
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
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "31.o7");
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
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "13");
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
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "31.07");
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
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "13");
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
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "31.07");
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
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "TRUE");
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyBool"));
    QVERIFY(valueMap["value"].toBool() == true);
}

void tst_Asn1ValueParser::testIA5StringValues()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::IA5String>();
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyString", "MyString", location, std::move(type));
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "Potato");
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyString"));
    QVERIFY(valueMap["value"].toString() == "Potato");
}

void tst_Asn1ValueParser::testEnumValues()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Enumerated>();
    type->addItem(Asn1Acn::Types::EnumeratedItem(0, "enum1", 0));
    type->addItem(Asn1Acn::Types::EnumeratedItem(1, "enum2", 1));
    type->addItem(Asn1Acn::Types::EnumeratedItem(2, "enum3", 2));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyEnum", "MyEnum", location, std::move(type));
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "enum2");

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
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "true");
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
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "enum");

    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyEnum"));
}

void tst_Asn1ValueParser::testChoiceValue()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Choice>();
    auto choice1 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
            "choiceInt",
            "choiceInt",
            "choiceInt",
            "choiceInt",
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Integer>("choiceInt"));
    type->addComponent(std::move(choice1));
    auto choice2 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
            "choiceReal",
            "choiceReal",
            "choiceReal",
            "choiceReal",
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Real>("choiceReal"));
    type->addComponent(std::move(choice2));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyChoice", "MyChoice", location, std::move(type));
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "choiceReal : 31.07");
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
    auto choice1 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
            "choiceInt",
            "choiceInt",
            "choiceInt",
            "choiceInt",
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Integer>("choiceInt"));
    type->addComponent(std::move(choice1));
    auto choice2 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
            "choiceReal",
            "choiceReal",
            "choiceReal",
            "choiceReal",
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Real>("choiceReal"));
    type->addComponent(std::move(choice2));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyChoice", "MyChoice", location, std::move(type));
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "choice : TRUE");
    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyChoice"));
}

void tst_Asn1ValueParser::testSequenceValue()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Sequence>();
    auto sequence1 = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            "intVal",
            "intVal",
            false,
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Integer>("intVal"));
    type->addComponent(std::move(sequence1));
    auto sequence2 = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            "realVal",
            "realVal",
            false,
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Real>("realVal"));
    type->addComponent(std::move(sequence2));
    auto sequence3 = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            "boolVal",
            "boolVal",
            false,
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Boolean>("boolVal"));
    type->addComponent(std::move(sequence3));
    auto choice = std::make_unique<Asn1Acn::Types::Choice>("choiceVal");
    auto choice1 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
            "choice1",
            "choice1",
            "choice1",
            "choice1",
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Boolean>("choice1"));
    choice->addComponent(std::move(choice1));
    auto choice2 = std::make_unique<Asn1Acn::Types::ChoiceAlternative>(
            "choice2",
            "choice2",
            "choice2",
            "choice2",
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Integer>("choice2"));
    choice->addComponent(std::move(choice2));
    auto sequence4 = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            "choiceVal",
            "choiceVal",
            false,
            "",
            Asn1Acn::SourceLocation(),
            std::move(choice));
    type->addComponent(std::move(sequence4));

    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MySequence", "MySequence", location, std::move(type));
    auto valueMap = valueParser->parseAsn1Value(
            assignment.get(), "{ intVal 3107, realVal 31.07, boolVal TRUE, choiceVal choice1 : FALSE }");

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
    auto sequence1 = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            "intVal",
            "intVal",
            false,
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Integer>("intVal"));
    type->addComponent(std::move(sequence1));
    auto sequence2 = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            "realVal",
            "realVal",
            false,
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Real>("realVal"));
    type->addComponent(std::move(sequence2));
    auto sequence3 = std::make_unique<Asn1Acn::AsnSequenceComponent>(
            "boolVal",
            "boolVal",
            false,
            "",
            Asn1Acn::SourceLocation(),
            std::make_unique<Asn1Acn::Types::Boolean>("boolVal"));
    type->addComponent(std::move(sequence3));
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MySequence", "MySequence", location, std::move(type));
    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "{ intVal 31o7, realVal 31.07 }");

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

    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "{ blue, green }");

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
    auto colorAssignment = std::make_unique<Asn1Acn::TypeAssignment>("Color", "Color", location, std::move(colorType));

    auto userType = std::make_unique<Asn1Acn::Types::UserdefinedType>("Color", "", colorAssignment.get());

    auto type = std::make_unique<Asn1Acn::Types::SequenceOf>();
    auto range = Range<typename IntegerValue::Type>(2, 2);
    type->constraints().append(range);
    type->setItemsType(std::move(userType));
    auto assignment =
            std::make_unique<Asn1Acn::TypeAssignment>("MySequenceOf", "MySequenceOf", location, std::move(type));

    auto valueMap = valueParser->parseAsn1Value(assignment.get(), "{ blue, green }");

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

QTEST_APPLESS_MAIN(tst_Asn1ValueParser)

#include "tst_asn1valueparser.moc"
