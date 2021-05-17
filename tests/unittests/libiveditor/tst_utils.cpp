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

#include "baseitems/common/ivutils.h"
#include "graphicsviewutils.h"
#include "interface/graphicsitemhelpers.h"
#include "interface/ivfunctiongraphicsitem.h"
#include "iveditor.h"
#include "ivfunction.h"
#include "ivlibrary.h"
#include "sharedlibrary.h"

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMetaEnum>
#include <QObject>
#include <QtTest>

class tst_Utils : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testCheckCollision();
    void testSides();
    void testAdjustedRect();
    void testAlignRectToSide();
    void testCollidingRect();
    void testSiblingSceneRects();
    void testFindGeometryForRect();
};

void tst_Utils::initTestCase()
{
    ivm::initIVLibrary();
    ive::initIVEditor();
    shared::initSharedLibrary();
}

void tst_Utils::testCheckCollision()
{
    const QList<QRectF> rects {
        QRectF(100, 100, 100, 100),
        QRectF(300, 100, 100, 100),
        QRectF(100, 300, 100, 100),
        QRectF(300, 300, 100, 100),
    };
    const QRectF rectCollided { 350, 350, 100, 100 };
    const QRectF rectNotCollided { 225, 225, 50, 50 };
    QRectF intersected;
    QRectF notIntersected;
    QVERIFY(shared::graphicsviewutils::isCollided(rects, rectCollided, &intersected));
    QVERIFY(intersected == rects.at(3));
    QVERIFY(shared::graphicsviewutils::isCollided(rects, rectNotCollided, &notIntersected) == false);
    QVERIFY(!notIntersected.isValid());
}

void tst_Utils::testSides()
{
    const QMetaEnum me = QMetaEnum::fromType<Qt::Alignment>();
    for (int idx = 0; idx < me.keyCount(); ++idx) {
        const auto align = static_cast<Qt::Alignment>(me.value(idx));
        const int sideIdx = shared::graphicsviewutils::indexFromSide(align);
        if (shared::graphicsviewutils::kRectSides.contains(align)) {
            QVERIFY(align == shared::graphicsviewutils::sideFromIndex(sideIdx));
        } else {
            QCOMPARE(sideIdx, -1);
        }
    }
    QCOMPARE(shared::graphicsviewutils::sideFromIndex(-1),
            shared::graphicsviewutils::sideFromIndex(shared::graphicsviewutils::kRectSides.size() - 1));
    QCOMPARE(shared::graphicsviewutils::sideFromIndex(-2),
            shared::graphicsviewutils::sideFromIndex(shared::graphicsviewutils::kRectSides.size() - 2));
    QCOMPARE(shared::graphicsviewutils::sideFromIndex(-3),
            shared::graphicsviewutils::sideFromIndex(shared::graphicsviewutils::kRectSides.size() - 3));
    QCOMPARE(shared::graphicsviewutils::sideFromIndex(-4),
            shared::graphicsviewutils::sideFromIndex(shared::graphicsviewutils::kRectSides.size() - 4));
}

void tst_Utils::testAdjustedRect()
{
    const QRectF itemRect { 100, 100, 100, 100 };
    const QRectF intersectedItemRect { 150, 150, 100, 100 };
    QVERIFY(shared::graphicsviewutils::adjustedRect(itemRect, intersectedItemRect, Qt::AlignLeft, true).bottom()
            < intersectedItemRect.top());
    QVERIFY(shared::graphicsviewutils::adjustedRect(itemRect, intersectedItemRect, Qt::AlignLeft, false).top()
            > intersectedItemRect.bottom());
    QVERIFY(shared::graphicsviewutils::adjustedRect(itemRect, intersectedItemRect, Qt::AlignTop, true).left()
            > intersectedItemRect.right());
    QVERIFY(shared::graphicsviewutils::adjustedRect(itemRect, intersectedItemRect, Qt::AlignTop, false).right()
            < intersectedItemRect.left());
    QVERIFY(shared::graphicsviewutils::adjustedRect(itemRect, intersectedItemRect, Qt::AlignRight, true).top()
            > intersectedItemRect.bottom());
    QVERIFY(shared::graphicsviewutils::adjustedRect(itemRect, intersectedItemRect, Qt::AlignRight, false).bottom()
            < intersectedItemRect.top());
    QVERIFY(shared::graphicsviewutils::adjustedRect(itemRect, intersectedItemRect, Qt::AlignBottom, true).right()
            < intersectedItemRect.left());
    QVERIFY(shared::graphicsviewutils::adjustedRect(itemRect, intersectedItemRect, Qt::AlignBottom, false).left()
            > intersectedItemRect.right());

    const QMetaEnum me = QMetaEnum::fromType<Qt::Alignment>();
    for (int idx = 0; idx < me.keyCount(); ++idx) {
        const auto align = static_cast<Qt::Alignment>(me.value(idx));
        if (!shared::graphicsviewutils::kRectSides.contains(align)) {
            QVERIFY(!shared::graphicsviewutils::adjustedRect(itemRect, intersectedItemRect, align, true).isValid());
        }
    }
}

