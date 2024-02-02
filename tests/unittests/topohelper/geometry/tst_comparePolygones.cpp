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

class TestComparePolygones : public QObject
{
    Q_OBJECT

private slots:
    void testComparePolygones_data()
    {
        QTest::addColumn<PointsList>("polygon1");
        QTest::addColumn<PointsList>("polygon2");
        QTest::addColumn<bool>("expectedResult");

        static const PointsList points012 { { 0, 0 }, { 1, 1 }, { 2, 2 } };
        static const PointsList emptyList {};

        QTest::newRow("BothEmpty") << emptyList << emptyList << true;

        QTest::newRow("FirstEmpty") << emptyList << points012 << false;

        QTest::newRow("SecondEmpty") << points012 << emptyList << false;

        QTest::newRow("Equal") << points012 << points012 << true;

        QTest::newRow("DifferentSize") << points012 << PointsList({ { 0, 0 }, { 1, 1 } }) << false;

        QTest::newRow("DifferentPolygons") << points012 << PointsList({ { 5, 4 }, { 3, 2 }, { 1, 0 } }) << false;
    }

    void testComparePolygones()
    {
        QFETCH(PointsList, polygon1);
        QFETCH(PointsList, polygon2);
        QFETCH(bool, expectedResult);

        const auto &actualResult = comparePolygones(polygon1, polygon2);
        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestComparePolygones)
#include "tst_comparePolygones.moc"
