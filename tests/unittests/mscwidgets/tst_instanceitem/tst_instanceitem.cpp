/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include <exceptions.h>
#include <instanceitem.h>
#include <mscinstance.h>

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
    m_instanceItem = new InstanceItem(m_instance);
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

    m_instance->setName("FirstInst");
    QCOMPARE(m_instanceItem->name(), QString("FirstInst"));
}

void tst_InstanceItem::testKindUpdate()
{
    QCOMPARE(m_instanceItem->kind(), QString(""));

    m_instance->setKind("kindda");
    QCOMPARE(m_instanceItem->kind(), QString("kindda"));
}

QTEST_MAIN(tst_InstanceItem)

#include "tst_instanceitem.moc"
