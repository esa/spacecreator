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

#include "../common/xmldatahelper.h"
#include "graphicsviewutils.h"
#include "interfacedocument.h"
#include "ivarchetypelibraryreference.h"
#include "ivarchetypereference.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "iveditor.h"
#include "ivexporter.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "ivpropertytemplateconfig.h"
#include "ivtestutils.h"
#include "parameter.h"
#include "scversion.h"

#include <QObject>
#include <QTemporaryDir>
#include <QtTest>

static const QString kAttrTemplate = R"(
        <Attrs version="1.0">
            <Attr label="Name" name="name">
                <Type>
                    <String validator="[a-zA-Z][a-zA-Z0-9_]*"/>
                </Type>
            </Attr>
            <Attr label="Kind" name="kind">
                <Type>
                    <Enumeration defaultValue="Sporadic">
                        <Entry value="Any"/>
                        <Entry value="Cyclic"/>
                        <Entry value="Sporadic"/>
                        <Entry value="Protected"/>
                        <Entry value="Unprotected"/>
                    </Enumeration>
                </Type>
                <Scopes>
                    <Required_Interface/>
                    <Provided_Interface/>
                </Scopes>
            </Attr>
            <Attr name="origin" editable="false" optional="true">
                <Type>
                    <String validator="\{?[0-9a-fA-F]{8}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{12}\}?"/>
                </Type>
            </Attr>
            <Attr name="is_type">
                <Type>
                    <Enumeration defaultValue="NO">
                        <Entry value="YES"/>
                        <Entry value="NO"/>
                    </Enumeration>
                </Type>
                <Scopes>
                    <Function/>
                </Scopes>
            </Attr>
            <Attr label="Instance of" name="instance_of">
                <Type>
                    <String validator="[a-zA-Z][a-zA-Z0-9_]*"/>
                </Type>
                <Scopes>
                    <Function>
                        <AttrValidator name="is_type" value="NO"/>
                    </Function>
                </Scopes>
            </Attr>
            <Attr label="Inherit from PI" name="Taste::InheritPI" type="property">
                <Type>
                    <Boolean defaultValue="True"/>
                </Type>
                <Scopes>
                    <Required_Interface/>
                </Scopes>
            </Attr>
            <Attr name="Taste::coordinates" type="property" visible="false" editable="false" export="UI">
                <Type>
                    <String validator="[0-9 ]+"/>
                </Type>
                <Scopes/>
            </Attr>
            <Attr name="Taste::RootCoordinates" type="property" visible="false" editable="false" export="UI">
                <Type>
                    <String validator="[0-9 ]+"/>
                </Type>
                <Scopes/>
            </Attr>
            <Attr name="Taste::Autonamed" type="property" visible="false">
                <Type>
                    <Boolean defaultValue="True"/>
                </Type>
                <Scopes>
                    <Required_Interface/>
                </Scopes>
            </Attr>
            <Attr name="AttrName" export="UI">
                <Type>
                    <String/>
                </Type>
                <Scopes/>
            </Attr>
            <Attr name="Namespace::PropertyName::Type::value" type="property" export="UI">
                <Type>
                    <String/>
                </Type>
                <Scopes/>
            </Attr>
        </Attrs>
    )";

class tst_XmlDocExporter : public QObject
{
    Q_OBJECT

private:
    QByteArray testFileContent() const;
    std::unique_ptr<ive::InterfaceDocument> m_doc;
    QTemporaryDir m_testPath;
    QString m_testFilePath;

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();

    void testExportEmptyDoc();
    void testExportFunctions();
    void testExportComment();
    void testExportNestedComment();
    void testExportLayer();
    void testExportArchetypeLibraryReference();
    void testExportArchetypeReferenceInFunction();
    void testExportArchetypeReferenceInFunctionType();
    void testExportAsn1File();
    void testExportToBuffer();
    void testExportID();
    void testExportUI();
};

QByteArray tst_XmlDocExporter::testFileContent() const
{
    QFile file(m_testFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        return file.readAll();
    }

    return QByteArray();
}

