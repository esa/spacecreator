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

#include "exceptions.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccondition.h"
#include "msccoregion.h"
#include "msccreate.h"
#include "mscgate.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "msctimer.h"

#include <QtTest>

using namespace msc;

class tst_MscChart : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testDestructor();
    void testAddInstance();
    void testRemoveInstance();
    void testNoDuplicateInstance();
    void testNoNullPtrInstance();
    void testInstanceByName();
    void testAddMessage();
    void testRemoveMessage();
    void testNoDuplicateMessage();
    void testNoNullPtrMessage();
    void testMessageByName();
    void testAddGate();
    void testRemoveGate();
    void testAddCondition();
    void testRemoveCondition();
    void testTimerRelation();

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

void tst_MscChart::testDestructor()
{
    QVector<QPointer<MscEntity>> chartEntities;
    MscChart *chart = new MscChart;
    const QMetaEnum &chartEntitiesMeta = QMetaEnum::fromType<MscEntity::EntityType>();
    for (int i = 0; i < chartEntitiesMeta.keyCount(); ++i) {
        const MscEntity::EntityType entityType = static_cast<MscEntity::EntityType>(chartEntitiesMeta.value(i));
        switch (entityType) {
        case MscEntity::EntityType::Chart:
        case MscEntity::EntityType::Document:
            break;
        case MscEntity::EntityType::Instance:
            chart->addInstance(new MscInstance());
            chartEntities.append(chart->instances().first());
            break;
        case MscEntity::EntityType::Message:
            chart->addInstanceEvent(new MscMessage());
            chartEntities.append(chart->instanceEvents().first());
            break;
        case MscEntity::EntityType::Timer:
            chart->addInstanceEvent(new MscTimer);
            chartEntities.append(chart->instanceEvents().first());
            break;
        case MscEntity::EntityType::Gate:
            chart->addGate(new MscGate());
            chartEntities.append(chart->gates().first());
            break;
        case MscEntity::EntityType::Condition:
            chart->addInstanceEvent(new MscCondition());
            chartEntities.append(chart->instanceEvents().first());
            break;
        case MscEntity::EntityType::Action:
            chart->addInstanceEvent(new MscAction);
            chartEntities.append(chart->instanceEvents().first());
            break;
        case MscEntity::EntityType::Coregion:
            chart->addInstanceEvent(new MscCoregion());
            chartEntities.append(chart->instanceEvents().first());
            break;
        case MscEntity::EntityType::Create:
            chart->addInstanceEvent(new MscCreate());
            chartEntities.append(chart->instanceEvents().first());
            break;
        default:
            QFAIL("It seems a new MscEntity::EntityType has been introduced,\n"
                  "but it's not covered here.\n"
                  "Please add it to process or ignore explicitly.");
            break;
        }
    }

    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instanceEvents().size(), 6);
    QCOMPARE(chart->gates().size(), 1);

    delete chart;

    for (const QPointer<MscEntity> &chartEntity : chartEntities)
        QCOMPARE(chartEntity, QPointer<MscEntity>(nullptr));
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

void tst_MscChart::testRemoveInstance()
{
    QCOMPARE(m_chart->instances().size(), 0);

    QScopedPointer<MscInstance> instance1(new MscInstance("IN", m_chart));
    QScopedPointer<MscInstance> instance2(new MscInstance("OUT", m_chart));

    m_chart->addInstance(instance1.data());
    m_chart->addInstance(instance2.data());
    QCOMPARE(m_chart->instances().size(), 2);

    m_chart->removeInstance(instance1.data());
    QCOMPARE(m_chart->instances().size(), 1);
    m_chart->removeInstance(instance2.data());
    QCOMPARE(m_chart->instances().size(), 0);
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
    QCOMPARE(m_chart->instanceEvents().size(), 0);

    auto message1 = new MscMessage("IN", m_chart);
    m_chart->addInstanceEvent(message1);
    auto message2 = new MscMessage("OUT", m_chart);
    m_chart->addInstanceEvent(message2);
    QCOMPARE(m_chart->instanceEvents().size(), 2);
}

