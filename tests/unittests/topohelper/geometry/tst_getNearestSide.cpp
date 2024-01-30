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

        auto asFlags = [](Qt::Alignment flag) { return QFlags<Qt::AlignmentFlag>(flag); };

        const QRectF rect100(0, 0, 100, 100);
        const QRectF rect200(100, 100, 200, 200);

        // Point inside the bounding area
        QTest::newRow("InsideBoundingArea") << rect100 << QPointF(50, 50) << asFlags(Qt::AlignLeft);

        // Point close to the left side
        QTest::newRow("NearLeftSide") << rect100 << QPointF(10, 50) << asFlags(Qt::AlignLeft);

        // Point close to the top side
        QTest::newRow("NearTopSide") << rect100 << QPointF(50, 10) << asFlags(Qt::AlignTop);

        // Point close to the right side
        QTest::newRow("NearRightSide") << rect100 << QPointF(90, 50) << asFlags(Qt::AlignRight);

        // Point close to the bottom side
        QTest::newRow("NearBottomSide") << rect100 << QPointF(50, 90) << asFlags(Qt::AlignBottom);

        // Point outside, near the left side
        QTest::newRow("OutsideNearLeft") << rect200 << QPointF(50, 150) << asFlags(Qt::AlignLeft);

        // Point outside, near the top side
        QTest::newRow("OutsideNearTop") << rect200 << QPointF(150, 50) << asFlags(Qt::AlignTop);

        // Point outside, near the right side
        QTest::newRow("OutsideNearRight") << rect200 << QPointF(251, 150) << asFlags(Qt::AlignRight);

        // Point outside, near the bottom side
        QTest::newRow("OutsideNearBottom") << rect200 << QPointF(150, 251) << asFlags(Qt::AlignBottom);

        // Invalid bounding area
        QTest::newRow("InvalidBoundingArea") << QRectF() << QPointF(50, 50) << asFlags(Qt::AlignCenter);
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