void tst_XmlDocExporter::initTestCase()
{
    ive::initIVEditor();
    ivm::initIVLibrary();

    m_testFilePath = m_testPath.filePath("tst_xmldocex.xml");
}

void tst_XmlDocExporter::init()
{
    m_doc = std::make_unique<ive::InterfaceDocument>(this);
    if (QFile::exists(m_testFilePath)) {
        QFile::remove(m_testFilePath);
    }
    m_doc->setPath(m_testFilePath);
}

void tst_XmlDocExporter::cleanup()
{
    if (QFile::exists(m_testFilePath)) {
        QFile::remove(m_testFilePath);
    }
}

void tst_XmlDocExporter::testExportEmptyDoc()
{
    m_doc->exporter()->exportDocSilently(m_doc.get(), m_testFilePath);
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView/>";
    QVERIFY(XmlData(expected).isEqual(XmlData(text), false, { "version", "id", "UiFile" }, { "UI" }));
}

void tst_XmlDocExporter::testExportFunctions()
{
    auto testfunc1 = ivm::testutils::createFunction("TestFunc1", m_doc.get());
    testfunc1->setEntityAttribute(QLatin1String("foo"), QVariant::fromValue(11));
    testfunc1->setEntityAttribute(QLatin1String("instance_of"), QString());
    testfunc1->setEntityProperty(QLatin1String("bar"), QVariant::fromValue(22));
    testfunc1->addContextParam(
            shared::ContextParameter("Mo", shared::BasicParameter::Type::Other, "MyInt", QVariant::fromValue(33)));

    QVector<ivm::IVObject *> objects;
    objects.append(testfunc1);
    m_doc->setObjects(objects);

    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), m_testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView>\n"
                                "    <Function name=\"TestFunc1\" is_type=\"NO\" instance_of=\"\" language=\"SDL\" "
                                "default_implementation=\"default\" foo=\"11\">\n"
                                "        <Property name=\"bar\" value=\"22\"/>\n"
                                "        <ContextParameter name=\"Mo\" type=\"MyInt\" value=\"33\"/>\n"
                                "        <Implementations>\n"
                                "           <Implementation name=\"default\" language=\"SDL\"/>\n"
                                "        </Implementations>\n"
                                "    </Function>\n"
                                "</InterfaceView>";
    QVERIFY(XmlData(expected).isEqual(XmlData(text), false, { "version", "id", "UiFile" },
            { "Taste::coordinates", "Taste::InnerCoordinates", "UI" }));
}

void tst_XmlDocExporter::testExportComment()
{
    auto testcomment1 = ivm::testutils::createComment("TestComment1", m_doc.get());
    testcomment1->setEntityAttribute(QLatin1String("foo"), QVariant::fromValue(11));

    QVector<ivm::IVObject *> objects;
    objects.append(testcomment1);
    m_doc->setObjects(objects);

    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), m_testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView>\n"
                                "    <Comment name=\"TestComment1\" foo=\"11\"/>\n"
                                "</InterfaceView>";
    QVERIFY(XmlData(expected).isEqual(XmlData(text), false, { "version", "id", "UiFile" },
            { "Taste::coordinates", "Taste::InnerCoordinates", "UI" }));
}

void tst_XmlDocExporter::testExportNestedComment()
{
    auto testfunc1 = ivm::testutils::createFunction("TestFunc1", m_doc.get());
    testfunc1->setEntityAttribute(QLatin1String("instance_of"), QString());
    auto testcomment1 = ivm::testutils::createComment("TestComment1", testfunc1);
    testfunc1->addChild(testcomment1);

    QVector<ivm::IVObject *> objects;
    objects.append(testfunc1);
    m_doc->setObjects(objects);

    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), m_testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView>\n"
                                "    <Function name=\"TestFunc1\" is_type=\"NO\" instance_of=\"\" language=\"SDL\" "
                                "default_implementation=\"default\">\n"
                                "        <Comment name=\"TestComment1\"/>\n"
                                "        <Implementations>\n"
                                "           <Implementation name=\"default\" language=\"SDL\"/>\n"
                                "        </Implementations>\n"
                                "    </Function>\n"
                                "</InterfaceView>";
    QVERIFY(XmlData(expected).isEqual(XmlData(text), false, { "version", "id", "UiFile" },
            { "Taste::coordinates", "Taste::InnerCoordinates", "UI" }));
}

