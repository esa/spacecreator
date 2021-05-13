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
#include "chartlayoutmanager.h"
#include "chartviewtestbase.h"
#include "coregionitem.h"
#include "graphicsitemcompare.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccoregion.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "sharedlibrary.h"
#include "syntheticinteraction.h"
#include "ui/grippointshandler.h"

#include <QGraphicsView>
#include <QtTest>

using namespace msc;

class tsti_CoregionItem : public ChartViewTestBase
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
        shared::initSharedLibrary();
        initTestCaseBase();
    }
    void init() { initBase(); }
    void cleanup() { cleanupBase(); }

    void testMoveTopUp();
    void testMoveTopDown();
    void testMoveBottomDown();
    void testMoveBottomUp();
    void testMovemessageInside();
    void testMovemessageSourceInside();
    void testMoveMessageCloseToCoregion();
    void testPushedCoregionNotIncludesMessage();

private:
    bool isInCoregion(const msc::CoregionItem *coregion, InteractiveObject *eventItem) const;
};

void tsti_CoregionItem::testMoveTopUp()
{
    static const QString msc("MSCDOCUMENT doc1; \
                             MSC msc1; \
                                 INSTANCE i1; \
                                     ACTION 'Just do it'; \
                                     CONCURRENT; \
                                     ENDCONCURRENT; \
                                 ENDINSTANCE; \
                             ENDMSC; \
                         ENDMSCDOCUMENT;");
    loadView(msc);

    MscInstance *instance = m_chart->instances().at(0);
    auto action = qobject_cast<msc::MscAction *>(m_chart->eventsForInstance(instance).at(0));
    auto coregionBegin = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance).at(1));
    auto coregionEnd = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance).at(2));

    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);
    msc::CoregionItem *coregionItem = m_chartModel->itemForCoregion(coregionBegin);

    QVERIFY(!isInCoregion(coregionItem, actionItem));

    const QPoint topGrip = topCenter(coregionItem);
    const QPoint actionTop = topCenter(actionItem) - QPoint(0, 10);

    vstest::sendMouseMove(m_view->viewport(), topGrip + QPoint(0., 15.)); // so the correct grip is pressed
    vstest::sendMouseDrag(m_view->viewport(), topGrip, actionTop);
    waitForLayoutUpdate();

    QCOMPARE(m_chart->indexofEventAtInstance(coregionBegin, instance), 0);
    QCOMPARE(m_chart->indexofEventAtInstance(coregionEnd, instance), 2);
    QCOMPARE(m_chart->indexofEventAtInstance(action, instance), 1);
    QVERIFY(vstest::isVerticalInside(actionItem, coregionItem));
    QVERIFY(isInCoregion(coregionItem, actionItem));
}

void tsti_CoregionItem::testMoveTopDown()
{
    static const QString msc("MSCDOCUMENT doc1; \
                             MSC msc1; \
                                 INSTANCE i1; \
                                     CONCURRENT; \
                                     ACTION 'Impossible is nothing'; \
                                     ENDCONCURRENT; \
                                 ENDINSTANCE; \
                             ENDMSC; \
                         ENDMSCDOCUMENT;");
    loadView(msc);

    MscInstance *instance = m_chart->instances().at(0);
    auto action = qobject_cast<msc::MscAction *>(m_chart->eventsForInstance(instance).at(1));
    auto coregionBegin = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance).at(0));
    auto coregionEnd = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance).at(2));

    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);
    msc::CoregionItem *coregionItem = m_chartModel->itemForCoregion(coregionBegin);

    QVERIFY(isInCoregion(coregionItem, actionItem));

    const QPoint topGrip = topCenter(coregionItem);
    const QPoint actionBottom = bottomCenter(actionItem) + QPoint(0, 10);

    vstest::sendMouseMove(m_view->viewport(), topGrip + QPoint(0., 15.)); // so the correct grip is pressed
    vstest::sendMouseDrag(m_view->viewport(), topGrip, actionBottom);
    waitForLayoutUpdate();

    QCOMPARE(m_chart->indexofEventAtInstance(coregionBegin, instance), 1);
    QCOMPARE(m_chart->indexofEventAtInstance(coregionEnd, instance), 2);
    QCOMPARE(m_chart->indexofEventAtInstance(action, instance), 0);
    QVERIFY(vstest::isAbove(actionItem, coregionItem));
    QVERIFY(!isInCoregion(coregionItem, actionItem));
}

