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

class TestIntersectsPolygon : public QObject
{
    Q_OBJECT

private slots:
    void testIntersectsPolygon_data()
    {
        QTest::addColumn<QRectF>("rect");
        QTest::addColumn<QPolygonF>("polygon");
        QTest::addColumn<bool>("expectedResult");
        QTest::addColumn<QPointF>("expectedIntersectPos");

        constexpr QRectF testRect(0, 0, 10, 10);
        constexpr QLineF intersectBottomRight(5, 5, 15, 15);
        constexpr QPointF pnt5x5(5, 5);
        constexpr QPointF pnt15x5(15, 5);
        constexpr QPointF pnt15x15(15, 15);
        constexpr QPointF pnt5x15(5, 15);
        constexpr QPointF pnt10x5(10, 5);

        QTest::newRow("SimpleIntersection")
                << testRect << QPolygonF { pnt5x5, pnt15x5, pnt15x15, pnt5x15 } << true << pnt10x5;
        QTest::newRow("NoIntersectionNoRect")
                << QRectF() << QPolygonF { pnt5x5, pnt15x5, pnt15x15, pnt5x15 } << false << QPointF();
        QTest::newRow("NoIntersectionNoPolygon") << testRect << QPolygonF {} << false << QPointF();
        QTest::newRow("NoIntersectionNullPolygon") << testRect << QPolygonF {} << false << QPointF();
        QTest::newRow("ComplexPolygonWithIntersection")
                << testRect
                << QPolygonF { pnt5x5, pnt15x5, pnt15x15, pnt5x15, QPointF(0, 0), QPointF(20, 0), QPointF(20, 20),
                       QPointF(0, 20) }
                << true << pnt10x5;
    }

    void testIntersectsPolygon()
    {
        QFETCH(QRectF, rect);
        QFETCH(QPolygonF, polygon);
        QFETCH(bool, expectedResult);
        QFETCH(QPointF, expectedIntersectPos);

        QPointF actualIntersectPos;
        const bool actualResult = intersects(rect, polygon, &actualIntersectPos);

        QCOMPARE(actualResult, expectedResult);

        if (actualResult && expectedResult) {
            QCOMPARE(actualIntersectPos, expectedIntersectPos);
        }
    }
};

QTEST_MAIN(TestIntersectsPolygon)
#include "tst_intersects_polygon.moc"
