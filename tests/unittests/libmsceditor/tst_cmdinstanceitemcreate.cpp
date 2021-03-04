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
#include "commands/cmdinstanceitemcreate.h"
#include "messageitem.h"
#include "mscchart.h"
#include "msccommandsstack.h"
#include "mscinstance.h"
#include "sharedlibrary.h"

#include <QAction>
#include <QGraphicsScene>
#include <QScopedPointer>
#include <QVariant>
#include <QVariantList>
#include <QVector>
#include <QtTest>

using namespace msc;

class tst_CmdInstanceItemCreate : public QObject
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

    int itemsCount();
};

// make cpp11 happy for ODR-use:
constexpr int tst_CmdInstanceItemCreate::CommandsCount;

void tst_CmdInstanceItemCreate::initTestCase()
{
    shared::initSharedLibrary();
    m_undoStack.reset(new msc::MscCommandsStack);
    m_chart = new msc::MscChart();
    m_chartModel.reset(new msc::ChartLayoutManager(m_undoStack.data()));
    m_chartModel->setCurrentChart(m_chart);
    m_undoStack->setUndoLimit(CommandsCount);
}

void tst_CmdInstanceItemCreate::cleanupTestCase()
{
    delete m_chart;
}

void tst_CmdInstanceItemCreate::testCreate()
{
    m_chartModel->clearScene();
    m_undoStack->clear();

    QCOMPARE(itemsCount(), 0);
    QMetaObject::invokeMethod(m_chartModel.data(), "doLayout", Qt::DirectConnection);
    for (int i = 0; i < CommandsCount; ++i) {
        auto cmd = new msc::cmd::CmdInstanceItemCreate(nullptr, -1, m_chartModel.data());
        m_undoStack->push(cmd);
    }

    QCOMPARE(itemsCount(), CommandsCount);
}

void tst_CmdInstanceItemCreate::testUndo()
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

void tst_CmdInstanceItemCreate::testRedo()
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

int tst_CmdInstanceItemCreate::itemsCount()
{
    return m_chart->instances().size();
}

void tst_CmdInstanceItemCreate::testInsertingOrder()
{
    static const QStringList names = { "A", "B", "C", "D" };

    while (m_undoStack->canUndo()) {
        m_undoStack->undo();
    }
    m_chartModel->clearScene();
    m_undoStack->clear();

    QCOMPARE(itemsCount(), 0);

    QMetaObject::invokeMethod(m_chartModel.data(), "doLayout", Qt::DirectConnection);

    for (const QString &name : names) {
        auto cmd = new msc::cmd::CmdInstanceItemCreate(new msc::MscInstance(name), 0, m_chartModel.data());
        m_undoStack->push(cmd);
    }

    QCOMPARE(m_chart->instances().size(), names.size());

    int i = names.size() - 1;
    for (msc::MscInstance *instance : m_chart->instances())
        QCOMPARE(instance->name(), names.at(i--));
}

QTEST_MAIN(tst_CmdInstanceItemCreate)

#include "tst_cmdinstanceitemcreate.moc"
