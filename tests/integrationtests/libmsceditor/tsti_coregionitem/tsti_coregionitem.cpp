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
#include "commands/common/commandsstack.h"
#include "coregionitem.h"
#include "exceptions.h"
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
#include "mscreader.h"
#include "syntheticinteraction.h"

#include <QGraphicsView>
#include <QPointer>
#include <QScopedPointer>
#include <QUndoStack>
#include <QtTest>

using namespace msc;

class tsti_CoregionItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();
    void testMoveTopUp();
    void testMoveTopDown();
    void testMoveBottomDown();
    void testMoveBottomUp();

private:
    void loadView(const QString &mscDoc);
    QPointF topCenter(InteractiveObject *item) const;
    QPointF bottomCenter(InteractiveObject *item) const;
    bool isInCoregion(const msc::CoregionItem *coregion, MscInstanceEvent *event) const;

    QScopedPointer<ChartLayoutManager> m_chartModel;
    QScopedPointer<QGraphicsView> m_view;
    QScopedPointer<MscReader> m_reader;
    QScopedPointer<MscModel> m_model;
    QPointer<msc::MscChart> m_chart;

    // This could be usefull during local development,
    // but fails the test in CI environment:
    const bool m_isLocalBuild = false;
};

void tsti_CoregionItem::initTestCase()
{
    cmd::CommandsStack::setCurrent(new QUndoStack(this));
}

void tsti_CoregionItem::init()
{
    vstest::saveMousePosition();
    m_chartModel.reset(new ChartLayoutManager());
    m_view.reset(new QGraphicsView());
    m_view->setScene(m_chartModel->graphicsScene());
    m_reader.reset(new MscReader);
}