void tsti_CoregionItem::testMoveBottomDown()
{
    static const QString msc("MSCDOCUMENT doc1; \
                             MSC msc1; \
                                 INSTANCE i1; \
                                     CONCURRENT; \
                                     ENDCONCURRENT; \
                                     ACTION 'Impossible is nothing'; \
                                 ENDINSTANCE; \
                             ENDMSC; \
                         ENDMSCDOCUMENT;");
    loadView(msc);

    MscInstance *instance = m_chart->instances().at(0);
    auto action = qobject_cast<msc::MscAction *>(m_chart->eventsForInstance(instance).at(2));
    auto coregionBegin = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance).at(0));
    auto coregionEnd = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance).at(1));

    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);
    msc::CoregionItem *coregionItem = m_chartModel->itemForCoregion(coregionBegin);

    QVERIFY(!isInCoregion(coregionItem, actionItem));

    const QPoint bottomGrip = bottomCenter(coregionItem);
    const QPoint actionBottom = bottomCenter(actionItem) + QPoint(0, 10);

    vstest::sendMouseMove(m_view->viewport(), bottomGrip + QPoint(0., -15.)); // so the correct grip is pressed
    vstest::sendMouseDrag(m_view->viewport(), bottomGrip, actionBottom);
    waitForLayoutUpdate();

    QCOMPARE(m_chart->indexofEventAtInstance(coregionBegin, instance), 0);
    QCOMPARE(m_chart->indexofEventAtInstance(coregionEnd, instance), 2);
    QCOMPARE(m_chart->indexofEventAtInstance(action, instance), 1);
    QVERIFY(vstest::isVerticalInside(actionItem, coregionItem));
    QVERIFY(isInCoregion(coregionItem, actionItem));
}

void tsti_CoregionItem::testMoveBottomUp()
{
    static const QString msc("MSCDOCUMENT doc1; \
                             MSC msc1; \
                                 INSTANCE i1; \
                                     CONCURRENT; \
                                     ACTION 'Impossible is nothing'; \
                                     ENDCONCURRENT; \
                                 ENDINSTANCE; \
                             ENDMSC; \
                         ENDMSCDOCUMENT;");
    loadView(msc);

    MscInstance *instance = m_chart->instances().at(0);
    auto action = qobject_cast<msc::MscAction *>(m_chart->eventsForInstance(instance).at(1));
    auto coregionBegin = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance).at(0));
    auto coregionEnd = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance).at(2));

    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);
    msc::CoregionItem *coregionItem = m_chartModel->itemForCoregion(coregionBegin);

    QVERIFY(isInCoregion(coregionItem, actionItem));

    const QPoint bottomGrip = bottomCenter(coregionItem);
    const QPoint actionTop = topCenter(actionItem) - QPoint(0, 10);

    vstest::sendMouseMove(m_view->viewport(), bottomGrip + QPoint(0., -15.)); // so the correct grip is pressed
    vstest::sendMouseDrag(m_view->viewport(), bottomGrip, actionTop);
    waitForLayoutUpdate();

    QCOMPARE(m_chart->indexofEventAtInstance(coregionBegin, instance), 0);
    QCOMPARE(m_chart->indexofEventAtInstance(coregionEnd, instance), 1);
    QCOMPARE(m_chart->indexofEventAtInstance(action, instance), 2);
    QVERIFY(vstest::isBelow(actionItem, coregionItem));
    QVERIFY(!isInCoregion(coregionItem, actionItem));
}

