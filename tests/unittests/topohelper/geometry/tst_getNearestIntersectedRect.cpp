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

class TestGetNearestIntersectedRect : public QObject
{
    Q_OBJECT

private slots:
    void testGetNearestIntersectedRect_data()
    {
        QTest::addColumn<RectsList>("existingRects");
        QTest::addColumn<PointsList>("points");
        QTest::addColumn<IntersectionType>("intersectionType");
        QTest::addColumn<QRectF>("expectedResult");

        constexpr QRectF noRect = QRectF();
        constexpr QRectF testRect(5, 5, 10, 10);
        constexpr QPointF pnt0(0, 0);
        constexpr QPointF pnt3(3, 3);
        constexpr QPointF pnt5(5, 5);
        constexpr QPointF pnt6(6, 6);
        constexpr QPointF pnt9(9, 9);
        constexpr QPointF pnt10(10, 10);
        constexpr QPointF pnt15(15, 15);
        constexpr QPointF pnt20(20, 20);

        QTest::newRow("LineIntersectionEdge")
                << RectsList { testRect } << PointsList { pnt0, pnt20 } << IntersectionType::Edge << testRect;
        QTest::newRow("NoIntersectionEdge")
                << RectsList { testRect } << PointsList { pnt0, pnt3 } << IntersectionType::Edge << noRect;

        QTest::newRow("LineIntersectionMultiple")
                << RectsList { testRect } << PointsList { pnt0, pnt20 } << IntersectionType::Multiple << testRect;
        QTest::newRow("NoIntersectionMultiple")
                << RectsList { testRect } << PointsList { pnt0, pnt3 } << IntersectionType::Multiple << noRect;

        QTest::newRow("MultipleIntersection") << RectsList { testRect } << PointsList { pnt0, pnt10, pnt15 }
                                              << IntersectionType::Multiple << testRect;

        QTest::newRow("DiagIntersectionEdge")
                << RectsList { testRect } << PointsList { pnt5, pnt10 } << IntersectionType::Edge << noRect;
        QTest::newRow("DiagIntersectionMultiple")
                << RectsList { testRect } << PointsList { pnt5, pnt10 } << IntersectionType::Multiple << testRect;

        QTest::newRow("NoDiagIntersectionEdge")
                << RectsList { testRect } << PointsList { pnt6, pnt9 } << IntersectionType::Edge << noRect;
        QTest::newRow("NoDiagIntersectionMultiple")
                << RectsList { testRect } << PointsList { pnt6, pnt9 } << IntersectionType::Multiple << noRect;
    }

    void testGetNearestIntersectedRect()
    {
        QFETCH(RectsList, existingRects);
        QFETCH(PointsList, points);
        QFETCH(IntersectionType, intersectionType);
        QFETCH(QRectF, expectedResult);

        const QRectF &actualResult = getNearestIntersectedRect(existingRects, points, intersectionType);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestGetNearestIntersectedRect)
#include "tst_getNearestIntersectedRect.moc"
