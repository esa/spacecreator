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
#include <mscchart.h>
#include <mscinstance.h>
#include <mscmessage.h>

#include <QtTest>

using namespace msc;

class tst_MscChart : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testAddInstance();
    void testNoDuplicateInstance();
    void testNoNullPtrInstance();
    void testInstanceByName();
    void testAddMessage();
    void testNoDuplicateMessage();
    void testNoNullPtrMessage();
    void testMessageByName();

private:
    MscChart *m_chart = nullptr;
};

void tst_MscChart::init()
{
    m_chart = new MscChart;
}

void tst_MscChart::cleanup()
{
    delete m_chart;
    m_chart = nullptr;
}

void tst_MscChart::testAddInstance()
{
    QCOMPARE(m_chart->instances().size(), 0);

    auto instance1 = new MscInstance("IN", m_chart);
    m_chart->addInstance(instance1);
    auto instance2 = new MscInstance("OUT", m_chart);
    m_chart->addInstance(instance2);
    QCOMPARE(m_chart->instances().size(), 2);
}

void tst_MscChart::testNoDuplicateInstance()
{
    auto instance = new MscInstance("IN", m_chart);
    m_chart->addInstance(instance);
    m_chart->addInstance(instance);
    QCOMPARE(m_chart->instances().size(), 1);
}

void tst_MscChart::testNoNullPtrInstance()
{
    m_chart->addInstance(nullptr);
    QCOMPARE(m_chart->instances().size(), 0);
}

void tst_MscChart::testInstanceByName()
{
    auto instance = new MscInstance("IN", m_chart);
    m_chart->addInstance(instance);
    QCOMPARE(m_chart->instanceByName("IN"), instance);
    QCOMPARE(m_chart->instanceByName("OUT"), static_cast<MscInstance *>(nullptr));
}

void tst_MscChart::testAddMessage()
{
    QCOMPARE(m_chart->messages().size(), 0);

    auto message1 = new MscMessage("IN", m_chart);
    m_chart->addMessage(message1);
    auto message2 = new MscMessage("OUT", m_chart);
    m_chart->addMessage(message2);
    QCOMPARE(m_chart->messages().size(), 2);
}

void tst_MscChart::testNoDuplicateMessage()
{
    auto message = new MscMessage("IN", m_chart);
    m_chart->addMessage(message);
    m_chart->addMessage(message);
    QCOMPARE(m_chart->messages().size(), 1);
}

void tst_MscChart::testNoNullPtrMessage()
{
    m_chart->addMessage(nullptr);
    QCOMPARE(m_chart->messages().size(), 0);
}

void tst_MscChart::testMessageByName()
{
    auto message = new MscMessage("IN", m_chart);
    m_chart->addMessage(message);
    QCOMPARE(m_chart->messageByName("IN"), message);
    QCOMPARE(m_chart->messageByName("OUT"), static_cast<MscMessage *>(nullptr));
}

QTEST_APPLESS_MAIN(tst_MscChart)

#include "tst_mscchart.moc"
