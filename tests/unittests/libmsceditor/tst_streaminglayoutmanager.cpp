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
#include "baseitems/instanceheaditem.h"
#include "chartitem.h"
#include "conditionitem.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccondition.h"
#include "msccreate.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "msctimer.h"
#include "sharedlibrary.h"
#include "streaminglayoutmanager.h"
#include "timeritem.h"

#include <QDebug>
#include <QGraphicsView>
#include <QtTest>
#include <memory>

using namespace msc;

class tst_StreamingLayoutManager : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();

    void testAddInstances();
    void testRemoveInstance();
    void testGrowChartWithInstance();
    void testGrowInstancesWithEvents();
    void testNotGrowStoppedInstancesWithEvents();
    void testAddAction();
    void testRemoveAction();
    void testAddCondition();
    void testAddCreateMesage();
    void testAddMessage();
    void testAddTimer();

private:
    std::unique_ptr<MscModel> m_dataModel;
    std::unique_ptr<MscCommandsStack> m_undoStack;
    std::unique_ptr<StreamingLayoutManager> m_layoutManager;
    QGraphicsView m_view;
    std::unique_ptr<ChartItem> m_chartItem;
    QPointer<MscChart> m_chart;
};

void tst_StreamingLayoutManager::initTestCase()
{
    shared::initSharedLibrary();
}

void tst_StreamingLayoutManager::init()
{
    shared::initSharedLibrary();
    m_dataModel = MscModel::defaultModel();
    m_chart = m_dataModel->firstChart();
    m_undoStack = std::make_unique<MscCommandsStack>();
    m_layoutManager = std::make_unique<StreamingLayoutManager>(m_undoStack.get());
    m_view.setScene(m_layoutManager->graphicsScene());
    m_chartItem = std::make_unique<ChartItem>(m_dataModel->firstChart(), m_layoutManager.get());
    CoordinatesConverter::init(m_layoutManager->graphicsScene(), m_chartItem.get());
    m_layoutManager->setCurrentChart(m_chart);
    auto converter = CoordinatesConverter::instance();
    converter->setDPI(QPointF(100., 100.), QPointF(100., 100.));
}

void tst_StreamingLayoutManager::testAddInstances()
{
    MscInstance *instanceA = m_chart->makeInstance("A");
    InstanceItem *itemA = m_layoutManager->itemForInstance(instanceA);
    QCOMPARE_NE(itemA, nullptr);

    MscInstance *instanceB = m_chart->makeInstance("B");
    InstanceItem *itemB = m_layoutManager->itemForInstance(instanceB);
    QCOMPARE_NE(itemB, nullptr);
    instanceB->setCifGeometry({ { 400, 100 }, { 200, 0 }, { 0, 300 } });
    QCOMPARE_GT(itemB->sceneBoundingRect().left(), itemA->sceneBoundingRect().right());
}

void tst_StreamingLayoutManager::testRemoveInstance()
{
    MscInstance *instanceA = m_chart->makeInstance("A");
    InstanceItem *itemA = m_layoutManager->itemForInstance(instanceA);
    QCOMPARE_NE(itemA, nullptr);

    m_chart->removeInstance(instanceA);
    itemA = m_layoutManager->itemForInstance(instanceA);
    QCOMPARE(itemA, nullptr);
    delete instanceA;
}

void tst_StreamingLayoutManager::testGrowChartWithInstance()
{
    const QRectF originalChartRect = m_chartItem->sceneBoundingRect();

    m_chart->makeInstance("A");
    MscInstance *instanceB = m_chart->makeInstance("B");
    instanceB->setCifGeometry({ { 800, 100 }, { 200, 0 }, { 0, 300 } });

    const QRectF newChartRect = m_chartItem->sceneBoundingRect();
    QCOMPARE_GT(newChartRect.width(), originalChartRect.width());
    InstanceItem *itemB = m_layoutManager->itemForInstance(instanceB);
    QCOMPARE_GT(newChartRect.right(), itemB->extendedSceneBoundingRect().right());
}

