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
#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/common/mscutils.h"
#include "baseitems/instanceenditem.h"
#include "baseitems/instanceheaditem.h"
#include "chartitem.h"
#include "chartlayoutmanager.h"
#include "common/chartlayouttestbase.h"
#include "coregionitem.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccoregion.h"
#include "msccreate.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "mscreader.h"
#include "msctimer.h"
#include "sharedlibrary.h"
#include "timeritem.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScopedPointer>
#include <QVector>
#include <QtTest>
#include <cmath>

using namespace msc;

class tst_ChartLayoutManager : public ChartLayoutTestBase
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testNearestInstanceSimple();
    void testNearestInstanceCreate();

    void testTimerPositionWithCifInstance();
    void testDefaultChartSize();
    void testInstanceCifExtendedChartWidth();
    void testAddTwoMessages();
    void testMaxVisibleItems();

    void testCreateSetsInstanceY();
    void testCreateSetsYOfStoppedInstance();

    void testShiftHorizontalIfNeeded();
    void testNoHorizontalShiftOnEventAdd();

    void testShiftVerticalIfNeeded();

    void testMessageWithCifInformation();

    void testEventIndex();
    void testInstanceEventIndex();

protected:
    void parseMsc(const QString &mscDoc) override;

private:
    QVector<MscInstance *> m_instances;
    QVector<InstanceItem *> m_instanceItems;
    QVector<QRectF> m_instanceRects;

    const QSizeF defaultSize = { 200.0, 200.0 };
    const qreal m_maxOffset = 1.5; // used for size comparisons
};

void tst_ChartLayoutManager::parseMsc(const QString &mscText)
{
    ChartLayoutTestBase::parseMsc(mscText);

    for (MscInstance *instance : m_chart->instances()) {
        InstanceItem *instanceItem = m_chartModel->itemForInstance(instance);
        QVERIFY(instanceItem != nullptr);

        m_instances.append(instance);
        m_instanceItems.append(instanceItem);
        m_instanceRects.append(instanceItem->sceneBoundingRect());
    }
}

void tst_ChartLayoutManager::init()
{
    shared::initSharedLibrary();
    initBase();
}

void tst_ChartLayoutManager::cleanup()
{
    cleanupBase();
    m_instances.clear();
    m_instanceItems.clear();
    m_instanceRects.clear();
}

void tst_ChartLayoutManager::testNearestInstanceSimple()
{
    QString mscText = "MSC msc1; \
                          INSTANCE inst1; \
                              OUT Msg01 TO inst2; \
                          ENDINSTANCE; \
                          INSTANCE inst2; \
                              IN Msg01 FROM inst1; \
                          ENDINSTANCE; \
                       ENDMSC;";
    parseMsc(mscText);

    // point on the left
    MscInstance *inst = m_chartModel->nearestInstance({ m_instanceRects[0].left() - 10., 50. });
    QCOMPARE(inst, m_instances[0]);
    // point on the left
    inst = m_chartModel->nearestInstance({ m_instanceRects[1].left() + 10., 50. });
    QCOMPARE(inst, m_instances[1]);
    // point between itens, but closer to left
    qreal middle = m_instanceRects[0].right() + 20.;
    inst = m_chartModel->nearestInstance({ middle, 50. });
    QCOMPARE(inst, m_instances[0]);

    // point too far on the left
    inst = m_chartModel->nearestInstance({ m_instanceRects[0].left() - 100., 50. });
    QCOMPARE(inst, static_cast<MscInstance *>(nullptr));
    // point too far on the left
    inst = m_chartModel->nearestInstance({ m_instanceRects[1].right() + 100., 50. });
    QCOMPARE(inst, static_cast<MscInstance *>(nullptr));
}

void tst_ChartLayoutManager::testNearestInstanceCreate()
{
    QString mscText = "MSCDOCUMENT mscdoc;\
                        MSC msc1; \
                          INSTANCE Instance_A; \
                              ACTION 'Boot'; \
                              CREATE New_Instance1; \
                              IN Msg01 FROM New_Instance1; \
                          ENDINSTANCE; \
                          INSTANCE New_Instance1; \
                              OUT Msg01 TO Instance_A; \
                          ENDINSTANCE; \
                       ENDMSC;\
                        ENDMSCDOCUMENT;";
    parseMsc(mscText);
    QCOMPARE(m_instanceItems.size(), 2);

    // point on the left
    MscInstance *inst = m_chartModel->nearestInstance(m_instanceRects[1].center());
    QCOMPARE(inst, m_instances[1]);

    // point above the create instance (but on height of first instance)
    inst = m_chartModel->nearestInstance({ m_instanceRects[1].center().x(), m_instanceRects[0].top() });
    QCOMPARE(inst, static_cast<MscInstance *>(nullptr));
}

