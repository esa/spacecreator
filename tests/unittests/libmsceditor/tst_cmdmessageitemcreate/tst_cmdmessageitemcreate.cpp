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

#include "baseitems/common/mscutils.h"
#include "chartlayoutmanager.h"
#include "commands/cmdmessageitemcreate.h"
#include "messageitem.h"
#include "mscchart.h"
#include "mscmessage.h"

#include <QAction>
#include <QGraphicsScene>
#include <QUndoStack>
#include <QVariant>
#include <QVariantList>
#include <QVector>
#include <QtTest>

using namespace msc;

class tst_CmdMessageItemCreate : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testCreate();
    void testUndo();
    void testRedo();
    void testPerformance();
    void testInsertingOrder();

private:
    ChartLayoutManager m_chartModel;
    static constexpr int CommandsCount = 10;
    static constexpr bool SkipBenchmark = true; // not a really usefull thing to be run on the CI server
    msc::MscChart *m_chart = nullptr;
    QScopedPointer<QUndoStack> m_undoStack;

    static const QVariant m_dummyCif;

    int itemsCount();
};

// make cpp11 happy for ODR-use:
constexpr int tst_CmdMessageItemCreate::CommandsCount;
constexpr bool tst_CmdMessageItemCreate::SkipBenchmark;
const QVariant tst_CmdMessageItemCreate::m_dummyCif = QVariant::fromValue<QVector<QPoint>>(QVector<QPoint>());

void tst_CmdMessageItemCreate::initTestCase()
{
    m_chart = new msc::MscChart();
    m_chartModel.setCurrentChart(m_chart);
    m_undoStack.reset(new QUndoStack);
    m_undoStack->setUndoLimit(CommandsCount);
}

void tst_CmdMessageItemCreate::cleanupTestCase()
{
    delete m_chart;
}

void tst_CmdMessageItemCreate::testCreate()
{
    m_chartModel.clearScene();
    m_undoStack->clear();

    QCOMPARE(itemsCount(), 0);

    for (int i = 0; i < CommandsCount; ++i) {
        auto cmd = new msc::cmd::CmdMessageItemCreate(nullptr, -1, m_chart, &m_chartModel);
        m_undoStack->push(cmd);
    }

    QCOMPARE(itemsCount(), CommandsCount);
}

void tst_CmdMessageItemCreate::testUndo()
{
    QCOMPARE(m_undoStack->count(), CommandsCount);
    int undone(0);
    while (m_undoStack->canUndo()) {
        m_undoStack->undo();
        ++undone;
    }

    QCOMPARE(itemsCount(), 0);
    QCOMPARE(undone, CommandsCount);
}

void tst_CmdMessageItemCreate::testRedo()
{
    QCOMPARE(m_undoStack->count(), CommandsCount);
    int redone(0);
    while (m_undoStack->canRedo()) {
        m_undoStack->redo();
        ++redone;
    }

    QCOMPARE(itemsCount(), CommandsCount);
    QCOMPARE(redone, CommandsCount);
}

void tst_CmdMessageItemCreate::testPerformance()
{
    if (SkipBenchmark)
        QSKIP(qPrintable(QString("This benchmark detects the time spent to perform %1 "
                                 "CreateMessage commands (create, undo, redo).\n"
                                 "It's intended for manual testing, so skipped here.")
                                 .arg(CommandsCount)));

    m_chartModel.clearScene();
    m_undoStack->clear();

    QCOMPARE(itemsCount(), 0);

    QBENCHMARK {
        m_chartModel.graphicsScene()->setSceneRect(
                -CommandsCount, -CommandsCount, 2. * CommandsCount, 2. * CommandsCount);

        // create:
        for (int i = 0; i < CommandsCount; ++i) {
            auto cmd = new msc::cmd::CmdMessageItemCreate(nullptr, -1, m_chart, &m_chartModel);
            m_undoStack->push(cmd);
        }

        // undo:
        int undone(0);
        while (m_undoStack->canUndo()) {
            m_undoStack->undo();
            ++undone;
        }

        // redo:
        int redone(0);
        while (m_undoStack->canRedo()) {
            m_undoStack->redo();
            ++redone;
        }
    }
}

int tst_CmdMessageItemCreate::itemsCount()
{
    return m_chart->instanceEvents().size();
}

void tst_CmdMessageItemCreate::testInsertingOrder()
{
    static const QStringList names = { "A", "B", "C", "D" };

    while (m_undoStack->canUndo())
        m_undoStack->undo();
    m_chartModel.clearScene();
    m_undoStack->clear();

    QCOMPARE(itemsCount(), 0);

    for (const QString &name : names) {
        auto cmd = new msc::cmd::CmdMessageItemCreate(new msc::MscMessage(name), 0, m_chart, &m_chartModel);
        m_undoStack->push(cmd);
    }

    QCOMPARE(m_chart->instanceEvents().size(), names.size());

    int i = names.size() - 1;
    for (msc::MscInstanceEvent *event : m_chart->instanceEvents())
        QCOMPARE(event->name(), names.at(i--));
}

QTEST_MAIN(tst_CmdMessageItemCreate)

#include "tst_cmdmessageitemcreate.moc"