void tst_StreamingLayoutManager::testGrowInstancesWithEvents()
{
    MscInstance *instance = m_chart->makeInstance("A");
    InstanceItem *instanceItem = m_layoutManager->itemForInstance(instance);
    const QRectF originalInstanceRect = instanceItem->sceneBoundingRect();
    const QRectF originalChartRect = m_chartItem->sceneBoundingRect();

    // add 100 actions
    for (int i = 0; i < 100; ++i) {
        MscAction *action = new MscAction(QString("Action_%1").arg(i));
        action->setInstance(instance);
        ChartIndexList instanceIndexes { { instance, -1 } };
        m_chart->addInstanceEvent(action, instanceIndexes);
    }

    const QRectF newInstanceRect = instanceItem->sceneBoundingRect();
    QCOMPARE_GT(newInstanceRect.height(), originalInstanceRect.height());
    const QRectF newChartRect = m_chartItem->sceneBoundingRect();
    QCOMPARE_GT(newChartRect.height(), originalChartRect.height());
}

void tst_StreamingLayoutManager::testNotGrowStoppedInstancesWithEvents() { }

void tst_StreamingLayoutManager::testAddAction()
{
    m_chart->makeInstance("A");
    MscInstance *instanceB = m_chart->makeInstance("B");
    instanceB->setCifGeometry({ { 800, 100 }, { 200, 0 }, { 0, 300 } });

    MscAction *action = new MscAction("Call me");
    action->setInstance(instanceB);
    ChartIndexList instanceIndexes { { instanceB, 0 } };
    m_chart->addInstanceEvent(action, instanceIndexes);

    ActionItem *actionItem = m_layoutManager->itemForAction(action);
    QCOMPARE_NE(actionItem, nullptr);

    // check if instance and action are X aligned
    InstanceItem *instanceItem = m_layoutManager->itemForInstance(instanceB);
    qreal diff = instanceItem->sceneBoundingRect().center().x() - actionItem->sceneBoundingRect().center().x();
    QCOMPARE_LT(diff, 1.); // less than one pixel

    // check if action Y is plausible
    QCOMPARE_GT(actionItem->sceneBoundingRect().top(), instanceItem->headerItem()->sceneBoundingRect().bottom());

    // add second action
    MscAction *action2 = new MscAction("Ping");
    action2->setInstance(instanceB);
    instanceIndexes = { { instanceB, 1 } };
    m_chart->addInstanceEvent(action2, instanceIndexes);
    ActionItem *actionItem2 = m_layoutManager->itemForAction(action2);
    QCOMPARE_NE(actionItem2, nullptr);
    // check if second is below first
    QCOMPARE_GT(actionItem2->sceneBoundingRect().top(), actionItem->sceneBoundingRect().bottom());

    // move instance
    // check if instance and action are X aligned
    instanceB->setCifGeometry({ { 1000, 100 }, { 200, 0 }, { 0, 300 } });
    instanceItem = m_layoutManager->itemForInstance(instanceB);
    diff = instanceItem->sceneBoundingRect().center().x() - actionItem->sceneBoundingRect().center().x();
    QCOMPARE_LT(diff, 1.); // less than one pixel
}

void tst_StreamingLayoutManager::testRemoveAction()
{
    msc::MscInstance *instanceA = m_chart->makeInstance("A");
    MscAction *action = new MscAction("Call me");
    action->setInstance(instanceA);
    ChartIndexList instanceIndexes { { instanceA, 0 } };
    m_chart->addInstanceEvent(action, instanceIndexes);
    ActionItem *actionItem = m_layoutManager->itemForAction(action);
    QCOMPARE_NE(actionItem, nullptr);
    const int y = actionItem->y();

    // Remove the action
    m_chart->removeInstanceEvent(action);
    actionItem = m_layoutManager->itemForAction(action);
    QCOMPARE(actionItem, nullptr);

    // re-adding plces it at the same position
    m_chart->addInstanceEvent(action, instanceIndexes);
    actionItem = m_layoutManager->itemForAction(action);
    QCOMPARE_NE(actionItem, nullptr);
    QCOMPARE(actionItem->y(), y);
}

