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

class TestGenerateSegmentsLine : public QObject
{
    Q_OBJECT

private slots:
    void testGenerateSegmentsLine_data()
    {
        QTest::addColumn<QLineF>("startDirection");
        QTest::addColumn<QLineF>("endDirection");
        QTest::addColumn<PointsList>("expectedResult");

        QTest::newRow("SimpleVer") << QLineF(0, 0, 0, 10) << QLineF(0, 10, 0, 20)
                                   << PointsList { QPointF(0, 10), QPointF(0, 15), QPointF(0, 15), QPointF(0, 20) };

        QTest::newRow("SimpleHor") << QLineF(0, 0, 10, 0) << QLineF(10, 0, 20, 0)
                                   << PointsList { QPointF(10, 0), QPointF(15, 0), QPointF(15, 0), QPointF(20, 0) };

        QTest::newRow("Diagonal") << QLineF(0, 0, 10, 10) << QLineF(10, 10, 20, 0)
                                  << PointsList { QPointF(10, 10), QPointF(10, 10), QPointF(20, 0) };

        QTest::newRow("Hor&Ver") << QLineF(0, 0, 10, 0) << QLineF(10, 0, 10, 10)
                                 << PointsList { QPointF(10, 0), QPointF(10, 0), QPointF(10, 10) };

        QTest::newRow("NoStart") << QLineF() << QLineF(10, 10, 20, 20) << PointsList {};

        QTest::newRow("NoEnd") << QLineF(0, 0, 10, 10) << QLineF() << PointsList {};
    }

    void testGenerateSegmentsLine()
    {
        QFETCH(QLineF, startDirection);
        QFETCH(QLineF, endDirection);
        QFETCH(PointsList, expectedResult);

        const PointsList &actualResult = generateSegments(startDirection, endDirection);
        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestGenerateSegmentsLine)
#include "tst_generateSegments_line.moc"
