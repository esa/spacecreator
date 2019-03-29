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

#include "mscinstance.h"
#include "msctimer.h"

#include <QtTest>

using namespace msc;

class tst_MscTimer : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testConnectStartStop();
    void testConnectStartTimeout();
    void testDenyConnectionFromDifferentInstances();
    void testDenyConnectionWithDifferentNames();
    void testAllowStopStart();
    void testAllowTimeoutStart();
    void testDenyTimeoutStop();
    void testDenyStopStop();
    void testDenyStopTimeout();
    void testDenyTimeoutTimeout();

private:
    MscInstance m_instance1;
    MscInstance m_instance2;
};

void tst_MscTimer::testConnectStartStop()
{
    MscTimer t1("T1", MscTimer::TimerType::Start);
    t1.setInstance(&m_instance1);
    MscTimer t2("T1", MscTimer::TimerType::Stop);
    t2.setInstance(&m_instance1);

    t1.setFollowingTimer(&t2);
    QCOMPARE(t1.followingTimer(), &t2);
    t2.setPrecedingTimer(&t1);
    QCOMPARE(t2.precedingTimer(), &t1);
}

void tst_MscTimer::testConnectStartTimeout()
{
    MscTimer t1("T1", MscTimer::TimerType::Start);
    t1.setInstance(&m_instance1);
    MscTimer t2("T1", MscTimer::TimerType::Timeout);
    t2.setInstance(&m_instance1);

    t1.setFollowingTimer(&t2);
    QCOMPARE(t1.followingTimer(), &t2);
    t2.setPrecedingTimer(&t1);
    QCOMPARE(t2.precedingTimer(), &t1);
}

void tst_MscTimer::testDenyConnectionFromDifferentInstances()
{
    MscTimer t1("T1", MscTimer::TimerType::Start);
    t1.setInstance(&m_instance1);
    MscTimer t2("T1", MscTimer::TimerType::Stop);
    t2.setInstance(&m_instance2);

    t1.setFollowingTimer(&t2);
    QVERIFY(t1.followingTimer() == nullptr);
    t2.setPrecedingTimer(&t1);
    QVERIFY(t2.precedingTimer() == nullptr);
}

void tst_MscTimer::testDenyConnectionWithDifferentNames()
{
    MscTimer t1("T1", MscTimer::TimerType::Start);
    t1.setInstance(&m_instance2);
    MscTimer t2("T2", MscTimer::TimerType::Stop);
    t2.setInstance(&m_instance2);

    t1.setFollowingTimer(&t2);
    QVERIFY(t1.followingTimer() == nullptr);
    t2.setPrecedingTimer(&t1);
    QVERIFY(t2.precedingTimer() == nullptr);
}

void tst_MscTimer::testAllowStopStart()
{
    MscTimer t1("T1", MscTimer::TimerType::Stop);
    t1.setInstance(&m_instance1);
    MscTimer t2("T1", MscTimer::TimerType::Start);
    t2.setInstance(&m_instance1);

    t1.setFollowingTimer(&t2);
    QCOMPARE(t1.followingTimer(), &t2);
    t2.setPrecedingTimer(&t1);
    QCOMPARE(t2.precedingTimer(), &t1);
}

void tst_MscTimer::testAllowTimeoutStart()
{
    MscTimer t1("T1", MscTimer::TimerType::Timeout);
    t1.setInstance(&m_instance1);
    MscTimer t2("T1", MscTimer::TimerType::Start);
    t2.setInstance(&m_instance1);

    t1.setFollowingTimer(&t2);
    QCOMPARE(t1.followingTimer(), &t2);
    t2.setPrecedingTimer(&t1);
    QCOMPARE(t2.precedingTimer(), &t1);
}

void tst_MscTimer::testDenyTimeoutStop()
{
    MscTimer t1("T1", MscTimer::TimerType::Timeout);
    t1.setInstance(&m_instance1);
    MscTimer t2("T1", MscTimer::TimerType::Stop);
    t2.setInstance(&m_instance1);

    t1.setFollowingTimer(&t2);
    QVERIFY(t1.followingTimer() == nullptr);
    t2.setPrecedingTimer(&t1);
    QVERIFY(t2.precedingTimer() == nullptr);
}

void tst_MscTimer::testDenyStopStop()
{
    MscTimer t1("T1", MscTimer::TimerType::Stop);
    t1.setInstance(&m_instance1);
    MscTimer t2("T1", MscTimer::TimerType::Stop);
    t2.setInstance(&m_instance1);

    t1.setFollowingTimer(&t2);
    QVERIFY(t1.followingTimer() == nullptr);
    t2.setPrecedingTimer(&t1);
    QVERIFY(t2.precedingTimer() == nullptr);
}

void tst_MscTimer::testDenyStopTimeout()
{
    MscTimer t1("T1", MscTimer::TimerType::Stop);
    t1.setInstance(&m_instance1);
    MscTimer t2("T1", MscTimer::TimerType::Timeout);
    t2.setInstance(&m_instance1);

    t1.setFollowingTimer(&t2);
    QVERIFY(t1.followingTimer() == nullptr);
    t2.setPrecedingTimer(&t1);
    QVERIFY(t2.precedingTimer() == nullptr);
}

void tst_MscTimer::testDenyTimeoutTimeout()
{
    MscTimer t1("T1", MscTimer::TimerType::Timeout);
    t1.setInstance(&m_instance1);
    MscTimer t2("T1", MscTimer::TimerType::Timeout);
    t2.setInstance(&m_instance1);

    t1.setFollowingTimer(&t2);
    QVERIFY(t1.followingTimer() == nullptr);
    t2.setPrecedingTimer(&t1);
    QVERIFY(t2.precedingTimer() == nullptr);
}

QTEST_APPLESS_MAIN(tst_MscTimer)

#include "tst_msctimer.moc"