void tst_StreamingLayoutManager::testAddCondition()
{
    MscInstance *instanceA = m_chart->makeInstance("A");
    MscInstance *instanceB = m_chart->makeInstance("B");
    instanceB->setCifGeometry({ { 800, 100 }, { 200, 0 }, { 0, 300 } });

    MscCondition *condition = new MscCondition("If stopped");
    condition->setInstance(instanceB);
    ChartIndexList instanceIndexes { { instanceB, 0 } };
    m_chart->addInstanceEvent(condition, instanceIndexes);

    ConditionItem *conditionItem = m_layoutManager->itemForCondition(condition);
    QCOMPARE_NE(conditionItem, nullptr);

    // check if instance and action are X aligned
    InstanceItem *instanceItem = m_layoutManager->itemForInstance(instanceB);
    qreal diff = instanceItem->sceneBoundingRect().center().x() - conditionItem->sceneBoundingRect().center().x();
    QCOMPARE_LT(diff, 1.); // less than one pixel

    // check if action Y is plausible
    QCOMPARE_GT(conditionItem->sceneBoundingRect().top(), instanceItem->headerItem()->sceneBoundingRect().bottom());

    // move instance
    // check if instance and action are X aligned
    instanceB->setCifGeometry({ { 1000, 100 }, { 200, 0 }, { 0, 300 } });
    instanceItem = m_layoutManager->itemForInstance(instanceB);
    diff = instanceItem->sceneBoundingRect().center().x() - conditionItem->sceneBoundingRect().center().x();
    QCOMPARE_LT(diff, 1.); // less than one pixel

    // add shared condition
    MscCondition *condition2 = new MscCondition("Check");
    condition2->setShared(true);
    //    condition2->setInstance(instanceB);
    instanceIndexes = { { instanceA, 0 }, { instanceB, 1 } };
    m_chart->addInstanceEvent(condition2, instanceIndexes);
    ConditionItem *conditionItem2 = m_layoutManager->itemForCondition(condition2);
    QCOMPARE_NE(conditionItem2, nullptr);
    // check if second is below first
    QCOMPARE_GT(conditionItem2->sceneBoundingRect().top(), conditionItem->sceneBoundingRect().bottom());
}

void tst_StreamingLayoutManager::testAddCreateMesage()
{
    // Add instance with one action
    MscInstance *instanceA = m_chart->makeInstance("A");
    MscAction *action = new MscAction("Call me");
    action->setInstance(instanceA);
    ChartIndexList instanceIndexes { { instanceA, 0 } };
    m_chart->addInstanceEvent(action, instanceIndexes);
    ActionItem *actionItem = m_layoutManager->itemForAction(action);

    // Add instance and create message from insatnceA
    MscInstance *instanceB = m_chart->makeInstance("B");
    instanceB->setCifGeometry({ { 800, 100 }, { 200, 0 }, { 0, 300 } });

    MscCreate *message = new MscCreate("CreateB", instanceA, instanceB);
    ChartIndexList instanceMessageIndexes { { instanceA, 0 }, { instanceB, 0 } };
    m_chart->addInstanceEvent(message, instanceMessageIndexes);

    // Message is below action
    MessageItem *messageItem = m_layoutManager->itemForMessage(message);
    QCOMPARE_NE(messageItem, nullptr);
    QCOMPARE_GT(messageItem->sceneBoundingRect().top(), actionItem->sceneBoundingRect().bottom());

    // Instance B is below instance A
    InstanceItem *instanceItemA = m_layoutManager->itemForInstance(instanceA);
    InstanceItem *instanceItemB = m_layoutManager->itemForInstance(instanceB);
    QCOMPARE_GT(instanceItemB->sceneBoundingRect().top(), instanceItemA->sceneBoundingRect().top());

    // Both instance have end level/height
    QCOMPARE_LT(instanceItemA->sceneBoundingRect().bottom() - instanceItemB->sceneBoundingRect().bottom(),
            1.); // less than one pixel

    // Add action to B - has to be placed below instance header
    MscAction *actionB = new MscAction("Go west");
    actionB->setInstance(instanceB);
    instanceIndexes = { { instanceB, 0 } };
    m_chart->addInstanceEvent(actionB, instanceIndexes);
    ActionItem *actionItemB = m_layoutManager->itemForAction(actionB);
    QCOMPARE_GT(actionItemB->sceneBoundingRect().top(), instanceItemB->headerItem()->sceneBoundingRect().bottom());
}