void tst_ChartLayoutManager::testTimerPositionWithCifInstance()
{
    QString mscText = "mscdocument Untitled_Document /* MSC AND */;\
                      mscdocument Untitled_Leaf /* MSC LEAF */;\
            MSC alarm; \
            /* CIF TextMode 4 */ \
            /* CIF Modified */ \
            /* CIF INSTANCE (261, 65), (349, 150), (800, 696) */ \
            INSTANCE inst_1_cu_nominal.cu_controller : PROCESS /* MSC AT [206] */ ; \
                /* CIF TIMEOUT (436, 303), (95, 110) */ \
                /* CIF TextMode 3 */ \
                /* CIF Modified */ \
                TIMEOUT watchdog /* MSC AT [195] */ ; \
            ENDINSTANCE; \
            ENDMSC;\
            endmscdocument;\
        endmscdocument;";

    CoordinatesConverter::setDPI(QPointF(128., 128.), QPointF(96., 96.));

    parseMsc(mscText);
    QCOMPARE(m_instanceItems.size(), 1);
    QCOMPARE(m_chart->totalEventNumber(), 1);

    MscTimer *watchdogEntity = qobject_cast<MscTimer *>(m_chart->instanceEvents().at(0));
    TimerItem *watchdogItem = m_chartModel->itemForTimer(watchdogEntity);
    QVERIFY(watchdogItem != nullptr);

    // Check that the timer is below the instance head
    const QPointF instanceHeadBottom = m_instanceItems[0]->axis().p1();
    QVERIFY(watchdogItem->scenePos().y() > instanceHeadBottom.y());
}

void tst_ChartLayoutManager::testDefaultChartSize()
{
    QString mscText = "mscdocument Untitled_Document /* MSC AND */;\
                      mscdocument Untitled_Leaf /* MSC LEAF */;\
                          msc Untitled_MSC;\
                              instance Instance_1;\
                              endinstance;\
                          endmsc;\
                      endmscdocument;\
                  endmscdocument;";
    parseMsc(mscText);

    QPointer<ChartItem> chartItem = m_chartModel->chartItem();
    // content area is forced to default minimum size
    QVERIFY(qFuzzyCompare(chartItem->contentRect().width(), defaultSize.width()));
    QVERIFY(qFuzzyCompare(chartItem->contentRect().height(), defaultSize.height()));
}

void tst_ChartLayoutManager::testInstanceCifExtendedChartWidth()
{
    QString mscText = "mscdocument Untitled_Document /* MSC AND */;\
                      mscdocument Untitled_Leaf /* MSC LEAF */;\
                          msc Untitled_MSC;\
                              instance Instance_1;\
                              endinstance;\
                              /* CIF INSTANCE (695, 42) (143, 69) (800, 264) */\
                              instance Instance_2;\
                              endinstance;\
                          endmsc;\
                      endmscdocument;\
                  endmscdocument;";

    CoordinatesConverter::setDPI(QPointF(128., 128.), QPointF(96., 96.));

    parseMsc(mscText);

    QPointer<ChartItem> chartItem = m_chartModel->chartItem();
    // content area width and height are stretched accordingly to the instance1's CIF
    const QRectF &inst2Rect = m_instanceItems[1]->sceneBoundingRect();

    QVERIFY(std::abs(chartItem->contentRect().width() - inst2Rect.right()) <= m_maxOffset);
    QVERIFY(qFuzzyCompare(chartItem->contentRect().height(), inst2Rect.bottom()));
}

