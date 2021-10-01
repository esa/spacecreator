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
#include "asn1reader.h"
#include "definitions.h"
#include "file.h"
#include "typeassignment.h"
#include "types/choice.h"
#include "types/sequence.h"
#include "asnsequencecomponent.h"

#include <QSignalSpy>
#include <QtTest>
#include <iostream>

using namespace Asn1Acn;

class tst_Asn1Reader : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testFileOpenError();
    void testInvalidXMLFormat();
    void testEmptyFile();
    void testLineNumber_data();
    void testLineNumber();
    void testOptional();

private:
    Asn1Reader *xmlParser = nullptr;
    Asn1Acn::Types::Type::ASN1Type toAsn1Type(const QVariant &value)
    {
        return static_cast<Asn1Acn::Types::Type::ASN1Type>(value.toInt());
    }
};

void tst_Asn1Reader::init()
{
    xmlParser = new Asn1Reader;
    connect(xmlParser, &Asn1Reader::parseError,
            [](const QString &msg) { std::cerr << "!!!! " << msg.toStdString() << std::endl; });
}

void tst_Asn1Reader::cleanup()
{
    delete xmlParser;
    xmlParser = nullptr;
}

void tst_Asn1Reader::testFileOpenError()
{
    QSignalSpy spy(xmlParser, SIGNAL(parseError(const QString &)));

    std::unique_ptr<Asn1Acn::File> asn1Types = xmlParser->parseAsn1XmlFile("some_dummy_file_name.xml");
    QVERIFY(!asn1Types);

    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("File not found"));
}

void tst_Asn1Reader::testInvalidXMLFormat()
{
    QSignalSpy spy(xmlParser, SIGNAL(parseError(const QString &)));
    xmlParser->parseAsn1XmlFile(QFINDTESTDATA("invalid_format.xml"));
    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), tr("Invalid XML format"));
}

void tst_Asn1Reader::testEmptyFile()
{
    std::unique_ptr<Asn1Acn::File> asn1Types = xmlParser->parseAsn1XmlFile(QFINDTESTDATA("empty.xml"));
    QCOMPARE(asn1Types->definitionsList().size(), 1);
    QCOMPARE(asn1Types->definitionsList().at(0)->types().size(), 0);
}

void tst_Asn1Reader::testLineNumber_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<int>("result");

    QTest::newRow("empty") << "" << -1;
    QTest::newRow("nonsense") << "nonsense" << -1;
    QTest::newRow("missing line") << "/tmp/Taste07.asn: error: no viable alternative at input 'INTEGER'\n" << -1;
    QTest::newRow("correct") << "/tmp/Taste07.asn:3:10: error: no viable alternative at input 'INTEGER'\n" << 3;
}

void tst_Asn1Reader::testLineNumber()
{
    QFETCH(QString, input);
    QFETCH(int, result);
    QCOMPARE(xmlParser->lineNumberFromError(input), result);
}

void tst_Asn1Reader::testOptional()
{
    QStringList errorsList;
    std::unique_ptr<Asn1Acn::File> asn1Types =
            xmlParser->parseAsn1File(QFileInfo(QFINDTESTDATA("ip.asn")), &errorsList);
    const Asn1Acn::Definitions *definitions = asn1Types->definitions("POHICDRIVER-IP");
    QCOMPARE(definitions->types().size(), 3);

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign1 = definitions->types().back();
    QCOMPARE(typeAssign1->typeEnum(), Asn1Acn::Types::Type::SEQUENCE);

    auto sequence = dynamic_cast<const Asn1Acn::Types::Sequence *>(typeAssign1->type());
    QCOMPARE(sequence->components().size(), 8);

    const auto *sequence1 = dynamic_cast<Asn1Acn::AsnSequenceComponent*>(sequence->components().at(0).get());
    QCOMPARE(sequence1->name(), QString("devname"));
    QCOMPARE(sequence1->type()->typeName(), QString("IA5String"));
    QVERIFY(!sequence1->isOptional());

    const auto &sequence2 = dynamic_cast<Asn1Acn::AsnSequenceComponent*>(sequence->components().at(1).get());
    QCOMPARE(sequence2->name(), QString("address"));
    QCOMPARE(sequence2->type()->typeName(), QString("IA5String"));
    QVERIFY(!sequence2->isOptional());

    const auto &sequence3 = dynamic_cast<Asn1Acn::AsnSequenceComponent*>(sequence->components().at(2).get());
    QCOMPARE(sequence3->name(), QString("broadcast"));
    QCOMPARE(sequence3->type()->typeName(), QString("IA5String"));
    QVERIFY(sequence3->isOptional());

    const auto &sequence4 = dynamic_cast<Asn1Acn::AsnSequenceComponent*>(sequence->components().at(7).get());
    QCOMPARE(sequence4->name(), QString("port"));
    QCOMPARE(sequence4->type()->typeName(), QString("Port-T"));
    QVERIFY(!sequence4->isOptional());
}

QTEST_APPLESS_MAIN(tst_Asn1Reader)

#include "tst_asn1reader.moc"
