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

#include "commands/cmdentitycommentchange.h"
#include "commands/common/commandsfactory.h"
#include "commands/common/commandsstack.h"
#include "mscchart.h"
#include "msccomment.h"

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
    void cleanup();

    void testRedo();
    void testUndo();

private:
    msc::MscChart *m_chart = nullptr;
};

void tst_CmdEntityCommentChange::init()
{
    m_chart = new msc::MscChart();
    cmd::CommandsStack::setCurrent(new QUndoStack(this));
}

void tst_CmdEntityCommentChange::cleanup()
{
    delete m_chart;
    m_chart = nullptr;
}

void tst_CmdEntityCommentChange::testRedo()
{
    QCOMPARE(m_chart->commentString(), QString(""));

    // no comment entity exists yet
    msc::cmd::CommandsStack::push(msc::cmd::Id::ChangeComment,
            { QVariant::fromValue<msc::MscChart *>(m_chart), QVariant::fromValue<msc::MscEntity *>(m_chart),
                    "Test01" });
    QCOMPARE(m_chart->commentString(), QString("Test01"));

    // update comment entity
    msc::cmd::CommandsStack::push(msc::cmd::Id::ChangeComment,
            { QVariant::fromValue<msc::MscChart *>(m_chart), QVariant::fromValue<msc::MscEntity *>(m_chart),
                    "Test02" });
    QCOMPARE(m_chart->commentString(), QString("Test02"));

    // Set empty comment (remove comment)
    msc::cmd::CommandsStack::push(msc::cmd::Id::ChangeComment,
            { QVariant::fromValue<msc::MscChart *>(m_chart), QVariant::fromValue<msc::MscEntity *>(m_chart), "" });
    QVERIFY(m_chart->comment() == nullptr);
    QCOMPARE(m_chart->commentString(), QString(""));
}

void tst_CmdEntityCommentChange::testUndo()
{
    QCOMPARE(m_chart->commentString(), QString(""));
    QVERIFY(m_chart->comment() == nullptr);

    msc::cmd::CommandsStack::push(msc::cmd::Id::ChangeComment,
            { QVariant::fromValue<msc::MscChart *>(m_chart), QVariant::fromValue<msc::MscEntity *>(m_chart),
                    "Test01" });
    QVERIFY(m_chart->comment() != nullptr);
    QCOMPARE(m_chart->commentString(), QString("Test01"));

    // Undo to empty string removes the comment
    msc::cmd::CommandsStack::current()->undo();
    QVERIFY(m_chart->comment() == nullptr);
    QCOMPARE(m_chart->commentString(), QString(""));

    // Normal undo
    m_chart->setCommentString("TestXY");
    msc::cmd::CommandsStack::push(msc::cmd::Id::ChangeComment,
            { QVariant::fromValue<msc::MscChart *>(m_chart), QVariant::fromValue<msc::MscEntity *>(m_chart),
                    "Test02" });
    QVERIFY(m_chart->comment() != nullptr);
    QCOMPARE(m_chart->commentString(), QString("Test02"));

    // Undo to empty string removes the comment
    msc::cmd::CommandsStack::current()->undo();
    QVERIFY(m_chart->comment() != nullptr);
    QCOMPARE(m_chart->commentString(), QString("TestXY"));
}

QTEST_MAIN(tst_CmdEntityCommentChange)

#include "tst_cmdentitycommentchange.moc"
