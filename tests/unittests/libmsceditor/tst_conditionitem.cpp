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

#include "baseitems/common/coordinatesconverter.h"
#include "chartlayoutmanager.h"
#include "conditionitem.h"
#include "msccommandsstack.h"
#include "msccondition.h"
#include "mscmodel.h"
#include "sharedlibrary.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtTest>
#include <cmath>

using namespace msc;

class tst_ConditionItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testCifUpdate();

private:
    MscCondition *m_condition = nullptr;
    QPointer<ConditionItem> m_conditionItem = nullptr;

    QGraphicsView *m_view = nullptr;
    QScopedPointer<msc::MscCommandsStack> m_undoStack;

    ChartLayoutManager *m_chartModel = nullptr;
    QPointer<MscChart> m_chart;
};

void tst_ConditionItem::init()
{
    shared::initSharedLibrary();
    m_undoStack.reset(new msc::MscCommandsStack);
    m_chartModel = new ChartLayoutManager(m_undoStack.data());
    m_view = new QGraphicsView();
    m_view->setScene(m_chartModel->graphicsScene());
    CoordinatesConverter::setDPI(QPointF(128., 128.), QPointF(96., 96.));
    CoordinatesConverter::instance()->setScene(m_chartModel->graphicsScene());
}

void tst_ConditionItem::cleanup()
{
    delete m_conditionItem;
    m_conditionItem = nullptr;
    delete m_condition;
    m_condition = nullptr;
    delete m_view;
    m_view = nullptr;
    delete m_chartModel;
    m_chartModel = nullptr;
}

void tst_ConditionItem::testCifUpdate()
{
    m_condition = new MscCondition();
    m_condition->setName("Hello_again");
    m_conditionItem = new ConditionItem(m_condition, m_chartModel);
    m_conditionItem->setPos({ 50, 70 });
    m_conditionItem->updateCif();

    auto conditionItem2 = new ConditionItem(m_condition, m_chartModel);
    QCOMPARE(conditionItem2->pos(), QPointF(0., 0.));

    conditionItem2->applyCif();
    // check that Y-position is restored
    QCOMPARE(conditionItem2->pos(), QPointF(0., 70.));
}

QTEST_MAIN(tst_ConditionItem)

#include "tst_conditionitem.moc"