void tst_ChartLayoutManager::testAddTwoMessages()
{
    QString mscText = "mscdocument Untitled_Document /* MSC AND */;\
                      mscdocument Untitled_Leaf /* MSC LEAF */;\
                          msc Untitled_MSC;\
                              instance Instance_A;\
                              endinstance;\
                              instance Instance_B;\
                              endinstance;\
                          endmsc;\
                      endmscdocument;\
                  endmscdocument;";
    parseMsc(mscText);

    const QVector<MscInstance *> &instances = m_chart->instances();
    MscInstance *instanceA = instances.at(0);
    MscInstance *instanceB = instances.at(1);

    auto message1 = new MscMessage("Msg1");
    message1->setSourceInstance(instanceA);
    message1->setTargetInstance(instanceB);
    m_chart->addInstanceEvent(message1);
    waitForLayoutUpdate();

    InstanceItem *instanceItemA = m_chartModel->instanceItems().at(0);
    MessageItem *msgItem1 = m_chartModel->itemForMessage(message1);
    InstanceHeadItem *headItem = instanceItemA->headerItem();
    const QRectF headRect = headItem->sceneBoundingRect();
    const QRectF message1Rect = msgItem1->sceneBoundingRect();
    QVERIFY2(headRect.bottom() <= message1Rect.top(), "The message top is not below the instance head");

    auto message2 = new msc::MscMessage("Msg2");
    message2->setSourceInstance(instanceA);
    message2->setTargetInstance(instanceB);
    m_chart->addInstanceEvent(message2);
    waitForLayoutUpdate();

    // Message 1 still at same y position? - check from issue #65
    msgItem1 = m_chartModel->itemForMessage(message1);
    const QRectF headRect2 = headItem->sceneBoundingRect();
    const QRectF message1Rect2 = msgItem1->sceneBoundingRect();
    QVERIFY2(headRect2.bottom() <= message1Rect2.top(), "The message top is not below the instance head");

    const qreal delta = 1.;
    QVERIFY2(std::abs(message1Rect2.top() - message1Rect2.top()) < delta, "The message Y moved");
}

void tst_ChartLayoutManager::testMaxVisibleItems()
{
    m_chartModel->setVisibleItemLimit(2);

    QString mscText = "mscdocument Untitled_Document /* MSC AND */;\
                      mscdocument Untitled_Leaf /* MSC LEAF */;\
                          msc Untitled_MSC;\
                              instance Instance_A;\
                              endinstance;\
                              instance Instance_B;\
                              endinstance;\
                          endmsc;\
                      endmscdocument;\
                  endmscdocument;";
    parseMsc(mscText);

    const QVector<MscInstance *> &instances = m_chart->instances();
    MscInstance *instanceA = instances.at(0);
    MscInstance *instanceB = instances.at(1);

    auto message1 = new MscMessage("Msg1");
    message1->setSourceInstance(instanceA);
    message1->setTargetInstance(instanceB);
    m_chart->addInstanceEvent(message1);
    waitForLayoutUpdate();

    auto message2 = new MscMessage("Msg2");
    message2->setSourceInstance(instanceA);
    message2->setTargetInstance(instanceB);
    m_chart->addInstanceEvent(message2);
    waitForLayoutUpdate();

    MessageItem *msgItem1 = m_chartModel->itemForMessage(message1);
    MessageItem *msgItem2 = m_chartModel->itemForMessage(message2);
    const int msg1Y = msgItem1->sceneBoundingRect().top();
    const int msg2Y = msgItem2->sceneBoundingRect().top();

    // The 3rd messages starts a "scroll"
    auto message3 = new MscMessage("Msg3");
    message3->setSourceInstance(instanceA);
    message3->setTargetInstance(instanceB);
    m_chart->addInstanceEvent(message3);
    waitForLayoutUpdate();

    msgItem1 = m_chartModel->itemForMessage(message1);
    QCOMPARE(msgItem1, nullptr); // as only 2 events are visible, the first was removed

    msgItem2 = m_chartModel->itemForMessage(message2);
    MessageItem *msgItem3 = m_chartModel->itemForMessage(message3);
    const int msg2YScrolled = msgItem2->sceneBoundingRect().top();
    const int msg3YScrolled = msgItem3->sceneBoundingRect().top();
    QCOMPARE(msg2YScrolled, msg1Y); // Now message 2 is at the former position of message 1
    QCOMPARE(msg3YScrolled, msg2Y); // Message 3 is now at position of message 2
}

