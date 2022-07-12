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

#include "ivconnectiongroup.h"
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
    void test_readFunctionLanguages();
    void test_connectionGroup();
    void test_multicast();
};

void IVXMLReader::runReader(const XmlFileMock &xml)
{
    QByteArray result = xml.m_xmlContent.toUtf8();
    QBuffer buffer(&result);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    ivm::IVXMLReader reader;
    const bool ok = reader.read(&buffer);
    QCOMPARE(ok, xml.m_canBeParsed);
    if (ok) {
        const QVector<ivm::IVObject *> objectsList = reader.parsedObjects();
        QCOMPARE(objectsList.size(), xml.expectedObjectCount());
    } else {
        qWarning() << reader.errorString();
    }
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

    ivm::IVXMLReader reader;
    const bool ok = reader.read(&buffer);
    QVERIFY(ok);

    const QVariantMap metadata = reader.metaData();
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

    ivm::IVXMLReader reader;

    const bool ok = reader.read(&buffer);
    QVERIFY(ok);
    const QVector<ivm::IVObject *> objectsList = reader.parsedObjects();
    QCOMPARE(objectsList.size(), 1);
    ivm::IVFunction *function = qobject_cast<ivm::IVFunction *>(objectsList[0]);

    QVERIFY(function != nullptr);
    QCOMPARE(function->contextParams().size(), 2);

    shared::ContextParameter param1 = function->contextParam("duration");
    QVERIFY(!param1.isNull());
    QCOMPARE(param1.paramType(), shared::BasicParameter::Type::Other);
    QCOMPARE(param1.paramTypeName(), QString("MyInt"));
    QCOMPARE(param1.defaultValue(), QVariant::fromValue(60));

    shared::ContextParameter param2 = function->contextParam("trigger");
    QVERIFY(!param2.isNull());
    QCOMPARE(param2.paramType(), shared::BasicParameter::Type::Timer);
}

void IVXMLReader::test_readFunctionLanguages()
{
    QByteArray xml("<InterfaceView>"
                   "<Function name=\"BigMamma\" is_type=\"NO\" default_implementation=\"default\">"
                   "<ContextParameter name=\"duration\" type=\"MyInt\" value=\"60\"/>"
                   "<Implementations>"
                   "<Implementation name=\"default\" language=\"cpp\"/>"
                   "<Implementation name=\"secondary\" language=\"SDL\"/>"
                   "</Implementations></Function></InterfaceView>");

    QBuffer buffer(&xml);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    ivm::IVXMLReader reader;

    const bool ok = reader.read(&buffer);
    QVERIFY(ok);
    const QVector<ivm::IVObject *> objectsList = reader.parsedObjects();
    QCOMPARE(objectsList.size(), 1);
    ivm::IVFunction *function = qobject_cast<ivm::IVFunction *>(objectsList[0]);
    QVERIFY(function != nullptr);
    QCOMPARE(function->implementations().size(), 2);

    const EntityAttribute &lang1 = function->implementations().at(0);
    QCOMPARE(lang1.name(), "default");
    QCOMPARE(lang1.value(), "cpp");

    const EntityAttribute &lang2 = function->implementations().at(1);
    QCOMPARE(lang2.name(), "secondary");
    QCOMPARE(lang2.value(), "SDL");
}

void IVXMLReader::test_connectionGroup()
{
    ivm::IVXMLReader reader;
    reader.readFile(":/data/connection_group.xml");
    const QVector<ivm::IVObject *> objectsList = reader.parsedObjects();
    ivm::IVConnectionGroup *cgroup = nullptr;
    for (ivm::IVObject *object : objectsList) {
        if (object->type() == ivm::IVObject::Type::ConnectionGroup) {
            cgroup = static_cast<ivm::IVConnectionGroup *>(object);
            break;
        }
    }
    QVERIFY(cgroup != nullptr);

    QList<QPointer<ivm::IVConnection>> groupedConnection = cgroup->groupedConnections();
    QCOMPARE(groupedConnection.size(), 2);
}

void IVXMLReader::test_multicast()
{
    ivm::IVXMLReader reader;
    reader.readFile(":/data/multicast.xml");
    const QVector<ivm::IVObject *> objectsList = reader.parsedObjects();

    QVector<ivm::IVConnection *> connections;
    for (ivm::IVObject *object : objectsList) {
        if (object->type() == ivm::IVObject::Type::Connection) {
            auto connection = static_cast<ivm::IVConnection *>(object);
            connections.append(connection);
        }
    }
    QCOMPARE(connections.size(), 2);
}

QTEST_APPLESS_MAIN(IVXMLReader)

#include "tst_ivxmlreader.moc"
