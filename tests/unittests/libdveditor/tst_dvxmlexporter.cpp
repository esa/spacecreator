/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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
#include "dvcommonprops.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dveditor.h"
#include "dvexporter.h"
#include "dvfunction.h"
#include "dvnode.h"
#include "dvpartition.h"
#include "dvxmlreader.h"

#include <QFile>
#include <QObject>
#include <QtTest>

static QString testFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/tst_dvxmldocex.xml";

class tst_DVXmlExporter : public QObject
{
    Q_OBJECT

private:
    QByteArray testFileContent() const;

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();

    void testExportEmptyDoc();
    void testExportNode();
    void testExportPartition();
    void testExportFunctions();
    void testExportDevice();
    void testExportConnection();
    void testExportPartitionAndDevice();
    void testExportFunctionsAndDevice();
    void testExportMessage();
    void testAll();
    void testExportToBuffer();

private:
    std::unique_ptr<dve::DVExporter> m_exporter;
};

QByteArray tst_DVXmlExporter::testFileContent() const
{
    QFile file(testFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        return file.readAll();
    }

    return QByteArray();
}

void tst_DVXmlExporter::initTestCase()
{
    dve::initDvEditor();
}

void tst_DVXmlExporter::init()
{
    m_exporter = std::make_unique<dve::DVExporter>();
    if (QFile::exists(testFilePath)) {
        QFile::remove(testFilePath);
    }
}

void tst_DVXmlExporter::cleanup()
{
    if (QFile::exists(testFilePath)) {
        QFile::remove(testFilePath);
    }
}