void tst_MscChart::testRemoveMessage()
{
    QCOMPARE(m_chart->instanceEvents().size(), 0);

    QScopedPointer<MscMessage> message1(new MscMessage("IN", m_chart));
    QScopedPointer<MscMessage> message2(new MscMessage("OUT", m_chart));

    m_chart->addInstanceEvent(message1.data());
    m_chart->addInstanceEvent(message2.data());
    QCOMPARE(m_chart->instanceEvents().size(), 2);

    m_chart->removeInstanceEvent(message1.data());
    QCOMPARE(m_chart->instanceEvents().size(), 1);
    m_chart->removeInstanceEvent(message2.data());
    QCOMPARE(m_chart->instanceEvents().size(), 0);
}

void tst_MscChart::testNoDuplicateMessage()
{
    auto message = new MscMessage("IN", m_chart);
    m_chart->addInstanceEvent(message);
    m_chart->addInstanceEvent(message);
    QCOMPARE(m_chart->instanceEvents().size(), 1);
}

void tst_MscChart::testNoNullPtrMessage()
{
    m_chart->addInstanceEvent(nullptr);
    QCOMPARE(m_chart->instanceEvents().size(), 0);
}

void tst_MscChart::testMessageByName()
{
    auto message = new MscMessage("IN", m_chart);
    m_chart->addInstanceEvent(message);
    QCOMPARE(m_chart->messageByName("IN"), message);
    QCOMPARE(m_chart->messageByName("OUT"), static_cast<MscMessage *>(nullptr));
}

void tst_MscChart::testAddGate()
{
    auto gate = new MscGate("testGateName", m_chart);
    m_chart->addGate(gate);
    QCOMPARE(m_chart->gates().size(), 1);
    m_chart->addGate(gate);
    QCOMPARE(m_chart->gates().size(), 1);
    m_chart->addGate(nullptr);
    QCOMPARE(m_chart->gates().size(), 1);
}

void tst_MscChart::testRemoveGate()
{
    QScopedPointer<MscGate> gate1(new MscGate("A", m_chart));
    m_chart->addGate(gate1.data());
    MscGate *gate2 = new MscGate("B", m_chart);
    m_chart->addGate(gate2);
    QCOMPARE(m_chart->gates().size(), 2);
    m_chart->removeGate(nullptr);
    QCOMPARE(m_chart->gates().size(), 2);
    auto gateOne = m_chart->gates().first();
    m_chart->removeGate(gateOne);
    QCOMPARE(m_chart->gates().size(), 1);
    m_chart->removeGate(gateOne);
    QCOMPARE(m_chart->gates().size(), 1);
}

void tst_MscChart::testAddCondition()
{
    auto condition = new MscCondition("Condition_1", m_chart);

    m_chart->addInstanceEvent(condition);
    QCOMPARE(m_chart->instanceEvents().size(), 1);

    m_chart->addInstanceEvent(condition);
    QCOMPARE(m_chart->instanceEvents().size(), 1);

    m_chart->addInstanceEvent(nullptr);
    QCOMPARE(m_chart->instanceEvents().size(), 1);
}

void tst_MscChart::testRemoveCondition()
{
    QScopedPointer<MscCondition> condition1(new MscCondition("Condition_1", m_chart));
    m_chart->addInstanceEvent(condition1.data());
    MscCondition *condition2 = new MscCondition("Condition_2", m_chart);
    m_chart->addInstanceEvent(condition2);

    QCOMPARE(m_chart->instanceEvents().size(), 2);

    m_chart->removeInstanceEvent(nullptr);
    QCOMPARE(m_chart->instanceEvents().size(), 2);

    auto condition = m_chart->instanceEvents().first();
    m_chart->removeInstanceEvent(condition);
    QCOMPARE(m_chart->instanceEvents().size(), 1);

    m_chart->removeInstanceEvent(condition);
    QCOMPARE(m_chart->instanceEvents().size(), 1);
}

