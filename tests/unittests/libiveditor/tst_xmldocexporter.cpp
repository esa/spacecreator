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

#include "aadlcomment.h"
#include "aadlfunction.h"
#include "aadllibrary.h"
#include "aadlparameter.h"
#include "asn1modelstorage.h"
#include "interface/interfacedocument.h"
#include "iveditor.h"
#include "xmldocexporter.h"

#include <QFile>
#include <QObject>
#include <QtTest>

class XmlData
{
public:
    struct Node {
        QString type;
        QVariant data;
        QVariantHash attrs;
        QList<Node> children;

        bool operator==(const Node &n)
        {
            return n.type == type && n.data == data && n.attrs == attrs && n.children == children;
        }
    };

    explicit XmlData(const QByteArray &data)
    {
        QXmlStreamReader xml(data);
        processXml(xml);
    }
    explicit XmlData(const QString &path)
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            qFatal(qUtf8Printable(file.errorString()));
            return;
        }

        QXmlStreamReader xml(&file);
        processXml(xml);
    }

    bool operator==(const XmlData &other) { return other.nodes == nodes; }

private:
    Node parseNode(QXmlStreamReader &xml)
    {
        Node node;
        node.type = xml.name().toString();
        node.data = xml.text().toString();
        for (const QXmlStreamAttribute &attr : xml.attributes()) {
            node.attrs.insert(attr.name().toString(), attr.value().toString());
        }
        while (xml.readNextStartElement()) {
            node.children.append(parseNode(xml));
        }
        return node;
    }

    void processXml(QXmlStreamReader &xml)
    {
        while (!xml.atEnd() && !xml.hasError()) {
            const QXmlStreamReader::TokenType token = xml.readNext();
            if (token == QXmlStreamReader::StartDocument) {
                continue;
            }
            while (xml.readNextStartElement()) {
                nodes.append(parseNode(xml));
            }
        }
    }

private:
    QList<Node> nodes;
};

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
    ivm::initAadlLibrary();
    ive::initIvEditor();
}

void tst_XmlDocExporter::init()
{
    m_doc = std::make_unique<ive::InterfaceDocument>(this);
    m_doc->asn1ModelStorage()->clear();
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
    ive::XmlDocExporter::exportDocSilently(m_doc.get(), testFilePath);
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView/>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_XmlDocExporter::testExportFunctions()
{
    auto testfunc1 = new ivm::AADLFunction("TestFunc1", m_doc.get());
    testfunc1->setAttr("foo", QVariant::fromValue(11));
    testfunc1->setProp("bar", QVariant::fromValue(22));
    testfunc1->addContextParam(
            ivm::ContextParameter("Mo", ivm::BasicParameter::Type::Other, "MyInt", QVariant::fromValue(33)));

    QVector<ivm::AADLObject *> objects;
    objects.append(testfunc1);
    m_doc->setObjects(objects);

    ive::XmlDocExporter::exportDocSilently(m_doc.get(), testFilePath);
    const QByteArray text = testFileContent();
    const QByteArray expected =
            "<?xml version=\"1.0\"?>\n<InterfaceView>\n"
            "    <Function name=\"TestFunc1\" is_type=\"NO\" instance_of=\"\" language=\"SDL\" foo=\"11\">\n"
            "        <Property name=\"bar\" value=\"22\"/>\n"
            "        <ContextParameter name=\"Mo\" type=\"MyInt\" value=\"33\"/>\n"
            "    </Function>\n"
            "</InterfaceView>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_XmlDocExporter::testExportComment()
{
    auto testcomment1 = new ivm::AADLComment("TestComment1", m_doc.get());
    testcomment1->setAttr("foo", QVariant::fromValue(11));
    testcomment1->setProperty("bar", QVariant::fromValue(22)); // ignored for comment

    QVector<ivm::AADLObject *> objects;
    objects.append(testcomment1);
    m_doc->setObjects(objects);

    ive::XmlDocExporter::exportDocSilently(m_doc.get(), testFilePath);
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView>\n"
                                "    <Comment name=\"TestComment1\" foo=\"11\"/>\n"
                                "</InterfaceView>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_XmlDocExporter::testExportNestedComment()
{
    auto testfunc1 = new ivm::AADLFunction("TestFunc1", m_doc.get());
    auto testcomment1 = new ivm::AADLComment("TestComment1", testfunc1);
    testfunc1->addChild(testcomment1);

    QVector<ivm::AADLObject *> objects;
    objects.append(testfunc1);
    m_doc->setObjects(objects);

    ive::XmlDocExporter::exportDocSilently(m_doc.get(), testFilePath);
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView>\n"
                                "    <Function name=\"TestFunc1\" is_type=\"NO\" instance_of=\"\" language=\"SDL\">\n"
                                "        <Comment name=\"TestComment1\"/>\n"
                                "    </Function>\n"
                                "</InterfaceView>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_XmlDocExporter::testExportAsn1File()
{
    m_doc->setAsn1FileName("fake.asn");
    ive::XmlDocExporter::exportDocSilently(m_doc.get(), testFilePath);
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView asn1file=\"fake.asn\"/>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_XmlDocExporter::testExportToBuffer()
{
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    bool ok = ive::XmlDocExporter::exportDoc(m_doc.get(), &buffer);
    QCOMPARE(ok, true);
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<InterfaceView/>";
    QVERIFY(XmlData(expected) == XmlData(buffer.data()));
}

QTEST_MAIN(tst_XmlDocExporter)

#include "tst_xmldocexporter.moc"
