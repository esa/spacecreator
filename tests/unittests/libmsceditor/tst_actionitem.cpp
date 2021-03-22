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

#include "actionitem.h"
#include "baseitems/common/coordinatesconverter.h"
#include "chartlayoutmanager.h"
#include "mscaction.h"
#include "msccommandsstack.h"
#include "mscmodel.h"
#include "sharedlibrary.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtTest>
#include <cmath>

using namespace msc;

class tst_ActionItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testNameUpdate();

private:
    MscAction *m_action = nullptr;
    QPointer<ActionItem> m_actionItem = nullptr;

    QGraphicsView *m_view = nullptr;
    QScopedPointer<msc::MscCommandsStack> m_undoStack;

    ChartLayoutManager *m_chartModel = nullptr;
    QPointer<MscChart> m_chart;
};

void tst_ActionItem::init()
{
    shared::initSharedLibrary();
    m_undoStack.reset(new msc::MscCommandsStack);
    m_chartModel = new ChartLayoutManager(m_undoStack.data());
    m_view = new QGraphicsView();
    m_view->setScene(m_chartModel->graphicsScene());
    CoordinatesConverter::setDPI(QPointF(128., 128.), QPointF(96., 96.));
    CoordinatesConverter::instance()->setScene(m_chartModel->graphicsScene());
}

void tst_ActionItem::cleanup()
{
    delete m_actionItem;
    m_actionItem = nullptr;
    delete m_action;
    m_action = nullptr;
    delete m_view;
    m_view = nullptr;
    delete m_chartModel;
    m_chartModel = nullptr;
}

void tst_ActionItem::testNameUpdate()
{
    m_action = new MscAction();
    m_action->setName("Hello_again");
    m_actionItem = new ActionItem(m_action, m_chartModel);
    m_actionItem->setPos({ 50, 70 });
    m_actionItem->updateCif();

    auto actionItem2 = new ActionItem(m_action, m_chartModel);
    QCOMPARE(actionItem2->pos(), QPointF(0., 0.));

    actionItem2->applyCif();
    // check that Y-position is restored
    QCOMPARE(actionItem2->pos(), QPointF(0., 70.));
}

QTEST_MAIN(tst_ActionItem)

#include "tst_actionitem.moc"
