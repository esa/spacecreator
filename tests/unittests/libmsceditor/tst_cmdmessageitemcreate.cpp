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
#include "msccommandsstack.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "sharedlibrary.h"

#include <QAction>
#include <QGraphicsScene>
#include <QPointer>
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
    void testInsertingOrder();

private:
    QScopedPointer<ChartLayoutManager> m_chartModel;
    static constexpr int CommandsCount = 10;
    msc::MscChart *m_chart = nullptr;
    QScopedPointer<msc::MscCommandsStack> m_undoStack;
    QPointer<MscInstance> m_instance;

    static const QVariant m_dummyCif;

    int itemsCount();
};

// make cpp11 happy for ODR-use:
constexpr int tst_CmdMessageItemCreate::CommandsCount;
const QVariant tst_CmdMessageItemCreate::m_dummyCif = QVariant::fromValue<QVector<QPoint>>(QVector<QPoint>());

void tst_CmdMessageItemCreate::initTestCase()
{
    shared::initSharedLibrary();
    m_undoStack.reset(new msc::MscCommandsStack);
    m_chart = new msc::MscChart();
    m_chartModel.reset(new msc::ChartLayoutManager(m_undoStack.data()));
    m_chartModel->setCurrentChart(m_chart);
    m_undoStack->setUndoLimit(CommandsCount);

    m_instance = new MscInstance("Tower");
    m_chart->addInstance(m_instance);
}

void tst_CmdMessageItemCreate::cleanupTestCase()
{
    delete m_chart;
}

void tst_CmdMessageItemCreate::testCreate()
{
    for (int i = 0; i < CommandsCount; ++i) {
        auto message = new MscMessage(QString("M%").arg(i), m_instance, nullptr, nullptr);
        auto cmd = new msc::cmd::CmdMessageItemCreate(message, { { m_instance, -1 } }, m_chartModel.data());
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

int tst_CmdMessageItemCreate::itemsCount()
{
    return m_chart->totalEventNumber();
}

void tst_CmdMessageItemCreate::testInsertingOrder()
{
    static const QStringList names = { "A", "B", "C", "D" };

    while (m_undoStack->canUndo())
        m_undoStack->undo();
    m_chartModel->clearScene();
    m_undoStack->clear();

    QCOMPARE(itemsCount(), 0);

    for (const QString &name : names) {
        auto cmd = new msc::cmd::CmdMessageItemCreate(
                new msc::MscMessage(name, m_instance, nullptr, nullptr), { { m_instance, 0 } }, m_chartModel.data());
        m_undoStack->push(cmd);
    }

    QCOMPARE(m_chart->totalEventNumber(), names.size());

    int i = names.size() - 1;
    for (msc::MscInstanceEvent *event : m_chart->instanceEvents())
        QCOMPARE(event->name(), names.at(i--));
}

QTEST_MAIN(tst_CmdMessageItemCreate)

#include "tst_cmdmessageitemcreate.moc"