void tst_XmlDocExporter::testExportLayer()
{
    auto testLayer = ivm::testutils::createConnectionLayer(m_doc.get());
    testLayer->setTitle("TestLayer");

    QVector<ivm::IVObject *> objects;
    objects.append(testLayer);
    m_doc->setObjects(objects);

    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), m_testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = R"(<?xml version="1.0"?>
                                   <InterfaceView>
                                   <Layer name="TestLayer"/>
                                   </InterfaceView>)";
    QVERIFY(XmlData(expected).isEqual(XmlData(text), false, { "version", "id", "UiFile" },
            { "Taste::coordinates", "Taste::InnerCoordinates", "UI" }));
}

void tst_XmlDocExporter::testExportArchetypeLibraryReference()
{
    QString libraryName = QString("SomeLibraryName");
    QString libraryPath = QString("SomeLibraryPath.xml");
    auto testReference = ivm::testutils::createArchetypeLibraryReference(libraryName, libraryPath, m_doc.get());

    QVector<ivm::IVObject *> objects;
    objects.append(testReference);
    m_doc->setObjects(objects);

    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), m_testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = R"(<?xml version="1.0"?>
                                   <InterfaceView>
                                   <ArchetypeLibraryReference archetype_library="SomeLibraryName" path="SomeLibraryPath.xml"/>
                                   </InterfaceView>)";
    QVERIFY(XmlData(expected).isEqual(XmlData(text), false, { "version", "id", "UiFile" },
            { "Taste::coordinates", "Taste::InnerCoordinates", "UI" }));
}

void tst_XmlDocExporter::testExportArchetypeReferenceInFunction()
{
    auto testfunc = ivm::testutils::createFunction("TestFunc", m_doc.get());
    testfunc->setEntityAttribute(QLatin1String("instance_of"), QString());
    QString libraryName = QString("SomeLibraryName");
    QString functionName = QString("SomeFunctionName");
    auto testReference = ivm::testutils::createArchetypeReference(libraryName, functionName, m_doc.get());
    testfunc->addChild(testReference);

    QVector<ivm::IVObject *> objects;
    objects.append(testfunc);
    m_doc->setObjects(objects);

    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), m_testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = R"(<?xml version="1.0"?>
                                   <InterfaceView>
                                   <Function name="TestFunc" is_type="NO" instance_of="" language="SDL" default_implementation="default">
                                       <Implementations>
                                           <Implementation name="default" language="SDL"/>
                                       </Implementations>
                                       <ArchetypeReference archetype_library="SomeLibraryName" archetype_function="SomeFunctionName"/>
                                   </Function>
                                   </InterfaceView>)";
    QVERIFY(XmlData(expected).isEqual(XmlData(text), false, { "version", "id", "UiFile" },
            { "Taste::coordinates", "Taste::InnerCoordinates", "UI" }));
}

void tst_XmlDocExporter::testExportArchetypeReferenceInFunctionType()
{
    auto testfunc = ivm::testutils::createFunctionType("TestFunc", m_doc.get());
    QString libraryName = QString("SomeLibraryName");
    QString functionName = QString("SomeFunctionName");
    auto testReference = ivm::testutils::createArchetypeReference(libraryName, functionName, m_doc.get());
    testfunc->addChild(testReference);

    QVector<ivm::IVObject *> objects;
    objects.append(testfunc);
    m_doc->setObjects(objects);

    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), m_testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = R"(<?xml version="1.0"?>
                                   <InterfaceView>
                                   <Function name="TestFunc" type_language="SDL" is_type="YES">
                                       <ArchetypeReference archetype_library="SomeLibraryName" archetype_function="SomeFunctionName"/>
                                   </Function>
                                   </InterfaceView>)";
    QVERIFY(XmlData(expected).isEqual(XmlData(text), false, { "version", "id", "UiFile" },
            { "Taste::coordinates", "Taste::InnerCoordinates", "UI" }));
}

