/*
   Copyright (C) 2024 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QObject>
#include <QtTest>
#include <memory>

class tst_GraphicsViewUtils : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();

    void testNearestItemNoItem();
    void testNearestItemWrongType();
    void testNearestItemWrongPlace();
    void testNearestItemFirstItem();
    void testNearestItemSecondItem();
    void testNearestItemOverlappingItems();

private:
    std::unique_ptr<QGraphicsScene> m_scene;
};

void tst_GraphicsViewUtils::init()
{
    m_scene = std::make_unique<QGraphicsScene>();
}

void tst_GraphicsViewUtils::testNearestItemNoItem()
{
    QList<int> acceptableTypes = {};
    QRectF area(10, 10, 5, 5);
    QCOMPARE(shared::graphicsviewutils::nearestItem(m_scene.get(), area, acceptableTypes), nullptr);
}

void tst_GraphicsViewUtils::testNearestItemWrongType()
{
    auto rect1 = new QGraphicsRectItem(50, 50, 20, 20);
    m_scene->addItem(rect1);
    QList<int> acceptableTypes = { QGraphicsLineItem::Type };
    QRectF area(60, 60, 5, 5);
    QCOMPARE(shared::graphicsviewutils::nearestItem(m_scene.get(), area, acceptableTypes), nullptr);
}

void tst_GraphicsViewUtils::testNearestItemWrongPlace()
{
    auto rect1 = new QGraphicsRectItem(50, 50, 20, 20);
    m_scene->addItem(rect1);
    QList<int> acceptableTypes = { QGraphicsRectItem::Type };
    QRectF area(160, 160, 5, 5);
    QCOMPARE(shared::graphicsviewutils::nearestItem(m_scene.get(), area, acceptableTypes), nullptr);
}

void tst_GraphicsViewUtils::testNearestItemFirstItem()
{
    auto rect1 = new QGraphicsRectItem(50, 50, 20, 20);
    m_scene->addItem(rect1);
    auto rect2 = new QGraphicsRectItem(150, 150, 20, 20);
    m_scene->addItem(rect2);
    QList<int> acceptableTypes = { QGraphicsRectItem::Type };
    QRectF area(60, 60, 5, 5);
    QCOMPARE(shared::graphicsviewutils::nearestItem(m_scene.get(), area, acceptableTypes), rect1);
}

void tst_GraphicsViewUtils::testNearestItemSecondItem()
{
    auto rect1 = new QGraphicsRectItem(50, 50, 20, 20);
    m_scene->addItem(rect1);
    auto rect2 = new QGraphicsRectItem(150, 150, 20, 20);
    m_scene->addItem(rect2);
    QList<int> acceptableTypes = { QGraphicsRectItem::Type };
    QRectF area(160, 160, 5, 5);
    QCOMPARE(shared::graphicsviewutils::nearestItem(m_scene.get(), area, acceptableTypes), rect2);
}

void tst_GraphicsViewUtils::testNearestItemOverlappingItems()
{
    using namespace shared::graphicsviewutils;

    auto rect1 = new QGraphicsRectItem(50, 50, 20, 20);
    m_scene->addItem(rect1);
    auto rect2 = new QGraphicsRectItem(60, 50, 20, 20);
    m_scene->addItem(rect2);
    QList<int> acceptableTypes = { QGraphicsRectItem::Type };

    // test horizontally
    QRectF area(61, 60, 5, 5); // only close to border of rect2
    QCOMPARE(shared::graphicsviewutils::nearestItem(m_scene.get(), area, acceptableTypes), rect2);
    QCOMPARE(
            shared::graphicsviewutils::nearestItem(m_scene.get(), area, acceptableTypes, DistanceCondition::InsideItem),
            rect1);

    area.setRect(68, 60, 5, 5); // only close to border of rect1
    QCOMPARE(shared::graphicsviewutils::nearestItem(m_scene.get(), area, acceptableTypes), rect1);
    QCOMPARE(
            shared::graphicsviewutils::nearestItem(m_scene.get(), area, acceptableTypes, DistanceCondition::InsideItem),
            rect2);

    // test vertically
    rect2->setRect(50, 60, 20, 20);

    area.setRect(60, 61, 5, 5); // only close to border of rect2
    QCOMPARE(shared::graphicsviewutils::nearestItem(m_scene.get(), area, acceptableTypes), rect2);
    QCOMPARE(
            shared::graphicsviewutils::nearestItem(m_scene.get(), area, acceptableTypes, DistanceCondition::InsideItem),
            rect1);

    area.setRect(60, 68, 5, 5); // only close to border of rect1
    QCOMPARE(shared::graphicsviewutils::nearestItem(m_scene.get(), area, acceptableTypes), rect1);
    QCOMPARE(
            shared::graphicsviewutils::nearestItem(m_scene.get(), area, acceptableTypes, DistanceCondition::InsideItem),
            rect2);
}

QTEST_MAIN(tst_GraphicsViewUtils)

#include "tst_graphicsviewutils.moc"