void tsti_CoregionItem::testMovemessageInside()
{
    static const QString msc("MSCDOCUMENT doc1; \
                             MSC msc1; \
                                 INSTANCE i1; \
                                     CONCURRENT; \
                                     ENDCONCURRENT; \
                                     OUT Msg1 i2; \
                                 ENDINSTANCE; \
                                 INSTANCE i2; \
                                     IN Msg1 i1; \
                                 ENDINSTANCE; \
                             ENDMSC; \
                         ENDMSCDOCUMENT;");
    loadView(msc);

    MscInstance *instance = m_chart->instances().at(0);
    auto coregionBegin = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance).at(0));
    auto coregionEnd = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance).at(1));
    auto message = qobject_cast<msc::MscMessage *>(m_chart->eventsForInstance(instance).at(2));

    msc::CoregionItem *coregionItem = m_chartModel->itemForCoregion(coregionBegin);
    msc::MessageItem *messageItem = m_chartModel->itemForMessage(message);

    QVERIFY(!isInCoregion(coregionItem, messageItem));

    // move message source
    const QPoint movePos = m_view->mapFromScene(((messageItem->head() + messageItem->tail()) / 2).toPoint());
    const QPoint insideRegion = QPoint(movePos.x(), bottomCenter(coregionItem).y() - 10);

    vstest::sendMouseDrag(m_view->viewport(), movePos, insideRegion);
    waitForLayoutUpdate();

    QCOMPARE(m_chart->indexofEventAtInstance(coregionBegin, instance), 0);
    QCOMPARE(m_chart->indexofEventAtInstance(coregionEnd, instance), 2);
    QCOMPARE(m_chart->indexofEventAtInstance(message, instance), 1);
    QVERIFY(isInCoregion(coregionItem, messageItem));
}

void tsti_CoregionItem::testMovemessageSourceInside()
{
    static const QString msc("MSCDOCUMENT doc1; \
                             MSC msc1; \
                                 INSTANCE i1; \
                                     CONCURRENT; \
                                     ENDCONCURRENT; \
                                     OUT Msg1 i2; \
                                 ENDINSTANCE; \
                                 INSTANCE i2; \
                                     ACTION 'init'; \
                                     IN Msg1 i1; \
                                 ENDINSTANCE; \
                             ENDMSC; \
                         ENDMSCDOCUMENT;");
    loadView(msc);

    MscInstance *instance1 = m_chart->instances().at(0);
    MscInstance *instance2 = m_chart->instances().at(1);
    auto coregionBegin = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance1).at(0));
    auto coregionEnd = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance1).at(1));
    auto action = qobject_cast<msc::MscAction *>(m_chart->eventsForInstance(instance2).at(0));
    auto message = qobject_cast<msc::MscMessage *>(m_chart->eventsForInstance(instance2).at(1));

    msc::CoregionItem *coregionItem = m_chartModel->itemForCoregion(coregionBegin);
    msc::MessageItem *messageItem = m_chartModel->itemForMessage(message);
    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);
    QVERIFY(!isInCoregion(coregionItem, actionItem));
    QVERIFY(!isInCoregion(coregionItem, messageItem));

    // move message source inside coregion
    const QPoint targetPos = m_view->mapFromScene(messageItem->head().toPoint());
    const QPoint sourcePos = m_view->mapFromScene(messageItem->tail().toPoint());
    const QPoint insideRegion = bottomCenter(coregionItem) + QPoint(0, -10);

    vstest::sendMouseMove(m_view->viewport(), targetPos); // so the correct grip is pressed
    vstest::sendMouseDrag(m_view->viewport(), sourcePos, insideRegion);
    waitForLayoutUpdate();

    QVERIFY(m_chart->indexofEventAtInstance(message, instance1)
            > m_chart->indexofEventAtInstance(coregionBegin, instance1));
    QVERIFY(m_chart->indexofEventAtInstance(message, instance1)
            < m_chart->indexofEventAtInstance(coregionEnd, instance1));
    QVERIFY(m_chart->indexofEventAtInstance(message, instance1) > m_chart->indexofEventAtInstance(action, instance1));
    QVERIFY(!isInCoregion(coregionItem, actionItem));
    QVERIFY(isInCoregion(coregionItem, messageItem));
}

