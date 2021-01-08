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
#include "types/builtintypes.h"

#include <QSignalSpy>
#include <QtTest>

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
    void testIntRealTypes();
    void testBoolEnumTypes();
    void testChoiceType();
    void testSequenceType();
    void testMixedTypes();
    void testChoiceReference();

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

void tst_Asn1Reader::testIntRealTypes()
{
    std::unique_ptr<Asn1Acn::File> asn1Types = xmlParser->parseAsn1XmlFile(QFINDTESTDATA("number_type.xml"));
    QVERIFY(asn1Types);

    const Asn1Acn::Definitions *definitions = asn1Types->definitions("ModuleTest");
    QVERIFY(definitions != nullptr);

    QCOMPARE(definitions->types().size(), 3);

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign1 = definitions->types().at(0);
    QCOMPARE(typeAssign1->name(), QString("MyInt"));
    QCOMPARE(typeAssign1->type()->typeName(), QString("INTEGER"));
    const QVariantMap &data1 = typeAssign1->type()->parameters();
    QCOMPARE(data1.size(), 2);
    QCOMPARE(data1[ASN1_MIN].toLongLong(), static_cast<qlonglong>(0));
    QCOMPARE(data1[ASN1_MAX].toLongLong(), static_cast<qlonglong>(20));

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign2 = definitions->types().at(1);
    QCOMPARE(typeAssign2->name(), QString("MyReal"));
    QCOMPARE(typeAssign2->type()->typeName(), QString("REAL"));
    const QVariantMap &data2 = typeAssign2->type()->parameters();
    QCOMPARE(data2.size(), 2);
    QCOMPARE(data2[ASN1_MIN].toDouble(), 0.0);
    QCOMPARE(data2[ASN1_MAX].toLongLong(), 1000.0);

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign3 = definitions->types().at(2);
    QCOMPARE(typeAssign3->name(), QString("T-UInt32"));
    QCOMPARE(typeAssign3->type()->typeName(), QString("INTEGER"));
    const QVariantMap &data3 = typeAssign3->type()->parameters();
    QCOMPARE(data3.size(), 2);
    QCOMPARE(data3[ASN1_MIN].toLongLong(), static_cast<qlonglong>(0));
    QCOMPARE(data3[ASN1_MAX].toLongLong(), static_cast<qlonglong>(4294967295));
}

void tst_Asn1Reader::testBoolEnumTypes()
{
    std::unique_ptr<Asn1Acn::File> asn1Types = xmlParser->parseAsn1XmlFile(QFINDTESTDATA("boolenum_type.xml"));
    const Asn1Acn::Definitions *definitions = asn1Types->definitions("ModuleTest");

    QCOMPARE(definitions->types().size(), 2);

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign1 = definitions->types().at(0);
    QCOMPARE(typeAssign1->name(), QString("MyBOOL"));
    QCOMPARE(typeAssign1->type()->typeName(), QString("BOOLEAN"));

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign2 = definitions->types().at(1);
    QCOMPARE(typeAssign2->name(), QString("TypeEnumerated"));
    QCOMPARE(typeAssign2->type()->typeName(), QString("ENUMERATED"));
    const QVariantMap &data2 = typeAssign2->type()->parameters();
    QCOMPARE(data2.size(), 1);
    QVariantList enumValues = data2[ASN1_VALUES].toList();
    QCOMPARE(enumValues.count(), 3);
    QCOMPARE(enumValues.at(0).toString(), QString("red"));
    QCOMPARE(enumValues.at(1).toString(), QString("green"));
    QCOMPARE(enumValues.at(2).toString(), QString("blue"));
}

void tst_Asn1Reader::testChoiceType()
{
    std::unique_ptr<Asn1Acn::File> asn1Types = xmlParser->parseAsn1XmlFile(QFINDTESTDATA("choice_type.xml"));
    const Asn1Acn::Definitions *definitions = asn1Types->definitions("ModuleTest");
    QCOMPARE(definitions->types().size(), 1);

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign1 = definitions->types().at(0);
    QCOMPARE(typeAssign1->name(), QString("MyChoice"));
    QCOMPARE(typeAssign1->type()->typeName(), QString("CHOICE"));

    auto choice = dynamic_cast<const Asn1Acn::Types::Choice *>(typeAssign1->type());
    QCOMPARE(choice->children().size(), 2);

    const std::unique_ptr<Asn1Acn::Types::Type> &choice1 = choice->children().at(0);
    QCOMPARE(choice1->typeName(), QString("BOOLEAN"));

    const std::unique_ptr<Asn1Acn::Types::Type> &choice2 = choice->children().at(1);
    QCOMPARE(choice2->typeName(), QString("REAL"));
    const QVariantMap &data = choice2->parameters();
    QCOMPARE(data.size(), 2);
    QCOMPARE(data[ASN1_MIN].toDouble(), -90.0);
    QCOMPARE(data[ASN1_MAX].toLongLong(), 90.0);
}

