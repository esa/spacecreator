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

#include "chartlayoutmanager.h"
#include "exceptions.h"
#include "mscinstance.h"
#include "msctimer.h"
#include "timeritem.h"

#include <QScopedPointer>
#include <QUndoStack>
#include <QtTest>

using namespace msc;

class tst_TimerItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testBlockDifferentInsatncesConnection();
    void testAllowStartStartTimerConnection();
    void testBlockTimeoutTimeoutConnection();
    void testDenyStopStopTimerConnection();
    void testAllowStartBeforeStopConnection();
    void testBlockStartAfterStopConnection();
    void testBlockTimeoutBeforeStopConnection();

private:
    MscTimer *m_timer = nullptr;
    TimerItem *m_timerItem = nullptr;
    MscInstance *m_instance = nullptr;
    ChartLayoutManager *m_model = nullptr;
    QScopedPointer<QUndoStack> m_undoStack;
};

void tst_TimerItem::init()
{
    m_undoStack.reset(new QUndoStack);
    m_model = new ChartLayoutManager(m_undoStack.data());
    m_instance = new MscInstance;

    m_timer = new MscTimer();
    m_timer->setInstance(m_instance);
    m_timer->setName("T1");
    m_timer->setTimerType(msc::MscTimer::TimerType::Start);
    m_timerItem = new TimerItem(m_timer, m_model, nullptr);
}

void tst_TimerItem::cleanup()
{
    delete m_timerItem;
    m_timerItem = nullptr;

    delete m_timer;
    m_timer = nullptr;

    delete m_instance;
    m_instance = nullptr;

    delete m_model;
    m_model = nullptr;
}

void tst_TimerItem::testBlockDifferentInsatncesConnection()
{
    MscInstance inst;

    MscTimer t2;
    t2.setTimerType(msc::MscTimer::TimerType::Stop);
    t2.setInstance(&inst);

    QCOMPARE(m_timerItem->canConnectTimers(&t2, QPointF(8, 8)), false);
}

void tst_TimerItem::testAllowStartStartTimerConnection()
{
    m_timer->setTimerType(msc::MscTimer::TimerType::Start);
    MscTimer t2;
    t2.setTimerType(msc::MscTimer::TimerType::Start);
    t2.setInstance(m_instance);

    QCOMPARE(m_timerItem->canConnectTimers(&t2, QPointF(8, 8)), true);
}

void tst_TimerItem::testBlockTimeoutTimeoutConnection()
{
    m_timer->setTimerType(msc::MscTimer::TimerType::Timeout);
    MscTimer t2;
    t2.setTimerType(msc::MscTimer::TimerType::Timeout);
    t2.setInstance(m_instance);

    QCOMPARE(m_timerItem->canConnectTimers(&t2, QPointF(8, 8)), false);
}

void tst_TimerItem::testDenyStopStopTimerConnection()
{
    m_timer->setTimerType(msc::MscTimer::TimerType::Stop);
    MscTimer t2;
    t2.setTimerType(msc::MscTimer::TimerType::Stop);
    t2.setInstance(m_instance);

    QCOMPARE(m_timerItem->canConnectTimers(&t2, QPointF(8, 8)), false);
}

void tst_TimerItem::testAllowStartBeforeStopConnection()
{
    m_timer->setTimerType(msc::MscTimer::TimerType::Start);
    MscTimer t2;
    t2.setName(m_timer->name());
    t2.setTimerType(msc::MscTimer::TimerType::Stop);
    t2.setInstance(m_instance);

    QCOMPARE(m_timerItem->canConnectTimers(&t2, QPointF(8, 8)), true);
}

void tst_TimerItem::testBlockStartAfterStopConnection()
{
    m_timer->setTimerType(msc::MscTimer::TimerType::Start);
    MscTimer t2;
    t2.setTimerType(msc::MscTimer::TimerType::Stop);
    t2.setInstance(m_instance);

    QCOMPARE(m_timerItem->canConnectTimers(&t2, QPointF(8, -8)), false);
}

void tst_TimerItem::testBlockTimeoutBeforeStopConnection()
{
    m_timer->setTimerType(msc::MscTimer::TimerType::Timeout);
    MscTimer t2;
    t2.setTimerType(msc::MscTimer::TimerType::Stop);
    t2.setInstance(m_instance);

    QCOMPARE(m_timerItem->canConnectTimers(&t2, QPointF(8, 8)), false);
}

QTEST_MAIN(tst_TimerItem)

#include "tst_timeritem.moc"
