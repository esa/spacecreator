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
#include "commands/common/commandsstack.h"
#include "exceptions.h"
#include "instanceitem.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmodel.h"
#include "mscreader.h"
#include "syntheticinteraction.h"

#include <QGraphicsView>
#include <QUndoStack>
#include <QtTest>

using namespace msc;

class tsti_InstanceItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();

    void testMoveByHead();
};

void tsti_InstanceItem::init()
{
    vstest::saveMousePosition();
}

void tsti_InstanceItem::cleanup()
{
    vstest::restoreMousePosition();
}

void tsti_InstanceItem::testMoveByHead()
{
    static constexpr bool isLocalBuild = false;
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

    QScopedPointer<ChartLayoutManager> chartModel(new ChartLayoutManager());
    QScopedPointer<MscReader> reader(new MscReader);
    QScopedPointer<MscModel> model(reader->parseText(msc));
    QScopedPointer<QGraphicsView> view(new QGraphicsView());
    view->setScene(chartModel->graphicsScene());

    chartModel->setCurrentChart(model->documents().first()->charts().first());

    cmd::CommandsStack::setCurrent(new QUndoStack(this));

    // This could be usefull during local development,
    // but fails the test in CI environment:
    if (isLocalBuild)
        view->show();

    auto getHead = [&](InstanceItem *instance) {
        const QRectF &r = instance->sceneBoundingRect();
        return view->mapFromScene({ r.center().x(), r.top() + headVerticalOffsetPixels });
    };

    for (int i = 0; i < iterationsCount; ++i) {
        MscInstance *instanceA = chartModel->currentChart()->instances().first();
        MscInstance *instanceB = chartModel->currentChart()->instances().last();

        InstanceItem *itemA = chartModel->itemForInstance(instanceA);
        InstanceItem *itemB = chartModel->itemForInstance(instanceB);

        const QPoint &rightHead = getHead(itemB);
        const QPoint &destination = getHead(itemA);

        vstest::sendMouseDrag(view.data()->viewport(), rightHead, destination);

        // Now instances should be in reversed order
        // and no crash, ofcourse - see https://git.vikingsoftware.com/esa/msceditor/issues/134
        // (crash when moving an instance item by head to the leftmost position).
        QCOMPARE(instanceA, chartModel->currentChart()->instances().last());
        QCOMPARE(instanceB, chartModel->currentChart()->instances().first());
    }
}

QTEST_MAIN(tsti_InstanceItem)

#include "tsti_instanceitem.moc"
