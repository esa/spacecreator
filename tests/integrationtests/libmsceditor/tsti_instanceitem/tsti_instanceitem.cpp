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
#include "sharedlibrary.h"
#include "syntheticinteraction.h"

#include <QtTest>

using namespace msc;

class tsti_InstanceItem : public ChartViewTestBase
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

    void testMoveByHead();
};

void tsti_InstanceItem::testMoveByHead()
{
    static constexpr int iterationsCount = 2;
    static constexpr int headVerticalOffsetPixels = 30;

    static const QString msc("MSCDOCUMENT doc1; \
                             MSC msc1; \
                                 INSTANCE A; \
                                 ENDINSTANCE; \
                                 INSTANCE B; \
                                 ENDINSTANCE; \
                             ENDMSC; \
                         ENDMSCDOCUMENT;");

    loadView(msc);

    auto getHead = [&](InstanceItem *instance) {
        const QRectF &r = instance->sceneBoundingRect();
        return m_view->mapFromScene({ r.center().x(), r.top() + headVerticalOffsetPixels });
    };

    for (int i = 0; i < iterationsCount; ++i) {
        MscInstance *instanceA = m_chartModel->currentChart()->instances().first();
        MscInstance *instanceB = m_chartModel->currentChart()->instances().last();

        InstanceItem *itemA = m_chartModel->itemForInstance(instanceA);
        InstanceItem *itemB = m_chartModel->itemForInstance(instanceB);

        const QPoint &rightHead = getHead(itemB);
        const QPoint &destination = getHead(itemA);

        vstest::sendMouseDrag(m_view.data()->viewport(), rightHead, destination);

        // Now instances should be in reversed order
        // and no crash, ofcourse - see https://git.vikingsoftware.com/esa/msceditor/issues/134
        // (crash when moving an instance item by head to the leftmost position).
        QCOMPARE(instanceA, m_chartModel->currentChart()->instances().last());
        QCOMPARE(instanceB, m_chartModel->currentChart()->instances().first());
    }
}

QTEST_MAIN(tsti_InstanceItem)

#include "tsti_instanceitem.moc"
