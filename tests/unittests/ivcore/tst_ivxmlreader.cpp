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

#include "ivarchetypelibraryreference.h"
#include "ivarchetypereference.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "ivobject.h"
#include "ivpropertytemplateconfig.h"
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
    void initTestCase();
    void test_emptyInterfaceViewDoc();
    void test_singleItems();
    void test_allItems();
    void test_readMetaData();
    void test_readFunction();
    void test_readFunctionLanguages();
    void test_connectionGroup();
    void test_readLayer();
    void test_readArchetypeLibraryReference();
    void test_readArchetypeReference();
    void test_multicast();

private:
    ivm::IVPropertyTemplateConfig *conf { nullptr };
};

void IVXMLReader::initTestCase()
{
    ivm::initIVLibrary();
    conf = ivm::IVPropertyTemplateConfig::instance();
    conf->init(QLatin1String("default_attrinbutes.xml"));
}

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
    QByteArray xml("<InterfaceView version=\"1.99\" asn1file=\"dataview.asn\" mscfile=\"newfile.msc\"/>");

    QBuffer buffer(&xml);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    ivm::IVXMLReader reader;
    const bool ok = reader.read(&buffer);
    QVERIFY(ok);

    const QVariantMap metadata = reader.metaData();
    QCOMPARE(metadata.size(), 3);
    QCOMPARE(metadata["version"].toString(), QString("1.99"));
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

void IVXMLReader::test_readLayer()
{
    QByteArray xml(R"(<InterfaceView>
                      <Layer name="TestLayer"/>
                      </InterfaceView>)");

    QBuffer buffer(&xml);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    ivm::IVXMLReader reader;

    const bool ok = reader.read(&buffer);
    QVERIFY(ok);
    const QVector<ivm::IVObject *> layersList = reader.parsedLayers();
    QCOMPARE(layersList.size(), 1);
    ivm::IVConnectionLayerType *layer = qobject_cast<ivm::IVConnectionLayerType *>(layersList[0]);

    QVERIFY(layer != nullptr);
    QCOMPARE(layer->title(), "TestLayer");
}

void IVXMLReader::test_readArchetypeLibraryReference()
{
    QByteArray xml(R"(<InterfaceView>
                      <ArchetypeLibraryReference archetype_library="SomeLibrary" path="SomeLibrary.xml"/>
                      </InterfaceView>)");

    QBuffer buffer(&xml);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    ivm::IVXMLReader reader;

    const bool ok = reader.read(&buffer);
    QVERIFY(ok);
    const QVector<ivm::IVObject *> referencesList = reader.parsedObjects();
    QCOMPARE(referencesList.size(), 1);
    ivm::IVArchetypeLibraryReference *reference = qobject_cast<ivm::IVArchetypeLibraryReference *>(referencesList[0]);

    QVERIFY(reference != nullptr);
    QCOMPARE(reference->getLibraryName(), "SomeLibrary");
    QCOMPARE(reference->getLibraryPath(), "SomeLibrary.xml");
}

void IVXMLReader::test_readArchetypeReference()
{
    QByteArray xml(R"(<InterfaceView>
                      <Function name="FN1" is_type="NO">
                      <ArchetypeReference archetype_library="SomeLibraryName" archetype_function="SomeArchetyFunctionName"/>
                      </Function>
                      </InterfaceView>)");

    QBuffer buffer(&xml);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    ivm::IVXMLReader reader;

    const bool ok = reader.read(&buffer);
    QVERIFY(ok);
    const QVector<ivm::IVObject *> objectList = reader.parsedObjects();
    QCOMPARE(objectList.size(), 2);

    ivm::IVFunction *function { nullptr };
    ivm::IVArchetypeReference *reference { nullptr };

    for (auto ivObject : objectList) {
        switch (ivObject->type()) {
        case ivm::IVObject::Type::Function:
            function = ivObject->as<ivm::IVFunction *>();
            break;
        case ivm::IVObject::Type::ArchetypeReference:
            reference = ivObject->as<ivm::IVArchetypeReference *>();
            break;
        default:
            break;
        }
    }

    QVERIFY(function != nullptr);
    QCOMPARE(function->title(), "FN1");
    QCOMPARE(function->archetypeReferences().size(), 1);

    QVERIFY(reference != nullptr);
    QCOMPARE(reference->getLibraryName(), "SomeLibraryName");
    QCOMPARE(reference->getFunctionName(), "SomeArchetyFunctionName");
}

void IVXMLReader::test_multicast()
{
    ivm::IVXMLReader reader;
    reader.readFile(":/data/multicast.xml");
    const QVector<ivm::IVObject *> objectsList = reader.parsedObjects();

    // This object is required to run postInit functions
    // Without postInit, there is no access to the read data
    ivm::IVModel model(conf);
    model.initFromObjects(objectsList);

    // Direct iteration over objectsList may yield objects without private data
    // which segfault upon query
    QVector<ivm::IVConnection *> connections = model.allObjectsByType<ivm::IVConnection>();

    QCOMPARE(connections.size(), 2);
    // This code is required because IVModel stores objects internally in non-deterministic
    // hashes and maps
    const auto expectedDstAIndex = connections[0]->targetName() == "dstA" ? 0 : 1;
    const auto connection1 = connections[expectedDstAIndex];
    const auto connection2 = connections[1 - expectedDstAIndex];

    QCOMPARE(connection1->sourceName(), "src");
    QCOMPARE(connection1->targetName(), "dstA");
    QCOMPARE(connection1->sourceInterfaceName(), "pi_a");
    QCOMPARE(connection1->targetInterfaceName(), "pi_a");
    QCOMPARE(connection2->sourceName(), "src");
    QCOMPARE(connection2->targetName(), "dstB");
    QCOMPARE(connection2->sourceInterfaceName(), "pi_a");
    QCOMPARE(connection2->targetInterfaceName(), "pi_b");
}

QTEST_APPLESS_MAIN(IVXMLReader)

#include "tst_ivxmlreader.moc"