void tsti_CoregionItem::testMoveMessageCloseToCoregion()
{
    static const QString msc("MSCDOCUMENT doc1; \
                             MSC msc1; \
                                 INSTANCE i1; \
                                     CONCURRENT; \
                                     ENDCONCURRENT; \
                                     OUT Msg1 i2; \
                                 ENDINSTANCE; \
                                 INSTANCE i2; \
                                     IN Msg1 i1; \
                                 ENDINSTANCE; \
                             ENDMSC; \
                         ENDMSCDOCUMENT;");
    loadView(msc);

    MscInstance *instance = m_chart->instances().at(0);
    auto coregionBegin = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance).at(0));
    auto message = qobject_cast<msc::MscMessage *>(m_chart->eventsForInstance(instance).at(2));

    msc::CoregionItem *coregionItem = m_chartModel->itemForCoregion(coregionBegin);
    msc::MessageItem *messageItem = m_chartModel->itemForMessage(message);

    QVERIFY(!isInCoregion(coregionItem, messageItem));

    // move message close below the coregion
    const QPoint initPos = m_view->mapFromScene(messageItem->head().toPoint());
    const QPoint sourcePos = center(messageItem);
    const QPoint closeBelowRegion(sourcePos.x(), bottomCenter(coregionItem).y() + 5);

    vstest::sendMouseMove(m_view->viewport(), initPos); // so the correct grip is pressed
    vstest::sendMouseDrag(m_view->viewport(), sourcePos, closeBelowRegion);
    waitForLayoutUpdate();

    QCOMPARE(m_chart->indexofEventAtInstance(coregionBegin, instance), 0);
    QCOMPARE(m_chart->indexofEventAtInstance(message, instance), 2);
    QVERIFY(!isInCoregion(coregionItem, messageItem));
}

void tsti_CoregionItem::testPushedCoregionNotIncludesMessage()
{
    static const QString msc("MSCDOCUMENT doc1; \
                             MSC msc1; \
                                 INSTANCE i1; \
                                     CONCURRENT; \
                                     ENDCONCURRENT; \
                                     OUT Msg1 i2; \
                                 ENDINSTANCE; \
                                 INSTANCE i2; \
                                     IN Msg1 i1; \
                                     ACTION 'init'; \
                                 ENDINSTANCE; \
                             ENDMSC; \
                         ENDMSCDOCUMENT;");
    loadView(msc);

    MscInstance *instance1 = m_chart->instances().at(0);
    MscInstance *instance2 = m_chart->instances().at(1);
    auto coregionBegin = qobject_cast<msc::MscCoregion *>(m_chart->eventsForInstance(instance1).at(0));
    auto message = qobject_cast<msc::MscMessage *>(m_chart->eventsForInstance(instance1).at(2));
    auto action = qobject_cast<msc::MscAction *>(m_chart->eventsForInstance(instance2).at(1));

    msc::CoregionItem *coregionItem = m_chartModel->itemForCoregion(coregionBegin);
    msc::MessageItem *messageItem = m_chartModel->itemForMessage(message);
    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);

    QVERIFY(!isInCoregion(coregionItem, messageItem));

    // Move actionto the top to push the coregion
    vstest::sendMouseMove(m_view->viewport(), center(actionItem) - QPoint(30, 0)); // so the action-move grip is pressed
    QPoint targetPos =
            QPoint(center(actionItem).x(), topCenter(coregionItem).y() - 10); // At the top of the 2nd instance
    vstest::sendMouseDrag(m_view->viewport(), center(actionItem), targetPos);
    waitForLayoutUpdate();

    QVERIFY(!isInCoregion(coregionItem, messageItem));
}

bool tsti_CoregionItem::isInCoregion(const CoregionItem *coregion, InteractiveObject *eventItem) const
{
    // Graphical check
    QRectF coregionBox = coregion->sceneBoundingRect();
    auto messageItem = qobject_cast<MessageItem *>(eventItem);
    if (messageItem) {
        if (!coregionBox.contains(messageItem->tail()) && !coregionBox.contains(messageItem->head())) {
            return false;
        }
    } else {
        QRectF eventBox = eventItem->sceneBoundingRect();
        if ((coregionBox.top() > eventBox.top()) || (coregionBox.bottom() < eventBox.bottom())) {
            return false;
        }
    }

    // Check in the model
    auto event = qobject_cast<MscInstanceEvent *>(eventItem->modelEntity());
    if (!event->relatesTo(coregion->begin()->instance())) {
        return false;
    }

    const int beginIdx = m_chart->indexofEventAtInstance(coregion->begin(), coregion->begin()->instance());
    const int endIdx = m_chart->indexofEventAtInstance(coregion->end(), coregion->end()->instance());
    const int eventIdx = m_chart->indexofEventAtInstance(event, coregion->begin()->instance());
    return (beginIdx < eventIdx) && (endIdx > eventIdx) && event->relatesTo(coregion->begin()->instance());
}

QTEST_MAIN(tsti_CoregionItem)

#include "tsti_coregionitem.moc"