void tst_Utils::testAlignRectToSide()
{
    struct Data {
        QRectF boundingRect;
        QRectF itemRect;
        QPointF offset;
    };

    static const QList<Data> testDataList = {
        Data { { 100, 100, 500, 500 }, { 550, 550, 100, 100 }, { -25, -25 } },
        Data { { 0, 0, 250, 250 }, { 0, 0, 250, 250 }, { -50, -50 } },
        Data { { 550, 550, 100, 100 }, { 100, 100, 500, 500 }, { -75, -75 } },
    };

    for (auto testData : testDataList) {
        const QMetaEnum me = QMetaEnum::fromType<Qt::Alignment>();
        for (int idx = 0; idx < me.keyCount(); ++idx) {
            const auto align = static_cast<Qt::Alignment>(me.value(idx));
            QVERIFY(!shared::graphicsviewutils::alignRectToSide({}, testData.itemRect, align, testData.offset)
                             .isValid());
            QVERIFY(!shared::graphicsviewutils::alignRectToSide(testData.boundingRect, {}, align, testData.offset)
                             .isValid());

            if (!shared::graphicsviewutils::kRectSides.contains(align)) {
                QVERIFY(!shared::graphicsviewutils::alignRectToSide(
                        testData.boundingRect, testData.itemRect, align, testData.offset)
                                 .isValid());
            } else {
                const QRectF alignedRect = shared::graphicsviewutils::alignRectToSide(
                        testData.boundingRect, testData.itemRect, align, testData.offset);
                QVERIFY(alignedRect.isValid());
                QVERIFY(testData.boundingRect.contains(alignedRect.topLeft() - testData.offset));
            }
        }
    }
}

void tst_Utils::testCollidingRect()
{
    const QList<QRectF> existingRects {
        { 0, 0, 100, 100 },
        { 200, 200, 100, 100 },
        { 0, 200, 100, 100 },
    };
    QVERIFY(shared::graphicsviewutils::collidingRect(QRectF(200, 0, 100, 100), existingRects).isNull());
    QCOMPARE(shared::graphicsviewutils::collidingRect(QRectF(150, 150, 100, 100), existingRects), existingRects.at(1));
    QCOMPARE(shared::graphicsviewutils::collidingRect(QRectF(-50, 150, 300, 300), existingRects), existingRects.at(1));
    QCOMPARE(shared::graphicsviewutils::collidingRect(QRectF(-50, 150, 200, 200), existingRects), existingRects.at(2));
}

void tst_Utils::testSiblingSceneRects()
{
    const QVector<QRectF> existingRects {
        { 0, 0, 100, 100 },
        { 0, 200, 100, 100 },
        { 200, 0, 100, 100 },
        { 200, 200, 400, 400 },
    };

    QGraphicsScene scene;
    ive::IVFunctionGraphicsItem *item { nullptr };
    for (int idx = 0; idx < existingRects.size(); ++idx) {
        item = new ive::IVFunctionGraphicsItem(new ivm::IVFunction);
        scene.addItem(item);
        item->setRect(existingRects.at(idx));
    }
    ive::IVFunctionGraphicsItem *child { nullptr };
    for (int idx = 0; idx < 3; ++idx) {
        child = new ive::IVFunctionGraphicsItem(new ivm::IVFunction, item);
        child->setRect(existingRects.at(idx));
    }
    const auto siblingItems0 = shared::graphicsviewutils::siblingItemsRects(item);
    QCOMPARE(siblingItems0.size(), 3);
    for (auto itemRect : siblingItems0) {
        const auto it = std::find(existingRects.cbegin(), existingRects.cend(), itemRect);
        QVERIFY(it != existingRects.cend());
        QVERIFY(std::distance(existingRects.cbegin(), it) != 3);
    }

    const auto siblingItems1 = shared::graphicsviewutils::siblingItemsRects(child);
    QCOMPARE(siblingItems1.size(), 2);
    for (auto itemRect : siblingItems1) {
        const auto it = std::find(existingRects.cbegin(), existingRects.cend(), itemRect);
        QVERIFY(it != existingRects.cend());
        QVERIFY(std::distance(existingRects.cbegin(), it) != 3);
    }
}

void tst_Utils::testFindGeometryForRect()
{
    QList<QRectF> existingRects {
        { 0, 0, 100, 100 },
        { 0, 200, 100, 100 },
        { 200, 0, 100, 100 },
    };
    QRectF itemRect { 0, 0, 100, 100 };
    QRectF br;
    for (auto r : existingRects)
        br |= r;

    QRectF boundingRect { br };
    shared::graphicsviewutils::findGeometryForRect(itemRect, boundingRect, existingRects, QMarginsF());
    QVERIFY(boundingRect.contains(itemRect));
    QVERIFY(br == boundingRect);

    existingRects << itemRect;
    shared::graphicsviewutils::findGeometryForRect(itemRect, boundingRect, existingRects, QMarginsF());
    QVERIFY(boundingRect.contains(itemRect));
    QVERIFY(br != boundingRect);
}

QTEST_MAIN(tst_Utils)

#include "tst_utils.moc"