void tst_XmlDocExporter::testExportAsn1File()
{
    QFile file(m_testPath.filePath("fake.asn"));
    file.open(QIODevice::WriteOnly);
    file.close();

    m_doc->setAsn1FileName(file.fileName());
    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), m_testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView asn1file=\"fake.asn\"/>";
    QVERIFY(XmlData(expected).isEqual(XmlData(text), false, { "version", "id", "UiFile" },
            { "Taste::coordinates", "Taste::InnerCoordinates", "UI" }));
}

void tst_XmlDocExporter::testExportToBuffer()
{
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    m_doc->exporter()->exportObjects(m_doc->objects().values(), &buffer);
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView/>";
    QVERIFY(XmlData(expected).isEqual(XmlData(buffer.data()), false, { "version", "id", "UiFile" }, { "UI" }));
}

void tst_XmlDocExporter::testExportID()
{
    auto testfunc1 = ivm::testutils::createFunction("Function_1", m_doc.get());
    m_doc->objectsModel()->addObject(testfunc1);
    auto reqIface = ivm::testutils::createRequiredIface(testfunc1, "PI_1");
    testfunc1->addChild(reqIface);
    m_doc->objectsModel()->addObject(reqIface);

    auto testfunc2 = ivm::testutils::createFunction("Function_2", m_doc.get());
    m_doc->objectsModel()->addObject(testfunc2);
    auto provIface = ivm::testutils::createProvidedIface(testfunc2, "PI_1");
    testfunc2->addChild(provIface);
    m_doc->objectsModel()->addObject(provIface);

    auto connection = ivm::testutils::createConnection(testfunc1, testfunc2, "PI_1");

    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), m_testFilePath));
    const QByteArray text = testFileContent();
    const QString expectedStr = R"(
        <InterfaceView version="1.1">
            <Function id="%2" is_type="NO" name="Function_2">
                <Provided_Interface id="%4" kind="Protected" name="PI_1"/>
                <Implementations>
                    <Implementation language="SDL" name="default"/>
                </Implementations>
            </Function>
            <Function id="%1" is_type="NO" language="SDL" name="Function_1">
                <Required_Interface enable_multicast="true" id="%3" kind="Protected" name="PI_1">
                    <Property name="Taste::InheritPI" value="true"/>
                    <Property name="Taste::Autonamed" value="true"/>
                </Required_Interface>
                <Implementations>
                    <Implementation language="SDL" name="default"/>
                </Implementations>
            </Function>
            <Connection id="%5">
                <Source func_name="Function_1" ri_name="PI_1"/>
                <Target func_name="Function_2" pi_name="PI_1"/>
            </Connection>
        </InterfaceView>
    )";
    const QByteArray expected =
            expectedStr
                    .arg(testfunc1->id().toString(), testfunc2->id().toString(),
                            connection->sourceInterface()->id().toString(),
                            connection->targetInterface()->id().toString(), connection->id().toString())
                    .toUtf8();

    QVERIFY(XmlData(expected).isEqual(XmlData(text), false,
            { "version", "UiFile", "default_implementation", "language", "wcet", "layer", "enable_multicast" }, {}));
}

