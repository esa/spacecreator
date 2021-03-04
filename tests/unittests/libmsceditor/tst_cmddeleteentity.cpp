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
#include "commands/cmddeleteentity.h"
#include "common/chartlayouttestbase.h"
#include "messageitem.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "sharedlibrary.h"

#include <QtTest>

using namespace msc;

class tst_CmdDeleteEntity : public ChartLayoutTestBase
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();

    void testUndoMessageDelete();
};

void tst_CmdDeleteEntity::init()
{
    shared::initSharedLibrary();
    initBase();
}

void tst_CmdDeleteEntity::cleanup()
{
    cleanupBase();
}

void tst_CmdDeleteEntity::testUndoMessageDelete()
{
    const QString msc("mscdocument doc1;\
                       msc Untitled_MSC;\
                         /* CIF INSTANCE (0, 56) (189, 79) (800, 508) */\
                         instance Instance_1;\
                           /* CIF MESSAGE (96, 210) (620, 403) */\
                           out Message to env;\
                         endinstance;\
                       endmsc;\
                       endmscdocument;");
    parseMsc(msc);

    // store message geometry
    auto message = qobject_cast<msc::MscMessage *>(m_chart->instanceEvents().at(0));
    msc::MessageItem *messageItem = m_chartModel->itemForMessage(message);
    const QRectF loadedGeometry = messageItem->sceneBoundingRect();

    // delete the message
    auto deleteCmd = new msc::cmd::CmdDeleteEntity({ message }, nullptr, m_chartModel.get());
    m_undoStack->push(deleteCmd);
    QCOMPARE(m_chart->instanceEvents().size(), 0);

    // undo
    m_undoStack->undo();
    waitForLayoutUpdate();
    QCOMPARE(m_chart->instanceEvents().size(), 1);
    message = qobject_cast<msc::MscMessage *>(m_chart->instanceEvents().at(0));
    messageItem = m_chartModel->itemForMessage(message);

    // check the geometry is the same as the stored one
    const QRectF restoredGeometry = messageItem->sceneBoundingRect();
    QVERIFY(std::abs(restoredGeometry.x() - loadedGeometry.x()) < 1.);
    QVERIFY(std::abs(restoredGeometry.y() - loadedGeometry.y()) < 1.);
    QVERIFY(std::abs(restoredGeometry.width() - loadedGeometry.width()) < 1.);
    QVERIFY(std::abs(restoredGeometry.height() - loadedGeometry.height()) < 1.);
}

QTEST_MAIN(tst_CmdDeleteEntity)

#include "tst_cmddeleteentity.moc"
