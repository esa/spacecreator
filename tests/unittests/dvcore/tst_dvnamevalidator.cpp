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

#include "dvmodel.h"
#include "dvnamevalidator.h"
#include "dvnode.h"
#include "dvpartition.h"

#include <QtTest>
#include <dvdevice.h>

using namespace dvm;

class tst_DVNameValidator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testAcceptableNodeName();
    void testAcceptablePartitionName_data();
    void testAcceptablePartitionName();
    void testAcceptableDeviceName_data();
    void testAcceptableDeviceName();

private:
    DVNode *addNode(const QString &name);
    DVPartition *addPartition(const QString &name, DVNode *node);
    DVDevice *addDevice(const QString &name, DVNode *node);

    std::unique_ptr<DVModel> m_model;
};

void tst_DVNameValidator::init()
{
    m_model = std::make_unique<DVModel>();
}

void tst_DVNameValidator::cleanup()
{
    m_model.reset();
}

void tst_DVNameValidator::testAcceptableNodeName()
{
    QCOMPARE(DVNameValidator::isAcceptableName(nullptr, "NewName"), false);

    DVNode *node = addNode("Node1");
    m_model->addObjectImpl(node);
    QCOMPARE(DVNameValidator::isAcceptableName(node, "NewName"), true); // totally new valid name
    QCOMPARE(DVNameValidator::isAcceptableName(node, "Ab+#=!"), true); // Node names can have any character
    QCOMPARE(DVNameValidator::isAcceptableName(node, "Node1"), true); // it's current name is ok

    DVNode *node2 = addNode("Node2");
    QCOMPARE(DVNameValidator::isAcceptableName(node2, "NewName"), true); // totally new valid name
    QCOMPARE(DVNameValidator::isAcceptableName(node2, "Node1"), false); // name already used by another node
}

void tst_DVNameValidator::testAcceptablePartitionName_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<bool>("result");

    QTest::newRow("totally new valid name") << "NewPartition" << true;
    QTest::newRow("it's current name is ok") << "Partition1" << true;
    QTest::newRow("restricted characters") << "Ab+#=!" << false;
    QTest::newRow("forbidden word") << "class" << false;
    QTest::newRow("name used already") << "Partition2" << false;
    QTest::newRow("name used already in another node") << "PartitionA" << false;
}

void tst_DVNameValidator::testAcceptablePartitionName()
{
    QFETCH(QString, name);
    QFETCH(bool, result);

    DVNode *node1 = addNode("Node1");
    DVPartition *testPartition = addPartition("Partition1", node1);
    addPartition("Partition2", node1);

    DVNode *node2 = addNode("Node2");
    addPartition("PartitionA", node2);

    QCOMPARE(dvm::DVNameValidator::isAcceptableName(testPartition, name), result);
}

void tst_DVNameValidator::testAcceptableDeviceName_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<bool>("result");

    QTest::newRow("totally new valid name") << "NewDevice" << true;
    QTest::newRow("it's current name is ok") << "eth0" << true;
    QTest::newRow("restricted characters") << "Ab+#=!" << false;
    QTest::newRow("forbidden word") << "class" << false;
    QTest::newRow("Name used laready in node") << "eth1" << false;
    QTest::newRow("Name only used in other node") << "eth2" << true;
}

void tst_DVNameValidator::testAcceptableDeviceName()
{
    QFETCH(QString, name);
    QFETCH(bool, result);

    DVNode *node1 = addNode("Node1");
    DVDevice *testDevice = addDevice("eth0", node1);
    addDevice("eth1", node1);

    DVNode *node2 = addNode("Node2");
    addDevice("eth2", node2);

    QCOMPARE(dvm::DVNameValidator::isAcceptableName(testDevice, name), result);
}

DVNode *tst_DVNameValidator::addNode(const QString &name)
{
    DVNode *node = new DVNode;
    node->setTitle(name);
    m_model->addObjectImpl(node);
    return node;
}

DVPartition *tst_DVNameValidator::addPartition(const QString &name, DVNode *node)
{
    auto partition = new DVPartition(node);
    partition->setTitle(name);
    node->addPartition(partition);
    m_model->addObjectImpl(partition);
    return partition;
}

DVDevice *tst_DVNameValidator::addDevice(const QString &name, DVNode *node)
{
    auto device = new DVDevice(node);
    device->setTitle(name);
    node->addDevice(device);
    m_model->addObjectImpl(device);
    return device;
}

QTEST_APPLESS_MAIN(tst_DVNameValidator)

#include "tst_dvnamevalidator.moc"
