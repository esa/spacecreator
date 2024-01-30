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

#include <QDebug>
#include <QPointF>
#include <QtTest>

using namespace topohelp;
using namespace topohelp::geom;

class TestSimplifyPoints : public QObject
{
    Q_OBJECT

private slots:
    void testSimplifyPoints_data()
    {
        QTest::addColumn<PointsList>("inputPoints");
        QTest::addColumn<PointsList>("expectedResult");

        QTest::newRow("SimplifyPath") << PointsList { QPointF(0, 0), QPointF(1, 1), QPointF(2, 2), QPointF(3, 3) }
                                      << PointsList { QPointF(0, 0), QPointF(3, 3) };

        QTest::newRow("NoSimplifyTwoPoints")
                << PointsList { QPointF(0, 0), QPointF(1, 1) } << PointsList { QPointF(0, 0), QPointF(1, 1) };

        QTest::newRow("NoSimplifyOnePoint") << PointsList { QPointF(0, 0) } << PointsList { QPointF(0, 0) };
    }

    void testSimplifyPoints()
    {
        QFETCH(PointsList, inputPoints);
        QFETCH(PointsList, expectedResult);

        const PointsList &actualResult = simplifyPoints(inputPoints);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestSimplifyPoints)
#include "tst_simplifyPoints.moc"