void tst_MscChart::testTimerRelation()
{
    MscInstance *instance1 = new MscInstance("IN", m_chart);
    m_chart->addInstance(instance1);
    MscTimer *timer1 = new MscTimer("T1", MscTimer::TimerType::Start);
    timer1->setInstance(instance1);
    m_chart->addInstanceEvent(timer1);
    MscTimer *timer2 = new MscTimer("T1", MscTimer::TimerType::Stop);
    timer2->setInstance(instance1);
    m_chart->addInstanceEvent(timer2);
    MscTimer *timer3 = new MscTimer("T3", MscTimer::TimerType::Timeout);
    timer3->setInstance(instance1);
    m_chart->addInstanceEvent(timer3);

    MscInstance *instance2 = new MscInstance("OUT", m_chart);
    m_chart->addInstance(instance2);
    MscTimer *timer4 = new MscTimer("T1", MscTimer::TimerType::Start);
    timer4->setInstance(instance2);
    m_chart->addInstanceEvent(timer4);

    m_chart->checkTimerRelations();

    QVERIFY(timer1->precedingTimer() == nullptr);
    QCOMPARE(timer1->followingTimer(), timer2);

    QCOMPARE(timer2->precedingTimer(), timer1);
    QVERIFY(timer2->followingTimer() == nullptr);

    QVERIFY(timer3->precedingTimer() == nullptr);
    QVERIFY(timer3->followingTimer() == nullptr);

    // update on name change
    timer2->setName("T3");
    QVERIFY(timer1->precedingTimer() == nullptr);
    QVERIFY(timer1->followingTimer() == nullptr);
    QVERIFY(timer2->precedingTimer() == nullptr);
    QCOMPARE(timer2->followingTimer(), timer3);
    QCOMPARE(timer3->precedingTimer(), timer2);
    QVERIFY(timer3->followingTimer() == nullptr);
    QVERIFY(timer4->precedingTimer() == nullptr);
    QVERIFY(timer4->followingTimer() == nullptr);

    // update on new timer
    MscTimer *timer5 = new MscTimer("T1", MscTimer::TimerType::Timeout);
    timer5->setInstance(instance2);
    m_chart->addInstanceEvent(timer5);
    QVERIFY(timer1->precedingTimer() == nullptr);
    QVERIFY(timer1->followingTimer() == nullptr);
    QVERIFY(timer2->precedingTimer() == nullptr);
    QCOMPARE(timer2->followingTimer(), timer3);
    QCOMPARE(timer3->precedingTimer(), timer2);
    QVERIFY(timer3->followingTimer() == nullptr);
    QVERIFY(timer4->precedingTimer() == nullptr);
    QCOMPARE(timer4->followingTimer(), timer5);
    QCOMPARE(timer5->precedingTimer(), timer4);
    QVERIFY(timer5->followingTimer() == nullptr);

    // update on deleted timer
    m_chart->removeInstanceEvent(timer5);
    delete timer5;
    QVERIFY(timer1->precedingTimer() == nullptr);
    QVERIFY(timer1->followingTimer() == nullptr);
    QVERIFY(timer2->precedingTimer() == nullptr);
    QCOMPARE(timer2->followingTimer(), timer3);
    QCOMPARE(timer3->precedingTimer(), timer2);
    QVERIFY(timer3->followingTimer() == nullptr);
    QVERIFY(timer4->precedingTimer() == nullptr);
    QVERIFY(timer4->followingTimer() == nullptr);

    // update on instance moved timer
    timer1->setInstance(instance2);
    QVERIFY(timer1->precedingTimer() == nullptr);
    QCOMPARE(timer1->followingTimer(), timer4);
    QVERIFY(timer2->precedingTimer() == nullptr);
    QCOMPARE(timer2->followingTimer(), timer3);
    QCOMPARE(timer3->precedingTimer(), timer2);
    QVERIFY(timer3->followingTimer() == nullptr);
    QCOMPARE(timer4->precedingTimer(), timer1);
    QVERIFY(timer4->followingTimer() == nullptr);
}

QTEST_APPLESS_MAIN(tst_MscChart)

#include "tst_mscchart.moc"