void tst_XmlDocExporter::testExportUI()
{
    ivm::IVPropertyTemplateConfig::instance()->initFromData(kAttrTemplate);

    auto testfunc1 = ivm::testutils::createFunction("Function_1", m_doc.get());
    m_doc->objectsModel()->addObject(testfunc1);
    auto reqIface = ivm::testutils::createRequiredIface(testfunc1, "PI_1");
    testfunc1->addChild(reqIface);
    m_doc->objectsModel()->addObject(reqIface);

    auto testfunc2 = ivm::testutils::createFunction("Function_2", m_doc.get());
    m_doc->objectsModel()->addObject(testfunc2);
    auto provIface = ivm::testutils::createProvidedIface(testfunc2, "PI_1");
    testfunc2->addChild(provIface);
    m_doc->objectsModel()->addObject(provIface);

    auto connection = ivm::testutils::createConnection(testfunc1, testfunc2, "PI_1");
    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), m_testFilePath));
    QByteArray text = testFileContent();
    const QString expectedStr = R"(
        <InterfaceView version="1.1" UiFile="%6" creatorHash="%7">
            <Function id="%2" name="Function_2">
                <Provided_Interface id="%4" name="PI_1"/>
            </Function>
            <Function id="%1" name="Function_1">
                <Required_Interface id="%3" name="PI_1">
                    <Property name="Taste::InheritPI" value="true"/>
                    <Property name="Taste::Autonamed" value="false"/>
                </Required_Interface>
            </Function>
            <Connection id="%5">
                <Source func_name="Function_1" ri_name="PI_1"/>
                <Target func_name="Function_2" pi_name="PI_1"/>
            </Connection>
        </InterfaceView>
    )";
    QByteArray expected = expectedStr
                                  .arg(testfunc1->id().toString(), testfunc2->id().toString(),
                                          connection->sourceInterface()->id().toString(),
                                          connection->targetInterface()->id().toString(), connection->id().toString(),
                                          m_doc->uiFileName(), spaceCreatorGitHash)
                                  .toUtf8();

    QVERIFY(XmlData(text).isEqual(XmlData(expected), false,
            { "modifierHash", "version", "is_type", "kind", "layer", "language", "enable_multicast", "wcet",
                    "default_implementation" },
            {}));

    connection->setEntityAttribute("AttrName", "AttributeData");
    testfunc1->setEntityProperty("Namespace::PropertyName::Type::value", "PropertyValue");
    testfunc2->setCoordinates(shared::graphicsviewutils::coordinates(QRectF(100, 100, 100, 100)));

    text.clear();
    QBuffer buffer(&text);
    QVERIFY(buffer.open(QIODevice::WriteOnly));
    QVERIFY(m_doc->exporter()->exportObjects(m_doc->objects().values(), &buffer));
    buffer.close();
    const QString expectedUIStr = R"(
        <InterfaceView version="1.1">
            <Function id="%2" name="Function_2">
                <Provided_Interface id="%4" name="PI_1"/>
            </Function>
            <Function id="%1" name="Function_1">
                <Required_Interface id="%3" name="PI_1">
                    <Property name="Taste::InheritPI" value="true"/>
                    <Property name="Taste::Autonamed" value="false"/>
                </Required_Interface>
            </Function>
            <Connection id="%5">
                <Source func_name="Function_1" ri_name="PI_1"/>
                <Target func_name="Function_2" pi_name="PI_1"/>
            </Connection>
            <UI>
                <Entity id="%5" AttrName="AttributeData"></Entity>
                <Entity id="%2">
                    <Taste coordinates="10000 10000 20000 20000"/>
                </Entity>
                <Entity id="%3"/>
                <Entity id="%4"/>
                <Entity id="%1">
                    <Namespace>
                        <PropertyName>
                            <Type value="PropertyValue"/>
                        </PropertyName>
                    </Namespace>
                </Entity>
            </UI>
        </InterfaceView>
    )";

    expected = expectedUIStr
                       .arg(testfunc1->id().toString(), testfunc2->id().toString(), reqIface->id().toString(),
                               provIface->id().toString(), connection->id().toString())
                       .toUtf8();

    QVERIFY(XmlData(text).isEqual(XmlData(expected), false,
            { "version", "is_type", "kind", "layer", "language", "enable_multicast", "wcet", "default_implementation" },
            {}));
}

QTEST_MAIN(tst_XmlDocExporter)

#include "tst_xmldocexporter.moc"
