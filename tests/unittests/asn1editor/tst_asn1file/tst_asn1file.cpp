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
    void testSequenceType();
    void testReferencedType();

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
    QCOMPARE(typeMap[ASN1_TYPE].toInt(), INTEGER);
    QCOMPARE(typeMap[ASN1_MIN].toInt(), 0);
    QCOMPARE(typeMap[ASN1_MAX].toInt(), 20);

    typeMap = asn1Types.at(1).toMap();
    QCOMPARE(typeMap.size(), 5);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MyReal"));
    QCOMPARE(typeMap[ASN1_TYPE].toInt(), DOUBLE);
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
    QCOMPARE(typeMap[ASN1_TYPE].toInt(), BOOL);

    typeMap = asn1Types.at(1).toMap();
    QCOMPARE(typeMap.size(), 4);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MyEnum"));
    QCOMPARE(typeMap[ASN1_TYPE].toInt(), ENUMERATED);

    auto values = typeMap[ASN1_VALUES].toList();

    QCOMPARE(values.count(), 3);
    QCOMPARE(values.at(0).toString(), QString("hello"));
    QCOMPARE(values.at(1).toString(), QString("world"));
    QCOMPARE(values.at(2).toString(), QString("how-are-you"));
}

void tst_Asn1File::testChoiceType()
{
    auto asn1Types = asn1File.parseText("TASTE-Dataview DEFINITIONS ::= BEGIN \
                                                MyChoice ::= CHOICE { hop BOOLEAN, lat REAL(-90.0..90.0), CareerEntry SEQUENCE { \
                                                    from  INTEGER, \
                                                    to    REAL (-1.1..3.14) OPTIONAL, \
                                                    team  OCTETSTRING (SIZE (1..256))  } } \
                                            END");

    QCOMPARE(asn1Types.count(), 1);

    auto typeMap = asn1Types.at(0).toMap();
    QCOMPARE(typeMap.size(), 4);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MyChoice"));
    QCOMPARE(typeMap[ASN1_TYPE].toInt(), CHOICE);

    auto choices = typeMap[ASN1_CHOICES].toList();

    QCOMPARE(choices.count(), 3);

    auto choiceItem = choices.at(0).toMap();
    QCOMPARE(choiceItem.size(), 3);
    QCOMPARE(choiceItem[ASN1_NAME].toString(), QString("hop"));
    QCOMPARE(choiceItem[ASN1_TYPE].toInt(), BOOL);

    choiceItem = choices.at(1).toMap();
    QCOMPARE(choiceItem.size(), 5);
    QCOMPARE(choiceItem[ASN1_NAME].toString(), QString("lat"));
    QCOMPARE(choiceItem[ASN1_TYPE].toInt(), DOUBLE);
    QCOMPARE(choiceItem[ASN1_MIN].toDouble(), -90.0);
    QCOMPARE(choiceItem[ASN1_MAX].toDouble(), 90.0);

    choiceItem = choices.at(2).toMap();
    QCOMPARE(choiceItem.size(), 4);
    QCOMPARE(choiceItem[ASN1_NAME].toString(), QString("CareerEntry"));
    QCOMPARE(choiceItem[ASN1_TYPE].toInt(), SEQUENCE);

    auto children = choiceItem[ASN1_CHILDREN].toList();
    QCOMPARE(children.size(), 3);

    auto child = children.at(0).toMap();
    QCOMPARE(child.size(), 3);
    QCOMPARE(child[ASN1_NAME].toString(), QString("from"));
    QCOMPARE(child[ASN1_TYPE].toInt(), INTEGER);

    child = children.at(1).toMap();
    QCOMPARE(child.size(), 5);
    QCOMPARE(child[ASN1_NAME].toString(), QString("to"));
    QCOMPARE(child[ASN1_TYPE].toInt(), DOUBLE);
    QCOMPARE(child[ASN1_MIN].toDouble(), -1.1);
    QCOMPARE(child[ASN1_MAX].toDouble(), 3.14);
    QVERIFY(child[ASN1_IS_OPTIONAL].toBool());

    child = children.at(2).toMap();
    QCOMPARE(child.size(), 5);
    QCOMPARE(child[ASN1_NAME].toString(), QString("team"));
    QCOMPARE(child[ASN1_TYPE].toInt(), STRING);
    QCOMPARE(child[ASN1_MIN].toInt(), 1);
    QCOMPARE(child[ASN1_MAX].toInt(), 256);
}

