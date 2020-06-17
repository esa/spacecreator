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

#include <QGraphicsView>
#include <QUndoStack>
#include <QtTest>

using namespace msc;

namespace mouse {

void syntheticPress(QWidget *widget, const QPoint &point)
{
    QMouseEvent event(QEvent::MouseButtonPress, point, widget->mapToGlobal(point), Qt::LeftButton, Qt::LeftButton,
            Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
    QApplication::processEvents();
}

void syntheticMove(QWidget *widget, const QPoint &point, Qt::MouseButton button)
{
    QMouseEvent event(QEvent::MouseMove, point, button, button, Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
    QApplication::processEvents();
}

void syntheticRelease(QWidget *widget, const QPoint &point)
{
    QMouseEvent event(QEvent::MouseButtonRelease, point, widget->mapToGlobal(point), Qt::LeftButton, Qt::NoButton,
            Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
    QApplication::processEvents();
}

} // ns mouse

class tst_InstanceItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testNameUpdate();
    void testKindUpdate();
    void testMoveByHead();

private:
    MscInstance *m_instance = nullptr;
    InstanceItem *m_instanceItem = nullptr;
};

void tst_InstanceItem::init()
{
    m_instance = new MscInstance("INST1");
    m_instanceItem = new InstanceItem(m_instance);
    m_instanceItem->setName(m_instance->name());
}

void tst_InstanceItem::cleanup()
{
    delete m_instanceItem;
    m_instanceItem = nullptr;
    delete m_instance;
    m_instance = nullptr;
}

void tst_InstanceItem::testNameUpdate()
{
    QCOMPARE(m_instanceItem->name(), QString("INST1"));
    QCOMPARE(m_instanceItem->modelItem()->name(), m_instanceItem->name());

    m_instance->setName("FirstInst");
    QCOMPARE(m_instanceItem->name(), QString("FirstInst"));
    QCOMPARE(m_instanceItem->modelItem()->name(), m_instanceItem->name());
}

void tst_InstanceItem::testKindUpdate()
{
    QCOMPARE(m_instanceItem->kind(), QString(""));

    static const QLatin1String testKind("kindda");
    m_instance->setKind(testKind);
    QCOMPARE(m_instanceItem->kind(), testKind);
    QCOMPARE(m_instanceItem->modelItem()->kind(), m_instanceItem->kind());
}

void tst_InstanceItem::testMoveByHead()
{
    static constexpr bool isLocalBuild = false;
    static constexpr int iterationsCount = 100;
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

        mouse::syntheticMove(view.data()->viewport(), rightHead, Qt::NoButton);

        mouse::syntheticPress(view.data()->viewport(), rightHead);
        mouse::syntheticMove(view.data()->viewport(), destination, Qt::LeftButton);
        mouse::syntheticRelease(view.data()->viewport(), destination);

        // Now instances should be in reversed order
        // and no crash, ofcourse - see https://git.vikingsoftware.com/esa/msceditor/issues/134
        // (crash when moving an instance item by head to the leftmost position).
        QCOMPARE(instanceA, chartModel->currentChart()->instances().last());
        QCOMPARE(instanceB, chartModel->currentChart()->instances().first());
    }
}

QTEST_MAIN(tst_InstanceItem)

#include "tst_instanceitem.moc"
