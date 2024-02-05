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

class TestIntersectionPoints : public QObject
{
    Q_OBJECT

private slots:
    void testIntersectionPoints_data()
    {
        QTest::addColumn<QRectF>("rect");
        QTest::addColumn<QPolygonF>("polygon");
        QTest::addColumn<PointsList>("expectedResult");

        constexpr QRectF testRect(0, 0, 100, 100);

        QTest::newRow("SimpleIntersection")
                << testRect << QPolygonF { QPointF(50, 50), QPointF(150, 50) } << PointsList { QPointF(100, 50) };

        QTest::newRow("SimpleIntersectionTwoSegments")
                << testRect << QPolygonF { QPointF(50, 50), QPointF(100, 50), QPointF(150, 50) }
                << PointsList { QPointF(100, 50), QPointF(100, 50) };

        QTest::newRow("NoIntersection") << testRect << QPolygonF { QPointF(200, 50), QPointF(200, 100) }
                                        << PointsList {};

        QTest::newRow("NoIntersectionEmptyPolygon") << testRect << QPolygonF {} << PointsList {};

        QTest::newRow("NoIntersectionNullRect")
                << QRectF() << QPolygonF { QPointF(5, 5), QPointF(15, 5), QPointF(15, 15), QPointF(5, 15) }
                << PointsList {};

        QTest::newRow("NoIntersectionNullRectEmptyPolygon") << QRectF() << QPolygonF {} << PointsList {};

        QTest::newRow("ComplexPolygon") << testRect
                                        << QPolygonF { QPointF(50, 50), QPointF(150, 50), QPointF(150, 150),
                                               QPointF(50, 150), QPointF(0, 0), QPointF(200, 0), QPointF(200, 200),
                                               QPointF(0, 200) }
                                        << PointsList { QPointF(100, 50), QPointF(0, 0), QPointF(100. / 3., 100),
                                               QPointF(0, 0), QPointF(100, 0), QPointF(0, 0) };

        // TODO: get rid of duplicates?
    }

    void testIntersectionPoints()
    {
        QFETCH(QRectF, rect);
        QFETCH(QPolygonF, polygon);
        QFETCH(PointsList, expectedResult);

        const PointsList &actualResult = intersectionPoints(rect, polygon);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestIntersectionPoints)
#include "tst_intersectionPoints.moc"
