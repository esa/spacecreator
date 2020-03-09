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
#include "tab_aadl/aadlobject.h"
#include "tab_aadl/aadlxmlreader.h"
#include "xmlcommon.h"

#include <QBuffer>
#include <QSignalSpy>
#include <QtTest>

// add necessary includes here

using namespace taste3::aadl;

class XMLReader : public QObject
{
    Q_OBJECT

    void runReader(const XmlFileMock &xml);
private slots:
    void test_emptyInterfaceViewDoc();
    void test_singleItems();
    void test_allItems();
};

void XMLReader::runReader(const XmlFileMock &xml)
{
    QByteArray result = xml.m_xmlContent.toUtf8();
    QBuffer buffer(&result);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    AADLXMLReader reader;
    connect(&reader, &AADLXMLReader::objectsParsed, this, [&xml](const QVector<AADLObject *> &objectsList) {
        QCOMPARE(objectsList.size(), xml.expectedObjectCount());
    });
    connect(&reader, &AADLXMLReader::error, [](const QString &msg) { qWarning() << msg; });

    QSignalSpy spyObjectsParsed(&reader, &AADLXMLReader::objectsParsed);
    QSignalSpy spyError(&reader, &AADLXMLReader::error);

    const bool ok = reader.read(&buffer);
    QCOMPARE(ok, xml.m_canBeParsed);

    if (xml.m_canBeParsed)
        QCOMPARE(spyObjectsParsed.count(), 1);

    QCOMPARE(spyError.count(), xml.m_expectedErrorCount);

    if (spyObjectsParsed.count()) {
        const QList<QVariant> &objectsList = spyObjectsParsed.takeFirst();
        const QVector<AADLObject *> &aadlObjects = objectsList.first().value<QVector<AADLObject *>>();
        QCOMPARE(aadlObjects.size(), xml.expectedObjectCount());
    }

    if (xml.m_canBeParsed)
        while (spyError.count())
            qWarning() << spyError.takeFirst().first().toString();
}

void XMLReader::test_emptyInterfaceViewDoc()
{
    for (auto xml : { XmlHelper::instance()->EmptyFile, XmlHelper::instance()->EmptyDoc })
        runReader(xml);
}

void XMLReader::test_singleItems()
{
    for (auto xml :
         { XmlHelper::instance()->SingleFunction, XmlHelper::instance()->SingleFunctionType,
           XmlHelper::instance()->SingleComment,
           /*XmlHelper::instance()->SingleIfaceInvalid, */ // for now there is an Q_ASSERT (crash)
           XmlHelper::instance()->SingleIfaceValid,
           XmlHelper::instance()->ParametrizedIfaces, XmlHelper::instance()->SingleConnectionOrphan,
           XmlHelper::instance()->SingleConnectionValid, XmlHelper::instance()->SingleConnectionValidMultipoint })
        runReader(xml);
}

void XMLReader::test_allItems()
{
    for (auto xml : { XmlHelper::instance()->AllItems })
        runReader(xml);
}

QTEST_APPLESS_MAIN(XMLReader)

#include "tst_aadlxmlreader.moc"
