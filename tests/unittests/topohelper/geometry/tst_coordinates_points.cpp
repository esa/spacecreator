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

class TestCoordinates_points : public QObject
{
    Q_OBJECT

private slots:
    void testCoordinates_points_data()
    {
        QTest::addColumn<PointsList>("points");
        QTest::addColumn<QVector<qint32>>("expectedResult");

        QTest::newRow("NonEmptyInput") << PointsList { QPointF(10.5, 20.9), QPointF(30.4, 40.1), QPointF(-1, -1),
            QPointF(0, 0), QPointF(1000000, 1000000), QPointF(2147483647, -2147483648) }
                                       << QVector<qint32> {
                                              11,
                                              21,
                                              30,
                                              40,
                                              -1,
                                              -1,
                                              0,
                                              0,
                                              1000000,
                                              1000000,
                                              2147483647,
                                              -2147483648,
                                          };

        QTest::newRow("EmptyInput") << PointsList {} << QVector<qint32> {};
    }

    void testCoordinates_points()
    {
        QFETCH(PointsList, points);
        QFETCH(QVector<qint32>, expectedResult);

        const auto &actualResult = coordinates(points);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestCoordinates_points)
#include "tst_coordinates_points.moc"
