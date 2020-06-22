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
#include "baseitems/instanceheaditem.h"
#include "chartitem.h"
#include "chartlayoutmanager.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscaction.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "mscreader.h"
#include "msctimer.h"
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

class tst_ChartLayoutManager : public QObject
{
    Q_OBJECT

private:
    void parseMsc(const QString &mscText);

private Q_SLOTS:
    void init();
    void cleanup();
    void testNearestInstanceSimple();
    void testNearestInstanceCreate();

    void testTimerPositionWithCifInstance();
    void testLoadedMessagePosition();
    void testLoadedCifMessagePosition();
    void testDefaultChartSize();
    void testInstanceCifExtendedChartWidth();
    void testAddTwoMessages();
    void testMaxVisibleItems();

    void testShiftVertialIfNeeded();

private:
    QGraphicsView m_view;

    QScopedPointer<ChartLayoutManager> m_chartModel;
    QScopedPointer<MscModel> m_mscModel;
    QPointer<MscChart> m_chart;
    QVector<MscInstance *> m_instances;
    QVector<InstanceItem *> m_instanceItems;
    QVector<QRectF> m_instanceRects;

    const QSizeF defaultSize = { 200.0, 200.0 };
    const qreal m_maxOffset = 1.5; // used for size comparisons
};

void tst_ChartLayoutManager::parseMsc(const QString &mscText)
{
    MscReader mscReader;
    m_mscModel.reset(mscReader.parseText(mscText));

    if (m_mscModel->charts().isEmpty()) {
        MscDocument *doc = m_mscModel->documents().at(0);
        while (doc->charts().isEmpty()) {
            doc = doc->documents().at(0);
        }
        m_chart = doc->charts().at(0);
    } else {
        m_chart = m_mscModel->charts().at(0);
    }
    m_chartModel->setCurrentChart(m_chart);
    QApplication::processEvents();

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
    m_chartModel.reset(new ChartLayoutManager);
    m_view.setScene(m_chartModel->graphicsScene());
    CoordinatesConverter::instance()->setScene(m_chartModel->graphicsScene());
    static const QPointF dpi1to1(CoordinatesConverter::Dpi1To1, CoordinatesConverter::Dpi1To1);
    CoordinatesConverter::setDPI(dpi1to1, dpi1to1); // results in cif <-> pixel as 1:1
}

void tst_ChartLayoutManager::cleanup()
{
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
    QCOMPARE(m_chart->instanceEvents().size(), 1);

    MscTimer *watchdogEntity = qobject_cast<MscTimer *>(m_chart->instanceEvents().at(0));
    TimerItem *watchdogItem = m_chartModel->itemForTimer(watchdogEntity);
    QVERIFY(watchdogItem != nullptr);

    // Check that the timer is below the instance head
    const QPointF instanceHeadBottom = m_instanceItems[0]->axis().p1();
    QVERIFY(watchdogItem->scenePos().y() > instanceHeadBottom.y());
}

void tst_ChartLayoutManager::testLoadedMessagePosition()
{
    QSKIP("Disabled dew some problems in GUI-less environment (CI)");

    QString mscText = "mscdocument Untitled_Document /* MSC AND */;\
                      mscdocument Untitled_Leaf /* MSC LEAF */;\
                          msc Untitled_MSC;\
                              instance A;\
                                  out AE to env;\
                              endinstance;\
                          endmsc;\
                      endmscdocument;\
                  endmscdocument;";
    parseMsc(mscText);
    QCOMPARE(m_instanceItems.size(), 1);
    QCOMPARE(m_chart->instanceEvents().size(), 1);

    ChartItem *chartItem = m_chartModel->chartItem();
    MscMessage *message = qobject_cast<MscMessage *>(m_chart->instanceEvents().at(0));
    MessageItem *messageItem = m_chartModel->itemForMessage(message);
    QVERIFY(messageItem != nullptr);

    // Check that the message is below the instance head
    const QPointF instanceHeadBottom = m_instanceItems[0]->axis().p1();
    QVERIFY(messageItem->scenePos().y() > instanceHeadBottom.y());
    // Check that the message is above the instance end
    const QPointF instanceEndTop = m_instanceItems[0]->axis().p2();
    QVERIFY(messageItem->sceneBoundingRect().bottom() < instanceEndTop.y());

    // Chart geometry is forced to default minimum (200)
    QVERIFY(qFuzzyCompare(chartItem->contentRect().width(), defaultSize.width()));
    QVERIFY(chartItem->contentRect().height() <= defaultSize.height());
    // message points to the left
    QVector<QPointF> points = messageItem->messagePoints();
    QVERIFY(qFuzzyCompare(points.at(0).x(), m_instanceItems[0]->axis().p1().x()));
    QVERIFY(qFuzzyCompare(points.at(1).x(), chartItem->sceneBoundingRect().left()));
}