void tst_Asn1File::testSequenceType()
{
    auto asn1Types = asn1File.parseText("TASTE-Dataview DEFINITIONS ::= BEGIN \
                                                MySeq ::= SEQUENCE { \
                                                    from  INTEGER, \
                                                    to    REAL (-1.1 .. 3.14) OPTIONAL, \
                                                    team  OCTETSTRING (SIZE (1..256)) } \
                                            END");

    QCOMPARE(asn1Types.count(), 1);

    auto typeMap = asn1Types.at(0).toMap();
    QCOMPARE(typeMap.size(), 4);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MySeq"));
    QCOMPARE(typeMap[ASN1_TYPE].toInt(), SEQUENCE);

    auto children = typeMap[ASN1_CHILDREN].toList();
    QCOMPARE(children.size(), 3);

    auto child = children.at(0).toMap();
    QCOMPARE(child.size(), 3);
    QCOMPARE(child[ASN1_NAME].toString(), QString("from"));
    QCOMPARE(child[ASN1_TYPE].toInt(), INTEGER);

    child = children.at(1).toMap();
    QCOMPARE(child.size(), 5);
    QCOMPARE(child[ASN1_NAME].toString(), QString("to"));
    QCOMPARE(child[ASN1_TYPE].toInt(), DOUBLE);
    QCOMPARE(child[ASN1_MIN].toDouble(), -1.1);
    QCOMPARE(child[ASN1_MAX].toDouble(), 3.14);
    QVERIFY(child[ASN1_IS_OPTIONAL].toBool());

    child = children.at(2).toMap();
    QCOMPARE(child.size(), 5);
    QCOMPARE(child[ASN1_NAME].toString(), QString("team"));
    QCOMPARE(child[ASN1_TYPE].toInt(), STRING);
    QCOMPARE(child[ASN1_MIN].toInt(), 1);
    QCOMPARE(child[ASN1_MAX].toInt(), 256);
}

void tst_Asn1File::testReferencedType()
{
    auto asn1Types = asn1File.parseText("TASTE-Dataview DEFINITIONS ::= BEGIN \
                                                MySeq ::= SEQUENCE { from  INTEGER, foo BOOLEAN } \
                                                MySeqOf ::= SET (SIZE(1..3)) OF MySeq \
                                            END");

    QCOMPARE(asn1Types.count(), 2);

    auto seqMap = asn1Types.at(0).toMap();
    QCOMPARE(seqMap.size(), 4);
    QCOMPARE(seqMap[ASN1_NAME].toString(), QString("MySeq"));
    QCOMPARE(seqMap[ASN1_TYPE].toInt(), SEQUENCE);

    auto seqOfMap = asn1Types.at(1).toMap();
    QCOMPARE(seqOfMap.size(), 6);
    QCOMPARE(seqOfMap[ASN1_NAME].toString(), QString("MySeqOf"));
    QCOMPARE(seqOfMap[ASN1_TYPE].toInt(), SEQUENCEOF);
    QCOMPARE(seqOfMap[ASN1_MIN].toInt(), 1);
    QCOMPARE(seqOfMap[ASN1_MAX].toInt(), 3);

    auto seqOfType = seqOfMap[ASN1_SEQOFTYPE].toList();
    QCOMPARE(seqOfType.size(), 1);

    auto typeMap = seqOfType.at(0).toMap();
    QCOMPARE(typeMap.size(), 4);
    QCOMPARE(typeMap[ASN1_TYPE].toInt(), SEQUENCE);
    QCOMPARE(seqMap[ASN1_CHILDREN].toList().size(), typeMap[ASN1_CHILDREN].toList().size());
}

QTEST_APPLESS_MAIN(tst_Asn1File)

#include "tst_asn1file.moc"
