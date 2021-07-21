/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "graphicsviewutils.h"
#include "itemeditor/common/ivutils.h"
#include "itemeditor/ivfunctiongraphicsitem.h"
#include "itemeditor/ivinterfacegraphicsitem.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivtestutils.h"
#include "positionlookuphelper.h"
#include "sharedlibrary.h"

#include <QGraphicsScene>
#include <QObject>
#include <QPainter>
#include <QtTest>

class tst_PositionLookupHelper : public QObject
{
    Q_OBJECT
private:
    void testOnSide(const QVector<QRectF> &itemRects, const QPointF &initialOffset, const QRectF &parentBoundingRect);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testLookupOnLeft();
    void testLookupOnTop();
    void testLookupOnRight();
    void testLookupOnBottom();
    void testLookupAllSides();

private:
    QHash<Qt::Alignment, QPainterPath> sidePaths;
    QGraphicsScene *scene = nullptr;
    ivm::IVFunction *function = nullptr;
    ivm::IVInterfaceProvided *iface = nullptr;
    ive::IVFunctionGraphicsItem *functionItem = nullptr;
    ive::IVInterfaceGraphicsItem *ifaceItem = nullptr;
};

void tst_PositionLookupHelper::testOnSide(
        const QVector<QRectF> &itemRects, const QPointF &initialOffset, const QRectF &parentBoundingRect)
{
    QList<QRectF> siblingsRects;
    for (int idx = 0; idx < itemRects.size(); ++idx) {
        shared::PositionLookupHelper clockwiseHelper(sidePaths, parentBoundingRect, siblingsRects, itemRects.value(idx),
                initialOffset, shared::graphicsviewutils::LookupDirection::Clockwise);
        shared::PositionLookupHelper counterClockwiseHelper(sidePaths, parentBoundingRect, siblingsRects,
                itemRects.value(idx), initialOffset, shared::graphicsviewutils::LookupDirection::CounterClockwise);
        while (clockwiseHelper.hasNext() || counterClockwiseHelper.hasNext()) {
            if (clockwiseHelper.lookup()) {
                if (clockwiseHelper.isSideChanged())
                    ifaceItem->updateInternalItems(clockwiseHelper.side());
                ifaceItem->setPos(clockwiseHelper.mappedOriginPoint());
                iface->setCoordinates(shared::graphicsviewutils::coordinates(clockwiseHelper.mappedOriginPoint()));
                ifaceItem->rebuildLayout();
                break;
            } else if (counterClockwiseHelper.lookup()) {
                if (counterClockwiseHelper.isSideChanged())
                    ifaceItem->updateInternalItems(counterClockwiseHelper.side());
                ifaceItem->setPos(counterClockwiseHelper.mappedOriginPoint());
                iface->setCoordinates(
                        shared::graphicsviewutils::coordinates(counterClockwiseHelper.mappedOriginPoint()));
                ifaceItem->rebuildLayout();
                break;
            }
        }
        QVERIFY(ifaceItem->parentItem()->boundingRect().contains(ifaceItem->pos()));
        siblingsRects.append(ifaceItem->mapRectToParent(ifaceItem->boundingRect()));
    }
}

void tst_PositionLookupHelper::initTestCase()
{
    shared::initSharedLibrary();
    scene = new QGraphicsScene;
    function = ivm::testutils::createFunction("Function");
    functionItem = new ive::IVFunctionGraphicsItem(function);
    scene->addItem(functionItem);
    functionItem->setBoundingRect(QRectF(0, 0, 500, 500));
    ivm::IVInterface::CreationInfo ci;
    ci.name = "PI";
    ci.function = function;
    iface = new ivm::IVInterfaceProvided(ci);
    ifaceItem = new ive::IVInterfaceGraphicsItem(iface, functionItem);
    ifaceItem->updateLabel();

    sidePaths = {
        { Qt::AlignLeft, ifaceItem->itemPath(Qt::AlignLeft) },
        { Qt::AlignTop, ifaceItem->itemPath(Qt::AlignTop) },
        { Qt::AlignRight, ifaceItem->itemPath(Qt::AlignRight) },
        { Qt::AlignBottom, ifaceItem->itemPath(Qt::AlignBottom) },
    };
}

void tst_PositionLookupHelper::cleanupTestCase()
{
    sidePaths.clear();

    delete ifaceItem;
    ifaceItem = nullptr;

    delete iface;
    iface = nullptr;

    delete functionItem;
    functionItem = nullptr;

    delete function;
    function = nullptr;

    delete scene;
    scene = nullptr;
}

