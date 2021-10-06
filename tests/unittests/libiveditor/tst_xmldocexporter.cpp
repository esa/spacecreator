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
#include "asn1modelstorage.h"
#include "interfacedocument.h"
#include "ivcomment.h"
#include "iveditor.h"
#include "ivexporter.h"
#include "ivfunction.h"
#include "ivlibrary.h"
#include "ivtestutils.h"
#include "parameter.h"

#include <QObject>
#include <QtTest>

static QString testFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/tst_xmldocex.xml";

class tst_XmlDocExporter : public QObject
{
    Q_OBJECT

private:
    QByteArray testFileContent() const;
    std::unique_ptr<ive::InterfaceDocument> m_doc;

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();

    void testExportEmptyDoc();
    void testExportFunctions();
    void testExportComment();
    void testExportNestedComment();
    void testExportAsn1File();
    void testExportToBuffer();
};

QByteArray tst_XmlDocExporter::testFileContent() const
{
    QFile file(testFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        return file.readAll();
    }

    return QByteArray();
}

void tst_XmlDocExporter::initTestCase()
{
    ivm::initIVLibrary();
    ive::initIVEditor();
}

void tst_XmlDocExporter::init()
{
    m_doc = std::make_unique<ive::InterfaceDocument>(this);
    if (QFile::exists(testFilePath)) {
        QFile::remove(testFilePath);
    }
}

void tst_XmlDocExporter::cleanup()
{
    if (QFile::exists(testFilePath)) {
        QFile::remove(testFilePath);
    }
}

void tst_XmlDocExporter::testExportEmptyDoc()
{
    m_doc->exporter()->exportDocSilently(m_doc.get(), testFilePath);
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView/>";
    QVERIFY(XmlData(expected) == XmlData(text));
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

    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView>\n"
                                "    <Function name=\"TestFunc1\" is_type=\"NO\" instance_of=\"\" language=\"SDL\" "
                                "default_language=\"default\" foo=\"11\">\n"
                                "        <Property name=\"bar\" value=\"22\"/>\n"
                                "        <ContextParameter name=\"Mo\" type=\"MyInt\" value=\"33\"/>\n"
                                "        <Languages>\n"
                                "           <ComputeLanguage name=\"default\" language=\"SDL\"/>\n"
                                "        </Languages>\n"
                                "    </Function>\n"
                                "</InterfaceView>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_XmlDocExporter::testExportComment()
{
    auto testcomment1 = ivm::testutils::createComment("TestComment1", m_doc.get());
    testcomment1->setEntityAttribute(QLatin1String("foo"), QVariant::fromValue(11));

    QVector<ivm::IVObject *> objects;
    objects.append(testcomment1);
    m_doc->setObjects(objects);

    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView>\n"
                                "    <Comment name=\"TestComment1\" foo=\"11\"/>\n"
                                "</InterfaceView>";
    QVERIFY(XmlData(expected) == XmlData(text));
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

    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView>\n"
                                "    <Function name=\"TestFunc1\" is_type=\"NO\" instance_of=\"\" language=\"SDL\" "
                                "default_language=\"default\">\n"
                                "        <Comment name=\"TestComment1\"/>\n"
                                "        <Languages>\n"
                                "           <ComputeLanguage name=\"default\" language=\"SDL\"/>\n"
                                "        </Languages>\n"
                                "    </Function>\n"
                                "</InterfaceView>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_XmlDocExporter::testExportAsn1File()
{
    m_doc->setAsn1FileName("fake.asn");
    QVERIFY(m_doc->exporter()->exportDocSilently(m_doc.get(), testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView asn1file=\"fake.asn\"/>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_XmlDocExporter::testExportToBuffer()
{
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    m_doc->exporter()->exportObjects(m_doc->objects().values(), &buffer);
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView/>";
    QVERIFY(XmlData(expected) == XmlData(buffer.data()));
}

QTEST_MAIN(tst_XmlDocExporter)

#include "tst_xmldocexporter.moc"
