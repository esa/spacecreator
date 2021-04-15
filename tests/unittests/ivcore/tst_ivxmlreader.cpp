/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivfunction.h"
#include "ivobject.h"
#include "ivxmlreader.h"
#include "xmlcommon.h"

#include <QBuffer>
#include <QSignalSpy>
#include <QtTest>

class IVXMLReader : public QObject
{
    Q_OBJECT
public:
private:
    void runReader(const XmlFileMock &xml);

private Q_SLOTS:
    void test_emptyInterfaceViewDoc();
    void test_singleItems();
    void test_allItems();
    void test_readMetaData();
    void test_readFunction();
};

void IVXMLReader::runReader(const XmlFileMock &xml)
{
    QByteArray result = xml.m_xmlContent.toUtf8();
    QBuffer buffer(&result);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    ivm::IVXMLReader reader;
    connect(&reader, &ivm::IVXMLReader::objectsParsed, this, [&xml](const QVector<ivm::IVObject *> &objectsList) {
        QCOMPARE(objectsList.size(), xml.expectedObjectCount());
    });
    connect(&reader, &ivm::IVXMLReader::error, [](const QString &msg) { qWarning() << msg; });

    QSignalSpy spyObjectsParsed(&reader, &ivm::IVXMLReader::objectsParsed);
    QSignalSpy spyError(&reader, &ivm::IVXMLReader::error);

    const bool ok = reader.read(&buffer);
    QCOMPARE(ok, xml.m_canBeParsed);

    if (xml.m_canBeParsed)
        QCOMPARE(spyObjectsParsed.count(), 1);

    QCOMPARE(spyError.count(), xml.m_expectedErrorCount);

    if (spyObjectsParsed.count()) {
        const QList<QVariant> &objectsList = spyObjectsParsed.takeFirst();
        const QVector<ivm::IVObject *> &ivObjects = objectsList.first().value<QVector<ivm::IVObject *>>();
        QCOMPARE(ivObjects.size(), xml.expectedObjectCount());
    }

    if (xml.m_canBeParsed)
        while (spyError.count())
            qWarning() << spyError.takeFirst().first().toString();
}

void IVXMLReader::test_emptyInterfaceViewDoc()
{
    for (auto xml : { XmlHelper::instance()->EmptyFile, XmlHelper::instance()->EmptyDoc })
        runReader(xml);
}

void IVXMLReader::test_singleItems()
{
    runReader(XmlHelper::instance()->SingleFunction);
    runReader(XmlHelper::instance()->SingleFunctionType);
    runReader(XmlHelper::instance()->SingleComment);
    /*runReader(XmlHelper::instance()->SingleIfaceInvalid, */ // for now there is an Q_ASSERT (crash)
    runReader(XmlHelper::instance()->SingleIfaceValid);
    runReader(XmlHelper::instance()->ParametrizedIfaces);
    runReader(XmlHelper::instance()->SingleConnectionOrphan);
    runReader(XmlHelper::instance()->SingleConnectionValid);
    runReader(XmlHelper::instance()->SingleConnectionValidMultipoint);
}

void IVXMLReader::test_allItems()
{
    for (auto xml : { XmlHelper::instance()->AllItems })
        runReader(xml);
}

void IVXMLReader::test_readMetaData()
{
    QByteArray xml("<InterfaceView asn1file=\"dataview.asn\" mscfile=\"newfile.msc\"/>");

    QBuffer buffer(&xml);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    QVariantMap metadata;
    ivm::IVXMLReader reader;
    connect(&reader, &ivm::IVXMLReader::metaDataParsed, this,
            [&metadata](const QVariantMap &data) { metadata = data; });

    QSignalSpy spyError(&reader, &ivm::IVXMLReader::error);

    const bool ok = reader.read(&buffer);
    QVERIFY(ok);

    QCOMPARE(spyError.count(), 0);
    QCOMPARE(metadata.count(), 2);
    QCOMPARE(metadata["asn1file"].toString(), QString("dataview.asn"));
    QCOMPARE(metadata["mscfile"].toString(), QString("newfile.msc"));
}

void IVXMLReader::test_readFunction()
{
    QByteArray xml("<InterfaceView>"
                   "<Function name=\"Global_Fn2\" is_type=\"NO\">"
                   "<ContextParameter name=\"duration\" type=\"MyInt\" value=\"60\"/>"
                   "<ContextParameter name=\"trigger\" type=\"Timer\" value=\"\"/>"
                   "</Function></InterfaceView>");

    QBuffer buffer(&xml);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    ivm::IVFunction *function = nullptr;
    ivm::IVXMLReader reader;
    connect(&reader, &ivm::IVXMLReader::objectsParsed, this,
            [&function](const QVector<ivm::IVObject *> &objectsList) {
                QCOMPARE(objectsList.size(), 1);
                function = qobject_cast<ivm::IVFunction *>(objectsList[0]);
            });

    QSignalSpy spyError(&reader, &ivm::IVXMLReader::error);

    const bool ok = reader.read(&buffer);
    QVERIFY(ok);
    QCOMPARE(spyError.count(), 0);

    QVERIFY(function != nullptr);
    QCOMPARE(function->contextParams().size(), 2);

    ivm::ContextParameter param1 = function->contextParam("duration");
    QVERIFY(!param1.isNull());
    QCOMPARE(param1.paramType(), ivm::BasicParameter::Type::Other);
    QCOMPARE(param1.paramTypeName(), QString("MyInt"));
    QCOMPARE(param1.defaultValue(), QVariant::fromValue(60));

    ivm::ContextParameter param2 = function->contextParam("trigger");
    QVERIFY(!param2.isNull());
    QCOMPARE(param2.paramType(), ivm::BasicParameter::Type::Timer);
}

QTEST_APPLESS_MAIN(IVXMLReader)

#include "tst_ivxmlreader.moc"
