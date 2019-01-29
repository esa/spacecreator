/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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
#include "chartviewmodel.h"
#include "commands/cmdmessageitemcreate.h"
#include "commands/common/commandsfactory.h"
#include "commands/common/commandsstack.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscchart.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "tst_common.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainterPath>
#include <QPointer>
#include <QtTest>

using namespace msc;

class tst100messages : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();

    void testPerformance();

private:
    ChartViewModel m_model;
    QPointer<QGraphicsView> m_view;
    QPointer<InstanceItem> m_instanceItem;
    static constexpr int CommandsCount = 100;
    static constexpr bool SkipBenchmark = true; // not a really usefull thing to be run on the CI server
    static constexpr bool IsLocalBuild = false; // show the view and process events after each action to see the result

    void moveInstance(const QPoint &from);
};

// make cpp11 happy for ODR-use:
constexpr int tst100messages::CommandsCount;
constexpr bool tst100messages::SkipBenchmark;

void tst100messages::initTestCase()
{
    QScopedPointer<msc::MscChart> chart(new msc::MscChart());
    m_model.fillView(chart.data());
    cmd::CommandsStack::setCurrent(new QUndoStack(this));
    cmd::CommandsStack::current()->setUndoLimit(CommandsCount);

    m_instanceItem = new InstanceItem(new MscInstance("Instance", this));
    m_instanceItem->setAxisHeight(2 * CommandsCount);
    m_instanceItem->setPos(CommandsCount, 0.);
    m_model.graphicsScene()->addItem(m_instanceItem);
    m_view = new QGraphicsView();
    m_view->setScene(m_model.graphicsScene());

    m_model.graphicsScene()->setSceneRect(0., 0, 2. * CommandsCount, 2. * CommandsCount);

    if (IsLocalBuild)
        m_view->show();
}

void tst100messages::testPerformance()
{
    if (SkipBenchmark)
        QSKIP(qPrintable(QString("This benchmark detects the time spent on:\n\t"
                                 "- Create %1 MessageItems connected to a single InstanceItem;\n\t"
                                 "- Move that InstanceItem by mouse;\n\t"
                                 "- Undo messages creation.\n"
                                 "Coudl be used to test the general smoothness of processes where "
                                 "the user interaction is involved.\n"
                                 "It's intended for manual testing, so skipped here.")
                                 .arg(CommandsCount)));
    QBENCHMARK {
        const QPointF &instacneCenter = m_instanceItem->boundingRect().translated(m_instanceItem->pos()).center();
        const QPointF &messagePos = { instacneCenter.x() - ArrowItem::DEFAULT_WIDTH / 2, instacneCenter.y() };

        QVariantList params = { QVariant::fromValue<QGraphicsScene *>(m_model.graphicsScene()),
                                QVariant::fromValue<ChartViewModel *>(&m_model), QPointF() };

        for (int j = 0; j < CommandsCount / 2; ++j) {

            params.replace(2, QPointF(messagePos.x(), messagePos.y() + j));
            cmd::CommandsStack::push(cmd::Id::CreateMessage, params);
            params.replace(2, QPointF(messagePos.x(), messagePos.y() - j));
            cmd::CommandsStack::push(cmd::Id::CreateMessage, params);
            if (IsLocalBuild)
                QApplication::processEvents();
        }

        moveInstance(m_view->mapFromScene(instacneCenter));

        while (cmd::CommandsStack::current()->canUndo()) {
            cmd::CommandsStack::current()->undo();
            if (IsLocalBuild)
                QApplication::processEvents();
        }
    }
}

void tst100messages::moveInstance(const QPoint &pntMove)
{
    const QVector<QPoint> path = {
        pntMove, pntMove + QPoint(CommandsCount, 0), // move right
        pntMove + QPoint(CommandsCount, CommandsCount), // then down
        pntMove + QPoint(0, CommandsCount), // then left
        pntMove // and up (back to initial pos)
    };

    test::ui::sendMouseMove(m_view->viewport(), QPoint());

    test::ui::sendMouseMove(m_view->viewport(), pntMove);
    test::ui::sendMousePress(m_view->viewport(), pntMove, Qt::LeftButton, Qt::LeftButton);

    for (const QPoint &pnt : path)
        test::ui::sendMouseMove(m_view->viewport(), pnt, Qt::LeftButton, Qt::LeftButton);
}

QTEST_MAIN(tst100messages)

#include "tst_100messages.moc"
