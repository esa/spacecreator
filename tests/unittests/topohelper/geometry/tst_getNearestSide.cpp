/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "topohelper/geometry.h"

#include <QDebug>
#include <QPointF>
#include <QtTest>

using namespace topohelp;
using namespace topohelp::geom;

class TestGetNearestSide : public QObject
{
    Q_OBJECT

private slots:
    void testGetNearestSide_data()
    {
        QTest::addColumn<QRectF>("boundingArea");
        QTest::addColumn<QPointF>("pos");
        QTest::addColumn<Qt::Alignment>("expectedResult");

        auto flag = [](Qt::Alignment v) { return QFlags<Qt::AlignmentFlag>(v); };

        // Point inside the bounding area
        QTest::newRow("InsideBoundingArea") << QRectF(0, 0, 100, 100) << QPointF(50, 50) << flag(Qt::AlignLeft);

        // Point close to the left side
        QTest::newRow("NearLeftSide") << QRectF(0, 0, 100, 100) << QPointF(10, 50) << flag(Qt::AlignLeft);

        // Point close to the top side
        QTest::newRow("NearTopSide") << QRectF(0, 0, 100, 100) << QPointF(50, 10) << flag(Qt::AlignTop);

        // Point close to the right side
        QTest::newRow("NearRightSide") << QRectF(0, 0, 100, 100) << QPointF(90, 50) << flag(Qt::AlignRight);

        // Test case 5: Point close to the bottom side
        QTest::newRow("NearBottomSide") << QRectF(0, 0, 100, 100) << QPointF(50, 90) << flag(Qt::AlignBottom);

        // Point outside, near the left side
        QTest::newRow("OutsideNearLeft") << QRectF(100, 100, 200, 200) << QPointF(50, 150) << flag(Qt::AlignLeft);

        // Point outside, near the top side
        QTest::newRow("OutsideNearTop") << QRectF(100, 100, 200, 200) << QPointF(150, 50) << flag(Qt::AlignTop);

        // Point outside, near the right side
        QTest::newRow("OutsideNearRight") << QRectF(100, 100, 200, 200) << QPointF(251, 150) << flag(Qt::AlignRight);

        // Point outside, near the bottom side
        QTest::newRow("OutsideNearBottom") << QRectF(100, 100, 200, 200) << QPointF(150, 251) << flag(Qt::AlignBottom);

        // Invalid bounding area
        QTest::newRow("InvalidBoundingArea") << QRectF() << QPointF(50, 50) << flag(Qt::AlignCenter);
    }

    void testGetNearestSide()
    {
        QFETCH(QRectF, boundingArea);
        QFETCH(QPointF, pos);
        QFETCH(Qt::Alignment, expectedResult);

        const Qt::Alignment &actualResult = getNearestSide(boundingArea, pos);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestGetNearestSide)
#include "tst_getNearestSide.moc"
