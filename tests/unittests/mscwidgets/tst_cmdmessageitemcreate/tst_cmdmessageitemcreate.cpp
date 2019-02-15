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

#include "baseitems/common/utils.h"
#include "chartviewmodel.h"
#include "commands/cmdmessageitemcreate.h"
#include "commands/common/commandsfactory.h"
#include "commands/common/commandsstack.h"
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
    ChartViewModel m_chartModel;
    static constexpr int CommandsCount = 10;
    static constexpr bool SkipBenchmark = true; // not a really usefull thing to be run on the CI server
    msc::MscChart *m_chart = nullptr;

    int itemsCount();
};

// make cpp11 happy for ODR-use:
constexpr int tst_CmdMessageItemCreate::CommandsCount;
constexpr bool tst_CmdMessageItemCreate::SkipBenchmark;

void tst_CmdMessageItemCreate::initTestCase()
{
    m_chart = new msc::MscChart();
    m_chartModel.fillView(m_chart);
    cmd::CommandsStack::setCurrent(new QUndoStack(this));
    cmd::CommandsStack::current()->setUndoLimit(CommandsCount);
}

void tst_CmdMessageItemCreate::cleanupTestCase()
{
    delete m_chart;
}

void tst_CmdMessageItemCreate::testCreate()
{
    m_chartModel.clearScene();
    cmd::CommandsStack::current()->clear();

    QCOMPARE(itemsCount(), 0);

    for (int i = 0; i < CommandsCount; ++i) {
        cmd::CommandsStack::push(
                cmd::Id::CreateMessage,
                { QVariant::fromValue<msc::MscMessage *>(nullptr), QVariant::fromValue<msc::MscChart *>(m_chart), -1 });
    }

    QCOMPARE(itemsCount(), CommandsCount);
}

void tst_CmdMessageItemCreate::testUndo()
{
    QCOMPARE(cmd::CommandsStack::current()->count(), CommandsCount);
    int undone(0);
    while (cmd::CommandsStack::current()->canUndo()) {
        cmd::CommandsStack::current()->undo();
        ++undone;
    }

    QCOMPARE(itemsCount(), 0);
    QCOMPARE(undone, CommandsCount);
}

void tst_CmdMessageItemCreate::testRedo()
{
    QCOMPARE(cmd::CommandsStack::current()->count(), CommandsCount);
    int redone(0);
    while (cmd::CommandsStack::current()->canRedo()) {
        cmd::CommandsStack::current()->redo();
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
    cmd::CommandsStack::current()->clear();

    QCOMPARE(itemsCount(), 0);

    QBENCHMARK {
        m_chartModel.graphicsScene()->setSceneRect(-CommandsCount, -CommandsCount, 2. * CommandsCount,
                                                   2. * CommandsCount);

        // create:
        for (int i = 0; i < CommandsCount; ++i) {
            cmd::CommandsStack::push(cmd::Id::CreateMessage,
                                     { QVariant::fromValue<msc::MscMessage *>(nullptr),
                                       QVariant::fromValue<msc::MscChart *>(m_chart), -1 });
        }

        // undo:
        int undone(0);
        while (cmd::CommandsStack::current()->canUndo()) {
            cmd::CommandsStack::current()->undo();
            ++undone;
        }

        // redo:
        int redone(0);
        while (cmd::CommandsStack::current()->canRedo()) {
            cmd::CommandsStack::current()->redo();
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

    while (cmd::CommandsStack::current()->canUndo())
        cmd::CommandsStack::current()->undo();
    m_chartModel.clearScene();
    cmd::CommandsStack::current()->clear();

    QCOMPARE(itemsCount(), 0);

    for (const QString &name : names) {
        cmd::CommandsStack::push(cmd::Id::CreateMessage,
                                 { QVariant::fromValue<msc::MscMessage *>(new msc::MscMessage(name)),
                                   QVariant::fromValue<msc::MscChart *>(m_chart), 0 }); // prepends message
    }

    QCOMPARE(m_chart->instanceEvents().size(), names.size());

    int i = names.size() - 1;
    for (msc::MscInstanceEvent *event : m_chart->instanceEvents())
        QCOMPARE(event->name(), names.at(i--));
}

QTEST_MAIN(tst_CmdMessageItemCreate)

#include "tst_cmdmessageitemcreate.moc"
