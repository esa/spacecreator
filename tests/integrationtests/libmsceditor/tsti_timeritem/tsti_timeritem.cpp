/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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
#include "mscmessage.h"
#include "msctimer.h"
#include "sharedlibrary.h"
#include "syntheticinteraction.h"
#include "timeritem.h"

#include <QtTest>

using namespace msc;

class tsti_TimerItem : public ChartViewTestBase
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

    void testMoveMessageOutOfStartStopTimer();
    void testMoveMessageInsideOfStartStopTimer();
};

void tsti_TimerItem::testMoveMessageOutOfStartStopTimer()
{
    const QString msc("MSCDOCUMENT doc1; \
                         MSC msc1; \
                             INSTANCE A; \
                                starttimer T1;\
                                out Message to env;\
                                timeout T1;\
                             ENDINSTANCE; \
                         ENDMSC; \
                     ENDMSCDOCUMENT;");
    loadView(msc);

    auto message = qobject_cast<msc::MscMessage *>(m_chart->instanceEvents().at(1));
    msc::MessageItem *messageItem = m_chartModel->itemForMessage(message);

    auto timeout = qobject_cast<msc::MscTimer *>(m_chart->instanceEvents().at(2));
    msc::TimerItem *timeoutItem = m_chartModel->itemForTimer(timeout);

    // Move the message "below" the stop timer
    const QPoint startPos = center(messageItem->gripPointItem(shared::ui::GripPoint::Center));
    const QPoint moveFromPos = startPos;
    const QPoint moveToPos(bottomCenter(timeoutItem) + QPoint(0, 20));
    vstest::sendMouseMove(m_view.data()->viewport(), startPos); // To grab the message
    vstest::sendMouseDrag(m_view.data()->viewport(), moveFromPos, moveToPos);

    QVERIFY(messageItem->tail().y() > timeoutItem->sceneBoundingRect().bottom());
}

void tsti_TimerItem::testMoveMessageInsideOfStartStopTimer()
{
    const QString msc("MSCDOCUMENT doc1; \
                         MSC msc1; \
                             INSTANCE A; \
                                starttimer T1;\
                                timeout T1;\
                                out Message to env;\
                             ENDINSTANCE; \
                         ENDMSC; \
                     ENDMSCDOCUMENT;");
    loadView(msc);

    auto start = qobject_cast<msc::MscTimer *>(m_chart->instanceEvents().at(0));
    msc::TimerItem *startItem = m_chartModel->itemForTimer(start);
    auto timeout = qobject_cast<msc::MscTimer *>(m_chart->instanceEvents().at(1));
    msc::TimerItem *timeoutItem = m_chartModel->itemForTimer(timeout);
    auto message = qobject_cast<msc::MscMessage *>(m_chart->instanceEvents().at(2));
    msc::MessageItem *messageItem = m_chartModel->itemForMessage(message);

    // Move the message "between" the start and stop timer
    const QPoint startPos = center(messageItem->gripPointItem(shared::ui::GripPoint::Center));
    const QPoint moveFromPos = startPos;
    const QPoint moveToPos((topCenter(startItem) + bottomCenter(timeoutItem)) / 2);
    vstest::sendMouseMove(m_view.data()->viewport(), startPos); // To grab the message
    vstest::sendMouseDrag(m_view.data()->viewport(), moveFromPos, moveToPos);

    QVERIFY(messageItem->tail().y() > startItem->sceneBoundingRect().top());
    QVERIFY(messageItem->tail().y() < timeoutItem->sceneBoundingRect().bottom());
}

QTEST_MAIN(tsti_TimerItem)

#include "tsti_timeritem.moc"