void tst_PositionLookupHelper::testLookupOnTop()
{
    QRectF itemRect = ifaceItem->itemPath(Qt::AlignTop).boundingRect();
    QPointF initialOffset = itemRect.topLeft();
    const QRectF parentBoundingRect = functionItem->boundingRect();
    itemRect.translate(parentBoundingRect.left() + parentBoundingRect.width() / 2, parentBoundingRect.top());
    const int ifaceCount = (parentBoundingRect.width() * 2 + parentBoundingRect.height() * 2)
            / (itemRect.width() + shared::graphicsviewutils::kInterfaceLayoutOffset);
    QVector<QRectF> itemRects(ifaceCount, itemRect);
    testOnSide(itemRects, initialOffset, parentBoundingRect);
}

void tst_PositionLookupHelper::testLookupOnLeft()
{
    QRectF itemRect = ifaceItem->itemPath(Qt::AlignLeft).boundingRect();
    QPointF initialOffset = itemRect.topLeft();
    const QRectF parentBoundingRect = functionItem->boundingRect();
    itemRect.translate(parentBoundingRect.left(), parentBoundingRect.top() + parentBoundingRect.height() / 2);
    const int ifaceCount = (parentBoundingRect.width() * 2 + parentBoundingRect.height() * 2)
            / (itemRect.width() + shared::graphicsviewutils::kInterfaceLayoutOffset);
    QVector<QRectF> itemRects(ifaceCount, itemRect);
    testOnSide(itemRects, initialOffset, parentBoundingRect);
}

void tst_PositionLookupHelper::testLookupOnRight()
{
    QRectF itemRect = ifaceItem->itemPath(Qt::AlignRight).boundingRect();
    QPointF initialOffset = itemRect.topLeft();
    const QRectF parentBoundingRect = functionItem->boundingRect();
    itemRect.translate(parentBoundingRect.right(), parentBoundingRect.top() + parentBoundingRect.height() / 2);
    const int ifaceCount = (parentBoundingRect.width() * 2 + parentBoundingRect.height() * 2)
            / (itemRect.width() + shared::graphicsviewutils::kInterfaceLayoutOffset);
    QVector<QRectF> itemRects(ifaceCount, itemRect);
    testOnSide(itemRects, initialOffset, parentBoundingRect);
}
void tst_PositionLookupHelper::testLookupOnBottom()
{
    QRectF itemRect = ifaceItem->itemPath(Qt::AlignBottom).boundingRect();
    QPointF initialOffset = itemRect.topLeft();
    const QRectF parentBoundingRect = functionItem->boundingRect();
    itemRect.translate(parentBoundingRect.left() + parentBoundingRect.width() / 2, parentBoundingRect.bottom());
    const int ifaceCount = (parentBoundingRect.width() * 2 + parentBoundingRect.height() * 2)
            / (itemRect.width() + shared::graphicsviewutils::kInterfaceLayoutOffset);
    QVector<QRectF> itemRects(ifaceCount, itemRect);
    testOnSide(itemRects, initialOffset, parentBoundingRect);
}

void tst_PositionLookupHelper::testLookupAllSides()
{
    const QRectF parentBoundingRect = functionItem->boundingRect();
    QRectF itemRect = ifaceItem->itemPath(Qt::AlignTop).boundingRect();
    QPointF initialOffset = itemRect.topLeft();
    const QList<QPointF> insertPoints { QPointF(parentBoundingRect.left() + parentBoundingRect.width() / 2,
                                                parentBoundingRect.top()),
        QPointF(parentBoundingRect.right(), parentBoundingRect.top() + parentBoundingRect.height() / 2),
        QPointF(parentBoundingRect.left() + parentBoundingRect.width() / 2, parentBoundingRect.bottom()),
        QPointF(parentBoundingRect.left(), parentBoundingRect.top() + parentBoundingRect.height() / 2) };

    const int ifaceCount = (parentBoundingRect.width() * 2 + parentBoundingRect.height() * 2)
            / (itemRect.width() + shared::graphicsviewutils::kInterfaceLayoutOffset);
    QVector<QRectF> itemRects(ifaceCount);
    for (int idx = 0; idx < ifaceCount; ++idx) {
        itemRect.moveTopLeft(insertPoints.value(idx % insertPoints.size()) + initialOffset);
        itemRects[idx] = itemRect;
    }
    testOnSide(itemRects, initialOffset, parentBoundingRect);
}

QTEST_MAIN(tst_PositionLookupHelper)

#include "tst_positionlookuphelper.moc"
