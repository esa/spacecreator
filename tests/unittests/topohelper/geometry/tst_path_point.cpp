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

class TestPathPoint : public QObject
{
    Q_OBJECT

private slots:
    void testPathPoint_data()
    {
        QTest::addColumn<RectsList>("existingRects");
        QTest::addColumn<QPointF>("startPoint");
        QTest::addColumn<QPointF>("endPoint");
        QTest::addColumn<PointsList>("expectedResult");

        QTest::newRow("Points::DirectPath") << RectsList {} // existingRects
                                            << QPointF(0, 0) // startPoint
                                            << QPointF(100, 100) // endPoint
                                            << PointsList { QPointF(0, 0), QPointF(100, 100) }; // expectedResult

        QTest::newRow("Points::IntersectedPath")
                << RectsList { QRectF(50, 50, 100, 100) } // existingRects
                << QPointF(0, 70) // startPoint
                << QPointF(200, 70) // endPoint
                << PointsList { QPointF(0, 70), QPointF(0, 34), QPointF(166, 34), QPointF(200, 70) }; // expectedResult
    }

    void testPathPoint()
    {
        QFETCH(RectsList, existingRects);
        QFETCH(QPointF, startPoint);
        QFETCH(QPointF, endPoint);
        QFETCH(PointsList, expectedResult);

        const PointsList &actualResult = path(existingRects, startPoint, endPoint);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestPathPoint)
#include "tst_path_point.moc"