void tst_ChartLayoutManager::testLoadedCifMessagePosition()
{
    QSKIP("Force to 200x200 does not work");

    QString mscText = "msc Untitled_MSC;\
                            instance Instance_1;\
                            /* CIF MESSAGE (37, 208) (3002, 681) */\
                                out A to env;\
                            endinstance;\
                        endmsc;";
    parseMsc(mscText);
    QCOMPARE(m_instanceItems.size(), 1);
    QCOMPARE(m_chart->instanceEvents().size(), 1);

    ChartItem *chartItem = m_chartModel->chartItem();
    MscMessage *message = qobject_cast<MscMessage *>(m_chart->instanceEvents().at(0));
    MessageItem *messageItem = m_chartModel->itemForMessage(message);
    QVERIFY(messageItem != nullptr);

    const QRectF msgRect = messageItem->sceneBoundingRect().normalized();
    const QRectF insRect = m_instanceItems[0]->sceneBoundingRect().normalized();
    QVERIFY(qFuzzyCompare(chartItem->contentRect().height(), (msgRect | insRect).height()));
    QVERIFY(qFuzzyCompare(insRect.height(), chartItem->contentRect().height()));

    // message should be non horizontal (from instance to the right edge)
    const QVector<QPointF> &points = messageItem->messagePoints();
    const QPointF &center = insRect.center();

    QVERIFY(std::abs(points.at(0).x() - center.x()) <= m_maxOffset);
    QVERIFY(qFuzzyCompare(points.at(1).x(), chartItem->sceneBoundingRect().right()));
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
    QApplication::processEvents(); // Perform layout update

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
    QApplication::processEvents(); // Perform layout update

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
    QApplication::processEvents(); // Perform layout update

    auto message2 = new MscMessage("Msg2");
    message2->setSourceInstance(instanceA);
    message2->setTargetInstance(instanceB);
    m_chart->addInstanceEvent(message2);
    QApplication::processEvents(); // Perform layout update

    MessageItem *msgItem1 = m_chartModel->itemForMessage(message1);
    MessageItem *msgItem2 = m_chartModel->itemForMessage(message2);
    const int msg1Y = msgItem1->sceneBoundingRect().top();
    const int msg2Y = msgItem2->sceneBoundingRect().top();

    // The 3rd messages starts a "scroll"
    auto message3 = new MscMessage("Msg3");
    message3->setSourceInstance(instanceA);
    message3->setTargetInstance(instanceB);
    m_chart->addInstanceEvent(message3);
    QApplication::processEvents(); // Perform layout update

    msgItem1 = m_chartModel->itemForMessage(message1);
    QCOMPARE(msgItem1, nullptr); // as only 2 events are visible, the first was removed

    msgItem2 = m_chartModel->itemForMessage(message2);
    MessageItem *msgItem3 = m_chartModel->itemForMessage(message3);
    const int msg2YScrolled = msgItem2->sceneBoundingRect().top();
    const int msg3YScrolled = msgItem3->sceneBoundingRect().top();
    QCOMPARE(msg2YScrolled, msg1Y); // Now message 2 is at the former position of message 1
    QCOMPARE(msg3YScrolled, msg2Y); // Message 3 is now at position of message 2
}

void tst_ChartLayoutManager::testShiftVertialIfNeeded()
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

    QApplication::processEvents(); // Perform layout update

    // event is narrow
    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);
    QVERIFY2(std::abs(instanceItem->scenePos().x()) < 1.0, "Instance is still 'close' to the chart box");
    QVERIFY2(std::abs(instanceItem->scenePos().x()) > -0.5, "Instance is still not negative");
    QVERIFY2(actionItem->scenePos().x() >= 0.0, "Event is not negative");

    // event is wide
    actionItem->setActionText("Action that is a lot wider than before is named here.");
    QApplication::processEvents(); // Perform layout update
    QVERIFY2(std::abs(instanceItem->scenePos().x()) > 5, "event is wide - instance got moved");
    QVERIFY2(actionItem->scenePos().x() >= 0.0, "event is wide - but still not negative");
}

QTEST_MAIN(tst_ChartLayoutManager)

#include "tst_chartlayoutmanager.moc"