void tsti_CoregionItem::cleanup()
{
    vstest::restoreMousePosition();
    m_model.reset();
}

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

    auto action = qobject_cast<msc::MscAction *>(m_chart->instanceEvents().at(0));
    auto coregionBegin = qobject_cast<msc::MscCoregion *>(m_chart->instanceEvents().at(1));
    auto coregionEnd = qobject_cast<msc::MscCoregion *>(m_chart->instanceEvents().at(2));

    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);
    msc::CoregionItem *coregionItem = m_chartModel->itemForCoregion(coregionBegin);

    QVERIFY(!isInCoregion(coregionItem, action));

    const QPoint topGrip = topCenter(coregionItem).toPoint();
    const QPoint actionTop = topCenter(actionItem).toPoint() - QPoint(0, 10);

    vstest::sendMouseMove(m_view->viewport(), topGrip + QPoint(0., 15.)); // so the correct grip is pressed
    vstest::sendMouseDrag(m_view->viewport(), topGrip, actionTop);

    QCOMPARE(m_chart->indexofEvent(coregionBegin), 0);
    QCOMPARE(m_chart->indexofEvent(coregionEnd), 2);
    QCOMPARE(m_chart->indexofEvent(action), 1);
    QVERIFY(vstest::isVerticalInside(actionItem, coregionItem));
    QVERIFY(isInCoregion(coregionItem, action));
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

    auto action = qobject_cast<msc::MscAction *>(m_chart->instanceEvents().at(1));
    auto coregionBegin = qobject_cast<msc::MscCoregion *>(m_chart->instanceEvents().at(0));
    auto coregionEnd = qobject_cast<msc::MscCoregion *>(m_chart->instanceEvents().at(2));

    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);
    msc::CoregionItem *coregionItem = m_chartModel->itemForCoregion(coregionBegin);

    QVERIFY(isInCoregion(coregionItem, action));

    const QPoint topGrip = topCenter(coregionItem).toPoint();
    const QPoint actionBottom = bottomCenter(actionItem).toPoint() + QPoint(0, 10);

    vstest::sendMouseMove(m_view->viewport(), topGrip + QPoint(0., 15.)); // so the correct grip is pressed
    vstest::sendMouseDrag(m_view->viewport(), topGrip, actionBottom);

    QCOMPARE(m_chart->indexofEvent(coregionBegin), 1);
    QCOMPARE(m_chart->indexofEvent(coregionEnd), 2);
    QCOMPARE(m_chart->indexofEvent(action), 0);
    QVERIFY(vstest::isAbove(actionItem, coregionItem));
    QVERIFY(!isInCoregion(coregionItem, action));
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

    auto action = qobject_cast<msc::MscAction *>(m_chart->instanceEvents().at(2));
    auto coregionBegin = qobject_cast<msc::MscCoregion *>(m_chart->instanceEvents().at(0));
    auto coregionEnd = qobject_cast<msc::MscCoregion *>(m_chart->instanceEvents().at(1));

    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);
    msc::CoregionItem *coregionItem = m_chartModel->itemForCoregion(coregionBegin);

    QVERIFY(!isInCoregion(coregionItem, action));

    const QPoint bottomGrip = bottomCenter(coregionItem).toPoint();
    const QPoint actionBottom = bottomCenter(actionItem).toPoint() + QPoint(0, 10);

    vstest::sendMouseMove(m_view->viewport(), bottomGrip + QPoint(0., -15.)); // so the correct grip is pressed
    vstest::sendMouseDrag(m_view->viewport(), bottomGrip, actionBottom);

    QCOMPARE(m_chart->indexofEvent(coregionBegin), 0);
    QCOMPARE(m_chart->indexofEvent(coregionEnd), 2);
    QCOMPARE(m_chart->indexofEvent(action), 1);
    QVERIFY(vstest::isVerticalInside(actionItem, coregionItem));
    QVERIFY(isInCoregion(coregionItem, action));
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

    auto action = qobject_cast<msc::MscAction *>(m_chart->instanceEvents().at(1));
    auto coregionBegin = qobject_cast<msc::MscCoregion *>(m_chart->instanceEvents().at(0));
    auto coregionEnd = qobject_cast<msc::MscCoregion *>(m_chart->instanceEvents().at(2));

    msc::ActionItem *actionItem = m_chartModel->itemForAction(action);
    msc::CoregionItem *coregionItem = m_chartModel->itemForCoregion(coregionBegin);

    QVERIFY(isInCoregion(coregionItem, action));

    const QPoint bottomGrip = bottomCenter(coregionItem).toPoint();
    const QPoint actionTop = topCenter(actionItem).toPoint() - QPoint(0, 10);

    vstest::sendMouseMove(m_view->viewport(), bottomGrip + QPoint(0., -15.)); // so the correct grip is pressed
    vstest::sendMouseDrag(m_view->viewport(), bottomGrip, actionTop);

    QCOMPARE(m_chart->indexofEvent(coregionBegin), 0);
    QCOMPARE(m_chart->indexofEvent(coregionEnd), 1);
    QCOMPARE(m_chart->indexofEvent(action), 2);
    QVERIFY(vstest::isBelow(actionItem, coregionItem));
    QVERIFY(!isInCoregion(coregionItem, action));
}

void tsti_CoregionItem::loadView(const QString &mscDoc)
{
    m_model.reset(m_reader->parseText(mscDoc));

    m_chart = m_model->documents().first()->charts().first();
    m_chartModel->setCurrentChart(m_chart);

    // This could be usefull during local development (to see the mouse interaction),
    // but fails the test in CI environment:
    if (m_isLocalBuild) {
        vstest::EventsDelayMs = 300;
        m_view->show();
        bool ok = QTest::qWaitForWindowActive(m_view.data());
        QVERIFY2(ok, "Unable to show the chart view");
    } else {
        QApplication::processEvents();
    }
}

QPointF tsti_CoregionItem::topCenter(InteractiveObject *item) const
{
    const QRectF &r = item->sceneBoundingRect();
    return m_view->mapFromScene({ r.center().x(), r.top() });
}

QPointF tsti_CoregionItem::bottomCenter(InteractiveObject *item) const
{
    const QRectF &r = item->sceneBoundingRect();
    return m_view->mapFromScene({ r.center().x(), r.bottom() });
}

bool tsti_CoregionItem::isInCoregion(const CoregionItem *coregion, MscInstanceEvent *event) const
{
    const int beginIdx = m_chart->indexofEvent(coregion->begin());
    const int endIdx = m_chart->indexofEvent(coregion->end());
    const int eventIdx = m_chart->indexofEvent(event);
    return (beginIdx < eventIdx) && (endIdx > eventIdx) && event->relatesTo(coregion->begin()->instance());
}

QTEST_MAIN(tsti_CoregionItem)

#include "tsti_coregionitem.moc"
