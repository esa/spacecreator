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

#include <QDebug>
#include <QPointF>
#include <QtTest>

using namespace topohelp;
using namespace topohelp::geom;

class TestIfaceSegment : public QObject
{
    Q_OBJECT

private slots:
    void testIfaceSegment_data()
    {
        QTest::addColumn<QRectF>("sceneRect");
        QTest::addColumn<QPointF>("firstEndPoint");
        QTest::addColumn<QPointF>("lastEndPoint");
        QTest::addColumn<QLineF>("expectedResult");

        QTest::newRow("NormalScenario") << QRectF(0, 0, 100, 100) << QPointF(10, 10) << QPointF(90, 90)
                                        << QLineF(10, 10, 26, 10);

        QTest::newRow("NullVector") << QRectF(0, 0, 100, 100) << QPointF(50, 50) << QPointF(50, 50)
                                    << QLineF(50, 50, 66, 50);

        QTest::newRow("LastEndPointOutside")
                << QRectF(0, 0, 100, 100) << QPointF(10, 10) << QPointF(110, 110) << QLineF(10, 10, -6, 10);
    }

    void testIfaceSegment()
    {
        QFETCH(QRectF, sceneRect);
        QFETCH(QPointF, firstEndPoint);
        QFETCH(QPointF, lastEndPoint);
        QFETCH(QLineF, expectedResult);

        const QLineF &actualResult = ifaceSegment(sceneRect, firstEndPoint, lastEndPoint);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestIfaceSegment)
#include "tst_ifaceSegment.moc"
