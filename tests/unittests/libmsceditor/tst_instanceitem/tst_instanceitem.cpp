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

#include "instanceitem.h"
#include "mscinstance.h"

#include <QtTest>

using namespace msc;

class tst_InstanceItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testNameUpdate();
    void testKindUpdate();

private:
    MscInstance *m_instance = nullptr;
    InstanceItem *m_instanceItem = nullptr;
};

void tst_InstanceItem::init()
{
    m_instance = new MscInstance("INST1");
    m_instanceItem = new InstanceItem(m_instance, nullptr);
    m_instanceItem->setName(m_instance->name());
}

void tst_InstanceItem::cleanup()
{
    delete m_instanceItem;
    m_instanceItem = nullptr;
    delete m_instance;
    m_instance = nullptr;
}

void tst_InstanceItem::testNameUpdate()
{
    QCOMPARE(m_instanceItem->name(), QString("INST1"));
    QCOMPARE(m_instanceItem->modelItem()->name(), m_instanceItem->name());

    m_instance->setName("FirstInst");
    QCOMPARE(m_instanceItem->name(), QString("FirstInst"));
    QCOMPARE(m_instanceItem->modelItem()->name(), m_instanceItem->name());
}

void tst_InstanceItem::testKindUpdate()
{
    QCOMPARE(m_instanceItem->kind(), QString(""));

    static const QLatin1String testKind("kindda");
    m_instance->setKind(testKind);
    QCOMPARE(m_instanceItem->kind(), testKind);
    QCOMPARE(m_instanceItem->modelItem()->kind(), m_instanceItem->kind());
}

QTEST_MAIN(tst_InstanceItem)

#include "tst_instanceitem.moc"
