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
#include "itemeditor/graphicsitemhelpers.h"
#include "itemeditor/ivconnectiongraphicsitem.h"
#include "itemeditor/ivfunctiongraphicsitem.h"
#include "itemeditor/ivfunctiontypegraphicsitem.h"
#include "itemeditor/ivinterfacegraphicsitem.h"
#include "iveditor.h"
#include "ivmodel.h"
#include "ivpropertytemplateconfig.h"
#include "sharedlibrary.h"
#include "standardpaths.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QPainter>
#include <QtTest>

class tst_IVConnectionGraphicsItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void tst_Overlapping();

private:
    bool checkIntersections(ive::IVConnectionGraphicsItem *connection);
};

void tst_IVConnectionGraphicsItem::initTestCase()
{
    ive::initIVEditor();
    shared::initSharedLibrary();
    shared::StandardPaths::setTestModeEnabled(true);
}

void tst_IVConnectionGraphicsItem::tst_Overlapping()
{
    auto cfg = ivm::IVPropertyTemplateConfig::instance();
    cfg->init(QLatin1String("default_attributes.xml"));
    ivm::IVModel model(cfg);

    ivm::IVInterface::CreationInfo ifc;
    ifc.model = &model;

    QGraphicsScene scene;
    auto parentFunc1 = new ive::IVFunctionGraphicsItem(new ivm::IVFunction);
    scene.addItem(parentFunc1);
    parentFunc1->setBoundingRect(QRectF(0., 0., 200., 200.));

    ifc.function = parentFunc1->entity();
    ifc.type = ivm::IVInterface::InterfaceType::Provided;
    auto ifaceItem1 = new ive::IVInterfaceGraphicsItem(ivm::IVInterface::createIface(ifc), parentFunc1);
    ifaceItem1->init();
    ifaceItem1->setBoundingRect(QRectF(0, 0, 10, 10));
    ifaceItem1->setPos(parentFunc1->mapFromScene(QPointF(100, 0)));

    auto parentFunc2 = new ive::IVFunctionGraphicsItem(new ivm::IVFunction);
    scene.addItem(parentFunc2);

    ifc.function = parentFunc1->entity();
    ifc.type = ivm::IVInterface::InterfaceType::Required;
    parentFunc2->setBoundingRect(QRectF(400., 0., 200., 200.));
    auto ifaceItem2 = new ive::IVInterfaceGraphicsItem(ivm::IVInterface::createIface(ifc), parentFunc2);
    ifaceItem2->init();
    ifaceItem2->setBoundingRect(QRectF(0, 0, 10, 10));
    ifaceItem2->setPos(parentFunc2->mapFromScene(QPointF(500, 0)));

    auto connection = new ive::IVConnectionGraphicsItem(
            new ivm::IVConnection(ifaceItem1->entity(), ifaceItem2->entity()), ifaceItem1, ifaceItem2);
    scene.addItem(connection);
    connection->init();
    ifaceItem1->addConnection(connection);
    ifaceItem2->addConnection(connection);
    connection->doLayout();
    QVERIFY(!checkIntersections(connection));

    auto funcItem = new ive::IVFunctionGraphicsItem(nullptr);
    scene.addItem(funcItem);
    funcItem->setBoundingRect(QRectF(100., 300., 400., 100.));
    connection->setPoints({ ifaceItem1->scenePos(), ifaceItem1->scenePos() - QPointF(0, 100), { 400, 450 },
            ifaceItem2->scenePos() - QPointF(0, 100), ifaceItem2->scenePos() });
    QVERIFY(checkIntersections(connection));
    connection->updateOverlappedSections();
    QVERIFY(!checkIntersections(connection));

    ifaceItem1->setPos(parentFunc1->mapFromScene(
            parentFunc1->sceneBoundingRect().right(), parentFunc1->sceneBoundingRect().center().y()));
    auto points = connection->points();
    points[0] = ifaceItem1->scenePos();
    points[1] = ifaceItem1->scenePos() + QPointF(50, 0);
    connection->setPoints(points);
    QVERIFY(checkIntersections(connection));
    connection->updateOverlappedSections();

    QVERIFY(!checkIntersections(connection));

    ifaceItem2->setPos(parentFunc2->mapFromScene(
            parentFunc2->sceneBoundingRect().left(), parentFunc2->sceneBoundingRect().center().y()));
    points = connection->points();
    points[points.size() - 1] = ifaceItem2->scenePos();
    points[points.size() - 2] = ifaceItem2->scenePos() - QPointF(50, 0);
    connection->setPoints(points);
    QVERIFY(checkIntersections(connection));
    connection->updateOverlappedSections();
    QVERIFY(!checkIntersections(connection));

    ifaceItem1->setPos(parentFunc1->mapFromScene(
            parentFunc1->sceneBoundingRect().center().x(), parentFunc1->sceneBoundingRect().bottom()));
    points = connection->points();
    points[0] = ifaceItem1->scenePos();
    points[1] = ifaceItem1->scenePos() + QPointF(0, 50);
    connection->setPoints(points);
    QVERIFY(checkIntersections(connection));
    connection->updateOverlappedSections();

    QVERIFY(!checkIntersections(connection));

    ifaceItem2->setPos(parentFunc2->mapFromScene(
            parentFunc2->sceneBoundingRect().center().x(), parentFunc2->sceneBoundingRect().bottom()));
    points = connection->points();
    points[points.size() - 1] = ifaceItem2->scenePos();
    points[points.size() - 2] = ifaceItem2->scenePos() + QPointF(0, 50);
    connection->setPoints(points);
    QVERIFY(checkIntersections(connection));
    connection->updateOverlappedSections();
    QVERIFY(!checkIntersections(connection));
}

bool tst_IVConnectionGraphicsItem::checkIntersections(ive::IVConnectionGraphicsItem *connection)
{
    const QRectF itemRect = shared::graphicsviewutils::getNearestIntersectedRect(
            shared::graphicsviewutils::siblingItemsRects(connection), connection->points(),
            shared::graphicsviewutils::IntersectionType::Single);

    return itemRect.isValid();
}

QTEST_MAIN(tst_IVConnectionGraphicsItem)

#include "tst_ivconnectiongraphicsitem.moc"