void tst_StreamingLayoutManager::testAddMessage()
{
    MscInstance *instanceA = m_chart->makeInstance("A");
    MscInstance *instanceB = m_chart->makeInstance("B");
    instanceB->setCifGeometry({ { 800, 100 }, { 200, 0 }, { 0, 300 } });

    MscMessage *message = new MscMessage("setAngle");
    message->setSourceInstance(instanceA);
    message->setTargetInstance(instanceB);
    ChartIndexList instanceIndexes { { instanceA, 0 }, { instanceB, 0 } };
    m_chart->addInstanceEvent(message, instanceIndexes);

    MessageItem *messageItem = m_layoutManager->itemForMessage(message);
    QCOMPARE_NE(messageItem, nullptr);

    // check if instance and action are X aligned
    InstanceItem *instanceItemA = m_layoutManager->itemForInstance(instanceA);
    qreal diff = instanceItemA->sceneBoundingRect().center().x() - messageItem->tail().x();
    qDebug() << instanceItemA->sceneBoundingRect().center() << messageItem->tail();
    QCOMPARE_LT(diff, 1.); // less than one pixel
    InstanceItem *instanceItemB = m_layoutManager->itemForInstance(instanceB);
    diff = instanceItemB->sceneBoundingRect().center().x() - messageItem->head().x();
    QCOMPARE_LT(diff, 1.); // less than one pixel

    // check if action Y is plausible
    QCOMPARE_GT(messageItem->sceneBoundingRect().top() + (m_layoutManager->interMessageSpan() * 0.5),
            instanceItemB->headerItem()->sceneBoundingRect().bottom());

    // move instance
    // check if instance and action are X aligned
    instanceB->setCifGeometry({ { 1000, 100 }, { 200, 0 }, { 0, 300 } });
    instanceItemA = m_layoutManager->itemForInstance(instanceA);
    diff = instanceItemA->sceneBoundingRect().center().x() - messageItem->tail().x();
    QCOMPARE_LT(diff, 1.); // less than one pixel
    instanceItemB = m_layoutManager->itemForInstance(instanceB);
    diff = instanceItemB->sceneBoundingRect().center().x() - messageItem->head().x();
    QCOMPARE_LT(diff, 1.); // less than one pixel
}

void tst_StreamingLayoutManager::testAddTimer()
{
    MscInstance *instanceA = m_chart->makeInstance("A");

    MscTimer *timer = new MscTimer("Start", MscTimer::TimerType::Start);
    timer->setInstance(instanceA);
    ChartIndexList instanceIndexes { { instanceA, 0 } };
    m_chart->addInstanceEvent(timer, instanceIndexes);

    TimerItem *timerItem = m_layoutManager->itemForTimer(timer);
    QCOMPARE_NE(timerItem, nullptr);

    // check if instance and action are X aligned
    InstanceItem *instanceItem = m_layoutManager->itemForInstance(instanceA);
    qreal diff = instanceItem->sceneBoundingRect().center().x() - timerItem->sceneBoundingRect().center().x();
    QCOMPARE_LT(diff, 1.); // less than one pixel

    // check if action Y is plausible
    QCOMPARE_GT(timerItem->sceneBoundingRect().top(), instanceItem->headerItem()->sceneBoundingRect().bottom());
}

QTEST_MAIN(tst_StreamingLayoutManager)

#include "tst_streaminglayoutmanager.moc"
