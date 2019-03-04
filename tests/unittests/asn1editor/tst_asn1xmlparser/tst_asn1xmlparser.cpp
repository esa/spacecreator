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
#include "asn1xmlparser.h"

#include <QDomElement>
#include <QDomNodeList>
#include <QSignalSpy>
#include <QtTest>

using namespace asn1;

class tst_Asn1XMLParser : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testFileOpenError();
    void testInvalidXMLFormat();
    void testEmptyFile();
    void testIntRealTypes();
    void testBoolEnumTypes();
    void testChoiceType();
    void testSequenceType();

private:
    Asn1XMLParser *xmlParser = nullptr;
    ASN1Type toAsn1Type(const QVariant &value) { return static_cast<ASN1Type>(value.toInt()); }
};

void tst_Asn1XMLParser::init()
{
    xmlParser = new Asn1XMLParser;
}

void tst_Asn1XMLParser::cleanup()
{
    delete xmlParser;
    xmlParser = nullptr;
}

void tst_Asn1XMLParser::testFileOpenError()
{
    QSignalSpy spy(xmlParser, SIGNAL(parseError(const QString &)));

    xmlParser->parseAsn1XmlFile("some_dummy_file_name.xml");

    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("File not found"));
}

void tst_Asn1XMLParser::testInvalidXMLFormat()
{
    QSignalSpy spy(xmlParser, SIGNAL(parseError(const QString &)));

    xmlParser->parseAsn1XmlFile(QFINDTESTDATA("invalid_format.xml"));

    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Invalid XML format"));
}

void tst_Asn1XMLParser::testEmptyFile()
{
    auto asn1Types = xmlParser->parseAsn1XmlFile(QFINDTESTDATA("empty.xml"));

    QCOMPARE(asn1Types.count(), 0);
}

void tst_Asn1XMLParser::testIntRealTypes()
{
    auto asn1Types = xmlParser->parseAsn1XmlFile(QFINDTESTDATA("number_type.xml"));

    QCOMPARE(asn1Types.count(), 2);

    auto typeMap = asn1Types.at(0).toMap();
    QCOMPARE(typeMap.size(), 5);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MyInt"));
    QCOMPARE(toAsn1Type(typeMap[ASN1_TYPE]), INTEGER);
    QCOMPARE(typeMap[ASN1_MIN].toInt(), 0);
    QCOMPARE(typeMap[ASN1_MAX].toInt(), 20);

    typeMap = asn1Types.at(1).toMap();
    QCOMPARE(typeMap.size(), 5);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MyReal"));
    QCOMPARE(toAsn1Type(typeMap[ASN1_TYPE]), DOUBLE);
    QCOMPARE(typeMap[ASN1_MIN].toDouble(), 0.0);
    QCOMPARE(typeMap[ASN1_MAX].toDouble(), 1000.0);
}

void tst_Asn1XMLParser::testBoolEnumTypes()
{
    auto asn1Types = xmlParser->parseAsn1XmlFile(QFINDTESTDATA("boolenum_type.xml"));

    QCOMPARE(asn1Types.count(), 2);

    auto typeMap = asn1Types.at(0).toMap();
    QCOMPARE(typeMap.size(), 4);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MyBOOL"));
    QCOMPARE(toAsn1Type(typeMap[ASN1_TYPE]), BOOL);

    typeMap = asn1Types.at(1).toMap();
    QCOMPARE(typeMap.size(), 4);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("TypeEnumerated"));
    QCOMPARE(toAsn1Type(typeMap[ASN1_TYPE]), ENUMERATED);

    auto values = typeMap[ASN1_VALUES].toList();

    QCOMPARE(values.count(), 3);

    QCOMPARE(values.at(0).toString(), QString("red"));
    QCOMPARE(values.at(1).toString(), QString("green"));
    QCOMPARE(values.at(2).toString(), QString("blue"));
}

void tst_Asn1XMLParser::testChoiceType()
{
    auto asn1Types = xmlParser->parseAsn1XmlFile(QFINDTESTDATA("choice_type.xml"));

    QCOMPARE(asn1Types.count(), 1);

    auto typeMap = asn1Types.at(0).toMap();
    QCOMPARE(typeMap.size(), 4);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MyChoice"));
    QCOMPARE(toAsn1Type(typeMap[ASN1_TYPE]), CHOICE);

    auto choices = typeMap[ASN1_CHOICES].toList();

    QCOMPARE(choices.count(), 2);

    auto choiceItem = choices.at(0).toMap();
    QCOMPARE(choiceItem.size(), 4);
    QCOMPARE(choiceItem[ASN1_NAME].toString(), QString("hop"));
    QCOMPARE(toAsn1Type(choiceItem[ASN1_TYPE]), BOOL);

    choiceItem = choices.at(1).toMap();
    QCOMPARE(choiceItem.size(), 5);
    QCOMPARE(choiceItem[ASN1_NAME].toString(), QString("lat"));
    QCOMPARE(toAsn1Type(choiceItem[ASN1_TYPE]), DOUBLE);
    QCOMPARE(choiceItem[ASN1_MIN].toDouble(), -90.0);
    QCOMPARE(choiceItem[ASN1_MAX].toDouble(), 90.0);
}

void tst_Asn1XMLParser::testSequenceType()
{
    auto asn1Types = xmlParser->parseAsn1XmlFile(QFINDTESTDATA("sequence_type.xml"));

    QCOMPARE(asn1Types.count(), 1);

    auto typeMap = asn1Types.at(0).toMap();
    QCOMPARE(typeMap.size(), 4);
    QCOMPARE(typeMap[ASN1_NAME].toString(), QString("MySeq"));
    QCOMPARE(toAsn1Type(typeMap[ASN1_TYPE]), SEQUENCE);

    auto children = typeMap[ASN1_CHILDREN].toList();
    QCOMPARE(children.size(), 2);

    auto child = children.at(0).toMap();
    QCOMPARE(child.size(), 4);
    QCOMPARE(child[ASN1_NAME].toString(), QString("foo"));
    QCOMPARE(toAsn1Type(child[ASN1_TYPE]), BOOL);

    child = children.at(1).toMap();
    QCOMPARE(child.size(), 5);
    QCOMPARE(child[ASN1_NAME].toString(), QString("int2Val"));
    QCOMPARE(toAsn1Type(child[ASN1_TYPE]), INTEGER);
    QCOMPARE(child[ASN1_MIN].toInt(), -10);
    QCOMPARE(child[ASN1_MAX].toInt(), 10);
}

QTEST_APPLESS_MAIN(tst_Asn1XMLParser)

#include "tst_asn1xmlparser.moc"
