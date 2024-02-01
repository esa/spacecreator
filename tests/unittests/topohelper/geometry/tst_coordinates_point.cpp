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

#include <QPointF>
#include <QtTest>

using namespace topohelp;
using namespace topohelp::geom;

class TestCoordinates_point : public QObject
{
    Q_OBJECT

private slots:
    void testCoordinates_point_data()
    {
        QTest::addColumn<QPointF>("point");
        QTest::addColumn<QVector<qint32>>("expectedResult");

        QTest::newRow("Ceil") << QPointF(10.5, 20.9) << QVector<qint32> { 11, 21 };
        QTest::newRow("Floor") << QPointF(30.4, 40.1) << QVector<qint32> { 30, 40 };
        QTest::newRow("EmptyInput") << QPointF {} << QVector<qint32> {};
        QTest::newRow("Zero") << QPointF(0, 0) << QVector<qint32> {}; // TODO: I'd expect {0,0} here…
        QTest::newRow("Large") << QPointF(1000000, 1000000) << QVector<qint32> { 1000000, 1000000 };
        QTest::newRow("MaxMin") << QPointF(2147483647, -2147483648) << QVector<qint32> { 2147483647, -2147483648 };
    }

    void testCoordinates_point()
    {
        QFETCH(QPointF, point);
        QFETCH(QVector<qint32>, expectedResult);

        const auto &actualResult = coordinates(point);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestCoordinates_point)
#include "tst_coordinates_point.moc"
