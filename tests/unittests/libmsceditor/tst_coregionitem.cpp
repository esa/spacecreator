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
#include "coregionitem.h"
#include "msccommandsstack.h"
#include "msccoregion.h"
#include "mscmodel.h"
#include "sharedlibrary.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtTest>
#include <cmath>

using namespace msc;

class tst_CoregionItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testCifUpdate();

private:
    MscCoregion *m_coregion = nullptr;
    QPointer<CoregionItem> m_coregionItem = nullptr;

    QGraphicsView *m_view = nullptr;
    QScopedPointer<msc::MscCommandsStack> m_undoStack;

    ChartLayoutManager *m_chartModel = nullptr;
    QPointer<MscChart> m_chart;
};

void tst_CoregionItem::init()
{
    shared::initSharedLibrary();
    m_undoStack.reset(new msc::MscCommandsStack);
    m_chartModel = new ChartLayoutManager(m_undoStack.data());
    m_view = new QGraphicsView();
    m_view->setScene(m_chartModel->graphicsScene());
    CoordinatesConverter::setDPI(QPointF(128., 128.), QPointF(96., 96.));
    CoordinatesConverter::instance()->setScene(m_chartModel->graphicsScene());
}

void tst_CoregionItem::cleanup()
{
    delete m_coregionItem;
    m_coregionItem = nullptr;
    delete m_coregion;
    m_coregion = nullptr;
    delete m_view;
    m_view = nullptr;
    delete m_chartModel;
    m_chartModel = nullptr;
}

void tst_CoregionItem::testCifUpdate()
{
    m_coregion = new MscCoregion();
    m_coregion->setName("Start_C");
    auto coregionEnd = new MscCoregion();
    coregionEnd->setName("End_C");
    coregionEnd->setType(MscCoregion::Type::End);

    m_coregionItem = new CoregionItem(m_chartModel);
    m_coregionItem->setBegin(m_coregion);
    m_coregionItem->setEnd(coregionEnd);
    m_coregionItem->setPos({ 50, 70 });
    m_coregionItem->setBoundingRect({ 0, 0, 40, 180 });
    m_coregionItem->updateCif();
    QCOMPARE(m_coregionItem->sceneBoundingRect().bottom(), 250);

    auto coregionItem2 = new CoregionItem(m_chartModel);
    coregionItem2->setBegin(m_coregion);
    coregionItem2->setEnd(coregionEnd);
    QCOMPARE(coregionItem2->pos(), QPointF(0., 0.));

    coregionItem2->applyCif();
    // check that Y-position is restored
    QCOMPARE(coregionItem2->pos(), QPointF(0., 70.));
    QCOMPARE(coregionItem2->sceneBoundingRect().bottom(), 250);
}

QTEST_MAIN(tst_CoregionItem)

#include "tst_coregionitem.moc"
