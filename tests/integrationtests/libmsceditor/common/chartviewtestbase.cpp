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

#include "commands/common/commandsfactory.h"
#include "commands/common/commandsstack.h"
#include "exceptions.h"
#include "mscdocument.h"
#include "syntheticinteraction.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QStandardPaths>
#include <QTest>
#include <QUndoStack>

namespace msc {

void ChartViewTestBase::initTestCaseBase()
{
    QStandardPaths::setTestModeEnabled(true);
    cmd::CommandsStack::setCurrent(new QUndoStack(this));
}

void ChartViewTestBase::initBase()
{
    vstest::saveMousePosition();
    m_chartModel.reset(new ChartLayoutManager());
    cmd::CommandsStack::instance()->factory()->setChartLayoutManager(m_chartModel.get());
    m_view.reset(new QGraphicsView());
    m_view->setScene(m_chartModel->graphicsScene());
    m_reader.reset(new MscReader);
}

void ChartViewTestBase::cleanupBase()
{
    vstest::restoreMousePosition();
    m_model.reset();
}

void ChartViewTestBase::loadView(const QString &mscDoc)
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
        waitForLayoutUpdate();
    }
}

void ChartViewTestBase::waitForLayoutUpdate()
{
    QApplication::processEvents();
    QTest::qWait(2);
    QApplication::processEvents();
}

QPoint ChartViewTestBase::center(const QGraphicsItem *item) const
{
    const QRectF &r = item->sceneBoundingRect();
    return m_view->mapFromScene(r.center());
}

QPoint ChartViewTestBase::topCenter(const QGraphicsItem *item) const
{
    const QRectF &r = item->sceneBoundingRect();
    return m_view->mapFromScene({ r.center().x(), r.top() });
}

QPoint ChartViewTestBase::bottomCenter(const QGraphicsItem *item) const
{
    const QRectF &r = item->sceneBoundingRect();
    return m_view->mapFromScene({ r.center().x(), r.bottom() });
}

}