void tst_ChartLayoutManager::testCreateSetsInstanceY()
{
    QString mscText = "mscdocument Untitled_Document /* MSC AND */;\
                      mscdocument Untitled_Leaf /* MSC LEAF */;\
                          msc Untitled_MSC;\
                              instance Instance_1;\
                              endinstance;\
                              instance Instance_2;\
                              endinstance;\
                          endmsc;\
                      endmscdocument;\
                  endmscdocument;";
    parseMsc(mscText);

    msc::MscChart *chart = m_chartModel->currentChart();

    msc::InstanceItem *instanceItem1 = m_chartModel->instanceItems().at(0);
    msc::MscInstance *instance1 = instanceItem1->modelItem();
    msc::InstanceItem *instanceItem2 = m_chartModel->instanceItems().at(1);
    msc::MscInstance *instance2 = instanceItem2->modelItem();

    const QRectF originalInst2Rect = instanceItem2->sceneBoundingRect();

    // now add a create for the second instance
    msc::MscCreate *create = new msc::MscCreate(chart);
    create->setSourceInstance(instance1);
    create->setTargetInstance(instance2);
    chart->addInstanceEvent(create);

    waitForLayoutUpdate();

    msc::MessageItem *createItem = m_chartModel->itemForMessage(create);

    QRectF newInst2Rect = instanceItem2->sceneBoundingRect();
    QVERIFY(newInst2Rect.y() > originalInst2Rect.y()); // the instance got shifted down
    QCOMPARE(instanceItem2->leftCreatorTarget(), createItem->messagePoints().last());

    // still correct after instance was moved?
    instanceItem2->moveBy(50.0, 0.0);
    waitForLayoutUpdate();
    QCOMPARE(instanceItem2->leftCreatorTarget(), createItem->messagePoints().last());

    // still correct after event insertion?
    const QRectF createRectbefore = createItem->sceneBoundingRect();
    auto action = new msc::MscAction(chart);
    action->setInstance(instance1);
    chart->addInstanceEvent(action, 0); // insert before create
    waitForLayoutUpdate();
    QVERIFY(createItem->sceneBoundingRect().top() > createRectbefore.top()); // Create item got pushed down
    QCOMPARE(instanceItem2->leftCreatorTarget(), createItem->messagePoints().last());
}

void tst_ChartLayoutManager::testCreateSetsYOfStoppedInstance()
{
    QString mscText = "mscdocument Untitled_Document /* MSC AND */;\
                      mscdocument Untitled_Leaf /* MSC LEAF */;\
                          msc Untitled_MSC;\
                              instance Instance_A;\
                                action 'info';\
                                create Instance_B;\
                                in init_b_done from Instance_B;\
                                condition 'upside';\
                                action 'close';\
                              endinstance;\
                              instance Instance_B;\
                                out init_b_done to Instance_A;\
                              stop;\
                          endmsc;\
                      endmscdocument;\
                  endmscdocument;";
    parseMsc(mscText);

    msc::InstanceItem *instanceItemA = m_chartModel->instanceItems().at(0);
    msc::InstanceItem *instanceItemB = m_chartModel->instanceItems().at(1);

    MscAction *action = qobject_cast<MscAction *>(m_chart->instanceEvents().at(0));
    QVERIFY(action);
    MscMessage *create = qobject_cast<MscMessage *>(m_chart->instanceEvents().at(1));
    QVERIFY(create);

    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);
    msc::MessageItem *createItem = m_chartModel->itemForMessage(create);

    // Check that the create message points to the instance head
    QCOMPARE(instanceItemB->leftCreatorTarget(), createItem->messagePoints().last());
    // The created instance should be below the first action event
    QVERIFY(instanceItemB->sceneBoundingRect().top() > actionItem->sceneBoundingRect().top());
    // The stop should be above the unstopped
    QVERIFY(instanceItemB->sceneBoundingRect().bottom() < instanceItemA->sceneBoundingRect().bottom());

    // Stop symbol is below the head symbol
    QRectF headRect = instanceItemB->headerItem()->sceneBoundingRect();
    QRectF endRect = instanceItemB->endItem()->sceneBoundingRect();

    QVERIFY(endRect.top() > headRect.bottom());
}

