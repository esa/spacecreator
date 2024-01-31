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

class TestCalcDistance : public QObject
{
    Q_OBJECT

private slots:
    void testTestCalcDistance_data()
    {
        QTest::addColumn<QPointF>("pnt1");
        QTest::addColumn<QPointF>("pnt2");
        QTest::addColumn<qreal>("expectedResult");

        QTest::newRow("SameQuadrant") << QPointF(1, 1) << QPointF(2, 1) << 1.0;
        QTest::newRow("DifferentQuadrants") << QPointF(-1, 1) << QPointF(2, 1) << 3.0;
        QTest::newRow("NegativeCoordinates") << QPointF(-1, -1) << QPointF(-4, -5) << 5.0;
        QTest::newRow("OneZeroCoordinate") << QPointF(3, 0) << QPointF(0, 4) << 5.0;
        QTest::newRow("AllZeroCoordinates") << QPointF(0, 0) << QPointF(0, 0) << 0.0;
    }

    void testTestCalcDistance()
    {
        QFETCH(QPointF, pnt1);
        QFETCH(QPointF, pnt2);
        QFETCH(qreal, expectedResult);

        const qreal &actualResult = calcDistance(pnt1, pnt2);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestCalcDistance)
#include "tst_calcDistance.moc"
