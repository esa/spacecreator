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

#include "actionitem.h"
#include "chartitem.h"
#include "chartviewtestbase.h"
#include "commands/cmdactionitemcreate.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscaction.h"
#include "mscchart.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "syntheticinteraction.h"

#include <QtTest>

using namespace msc;

class tsti_Chartitem : public ChartViewTestBase
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase() { initTestCaseBase(); }
    void init() { initBase(); }
    void cleanup() { cleanupBase(); }

    void testKeepSpaceAtBottom();
    void testItemLimit();
};

void tsti_Chartitem::testKeepSpaceAtBottom()
{
    const QString msc("MSCDOCUMENT doc1; \
                         MSC msc1; \
                             INSTANCE A; \
                                 OUT m1 TO B; \
                             ENDINSTANCE; \
                             INSTANCE B; \
                                 IN m1 FROM A; \
                             ENDINSTANCE; \
                         ENDMSC; \
                     ENDMSCDOCUMENT;");
    loadView(msc);

    msc::ChartItem *chartItem = m_chartModel->itemForChart();
    const QRectF oldRect = chartItem->sceneBoundingRect();

    auto message = qobject_cast<msc::MscMessage *>(m_chart->instanceEvents().at(0));
    msc::MessageItem *messageItem = m_chartModel->itemForMessage(message);

    auto instanceB = qobject_cast<msc::MscInstance *>(m_chart->instances().at(1));
    msc::InstanceItem *instanceBItem = m_chartModel->itemForInstance(instanceB);

    // Change, so message points from the left instance top to the right instance bottom
    vstest::sendMouseMove(m_view->viewport(), center(messageItem)); // so the correct grip is pressed
    const QPoint targetPoint = m_view->mapFromScene(messageItem->messagePoints().last());
    const QPoint newTarget = bottomCenter(instanceBItem) - QPoint(0.0, 15.0);

    vstest::sendMouseDrag(m_view->viewport(), targetPoint, newTarget);

    const QRectF newRect = chartItem->sceneBoundingRect();

    QTest::qWait(500);
    waitForLayoutUpdate();

    // The chart increases it's height, so a new item can be added easily (space at the bottom)
    QVERIFY(newRect.height() > oldRect.height());
}

void tsti_Chartitem::testItemLimit()
{
    const QString msc("MSCDOCUMENT doc1; \
                         MSC msc1; \
                             INSTANCE A; \
                                 ACTION 'a0';\
                                 OUT m2 TO B; \
                             ENDINSTANCE; \
                             INSTANCE B; \
                                 ACTION 'a1';\
                                 IN m2 FROM A; \
                             ENDINSTANCE; \
                         ENDMSC; \
                     ENDMSCDOCUMENT;");
    m_chartModel->setVisibleItemLimit(3);
    loadView(msc);

    QCOMPARE(m_chart->instanceEvents().size(), 3);
    QCOMPARE(m_chartModel->instanceEventItems().size(), 3);
    InteractiveObject *action0 = m_chartModel->itemForEntity(m_chart->instanceEvents().at(0));
    QVERIFY(action0 != nullptr);
    InteractiveObject *action1 = m_chartModel->itemForEntity(m_chart->instanceEvents().at(1));
    QVERIFY(action1 != nullptr);
    InteractiveObject *message2 = m_chartModel->itemForEntity(m_chart->instanceEvents().at(2));
    QVERIFY(message2 != nullptr);

    // append one action
    auto instanceA = qobject_cast<msc::MscInstance *>(m_chart->instances().at(0));
    auto addCommand = new cmd::CmdActionItemCreate(nullptr, instanceA, -1, m_chartModel.data());
    m_undoStack->push(addCommand);
    waitForLayoutUpdate();
    QCOMPARE(m_chart->instanceEvents().size(), 4);
    const QVector<InteractiveObject *> &items = m_chartModel->instanceEventItems();
    QCOMPARE(items.size(), 3);
    QVERIFY(!items.contains(action0)); // action0 is not shown anymore
    QVERIFY(items.contains(action1));
    QVERIFY(items.contains(message2));
    InteractiveObject *action3 = m_chartModel->itemForEntity(m_chart->instanceEvents().at(3));
    QVERIFY(action3 != nullptr);

    // append one more action, pushing the message to the top
    addCommand = new cmd::CmdActionItemCreate(nullptr, instanceA, -1, m_chartModel.data());
    m_undoStack->push(addCommand);
    waitForLayoutUpdate();
    QCOMPARE(m_chart->instanceEvents().size(), 5);
    QCOMPARE(items.size(), 3);
    QVERIFY(!items.contains(action0)); // action0 is not shown anymore
    QVERIFY(!items.contains(action1)); // action1 is not shown anymore
    QVERIFY(items.contains(message2));
    QVERIFY(items.contains(action3));
}

QTEST_MAIN(tsti_Chartitem)

#include "tsti_chartitem.moc"
