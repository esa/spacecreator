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

#include "baseitems/arrowitem.h"
#include "chartlayoutmanager.h"
#include "commands/cmdmessageitemcreate.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscchart.h"
#include "msccommandsstack.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "syntheticinteraction.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainterPath>
#include <QPointer>
#include <QtTest>

using namespace msc;

class tsti100messages : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();

    void testPerformance();

private:
    void waitForLayoutUpdate()
    {
        QApplication::processEvents();
        QTest::qWait(2);
        QApplication::processEvents();
    }

    QScopedPointer<msc::MscCommandsStack> m_undoStack;
    QScopedPointer<ChartLayoutManager> m_model;
    QScopedPointer<msc::MscChart> m_chart;
    QPointer<QGraphicsView> m_view;
    QPointer<InstanceItem> m_instanceItem;
    static constexpr int CommandsCount = 100;
    static constexpr bool SkipBenchmark = true; // not a really useful thing to be run on the CI server
    static constexpr bool IsLocalBuild = false; // show the view and process events after each action to see the result

    void moveInstance(const QPoint &from);
};

// make cpp11 happy for ODR-use:
constexpr int tsti100messages::CommandsCount;
constexpr bool tsti100messages::SkipBenchmark;

void tsti100messages::initTestCase()
{
    vstest::saveMousePosition();

    m_undoStack.reset(new msc::MscCommandsStack);
    m_model.reset(new ChartLayoutManager(m_undoStack.data()));
    m_chart.reset(new msc::MscChart());
    m_model->setCurrentChart(m_chart.data());
    m_undoStack->setUndoLimit(CommandsCount);

    m_instanceItem = new InstanceItem(new MscInstance("Instance", this), m_model.data());
    m_instanceItem->setAxisHeight(2 * CommandsCount);
    m_instanceItem->setPos(CommandsCount, 0.);
    m_model->graphicsScene()->addItem(m_instanceItem);
    m_view = new QGraphicsView();
    m_view->setScene(m_model->graphicsScene());

    m_model->graphicsScene()->setSceneRect(0., 0, 2. * CommandsCount, 2. * CommandsCount);

    if (IsLocalBuild)
        m_view->show();
}

void tsti100messages::init()
{
    vstest::saveMousePosition();
}

void tsti100messages::cleanup()
{
    vstest::restoreMousePosition();
}

void tsti100messages::testPerformance()
{
    if (SkipBenchmark)
        QSKIP(qPrintable(QString("This benchmark detects the time spent on:\n\t"
                                 "- Create %1 MessageItems connected to a single InstanceItem;\n\t"
                                 "- Move that InstanceItem by mouse;\n\t"
                                 "- Undo messages creation.\n"
                                 "Could be used to test the general smoothness of processes where "
                                 "the user interaction is involved.\n"
                                 "It's intended for manual testing, so skipped here.")
                                 .arg(CommandsCount)));
    QBENCHMARK {
        const QPointF &instacneCenter = m_instanceItem->boundingRect().translated(m_instanceItem->pos()).center();

        QVariantList params = { QVariant::fromValue<QGraphicsScene *>(m_model->graphicsScene()), QPointF() };

        for (int j = 0; j < CommandsCount / 2; ++j) {
            m_undoStack->push(new msc::cmd::CmdMessageItemCreate(nullptr, -1, m_model.data()));
            m_undoStack->push(new msc::cmd::CmdMessageItemCreate(nullptr, -1, m_model.data()));

            if (IsLocalBuild) {
                waitForLayoutUpdate();
            }
        }

        moveInstance(m_view->mapFromScene(instacneCenter));

        while (m_undoStack->canUndo()) {
            m_undoStack->undo();
            if (IsLocalBuild) {
                waitForLayoutUpdate();
            }
        }
    }
}

void tsti100messages::moveInstance(const QPoint &pntMove)
{
    const QVector<QPoint> path = {
        pntMove, pntMove + QPoint(CommandsCount, 0), // move right
        pntMove + QPoint(CommandsCount, CommandsCount), // then down
        pntMove + QPoint(0, CommandsCount), // then left
        pntMove // and up (back to initial pos)
    };

    vstest::sendMouseMove(m_view->viewport(), QPoint());

    vstest::sendMouseMove(m_view->viewport(), pntMove);
    vstest::sendMousePress(m_view->viewport(), pntMove, Qt::LeftButton, Qt::LeftButton);

    for (const QPoint &pnt : path)
        vstest::sendMouseMove(m_view->viewport(), pnt, Qt::LeftButton, Qt::LeftButton);
}

QTEST_MAIN(tsti100messages)

#include "tsti_100messages.moc"
