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

#include "chartviewtestbase.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "syntheticinteraction.h"

#include <QtTest>

using namespace msc;

class tsti_MessageItem : public ChartViewTestBase
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase() { initTestCaseBase(); }
    void init() { initBase(); }
    void cleanup() { cleanupBase(); }

    void testConnectToStoppedInstance();
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

QTEST_MAIN(tsti_MessageItem)

#include "tsti_messageitem.moc"
