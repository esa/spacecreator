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

class TestGenerateSegmentsPoint : public QObject
{
    Q_OBJECT

private slots:
    void testGenerateSegmentsPoint_data()
    {
        QTest::addColumn<QPointF>("startPoint");
        QTest::addColumn<QPointF>("endPoint");
        QTest::addColumn<QList<PointsList>>("expectedResult");

        constexpr QPointF pnt0x0 = QPointF(0, 0);
        constexpr QPointF pnt5x5(5, 5);
        constexpr QPointF pnt0x10(0, 10);
        constexpr QPointF pnt10x0(10, 0);
        constexpr QPointF pnt10x10(10, 10);

        QTest::newRow("Vertical") << pnt0x0 << pnt0x10 << QList<PointsList> { { pnt0x0, pnt0x10 } };

        QTest::newRow("Horizontal") << pnt0x0 << pnt10x0 << QList<PointsList> { { pnt0x0, pnt10x0 } };

        QTest::newRow("DiagonalUpDown") << pnt0x0 << pnt10x10
                                        << QList<PointsList> {
                                               { pnt0x0, pnt0x10, pnt10x10 },
                                               { pnt0x0, pnt10x0, pnt10x10 },
                                           };

        QTest::newRow("DiagonalDownUp") << pnt10x10 << pnt0x0
                                        << QList<PointsList> {
                                               { pnt10x10, pnt10x0, pnt0x0 },
                                               { pnt10x10, pnt0x10, pnt0x0 },
                                           };

        QTest::newRow("EqualPoints") << pnt5x5 << pnt5x5 << QList<PointsList> {};
    }

    void testGenerateSegmentsPoint()
    {
        QFETCH(QPointF, startPoint);
        QFETCH(QPointF, endPoint);
        QFETCH(QList<PointsList>, expectedResult);

        const QList<PointsList> &actualResult = generateSegments(startPoint, endPoint);
        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestGenerateSegmentsPoint)
#include "tst_generateSegments_point.moc"
