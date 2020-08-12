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

#include "aadlobjectcomment.h"
#include "aadlobjectfunction.h"
#include "datatypesstorage.h"
#include "interface/interfacedocument.h"
#include "iveditor.h"
#include "xmldocexporter.h"

#include <QFile>
#include <QObject>
#include <QtTest>

static QString testFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/tst_xmldocex.xml";

class tst_XmlDocExporter : public QObject
{
    Q_OBJECT
public:
    tst_XmlDocExporter();

private:
    QByteArray testFileContent() const;
    std::unique_ptr<aadlinterface::InterfaceDocument> m_doc;

private Q_SLOTS:
    void init();
    void cleanup();

    void testExportEmptyDoc();
    void testExportFunctions();
    void testExportComment();
    void testExportNestedComment();
    void testExportAsn1File();
    void testExportToBuffer();
};

tst_XmlDocExporter::tst_XmlDocExporter()
{
    aadlinterface::initIvEditor();
}

QByteArray tst_XmlDocExporter::testFileContent() const
{
    QFile file(testFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        return file.readAll();
    }

    return QByteArray();
}

void tst_XmlDocExporter::init()
{
    m_doc = std::make_unique<aadlinterface::InterfaceDocument>(this);
    m_doc->asn1DataTypes()->clear();
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
    aadlinterface::XmlDocExporter::exportDocSilently(m_doc.get(), testFilePath);
    QByteArray text = testFileContent();
    QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView/>";
    QCOMPARE(text, expected);
}

void tst_XmlDocExporter::testExportFunctions()
{
    auto testfunc1 = new aadl::AADLObjectFunction("TestFunc1", m_doc.get());
    testfunc1->setAttr("foo", QVariant::fromValue(11));
    testfunc1->setProp("bar", QVariant::fromValue(22));

    QVector<aadl::AADLObject *> objects;
    objects.append(testfunc1);
    m_doc->setObjects(objects);

    aadlinterface::XmlDocExporter::exportDocSilently(m_doc.get(), testFilePath);
    QByteArray text = testFileContent();

    QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView>\n"
                          "    <Function name=\"TestFunc1\" language=\"\" is_type=\"NO\" instance_of=\"\" foo=\"11\">\n"
                          "        <Property name=\"bar\" value=\"22\"/>\n"
                          "    </Function>\n"
                          "</InterfaceView>";
    QCOMPARE(text, expected);
}

void tst_XmlDocExporter::testExportComment()
{
    auto testcomment1 = new aadl::AADLObjectComment("TestComment1", m_doc.get());
    testcomment1->setAttr("foo", QVariant::fromValue(11));
    testcomment1->setProperty("bar", QVariant::fromValue(22)); // ignored for comment

    QVector<aadl::AADLObject *> objects;
    objects.append(testcomment1);
    m_doc->setObjects(objects);

    aadlinterface::XmlDocExporter::exportDocSilently(m_doc.get(), testFilePath);
    QByteArray text = testFileContent();

    QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView>\n"
                          "    <Comment name=\"TestComment1\" foo=\"11\"/>\n"
                          "</InterfaceView>";
    QCOMPARE(text, expected);
}

void tst_XmlDocExporter::testExportNestedComment()
{
    auto testfunc1 = new aadl::AADLObjectFunction("TestFunc1", m_doc.get());
    auto testcomment1 = new aadl::AADLObjectComment("TestComment1", testfunc1);
    testfunc1->addChild(testcomment1);

    QVector<aadl::AADLObject *> objects;
    objects.append(testfunc1);
    m_doc->setObjects(objects);

    aadlinterface::XmlDocExporter::exportDocSilently(m_doc.get(), testFilePath);
    QByteArray text = testFileContent();

    QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView>\n"
                          "    <Function name=\"TestFunc1\" language=\"\" is_type=\"NO\" instance_of=\"\">\n"
                          "        <Comment name=\"TestComment1\"/>\n"
                          "    </Function>\n"
                          "</InterfaceView>";
    QCOMPARE(text, expected);
}

void tst_XmlDocExporter::testExportAsn1File()
{
    m_doc->setAsn1FileName("fake.asn");
    aadlinterface::XmlDocExporter::exportDocSilently(m_doc.get(), testFilePath);
    QByteArray text = testFileContent();

    QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView asn1file=\"fake.asn\"/>";
    QCOMPARE(text, expected);
}

void tst_XmlDocExporter::testExportToBuffer()
{
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    bool ok = aadlinterface::XmlDocExporter::exportDoc(m_doc.get(), &buffer);
    QCOMPARE(ok, true);
    QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView/>";
    QCOMPARE(buffer.data(), expected);
}

QTEST_MAIN(tst_XmlDocExporter)

#include "tst_xmldocexporter.moc"
