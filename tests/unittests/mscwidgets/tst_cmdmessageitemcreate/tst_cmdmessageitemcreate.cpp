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

#include <commands/common/commandsfactory.h>
#include <commands/cmdmessageitemcreate.h>
#include <commands/common/commandsstack.h>

#include <chartviewmodel.h>
#include <mscmessage.h>
#include <messageitem.h>
#include <mscchart.h>

#include <QGraphicsScene>
#include <QVector>
#include <QVariant>
#include <QVariantList>
#include <QUndoStack>
#include <QAction>

#include <QtTest>

using namespace msc;

class tst_CmdMessageItemCreate : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();

    void testCreate();
    void testUndo();
    void testRedo();
    void testCreateUndoRedo();
    void testPerformance();

private:
    ChartViewModel m_chartModel;
    static constexpr int CommandsCount = 10;
    static constexpr bool SkipBenchmark = true; // not a really usefull thing to be run on the CI server

    int sceneItemsCount();
};

// make cpp11 happy for ODR-use:
constexpr int tst_CmdMessageItemCreate::CommandsCount;
constexpr bool tst_CmdMessageItemCreate::SkipBenchmark;

void tst_CmdMessageItemCreate::initTestCase()
{
    m_chartModel.fillView(new msc::MscChart());
    cmd::CommandsStack::setCurrent(new QUndoStack(this));
    cmd::CommandsStack::current()->setUndoLimit(CommandsCount);
}

void tst_CmdMessageItemCreate::testCreate()
{
    m_chartModel.clearScene();
    cmd::CommandsStack::current()->clear();

    QCOMPARE(sceneItemsCount(), 0);

    for (int j = 0; j < CommandsCount; ++j) {
        cmd::CommandsStack::push(cmd::Id::CreateMessage,
                                 { QVariant::fromValue<QGraphicsScene *>(m_chartModel.graphicsScene()),
                                   QVariant::fromValue<ChartViewModel *>(&m_chartModel),
                                   QPointF(j, 0) });
    }

    QCOMPARE(sceneItemsCount(), CommandsCount);
}

void tst_CmdMessageItemCreate::testUndo()
{
    QCOMPARE(cmd::CommandsStack::current()->count(), CommandsCount);
    int undone(0);
    while (cmd::CommandsStack::current()->canUndo()) {
        cmd::CommandsStack::current()->undo();
        ++undone;
    }

    QCOMPARE(sceneItemsCount(), 0);
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

    QCOMPARE(sceneItemsCount(), CommandsCount);
    QCOMPARE(redone, CommandsCount);
}

void tst_CmdMessageItemCreate::testCreateUndoRedo()
{
    m_chartModel.graphicsScene()->setSceneRect(-CommandsCount, -CommandsCount, 2. * CommandsCount, 2. * CommandsCount);
    testCreate();
    testUndo();
    testRedo();
}

void tst_CmdMessageItemCreate::testPerformance()
{
    if (SkipBenchmark)
        return;
    m_chartModel.clearScene();
    cmd::CommandsStack::current()->clear();

    QCOMPARE(sceneItemsCount(), 0);

    QBENCHMARK {
        m_chartModel.graphicsScene()->setSceneRect(-CommandsCount, -CommandsCount, 2. * CommandsCount, 2. * CommandsCount);

        // create:
        for (int i = 0; i < CommandsCount; ++i) {
            cmd::CommandsStack::push(cmd::Id::CreateMessage,
                                     { QVariant::fromValue<QGraphicsScene *>(m_chartModel.graphicsScene()),
                                       QVariant::fromValue<ChartViewModel *>(&m_chartModel),
                                       QPointF(i + 0, 0),
                                       QPointF(i + 2, 0),
                                       QPointF(i + 1, 0) });
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

int tst_CmdMessageItemCreate::sceneItemsCount()
{
    // one MessageItem = 16 QGraphicItems in the items list
    return m_chartModel.graphicsScene()->items().size() / 16;
}

QTEST_MAIN(tst_CmdMessageItemCreate)

#include "tst_cmdmessageitemcreate.moc"