void tst_DVXmlExporter::testExportEmptyDoc()
{
    m_exporter->exportObjectsSilently({}, testFilePath);
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<DeploymentView/>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_DVXmlExporter::testExportNode()
{
    auto node = new dvm::DVNode;
    node->setTitle(QLatin1String("TestNode"));
    node->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(11));
    node->setEntityAttribute(QLatin1String("attr2"), QString());

    QList<shared::VEObject *> objects;
    objects.append(node);

    QVERIFY(m_exporter->exportObjectsSilently(objects, testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<DeploymentView>\n"
                                "    <Node name=\"TestNode\" attr1=\"11\" attr2=\"\" />\n"
                                "</DeploymentView>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_DVXmlExporter::testExportDevice()
{
    std::unique_ptr<dvm::DVNode> node = std::make_unique<dvm::DVNode>();
    node->setTitle(QLatin1String("TestNode"));
    node->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(11));
    node->setEntityAttribute(QLatin1String("attr2"), QString());

    auto device = new dvm::DVDevice(node.get());
    device->setTitle(QLatin1String("TestDevice"));
    device->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(33));
    device->setEntityAttribute(QLatin1String("attr2"), QString());
    device->setEntityAttribute(QLatin1String("attr3"), QLatin1String("bar"));
    device->setEntityAttribute(QLatin1String("config"),
            QLatin1String("{devname \"eth0\", address \"127.0.0.1\", version ipv4, port 5116}"));

    node->addDevice(device);

    QList<shared::VEObject *> objects;
    objects.append(node.get());
    objects.append(device);

    QVERIFY(m_exporter->exportObjectsSilently(objects, testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected =
            "<?xml version=\"1.0\"?>\n<DeploymentView>\n"
            "    <Node name=\"TestNode\" attr1=\"11\" attr2=\"\">\n"
            "       <Device name=\"TestDevice\" attr1=\"33\" attr2=\"\" attr3=\"bar\" config=\"{devname "
            "&quot;eth0&quot;, address &quot;127.0.0.1&quot;, version ipv4, port 5116}\"/>\n"
            "    </Node>\n"
            "</DeploymentView>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_DVXmlExporter::testExportConnection()
{
    std::unique_ptr<dvm::DVNode> node = std::make_unique<dvm::DVNode>();
    node->setTitle(QLatin1String("TestNode"));
    node->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(11));
    node->setEntityAttribute(QLatin1String("attr2"), QString());

    std::unique_ptr<dvm::DVConnection> connection = std::make_unique<dvm::DVConnection>();
    connection->setTitle(QLatin1String("TestConnection"));
    connection->setEntityAttribute(
            dvm::meta::Props::token(dvm::meta::Props::Token::from_node), QLatin1String("Node_From"));
    connection->setEntityAttribute(
            dvm::meta::Props::token(dvm::meta::Props::Token::from_port), QLatin1String("Port_From"));
    connection->setEntityAttribute(dvm::meta::Props::token(dvm::meta::Props::Token::to_node), QLatin1String("Node_To"));
    connection->setEntityAttribute(dvm::meta::Props::token(dvm::meta::Props::Token::to_port), QLatin1String("Port_To"));
    connection->setEntityAttribute(dvm::meta::Props::token(dvm::meta::Props::Token::to_bus), QLatin1String("Bus_To"));

    QList<shared::VEObject *> objects;
    objects.append(node.get());
    objects.append(connection.get());

    QVERIFY(m_exporter->exportObjectsSilently(objects, testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected =
            "<?xml version=\"1.0\"?>\n<DeploymentView>\n"
            "    <Node name=\"TestNode\" attr1=\"11\" attr2=\"\" />\n"
            "    <Connection name=\"TestConnection\" from_node=\"Node_From\" from_port=\"Port_From\" to_bus=\"Bus_To\" "
            "to_node=\"Node_To\" to_port=\"Port_To\" />\n"
            "</DeploymentView>";

    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_DVXmlExporter::testExportPartitionAndDevice()
{
    std::unique_ptr<dvm::DVNode> node = std::make_unique<dvm::DVNode>();
    node->setTitle(QLatin1String("TestNode"));
    node->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(11));
    node->setEntityAttribute(QLatin1String("attr2"), QString());

    auto partition = new dvm::DVPartition(node.get());
    partition->setTitle(QLatin1String("TestPartition"));
    partition->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(22));
    partition->setEntityAttribute(QLatin1String("attr2"), QString());
    partition->setEntityAttribute(QLatin1String("attr3"), QLatin1String("foo"));

    node->addPartition(partition);

    auto device = new dvm::DVDevice(node.get());
    device->setTitle(QLatin1String("TestDevice"));
    device->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(33));
    device->setEntityAttribute(QLatin1String("attr2"), QString());
    device->setEntityAttribute(QLatin1String("attr3"), QLatin1String("bar"));

    node->addDevice(device);

    QList<shared::VEObject *> objects;
    objects.append(node.get());
    objects.append(partition);
    objects.append(device);

    QVERIFY(m_exporter->exportObjectsSilently(objects, testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<DeploymentView>\n"
                                "    <Node name=\"TestNode\" attr1=\"11\" attr2=\"\">\n"
                                "       <Partition name=\"TestPartition\" attr1=\"22\" attr2=\"\" attr3=\"foo\"/>\n"
                                "       <Device name=\"TestDevice\" attr1=\"33\" attr2=\"\" attr3=\"bar\"/>\n"
                                "    </Node>\n"
                                "</DeploymentView>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_DVXmlExporter::testExportFunctions()
{
    std::unique_ptr<dvm::DVNode> node = std::make_unique<dvm::DVNode>();
    node->setTitle(QLatin1String("TestNode"));
    node->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(11));
    node->setEntityAttribute(QLatin1String("attr2"), QString());

    auto partition = new dvm::DVPartition(node.get());
    partition->setTitle(QLatin1String("TestPartition"));
    partition->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(22));
    partition->setEntityAttribute(QLatin1String("attr2"), QString());
    partition->setEntityAttribute(QLatin1String("attr3"), QLatin1String("foo"));

    node->addPartition(partition);

    auto function1 = new dvm::DVFunction(partition);
    function1->setTitle(QLatin1String("Function_1"));
    auto function2 = new dvm::DVFunction(partition);
    function2->setTitle(QLatin1String("Function_2"));
    auto function3 = new dvm::DVFunction(partition);
    function3->setTitle(QLatin1String("Function_3"));

    partition->addFunction(function1);
    partition->addFunction(function2);
    partition->addFunction(function3);

    QList<shared::VEObject *> objects;
    objects.append(node.get());
    objects.append(partition);
    objects.append(function1);
    objects.append(function2);
    objects.append(function3);

    QVERIFY(m_exporter->exportObjectsSilently(objects, testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<DeploymentView>\n"
                                "    <Node name=\"TestNode\" attr1=\"11\" attr2=\"\">\n"
                                "       <Partition name=\"TestPartition\" attr1=\"22\" attr2=\"\" attr3=\"foo\">\n"
                                "           <function>Function_1</function>"
                                "           <function>Function_2</function>"
                                "           <function>Function_3</function>"
                                "       </Partition>"
                                "    </Node>\n"
                                "</DeploymentView>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_DVXmlExporter::testExportPartition()
{
    std::unique_ptr<dvm::DVNode> node = std::make_unique<dvm::DVNode>();
    node->setTitle(QLatin1String("TestNode"));
    node->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(11));
    node->setEntityAttribute(QLatin1String("attr2"), QString());

    auto partition = new dvm::DVPartition(node.get());
    partition->setTitle(QLatin1String("TestPartition"));
    partition->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(22));
    partition->setEntityAttribute(QLatin1String("attr2"), QString());
    partition->setEntityAttribute(QLatin1String("attr3"), QLatin1String("foo"));

    node->addPartition(partition);

    QList<shared::VEObject *> objects;
    objects.append(node.get());
    objects.append(partition);

    QVERIFY(m_exporter->exportObjectsSilently(objects, testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<DeploymentView>\n"
                                "    <Node name=\"TestNode\" attr1=\"11\" attr2=\"\">\n"
                                "       <Partition name=\"TestPartition\" attr1=\"22\" attr2=\"\" attr3=\"foo\"/>\n"
                                "    </Node>\n"
                                "</DeploymentView>";
    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_DVXmlExporter::testExportFunctionsAndDevice()
{
    std::unique_ptr<dvm::DVNode> node = std::make_unique<dvm::DVNode>();
    node->setTitle(QLatin1String("TestNode"));
    node->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(11));
    node->setEntityAttribute(QLatin1String("attr2"), QString());

    auto partition = new dvm::DVPartition(node.get());
    partition->setTitle(QLatin1String("TestPartition"));
    partition->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(22));
    partition->setEntityAttribute(QLatin1String("attr2"), QString());
    partition->setEntityAttribute(QLatin1String("attr3"), QLatin1String("foo"));

    node->addPartition(partition);

    auto function1 = new dvm::DVFunction(partition);
    function1->setTitle(QLatin1String("Function_1"));
    auto function2 = new dvm::DVFunction(partition);
    function2->setTitle(QLatin1String("Function_2"));
    auto function3 = new dvm::DVFunction(partition);
    function3->setTitle(QLatin1String("Function_3"));

    partition->addFunction(function1);
    partition->addFunction(function2);
    partition->addFunction(function3);

    auto device = new dvm::DVDevice(node.get());
    device->setTitle(QLatin1String("TestDevice"));
    device->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(33));
    device->setEntityAttribute(QLatin1String("attr2"), QString());
    device->setEntityAttribute(QLatin1String("attr3"), QLatin1String("bar"));

    node->addDevice(device);

    QList<shared::VEObject *> objects;
    objects.append(node.get());
    objects.append(partition);
    objects.append(function1);
    objects.append(function2);
    objects.append(function3);
    objects.append(device);

    QVERIFY(m_exporter->exportObjectsSilently(objects, testFilePath));
    const QByteArray text = testFileContent();
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<DeploymentView>\n"
                                "    <Node name=\"TestNode\" attr1=\"11\" attr2=\"\">\n"
                                "       <Partition name=\"TestPartition\" attr1=\"22\" attr2=\"\" attr3=\"foo\">\n"
                                "           <function>Function_1</function>"
                                "           <function>Function_2</function>"
                                "           <function>Function_3</function>"
                                "       </Partition>"
                                "       <Device name=\"TestDevice\" attr1=\"33\" attr2=\"\" attr3=\"bar\"/>\n"
                                "    </Node>\n"
                                "</DeploymentView>";

    QVERIFY(XmlData(expected) == XmlData(text));
}

void tst_DVXmlExporter::testExportMessage()
{
    const QByteArray source =
            "<DeploymentView>"
            "    <Node name=\"node_linux\">"
            "        <Partition ada_runtime=\"\""
            "             coverage=\"FALSE\" cpu=\"x86_linux\" cpu_classifier=\"ocarina_processors_x86::x86.linux\""
            "             cpu_platform=\"PLATFORM_NATIVE\" name=\"demo\" package=\"ocarina_processors_x86\""
            "             vp_classifier=\"\" vp_name=\"\" vp_platform=\"PLATFORM_NONE\">"
            "                 <function>gnc_fm</function>"
            "        </Partition>"
            "        <Device name=\"\" requires_bus_access=\"ocarina_buses::ip.i\" port=\"eth0\" "
            "             asn1file=\"/opt/ip.asn\" asn1type=\"IP-Conf-T\" asn1module=\"POHICDRIVER-IP\" />"
            "    </Node>"
            "    <Node name=\"node_bsd\">"
            "        <Partition ada_runtime=\"\""
            "             coverage=\"FALSE\" cpu=\"x86_linux\" cpu_classifier=\"ocarina_processors_x86::x86.linux\""
            "             cpu_platform=\"PLATFORM_NATIVE\" name=\"demo\" package=\"ocarina_processors_x86\""
            "             vp_classifier=\"\" vp_name=\"\" vp_platform=\"PLATFORM_NONE\">"
            "                 <function>gnc_gui</function>"
            "        </Partition>"
            "        <Device name=\"\" requires_bus_access=\"ocarina_buses::ip.i\" port=\"eth0\" "
            "             asn1file=\"/opt/ip.asn\" asn1type=\"IP-Conf-T\" asn1module=\"POHICDRIVER-IP\" />"
            "    </Node>"
            "    <Connection name=\"\" from_node=\"node_linux\" from_port=\"eth0\" to_bus=\"ocarina_buses::ip.i\""
            "         to_node=\"node_bsd\" to_port=\"eth0\">"
            "        <Message name=\"init\" from_function=\"gnc_fm\" to_function=\"gnc_gui\" />"
            "    </Connection>"
            "</DeploymentView>";

    QBuffer buffer;
    buffer.setData(source);
    if (!buffer.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    dvm::DVXMLReader reader;
    reader.read(&buffer);

    QList<shared::VEObject *> objects;
    for (dvm::DVObject *obj : reader.parsedObjects()) {
        objects.append(obj);
    }
    QVERIFY(m_exporter->exportObjectsSilently(objects, testFilePath));
    const QByteArray text = testFileContent();
    QVERIFY(XmlData(source) == XmlData(text));
}

void tst_DVXmlExporter::testAll()
{
    {
        std::unique_ptr<dvm::DVNode> node = std::make_unique<dvm::DVNode>();
        node->setTitle(QLatin1String("TestNode"));
        node->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(11));
        node->setEntityAttribute(QLatin1String("attr2"), QString());

        auto partition = new dvm::DVPartition(node.get());
        partition->setTitle(QLatin1String("TestPartition"));
        partition->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(22));
        partition->setEntityAttribute(QLatin1String("attr2"), QString());
        partition->setEntityAttribute(QLatin1String("attr3"), QLatin1String("foo"));

        node->addPartition(partition);

        auto function1 = new dvm::DVFunction(partition);
        function1->setTitle(QLatin1String("Function_1"));
        auto function2 = new dvm::DVFunction(partition);
        function2->setTitle(QLatin1String("Function_2"));
        auto function3 = new dvm::DVFunction(partition);
        function3->setTitle(QLatin1String("Function_3"));

        partition->addFunction(function1);
        partition->addFunction(function2);
        partition->addFunction(function3);

        auto device = new dvm::DVDevice(node.get());
        device->setTitle(QLatin1String("TestDevice"));
        device->setEntityAttribute(QLatin1String("attr1"), QVariant::fromValue(33));
        device->setEntityAttribute(QLatin1String("attr2"), QString());
        device->setEntityAttribute(QLatin1String("attr3"), QLatin1String("bar"));

        node->addDevice(device);

        std::unique_ptr<dvm::DVConnection> connection = std::make_unique<dvm::DVConnection>();
        connection->setTitle(QLatin1String("TestConnection"));
        connection->setEntityAttribute(
                dvm::meta::Props::token(dvm::meta::Props::Token::from_node), QLatin1String("Node_From"));
        connection->setEntityAttribute(
                dvm::meta::Props::token(dvm::meta::Props::Token::from_port), QLatin1String("Port_From"));
        connection->setEntityAttribute(
                dvm::meta::Props::token(dvm::meta::Props::Token::to_node), QLatin1String("Node_To"));
        connection->setEntityAttribute(
                dvm::meta::Props::token(dvm::meta::Props::Token::to_port), QLatin1String("Port_To"));
        connection->setEntityAttribute(
                dvm::meta::Props::token(dvm::meta::Props::Token::to_bus), QLatin1String("Bus_To"));

        QList<shared::VEObject *> objects;
        objects.append(node.get());
        objects.append(partition);
        objects.append(function1);
        objects.append(function2);
        objects.append(function3);
        objects.append(device);
        objects.append(connection.get());

        QVERIFY(m_exporter->exportObjectsSilently(objects, testFilePath));
        const QByteArray text = testFileContent();
        const QByteArray expected = "<?xml version=\"1.0\"?>\n<DeploymentView>\n"
                                    "    <Node name=\"TestNode\" attr1=\"11\" attr2=\"\">\n"
                                    "       <Partition name=\"TestPartition\" attr1=\"22\" attr2=\"\" attr3=\"foo\">\n"
                                    "           <function>Function_1</function>"
                                    "           <function>Function_2</function>"
                                    "           <function>Function_3</function>"
                                    "       </Partition>"
                                    "       <Device name=\"TestDevice\" attr1=\"33\" attr2=\"\" attr3=\"bar\"/>\n"
                                    "    </Node>\n"
                                    "    <Connection name=\"TestConnection\" from_node=\"Node_From\" "
                                    "from_port=\"Port_From\" to_bus=\"Bus_To\" "
                                    "to_node=\"Node_To\" to_port=\"Port_To\" />\n"
                                    "</DeploymentView>";
        QVERIFY(XmlData(expected) == XmlData(text));
    }
}

void tst_DVXmlExporter::testExportToBuffer()
{
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    m_exporter->exportObjects({}, &buffer);
    const QByteArray expected = "<?xml version=\"1.0\"?>\n<DeploymentView/>";
    QVERIFY(XmlData(expected) == XmlData(buffer.data()));
}

QTEST_MAIN(tst_DVXmlExporter)

#include "tst_dvxmlexporter.moc"
