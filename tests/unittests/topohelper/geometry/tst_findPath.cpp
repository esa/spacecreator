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

#include <QtTest>

using namespace topohelp;
using namespace topohelp::geom;

class TestFindPath : public QObject
{
    Q_OBJECT

private slots:
    void testFindPath_data()
    {
        QTest::addColumn<RectsList>("existingRects");
        QTest::addColumn<QLineF>("startDirection");
        QTest::addColumn<QLineF>("endDirection");
        QTest::addColumn<QRectF>("expectedIntersectedRect");
        QTest::addColumn<PointsList>("expectedResult");

        constexpr QRectF rect(100, 100, 200, 200);
        constexpr QLineF leftLine(50, 150, 70, 150);
        constexpr QLineF rightLine(350, 150, 370, 150);

        QTest::newRow("NoRect") << RectsList {} << QLineF(0, 0, 10, 10) << QLineF(20, 20, 30, 30) << QRectF {}
                                << PointsList { QPointF(10, 10), QPointF(20, 20), QPointF(20, 20), QPointF(30, 30) };

        QTest::newRow("RectIntersection") << RectsList { rect } << leftLine << rightLine << rect << PointsList {};

        QTest::newRow("RectNoIntersection")
                << RectsList { rect } << leftLine.translated(0, -100) << rightLine.translated(0, -100) << QRectF()
                << PointsList { QPointF(70, 50), QPointF(220, 50), QPointF(220, 50), QPointF(370, 50) };
    }

    void testFindPath()
    {
        QFETCH(RectsList, existingRects);
        QFETCH(QLineF, startDirection);
        QFETCH(QLineF, endDirection);
        QFETCH(QRectF, expectedIntersectedRect);
        QFETCH(PointsList, expectedResult);

        QRectF intersectedRect;
        const auto &actualResult = findPath(existingRects, startDirection, endDirection, &intersectedRect);

        QCOMPARE(intersectedRect, expectedIntersectedRect);
        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestFindPath)
#include "tst_findPath.moc"
