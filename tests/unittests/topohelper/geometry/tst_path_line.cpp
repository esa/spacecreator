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

#include <QtTest>

using namespace topohelp;
using namespace topohelp::geom;

class TestPathLine : public QObject
{
    Q_OBJECT

private slots:
    void testPathLine_data()
    {
        QTest::addColumn<RectsList>("existingRects");
        QTest::addColumn<QLineF>("startDirection");
        QTest::addColumn<QLineF>("endDirection");
        QTest::addColumn<PointsList>("expectedResult");

        QTest::newRow("Lines::DirectPath")
                << RectsList {} // existingRects
                << QLineF(0, 0, 10, 10) // startDirection
                << QLineF(90, 90, 100, 100) // endDirection
                << PointsList { QPointF(0, 0), QPointF(0, 0), QPointF(10, 10), QPointF(10, 10), QPointF(55, 55),
                       QPointF(55, 55), QPointF(100, 100), QPointF(90, 90) }; // expectedResult
        // TODO: I'd expect: { QPointF(0, 0), QPointF(10, 10), QPointF(55, 55), QPointF(90, 90), QPointF(100, 100) }

        QTest::newRow("Lines::IntersectedPath")
                << RectsList { QRectF(50, 50, 100, 100) } // existingRects
                << QLineF(0, 70, 25, 70) // startDirection
                << QLineF(175, 70, 200, 70) // endDirection
                << PointsList { QPointF(0, 70), QPointF(0, 70), QPointF(25, 70), QPointF(25, 34), QPointF(166, 34),
                       QPointF(166, 34), QPointF(183, 34), QPointF(183, 70), QPointF(200, 70),
                       QPointF(175, 70) }; // expectedResult
        // TODO: I'd expect:
        //{ QPointF(0, 70), QPointF(25, 70), QPointF(25, 34), QPointF(175, 34), QPointF(175, 70), QPointF(200, 70) }
    }

    void testPathLine()
    {
        QFETCH(RectsList, existingRects);
        QFETCH(QLineF, startDirection);
        QFETCH(QLineF, endDirection);
        QFETCH(PointsList, expectedResult);

        const PointsList &actualResult = path(existingRects, startDirection, endDirection);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestPathLine)
#include "tst_path_line.moc"
