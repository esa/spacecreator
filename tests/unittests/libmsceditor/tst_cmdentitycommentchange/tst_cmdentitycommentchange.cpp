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
#include "commands/cmdentitycommentchange.h"
#include "mscchart.h"
#include "msccomment.h"

#include <QScopedPointer>
#include <QUndoStack>
#include <QVariant>
#include <QVariantList>
#include <QtTest>

using namespace msc;

class tst_CmdEntityCommentChange : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();

    void testRedo();
    void testUndo();

    void testSettingEmptyString();

private:
    QScopedPointer<msc::MscChart> m_chart;
    QScopedPointer<msc::ChartLayoutManager> m_layoutManager;
    QScopedPointer<QUndoStack> m_undoStack;
};

void tst_CmdEntityCommentChange::init()
{
    m_chart.reset(new msc::MscChart());
    m_undoStack.reset(new QUndoStack);
    m_layoutManager.reset(new msc::ChartLayoutManager(m_undoStack.data()));
    m_layoutManager->setCurrentChart(m_chart.data());
}

void tst_CmdEntityCommentChange::testRedo()
{
    QCOMPARE(m_chart->commentString(), QString(""));

    // no comment entity exists yet
    auto cmd = new msc::cmd::CmdEntityCommentChange(m_chart.data(), "Test01", m_layoutManager.data());
    m_undoStack->push(cmd);
    QCOMPARE(m_chart->commentString(), QString("Test01"));

    // update comment entity
    cmd = new msc::cmd::CmdEntityCommentChange(m_chart.data(), "Test02", m_layoutManager.data());
    m_undoStack->push(cmd);
    QCOMPARE(m_chart->commentString(), QString("Test02"));

    // Set empty comment (remove comment)
    cmd = new msc::cmd::CmdEntityCommentChange(m_chart.data(), "", m_layoutManager.data());
    m_undoStack->push(cmd);
    QVERIFY(m_chart->comment() == nullptr);
    QCOMPARE(m_chart->commentString(), QString(""));
}

void tst_CmdEntityCommentChange::testUndo()
{
    QCOMPARE(m_chart->commentString(), QString(""));
    QVERIFY(m_chart->comment() == nullptr);

    auto cmd = new msc::cmd::CmdEntityCommentChange(m_chart.data(), "Test01", m_layoutManager.data());
    m_undoStack->push(cmd);
    QVERIFY(m_chart->comment() != nullptr);
    QCOMPARE(m_chart->commentString(), QString("Test01"));

    // Undo to empty string removes the comment
    m_undoStack->undo();
    QVERIFY(m_chart->comment() == nullptr);
    QCOMPARE(m_chart->commentString(), QString(""));

    // Normal undo
    m_chart->setCommentString("TestXY");
    cmd = new msc::cmd::CmdEntityCommentChange(m_chart.data(), "Test02", m_layoutManager.data());
    m_undoStack->push(cmd);
    QVERIFY(m_chart->comment() != nullptr);
    QCOMPARE(m_chart->commentString(), QString("Test02"));

    // Undo to empty string removes the comment
    m_undoStack->undo();
    QVERIFY(m_chart->comment() != nullptr);
    QCOMPARE(m_chart->commentString(), QString("TestXY"));
}

void tst_CmdEntityCommentChange::testSettingEmptyString()
{
    m_chart->setCommentString("Comment A");

    // Applying an empty string is the same as deleting the comment
    auto cmd = new msc::cmd::CmdEntityCommentChange(m_chart.data(), "", m_layoutManager.data());
    m_undoStack->push(cmd);

    QCOMPARE(m_chart->commentString(), QString(""));

    m_undoStack->undo();
    QCOMPARE(m_chart->commentString(), QString("Comment A"));
}

QTEST_MAIN(tst_CmdEntityCommentChange)

#include "tst_cmdentitycommentchange.moc"
