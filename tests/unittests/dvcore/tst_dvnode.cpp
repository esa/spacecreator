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

#include "dvnode.h"
#include "dvpartition.h"

#include <QScopedPointer>
#include <QtTest>

class tst_DVNode : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void testnewPartitionNameName();

private:
    QScopedPointer<dvm::DVNode> m_node;
};

void tst_DVNode::init()
{
    m_node.reset(new dvm::DVNode());
}

void tst_DVNode::testnewPartitionNameName()
{
    QCOMPARE(m_node->newPartitionName(), "Partition_1");

    auto partition1 = new dvm::DVPartition(m_node.data());
    m_node->addPartition(partition1);
    QCOMPARE(m_node->newPartitionName(), "Partition_2");

    partition1->setTitle("Partition_1");
    QCOMPARE(m_node->newPartitionName(), "Partition_2");

    auto partition2 = new dvm::DVPartition(m_node.data());
    partition2->setTitle("Partition_2");
    m_node->addPartition(partition2);
    QCOMPARE(m_node->newPartitionName(), "Partition_3");

    m_node->removePartition(partition1);
    delete partition1;
    QCOMPARE(m_node->newPartitionName(), "Partition_3");
}

QTEST_APPLESS_MAIN(tst_DVNode)

#include "tst_dvnode.moc"
