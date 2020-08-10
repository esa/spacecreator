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

#include "baseitems/common/aadlutils.h"

#include <QMetaEnum>
#include <QObject>
#include <QtTest>

class tst_Utils : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testCheckCollision();
    void testSides();
    void testAdjustedRect();
    void testAlignRectToSide();
};

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
    QVERIFY(aadlinterface::checkCollision(rects, rectCollided, &intersected));
    QVERIFY(intersected == rects.at(3));
    QVERIFY(aadlinterface::checkCollision(rects, rectNotCollided, &notIntersected) == false);
    QVERIFY(!notIntersected.isValid());
}

void tst_Utils::testSides()
{
    const QMetaEnum me = QMetaEnum::fromType<Qt::Alignment>();
    for (int idx = 0; idx < me.keyCount(); ++idx) {
        const auto align = static_cast<Qt::Alignment>(me.value(idx));
        const int sideIdx = aadlinterface::indexFromSide(align);
        if (aadlinterface::kRectSides.contains(align)) {
            QVERIFY(align == aadlinterface::sideFromIndex(sideIdx));
        } else {
            QCOMPARE(sideIdx, -1);
        }
    }
    QCOMPARE(aadlinterface::sideFromIndex(-1), aadlinterface::sideFromIndex(aadlinterface::kRectSides.size() - 1));
    QCOMPARE(aadlinterface::sideFromIndex(-2), aadlinterface::sideFromIndex(aadlinterface::kRectSides.size() - 2));
    QCOMPARE(aadlinterface::sideFromIndex(-3), aadlinterface::sideFromIndex(aadlinterface::kRectSides.size() - 3));
    QCOMPARE(aadlinterface::sideFromIndex(-4), aadlinterface::sideFromIndex(aadlinterface::kRectSides.size() - 4));
}

void tst_Utils::testAdjustedRect()
{
    const QRectF itemRect { 100, 100, 100, 100 };
    const QRectF intersectedItemRect { 150, 150, 100, 100 };
    QVERIFY(aadlinterface::adjustedRect(itemRect, intersectedItemRect, Qt::AlignLeft, true).bottom()
            < intersectedItemRect.top());
    QVERIFY(aadlinterface::adjustedRect(itemRect, intersectedItemRect, Qt::AlignLeft, false).top()
            > intersectedItemRect.bottom());
    QVERIFY(aadlinterface::adjustedRect(itemRect, intersectedItemRect, Qt::AlignTop, true).left()
            > intersectedItemRect.right());
    QVERIFY(aadlinterface::adjustedRect(itemRect, intersectedItemRect, Qt::AlignTop, false).right()
            < intersectedItemRect.left());
    QVERIFY(aadlinterface::adjustedRect(itemRect, intersectedItemRect, Qt::AlignRight, true).top()
            > intersectedItemRect.bottom());
    QVERIFY(aadlinterface::adjustedRect(itemRect, intersectedItemRect, Qt::AlignRight, false).bottom()
            < intersectedItemRect.top());
    QVERIFY(aadlinterface::adjustedRect(itemRect, intersectedItemRect, Qt::AlignBottom, true).right()
            < intersectedItemRect.left());
    QVERIFY(aadlinterface::adjustedRect(itemRect, intersectedItemRect, Qt::AlignBottom, false).left()
            > intersectedItemRect.right());

    const QMetaEnum me = QMetaEnum::fromType<Qt::Alignment>();
    for (int idx = 0; idx < me.keyCount(); ++idx) {
        const auto align = static_cast<Qt::Alignment>(me.value(idx));
        if (!aadlinterface::kRectSides.contains(align)) {
            QVERIFY(!aadlinterface::adjustedRect(itemRect, intersectedItemRect, align, true).isValid());
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
            QVERIFY(!aadlinterface::alignRectToSide({}, testData.itemRect, align, testData.offset).isValid());
            QVERIFY(!aadlinterface::alignRectToSide(testData.boundingRect, {}, align, testData.offset).isValid());

            if (!aadlinterface::kRectSides.contains(align)) {
                QVERIFY(!aadlinterface::alignRectToSide(
                        testData.boundingRect, testData.itemRect, align, testData.offset)
                                 .isValid());
            } else {
                const QRectF alignedRect = aadlinterface::alignRectToSide(
                        testData.boundingRect, testData.itemRect, align, testData.offset);
                QVERIFY(alignedRect.isValid());
                QVERIFY(testData.boundingRect.contains(alignedRect.topLeft() - testData.offset));
            }
        }
    }
}

QTEST_MAIN(tst_Utils)

#include "tst_utils.moc"
