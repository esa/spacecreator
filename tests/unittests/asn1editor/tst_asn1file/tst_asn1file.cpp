/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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
#include "asn1file.h"

#include <QtTest>

using namespace asn1;

class tst_Asn1File : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testIntRealTypes();
    void testBoolEnumTypes();
    void testChoiceType();
    void testSequenceOfType();

private:
    Asn1File asn1File;
};

void tst_Asn1File::init() {}

void tst_Asn1File::cleanup() {}

void tst_Asn1File::testIntRealTypes()
{
    auto asn1Types = asn1File.parseText("TASTE-Dataview DEFINITIONS ::= BEGIN \
                                            MyInt ::= INTEGER (0..20) \
                                            MyReal ::= REAL (0.0 .. 100.0) \
                                        END");

    QCOMPARE(asn1Types.count(), 2);

    auto typeMap = asn1Types.at(0).toMap();
    QCOMPARE(typeMap.size(), 5);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MyInt"));
    QCOMPARE(typeMap[ASN1_TYPE].toString(), QString(ASN1_TYPE_INTEGER));
    QCOMPARE(typeMap[ASN1_MIN].toInt(), 0);
    QCOMPARE(typeMap[ASN1_MAX].toInt(), 20);

    typeMap = asn1Types.at(1).toMap();
    QCOMPARE(typeMap.size(), 5);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MyReal"));
    QCOMPARE(typeMap[ASN1_TYPE].toString(), QString(ASN1_TYPE_DOUBLE));
    QCOMPARE(typeMap[ASN1_MIN].toDouble(), 0.0);
    QCOMPARE(typeMap[ASN1_MAX].toDouble(), 100.0);
}

void tst_Asn1File::testBoolEnumTypes()
{
    auto asn1Types = asn1File.parseText("TASTE-Dataview DEFINITIONS ::= BEGIN \
                                            MyBOOL ::= BOOLEAN \
                                            MyEnum ::= ENUMERATED { hello, world, how-are-you } \
                                        END");

    QCOMPARE(asn1Types.count(), 2);

    auto typeMap = asn1Types.at(0).toMap();
    QCOMPARE(typeMap.size(), 3);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MyBOOL"));
    QCOMPARE(typeMap[ASN1_TYPE].toString(), QString(ASN1_TYPE_BOOL));

    typeMap = asn1Types.at(1).toMap();
    QCOMPARE(typeMap.size(), 4);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MyEnum"));
    QCOMPARE(typeMap[ASN1_TYPE].toString(), QString(ASN1_TYPE_ENUMERATED));

    auto values = typeMap[ASN1_VALUES].toList();

    QCOMPARE(values.count(), 3);
    QCOMPARE(values.at(0).toString(), QString("hello"));
    QCOMPARE(values.at(1).toString(), QString("world"));
    QCOMPARE(values.at(2).toString(), QString("how-are-you"));
}

void tst_Asn1File::testChoiceType()
{
    auto asn1Types = asn1File.parseText("TASTE-Dataview DEFINITIONS ::= BEGIN \
                                            MyChoice ::= CHOICE { hop BOOLEAN, lat REAL(-90.0..90.0) } \
                                        END");

    QCOMPARE(asn1Types.count(), 1);

    auto typeMap = asn1Types.at(0).toMap();
    QCOMPARE(typeMap.size(), 4);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MyChoice"));
    QCOMPARE(typeMap[ASN1_TYPE].toString(), QString(ASN1_TYPE_CHOICE));

    auto choices = typeMap[ASN1_CHOICES].toList();

    QCOMPARE(choices.count(), 2);

    auto choiceItem = choices.at(0).toMap();
    QCOMPARE(choiceItem.size(), 3);
    QCOMPARE(choiceItem[ASN1_NAME].toString(), QString("hop"));
    QCOMPARE(choiceItem[ASN1_TYPE].toString(), QString(ASN1_TYPE_BOOL));

    choiceItem = choices.at(1).toMap();
    QCOMPARE(choiceItem.size(), 5);
    QCOMPARE(choiceItem[ASN1_NAME].toString(), QString("lat"));
    QCOMPARE(choiceItem[ASN1_TYPE].toString(), QString(ASN1_TYPE_DOUBLE));
    QCOMPARE(choiceItem[ASN1_MIN].toDouble(), -90.0);
    QCOMPARE(choiceItem[ASN1_MAX].toDouble(), 90.0);
}

void tst_Asn1File::testSequenceOfType()
{
    //    auto asn1Types = asn1File.parseText("TASTE-Dataview DEFINITIONS ::= BEGIN \
//                                            MySeqOf ::= SEQUENCE (SIZE (0..2)) OF CHOICE { hop BOOLEAN, lat REAL(-90.0..90.0) } \
//                                        END");

    //    QCOMPARE(asn1Types.count(), 1);

    //    auto typeMap = asn1Types.at(0).toMap();
    //    QCOMPARE(typeMap.size(), 4);
    //    QCOMPARE(typeMap["name"].toString(), QString("MySeq"));
    //    QCOMPARE(typeMap["type"].toString(), QString("sequence"));

    //    auto children = typeMap["children"].toList();
    //    QCOMPARE(children.size(), 2);

    //    auto child = children.at(0).toMap();
    //    QCOMPARE(child.size(), 4);
    //    QCOMPARE(child["name"].toString(), QString("foo"));
    //    QCOMPARE(child["type"].toString(), QString("bool"));
    //    QCOMPARE(child["default"].toBool(), false);

    //    child = children.at(1).toMap();
    //    QCOMPARE(child.size(), 5);
    //    QCOMPARE(child["name"].toString(), QString("int2Val"));
    //    QCOMPARE(child["type"].toString(), QString("integer"));
    //    QCOMPARE(child["min"].toInt(), -10);
    //    QCOMPARE(child["max"].toInt(), 10);
}

QTEST_APPLESS_MAIN(tst_Asn1File)

#include "tst_asn1file.moc"
