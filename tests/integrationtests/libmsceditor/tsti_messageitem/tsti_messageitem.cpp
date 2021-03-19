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
#include "chartviewtestbase.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscaction.h"
#include "sharedlibrary.h"
#include "syntheticinteraction.h"

#include <QtTest>

using namespace msc;

class tsti_MessageItem : public ChartViewTestBase
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

    void testConnectToStoppedInstance();
    void testMoveActionInMessage();
};

void tsti_MessageItem::testConnectToStoppedInstance()
{
    const QString msc("MSCDOCUMENT doc1; \
                         MSC msc1; \
                             INSTANCE A; \
                                OUT Msg1 TO B;\
                                ACTION 'Action1';\
                                OUT Msg2 TO ENV;\
                                ACTION 'Action2';\
                             ENDINSTANCE; \
                             INSTANCE B; \
                                IN Msg1 FROM A;\
                             STOP; \
                         ENDMSC; \
                     ENDMSCDOCUMENT;");
    loadView(msc);

    MscInstance *instanceB = m_chartModel->currentChart()->instances().at(1);
    auto message = qobject_cast<msc::MscMessage *>(m_chart->instanceEvents().at(2));

    InstanceItem *itemB = m_chartModel->itemForInstance(instanceB);
    msc::MessageItem *messageItem = m_chartModel->itemForMessage(message);

    const QRectF rectBOrig = itemB->sceneBoundingRect();

    // Move the message 2 target "below" the stopped instance
    const QPoint startPos = m_view->mapFromScene(((messageItem->head() + messageItem->tail()) / 2).toPoint());
    const QPoint moveFromPos = m_view->mapFromScene(messageItem->head().toPoint());
    const QPoint moveToPos(m_view->mapFromScene(rectBOrig.center()).x(), moveFromPos.y());
    vstest::sendMouseMove(m_view.data()->viewport(), startPos); // To grab the message
    vstest::sendMouseDrag(m_view.data()->viewport(), moveFromPos, moveToPos);

    QCOMPARE(message->targetInstance(), instanceB);
    const QRectF rectB = itemB->sceneBoundingRect();
    QVERIFY(rectB.bottom() > rectBOrig.bottom());
}

void tsti_MessageItem::testMoveActionInMessage()
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
    loadView(msc);

    auto message = qobject_cast<msc::MscMessage *>(m_chart->instanceEvents().at(0));
    auto action = qobject_cast<msc::MscAction *>(m_chart->instanceEvents().at(1));
    msc::MessageItem *messageItem = m_chartModel->itemForMessage(message);
    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);

    QVERIFY(bottomCenter(messageItem).y() < topCenter(actionItem).y());

    const QPoint startPos(center(messageItem).x(), center(actionItem).y());
    const QPoint moveFromPos = center(actionItem);
    const QPoint moveToPos(center(actionItem).x(), center(messageItem).y());
    vstest::sendMouseMove(m_view.data()->viewport(), startPos); // To grab the action
    vstest::sendMouseDrag(m_view.data()->viewport(), moveFromPos, moveToPos);

    QVERIFY(topCenter(messageItem).y() < topCenter(actionItem).y());
    QVERIFY(bottomCenter(messageItem).y() > bottomCenter(actionItem).y());
}

QTEST_MAIN(tsti_MessageItem)

#include "tsti_messageitem.moc"