void tst_Asn1Reader::testSequenceType()
{
    std::unique_ptr<Asn1Acn::File> asn1Types = xmlParser->parseAsn1XmlFile(QFINDTESTDATA("sequence_type.xml"));
    const Asn1Acn::Definitions *definitions = asn1Types->definitions("ModuleTest");
    QCOMPARE(definitions->types().size(), 1);

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign1 = definitions->types().at(0);
    QCOMPARE(typeAssign1->name(), QString("MySeq"));
    QCOMPARE(typeAssign1->type()->typeName(), QString("SEQUENCE"));

    auto sequence = dynamic_cast<const Asn1Acn::Types::Sequence *>(typeAssign1->type());
    QCOMPARE(sequence->children().size(), 2);

    const std::unique_ptr<Asn1Acn::Types::Type> &sequence1 = sequence->children().at(0);
    QCOMPARE(sequence1->typeName(), QString("BOOLEAN"));

    const std::unique_ptr<Asn1Acn::Types::Type> &sequence2 = sequence->children().at(1);
    QCOMPARE(sequence2->typeName(), QString("INTEGER"));
    const QVariantMap &data = sequence2->parameters();
    QCOMPARE(data.size(), 2);
    QCOMPARE(data[ASN1_MIN].toInt(), -10);
    QCOMPARE(data[ASN1_MAX].toInt(), 10);
}

void tst_Asn1Reader::testMixedTypes()
{
    std::unique_ptr<Asn1Acn::File> asn1Types = xmlParser->parseAsn1XmlFile(QFINDTESTDATA("mixed_types01.xml"));
    const Asn1Acn::Definitions *definitions = asn1Types->definitions("TASTE-BasicTypes");
    //    QCOMPARE(definitions->types().size(), 10);

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign1 = definitions->types().at(0);
    QCOMPARE(typeAssign1->name(), QString("T-UInt32"));
    QCOMPARE(typeAssign1->type()->typeName(), QString("INTEGER"));

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign2 = definitions->types().at(1);
    QCOMPARE(typeAssign2->name(), QString("TASTE-Peek-id"));
    QCOMPARE(typeAssign2->type()->typeName(), QString("INTEGER"));

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign3 = definitions->types().at(2);
    QCOMPARE(typeAssign3->name(), QString("TASTE-Peek-id-list"));
    QCOMPARE(typeAssign3->type()->typeName(), QString("SEQUENCE OF"));

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign4 = definitions->types().at(3);
    QCOMPARE(typeAssign4->name(), QString("FixedIntList"));
    QCOMPARE(typeAssign4->type()->typeName(), QString("SEQUENCE OF"));

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign5 = definitions->types().at(4);
    QCOMPARE(typeAssign5->name(), QString("MyEnum"));
    QCOMPARE(typeAssign5->type()->typeName(), QString("ENUMERATED"));

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign6 = definitions->types().at(5);
    QCOMPARE(typeAssign6->name(), QString("MyChoice"));
    QCOMPARE(typeAssign6->type()->typeName(), QString("CHOICE"));

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign7 = definitions->types().at(6);
    QCOMPARE(typeAssign7->name(), QString("MySeq"));
    QCOMPARE(typeAssign7->type()->typeName(), QString("SEQUENCE"));

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign8 = definitions->types().at(7);
    QCOMPARE(typeAssign8->name(), QString("MySetOf"));
    QCOMPARE(typeAssign8->type()->typeName(), QString("SEQUENCE OF"));

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign9 = definitions->types().at(8);
    QCOMPARE(typeAssign9->name(), QString("MySeqOf"));
    QCOMPARE(typeAssign9->type()->typeName(), QString("SEQUENCE OF"));

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign10 = definitions->types().at(9);
    QCOMPARE(typeAssign10->name(), QString("MySimpleSeq"));
    QCOMPARE(typeAssign10->type()->typeName(), QString("SEQUENCE"));
}

void tst_Asn1Reader::testChoiceReference()
{
    std::unique_ptr<Asn1Acn::File> asn1Types = xmlParser->parseAsn1XmlFile(QFINDTESTDATA("choice_reference.xml"));
    const Asn1Acn::Definitions *definitions = asn1Types->definitions("DV");
    QCOMPARE(definitions->types().size(), 2);

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign1 = definitions->types().at(0);
    QCOMPARE(typeAssign1->name(), QString("MyInt"));
    QCOMPARE(typeAssign1->type()->typeName(), QString("INTEGER"));
    const QVariantMap &data1 = typeAssign1->type()->parameters();
    QCOMPARE(data1.size(), 2);
    QCOMPARE(data1[ASN1_MIN].toLongLong(), static_cast<qlonglong>(0));
    QCOMPARE(data1[ASN1_MAX].toLongLong(), static_cast<qlonglong>(255));

    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssign2 = definitions->types().at(1);
    QCOMPARE(typeAssign2->name(), QString("MyChoice"));
    QCOMPARE(typeAssign2->type()->typeName(), QString("CHOICE"));

    auto choice = dynamic_cast<const Asn1Acn::Types::Choice *>(typeAssign2->type());
    QCOMPARE(choice->children().size(), 2);

    const std::unique_ptr<Asn1Acn::Types::Type> &choice1 = choice->children().at(0);
    QCOMPARE(choice1->typeName(), QString("MyInt"));
    const QVariantMap &data = choice1->parameters();
    QCOMPARE(data.size(), 2);
    QCOMPARE(data[ASN1_MIN].toLongLong(), static_cast<qlonglong>(0));
    QCOMPARE(data[ASN1_MAX].toLongLong(), static_cast<qlonglong>(255));

    const std::unique_ptr<Asn1Acn::Types::Type> &choice2 = choice->children().at(1);
    QCOMPARE(choice2->typeName(), QString("BOOLEAN"));
}

QTEST_APPLESS_MAIN(tst_Asn1Reader)

#include "tst_asn1reader.moc"
