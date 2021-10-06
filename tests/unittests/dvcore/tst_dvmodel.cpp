/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvconnection.h"
#include "dvdevice.h"
#include "dvmodel.h"
#include "dvnode.h"

#include <QScopedPointer>
#include <QtTest>
#include <dvpartition.h>

class tst_DVModel : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void testAddObject();
    void testAddObjects();
    void testRemoveObject();
    void testClear();
    void testInitFromObjects();
    void testConnectionName();
    void testGetObjectByName();
    void testUniqueNodeLabel();

private:
    QScopedPointer<dvm::DVModel> m_model;
};

void tst_DVModel::init()
{
    m_model.reset(new dvm::DVModel());
}

void tst_DVModel::testAddObject()
{
    QCOMPARE(m_model->objects().size(), 0);

    auto obj = new dvm::DVNode();
    bool ok = m_model->addObject(obj);
    QCOMPARE(ok, true);
    QCOMPARE(m_model->objects().size(), 1);
    QCOMPARE(m_model->getObject(obj->id()), obj);

    // Do not add the same object twice
    ok = m_model->addObject(obj);
    QCOMPARE(ok, false);
    QCOMPARE(m_model->objects().size(), 1);
}

void tst_DVModel::testAddObjects()
{
    m_model->addObjects<dvm::DVObject *>({ new dvm::DVNode(), new dvm::DVNode() });
    QCOMPARE(m_model->objects().size(), 2);

    m_model->addObjects<dvm::DVObject *>({ new dvm::DVNode(), new dvm::DVNode(), new dvm::DVNode() });
    QCOMPARE(m_model->objects().size(), 5);
}

void tst_DVModel::testRemoveObject()
{
    auto obj = new dvm::DVNode();

    bool ok = m_model->removeObject(obj);
    QCOMPARE(ok, false);

    m_model->addObject(obj);
    QCOMPARE(m_model->getObject(obj->id()), obj);

    ok = m_model->removeObject(obj);
    QCOMPARE(ok, true);
    QCOMPARE(m_model->getObject(obj->id()), nullptr);
}

void tst_DVModel::testClear()
{
    auto obj = new dvm::DVNode();
    m_model->addObject(obj);
    QCOMPARE(m_model->objects().size(), 1);

    m_model->clear();
    QCOMPARE(m_model->objects().size(), 0);
}

void tst_DVModel::testInitFromObjects()
{
    auto obj = new dvm::DVNode();
    m_model->addObject(obj);

    m_model->initFromObjects<dvm::DVObject *>({ new dvm::DVNode(), new dvm::DVNode() });
    QCOMPARE(m_model->objects().size(), 2);
}

void tst_DVModel::testConnectionName()
{
    auto node1 = new dvm::DVNode();
    node1->setTitle("Node_A");
    m_model->addObject(node1);
    auto device1 = new dvm::DVDevice(node1);
    device1->setTitle("Device_A");
    device1->setPortName("Port_A");
    device1->setQualifier("ipv6");
    node1->addDevice(device1);
    m_model->addObject(device1);

    auto node2 = new dvm::DVNode();
    node2->setTitle("Node_B");
    m_model->addObject(node2);
    auto device2 = new dvm::DVDevice(node2);
    device2->setTitle("Device_B");
    device2->setPortName("Port_B");
    device2->setQualifier("ipv6");
    node2->addDevice(device2);
    m_model->addObject(device2);

    auto connection = new dvm::DVConnection(device1, device2);

    QCOMPARE(m_model->newConnectionName(), "Connection_1");

    m_model->addObject(connection);
    QCOMPARE(m_model->newConnectionName(), "Connection_2");

    connection->setTitle("Connection_2");
    QCOMPARE(m_model->newConnectionName(), "Connection_3");

    connection->setTitle("New_name");
    QCOMPARE(m_model->newConnectionName(), "Connection_2");
}

void tst_DVModel::testGetObjectByName()
{
    auto node1 = new dvm::DVNode();
    node1->setTitle("Node_A");
    m_model->addObject(node1);
    auto partition1 = new dvm::DVPartition(node1);
    partition1->setTitle("Partition_A");
    node1->addPartition(partition1);
    m_model->addObject(partition1);

    QCOMPARE(m_model->getObjectByName("partition_a", dvm::DVObject::Type::Partition), partition1);
    QCOMPARE(m_model->getObjectByName("partition_a", dvm::DVObject::Type::Partition, Qt::CaseSensitive), nullptr);
    QCOMPARE(m_model->getObjectByName("partition_a", dvm::DVObject::Type::Node), nullptr);
}

void tst_DVModel::testUniqueNodeLabel()
{
    auto node1 = new dvm::DVNode();
    m_model->addObject(node1);
    QCOMPARE(node1->nodeLabel(), "Node_1");

    auto node2 = new dvm::DVNode();
    node2->setEntityAttribute(dvm::meta::Props::token(dvm::meta::Props::Token::node_label), "Node_1");
    m_model->addObject(node2);
    QCOMPARE(node2->nodeLabel(), "Node_2");
}

QTEST_APPLESS_MAIN(tst_DVModel)

#include "tst_dvmodel.moc"