void tst_ChartLayoutManager::testShiftHorizontalIfNeeded()
{
    QString mscText = "mscdocument Untitled_Document /* MSC AND */;\
                      mscdocument Untitled_Leaf /* MSC LEAF */;\
                          msc Untitled_MSC;\
                              instance Instance_1;\
                              endinstance;\
                          endmsc;\
                      endmscdocument;\
                  endmscdocument;";
    parseMsc(mscText);

    QPointer<ChartItem> chartItem = m_chartModel->chartItem();
    msc::InstanceItem *instanceItem = m_chartModel->instanceItems().at(0);

    // Blank instance item is at the total left
    QCOMPARE(instanceItem->scenePos().x(), 0.0);

    auto action = new msc::MscAction();
    action->setInstance(instanceItem->modelItem());
    action->setName("A");
    m_chart->addInstanceEvent(action);

    waitForLayoutUpdate();

    // event is narrow
    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);
    QVERIFY2(std::abs(instanceItem->scenePos().x()) < 1.0, "Instance is still 'close' to the chart box");
    QVERIFY2(std::abs(instanceItem->scenePos().x()) > -0.5, "Instance is still not negative");
    QVERIFY2(actionItem->scenePos().x() >= 0.0, "Event is not negative");

    // event is wide
    actionItem->setActionText("Action that is a lot wider than before is named here.");
    waitForLayoutUpdate();
    QVERIFY2(std::abs(instanceItem->scenePos().x()) > 5, "event is wide - instance got moved");
    QVERIFY2(actionItem->scenePos().x() >= 0.0, "event is wide - but still not negative");
}

void tst_ChartLayoutManager::testNoHorizontalShiftOnEventAdd()
{
    QString mscText = "mscdocument Untitled_Document /* MSC AND */;\
                      mscdocument Untitled_Leaf /* MSC LEAF */;\
                          msc Untitled_MSC;\
                              instance Instance_1;\
                              endinstance;\
                              instance Instance_2;\
                              endinstance;\
                          endmsc;\
                      endmscdocument;\
                  endmscdocument;";
    parseMsc(mscText);

    waitForLayoutUpdate();

    msc::InstanceItem *instanceItem1 = m_chartModel->instanceItems().at(0);
    msc::InstanceItem *instanceItem2 = m_chartModel->instanceItems().at(1);

    const QRectF originalReometry1 = instanceItem1->sceneBoundingRect();
    const QRectF originalReometry2 = instanceItem2->sceneBoundingRect();

    // add action at first instance
    auto action = new msc::MscAction();
    action->setInstance(instanceItem1->modelItem());
    action->setName("A");
    m_chart->addInstanceEvent(action);
    waitForLayoutUpdate();

    QCOMPARE(originalReometry1, instanceItem1->sceneBoundingRect());
    QCOMPARE(originalReometry2, instanceItem2->sceneBoundingRect());

    // add action at second instance
    auto action2 = new msc::MscAction();
    action2->setInstance(instanceItem2->modelItem());
    action2->setName("B");
    m_chart->addInstanceEvent(action2);
    waitForLayoutUpdate();

    QCOMPARE(originalReometry1.right(), instanceItem1->sceneBoundingRect().right());
    QCOMPARE(originalReometry2.left(), instanceItem2->sceneBoundingRect().left());
    QCOMPARE(originalReometry2.right(), instanceItem2->sceneBoundingRect().right());
}

void tst_ChartLayoutManager::testShiftVerticalIfNeeded()
{
    QString mscText = "mscdocument Untitled_Document /* MSC AND */;\
                      mscdocument Untitled_Leaf /* MSC LEAF */;\
                          msc Untitled_MSC;\
                              instance Instance_1;\
                              endinstance;\
                              instance Instance_2;\
                              endinstance;\
                          endmsc;\
                      endmscdocument;\
                  endmscdocument;";
    parseMsc(mscText);

    QPointer<ChartItem> chartItem = m_chartModel->chartItem();
    msc::InstanceItem *instanceItem1 = m_chartModel->instanceItems().at(0);
    msc::InstanceItem *instanceItem2 = m_chartModel->instanceItems().at(1);

    auto message = new msc::MscMessage("Message1");
    message->setSourceInstance(instanceItem1->modelItem());
    message->setTargetInstance(instanceItem2->modelItem());
    m_chart->addInstanceEvent(message);

    waitForLayoutUpdate();

    msc::MessageItem *messageItem = m_chartModel->itemForMessage(message);

    auto create = new msc::MscCreate("Create");
    create->setSourceInstance(instanceItem1->modelItem());
    create->setTargetInstance(instanceItem2->modelItem());
    m_chart->addInstanceEvent(create);

    waitForLayoutUpdate();

    msc::MessageItem *createItem = m_chartModel->itemForMessage(create);
    const QRectF createRect = createItem->sceneBoundingRect();
    const QRectF messageRect = messageItem->sceneBoundingRect();

    // the message has to be after/below the create
    QVERIFY(createRect.bottom() <= messageRect.top());
}

