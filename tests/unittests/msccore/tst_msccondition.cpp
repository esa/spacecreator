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

#include "mscchart.h"
#include "msccondition.h"
#include "mscinstance.h"

#include <QtTest>

using namespace msc;

class tst_MscCondition : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSharedConditionNewInstance();

private:
};

void tst_MscCondition::testSharedConditionNewInstance()
{
    MscChart chart;
    auto instance1 = new MscInstance("InstA");
    auto instance2 = new MscInstance("InstB");
    chart.addInstance(instance1);
    chart.addInstance(instance2);
    auto condition1 = new MscCondition("Cond_A");
    condition1->setShared(true);
    condition1->setInstance(instance1);
    chart.addInstanceEvent(condition1, { { instance1, 0 }, { instance2, 0 } });

    QVERIFY(instance1->events().contains(condition1));
    QVERIFY(instance2->events().contains(condition1));

    auto instance3 = new MscInstance("InstC");
    chart.addInstance(instance3);
    QVERIFY(instance3->events().contains(condition1));
}

QTEST_APPLESS_MAIN(tst_MscCondition)

#include "tst_msccondition.moc"
