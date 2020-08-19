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
#include "coregionitem.h"
#include "mscchart.h"
#include "msccoregion.h"
#include "mscdocument.h"
#include "mscmodel.h"
#include "mscreader.h"
#include "tools/entitydeletetool.h"

#include <QGraphicsView>
#include <QPointer>
#include <QScopedPointer>
#include <QUndoStack>
#include <QtTest>

using namespace msc;

class tst_EntityDeleteTool : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();
    void testDeleteCoregion();

private:
    void waitForLayoutUpdate()
    {
        QApplication::processEvents();
        QTest::qWait(2);
        QApplication::processEvents();
    }

    QScopedPointer<ChartLayoutManager> m_chartModel;
    QScopedPointer<QGraphicsView> m_view;
    QScopedPointer<MscReader> m_reader;
    QScopedPointer<MscModel> m_model;
    QPointer<msc::MscChart> m_chart;
};

void tst_EntityDeleteTool::initTestCase()
{
    cmd::CommandsStack::setCurrent(new QUndoStack(this));
}

void tst_EntityDeleteTool::init()
{
    m_chartModel.reset(new ChartLayoutManager());
    m_view.reset(new QGraphicsView());
    m_view->setScene(m_chartModel->graphicsScene());
    m_reader.reset(new MscReader);
}

void tst_EntityDeleteTool::testDeleteCoregion()
{
    static const QString msc("MSCDOCUMENT doc1; \
                             MSC msc1; \
                                 INSTANCE i1; \
                                     CONCURRENT; \
                                     ENDCONCURRENT; \
                                 ENDINSTANCE; \
                             ENDMSC; \
                         ENDMSCDOCUMENT;");

    m_model.reset(m_reader->parseText(msc));
    m_chart = m_model->documents().first()->charts().first();
    m_chartModel->setCurrentChart(m_chart);

    QCOMPARE(m_chart->instanceEvents().size(), 2); // Coregion has a begin- and end-event.
    auto coregion = qobject_cast<msc::MscCoregion *>(m_chart->instanceEvents().at(0));
    msc::CoregionItem *regionItem = m_chartModel->itemForCoregion(coregion);
    regionItem->setSelected(true);

    msc::EntityDeleteTool delTool(m_chartModel.data(), m_view.data());
    delTool.setCurrentChart(m_chart);
    delTool.action()->trigger(); // do the delete of the selected item
    waitForLayoutUpdate();
    QCOMPARE(m_chart->instanceEvents().size(), 0);
}

QTEST_MAIN(tst_EntityDeleteTool)

#include "tst_entitydeletetool.moc"