void tst_ChartLayoutManager::testMessageWithCifInformation()
{
    const QString msc("mscdocument doc1;\
                       msc Untitled_MSC;\
                         /* CIF INSTANCE (0, 56) (189, 79) (800, 508) */\
                         instance Instance_1;\
                           /* CIF MESSAGE (96, 210) (620, 634) */\
                           out Message to env;\
                         endinstance;\
                       endmsc;\
                       endmscdocument;");
    parseMsc(msc);

    auto message = qobject_cast<msc::MscMessage *>(m_chart->instanceEvents().at(0));
    msc::MessageItem *messageItem = m_chartModel->itemForMessage(message);
    const QRectF loadedGeometry = messageItem->sceneBoundingRect();
    // Check that the message is pointing bottom right
    qreal widthHeightRatio = std::abs(loadedGeometry.width() / loadedGeometry.height());
    QVERIFY(widthHeightRatio > 0.7 && widthHeightRatio < 1.3);
}

void tst_ChartLayoutManager::testEventIndex()
{
    const QString msc("/* CIF MSCDOCUMENT (0, 0) (1027, 718) */\
        mscdocument Untitled_Leaf /* MSC LEAF */;\
            msc Untitled_MSC;\
                /* CIF INSTANCE (0, 43) (146, 68) (800, 611) */\
                instance Instance_1;\
                    /* CIF MESSAGE (74, 146) (721, 440) */\
                    out Message to Instance_2;\
                endinstance;\
                /* CIF INSTANCE (647, 43) (146, 68) (800, 611) */\
                instance Instance_2;\
                    /* CIF MESSAGE (74, 146) (721, 440) */\
                    in Message from Instance_1;\
                    action 'Action_1';\
                endinstance;\
            endmsc;\
        endmscdocument;");
    parseMsc(msc);

    auto message = qobject_cast<msc::MscMessage *>(m_chart->instanceEvents().at(0));
    auto action = qobject_cast<msc::MscAction *>(m_chart->instanceEvents().at(1));
    msc::MessageItem *messageItem = m_chartModel->itemForMessage(message);
    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);

    const QPointF messageCenter = messageItem->sceneBoundingRect().center();
    const QPointF actionCenter = actionItem->sceneBoundingRect().center();

    int idx = m_chartModel->eventIndex(QPointF(actionCenter.x(), messageCenter.y()), action);
    QCOMPARE(idx, 0);
}

void tst_ChartLayoutManager::testInstanceEventIndex()
{
    const QString msc("mscdocument Untitled_Leaf;\
            msc Untitled_MSC;\
                instance Instance_1;\
                    action 'Woohooo';\
                    concurrent;\
                    action 'Woohooo';\
                    endconcurrent;\
                endinstance;\
            endmsc;\
        endmscdocument;");
    parseMsc(msc);

    msc::MscInstance *instance = m_chart->instances().at(0);
    auto action1 = qobject_cast<msc::MscAction *>(m_chart->eventsForInstance(instance).at(0));
    auto begin = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance).at(1));
    msc::ActionItem *action1Item = m_chartModel->itemForAction(action1);
    msc::CoregionItem *coregionItem = m_chartModel->itemForCoregion(begin);

    QPointF pt = action1Item->sceneBoundingRect().center();

    pt.setY(action1Item->sceneBoundingRect().bottom() + 2);
    QCOMPARE(m_chartModel->eventInstanceIndex(pt, instance), 1);

    pt.setY(coregionItem->sceneBoundingRect().top() + 2);
    QCOMPARE(m_chartModel->eventInstanceIndex(pt, instance), 2);

    pt.setY(coregionItem->sceneBoundingRect().bottom() - 2);
    QCOMPARE(m_chartModel->eventInstanceIndex(pt, instance), 3);

    pt.setY(coregionItem->sceneBoundingRect().bottom() + 2);
    QCOMPARE(m_chartModel->eventInstanceIndex(pt, instance), 4);
}

QTEST_MAIN(tst_ChartLayoutManager)

#include "tst_chartlayoutmanager.moc"
