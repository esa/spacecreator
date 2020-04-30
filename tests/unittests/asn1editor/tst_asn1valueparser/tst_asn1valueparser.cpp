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

#include <QSignalSpy>
#include <QtTest>

using namespace asn1;

class tst_Asn1ValueParser : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();

    void testIntValues();
    void testRealValues();
    void testIntValuesFormatError();
    void testRealValuesFormatError();
    void testIntValuesWithRange();
    void testRealValuesWithRange();
    void testIntValuesWithRangeError();
    void testRealValuesWithRangeError();
    void testBoolValues();
    void testEnumValues();
    void testBoolValuesError();
    void testEnumValuesError();
    void testChoiceValue();
    void testChoiceValueError();
    void testSequenceValue();
    void testSequenceValueError();

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
    auto valueMap = valueParser->parseAsn1Value({ { "name", "MyInt" }, { "type", INTEGER } }, "3107");
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyInt"));
    QCOMPARE(valueMap["value"].toInt(), 3107);
}

void tst_Asn1ValueParser::testRealValues()
{
    auto valueMap = valueParser->parseAsn1Value({ { "name", "MyDouble" }, { "type", DOUBLE } }, "31.07");
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyDouble"));
    QCOMPARE(valueMap["value"].toDouble(), 31.07);
}

void tst_Asn1ValueParser::testIntValuesFormatError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    auto valueMap = valueParser->parseAsn1Value({ { "name", "MyInt" }, { "type", INTEGER } }, "31o7");
    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyInt"));
}

void tst_Asn1ValueParser::testRealValuesFormatError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    auto valueMap = valueParser->parseAsn1Value({ { "name", "MyDouble" }, { "type", DOUBLE } }, "31.o7");
    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyDouble"));
}

void tst_Asn1ValueParser::testIntValuesWithRange()
{
    auto valueMap = valueParser->parseAsn1Value(
            { { "name", "MyInt" }, { "type", INTEGER }, { "min", 5 }, { "max", 15 } }, "13");
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyInt"));
    QCOMPARE(valueMap["value"].toInt(), 13);
}

void tst_Asn1ValueParser::testRealValuesWithRange()
{
    auto valueMap = valueParser->parseAsn1Value(
            { { "name", "MyDouble" }, { "type", DOUBLE }, { "min", 10.0 }, { "max", 50.0 } }, "31.07");
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyDouble"));
    QCOMPARE(valueMap["value"].toDouble(), 31.07);
}

void tst_Asn1ValueParser::testIntValuesWithRangeError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    auto valueMap = valueParser->parseAsn1Value(
            { { "name", "MyInt" }, { "type", INTEGER }, { "min", 1 }, { "max", 10 } }, "13");
    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyInt"));
}

void tst_Asn1ValueParser::testRealValuesWithRangeError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    auto valueMap = valueParser->parseAsn1Value(
            { { "name", "MyDouble" }, { "type", DOUBLE }, { "min", 10.0 }, { "max", 30.0 } }, "31.07");
    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyDouble"));
}

void tst_Asn1ValueParser::testBoolValues()
{
    auto valueMap = valueParser->parseAsn1Value({ { "name", "MyBool" }, { "type", BOOL } }, "TRUE");
    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyBool"));
    QVERIFY(valueMap["value"].toBool() == true);
}

void tst_Asn1ValueParser::testEnumValues()
{
    QVariantList enumValues = { "enum1", "enum2", "enum3" };
    auto valueMap = valueParser->parseAsn1Value(
            { { "name", "MyEnum" }, { "type", ENUMERATED }, { "values", enumValues } }, "enum2");

    QCOMPARE(valueMap.size(), 2);

    QCOMPARE(valueMap["name"].toString(), QString("MyEnum"));
    QCOMPARE(valueMap["value"].toString(), QString("enum2"));
}

void tst_Asn1ValueParser::testBoolValuesError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    auto valueMap = valueParser->parseAsn1Value({ { "name", "MyBool" }, { "type", BOOL } }, "true");
    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyBool"));
}

void tst_Asn1ValueParser::testEnumValuesError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    QVariantList enumValues = { "enum1", "enum2", "enum3" };
    auto valueMap = valueParser->parseAsn1Value(
            { { "name", "MyEnum" }, { "type", ENUMERATED }, { "values", enumValues } }, "enum");

    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyEnum"));
}

void tst_Asn1ValueParser::testChoiceValue()
{
    QVariantList choices;
    QVariantMap choice = { { "name", "choiceInt" }, { "type", INTEGER } };
    choices.append(choice);

    choice = { { "name", "choiceReal" }, { "type", DOUBLE } };
    choices.append(choice);

    auto valueMap = valueParser->parseAsn1Value({ { "name", "MyChoice" }, { "type", CHOICE }, { "choices", choices } },
                                                "choiceReal : 31.07");
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

    QVariantList choices;
    QVariantMap choice = { { "name", "choiceInt" }, { "type", INTEGER } };
    choices.append(choice);

    choice = { { "name", "choiceReal" }, { "type", DOUBLE } };
    choices.append(choice);

    auto valueMap = valueParser->parseAsn1Value({ { "name", "MyChoice" }, { "type", CHOICE }, { "choices", choices } },
                                                "choice : TRUE");
    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 1);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MyChoice"));
}

void tst_Asn1ValueParser::testSequenceValue()
{
    QVariantList children;
    QVariantMap child = { { "name", "intVal" }, { "type", INTEGER } };
    children.append(child);

    child = { { "name", "realVal" }, { "type", DOUBLE } };
    children.append(child);

    auto valueMap =
            valueParser->parseAsn1Value({ { "name", "MySequence" }, { "type", SEQUENCE }, { "children", children } },
                                        "{ intVal 3107, realVal 31.07 }");

    QCOMPARE(valueMap.size(), 2);
    QCOMPARE(valueMap["name"].toString(), QString("MySequence"));

    auto childrenValue = valueMap["children"].toList();
    QCOMPARE(childrenValue.size(), 2);

    auto childValue = childrenValue.at(0).toMap();
    QCOMPARE(childValue.size(), 2);
    QCOMPARE(childValue["name"].toString(), QString("intVal"));
    QCOMPARE(childValue["value"].toInt(), 3107);

    childValue = childrenValue.at(1).toMap();
    QCOMPARE(childValue.size(), 2);
    QCOMPARE(childValue["name"].toString(), QString("realVal"));
    QCOMPARE(childValue["value"].toDouble(), 31.07);
}

void tst_Asn1ValueParser::testSequenceValueError()
{
    QSignalSpy spy(valueParser, SIGNAL(parseError(const QString &)));

    QVariantList children;
    QVariantMap child = { { "name", "intVal" }, { "type", INTEGER } };
    children.append(child);

    child = { { "name", "realVal" }, { "type", DOUBLE } };
    children.append(child);

    auto valueMap =
            valueParser->parseAsn1Value({ { "name", "MySequence" }, { "type", SEQUENCE }, { "children", children } },
                                        "{ intVal 31o7, realVal 31.07 }");

    QCOMPARE(valueMap.size(), 0);
    QCOMPARE(spy.count(), 2);

    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for intVal"));

    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Incorrect value for MySequence"));
}

QTEST_APPLESS_MAIN(tst_Asn1ValueParser)

#include "tst_asn1valueparser.moc"
