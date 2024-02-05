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

class TestIntersectsLine : public QObject
{
    Q_OBJECT

private slots:
    void testIntersectsLine_data()
    {
        QTest::addColumn<QRectF>("rect");
        QTest::addColumn<QLineF>("line");
        QTest::addColumn<bool>("expectedResult");
        QTest::addColumn<QPointF>("expectedIntersectPos");

        constexpr QRectF testRect(0, 0, 10, 10);
        constexpr QLineF intersectBottomRight(5, 5, 15, 15);

        QTest::newRow("SimpleIntersection") << testRect << intersectBottomRight << true << QPointF(10, 10);
        QTest::newRow("NoIntersectionNoRect") << QRectF() << intersectBottomRight << false << QPointF();
        QTest::newRow("NoIntersectionNoLine") << testRect << QLineF() << false << QPointF();
        QTest::newRow("NoIntersectionNonIntersectingLine") << testRect << QLineF(15, 0, 15, 10) << false << QPointF();
        QTest::newRow("IntersectionWithOneSide") << testRect << QLineF(5, 5, 15, 5) << true << QPointF(10, 5);
        QTest::newRow("IntersectionDiagonal") << testRect << QLineF(-15, -15, 15, 15) << true << testRect.topLeft();
    }

    void testIntersectsLine()
    {
        QFETCH(QRectF, rect);
        QFETCH(QLineF, line);
        QFETCH(bool, expectedResult);
        QFETCH(QPointF, expectedIntersectPos);

        QPointF actualIntersectPos;
        const bool actualResult = intersects(rect, line, &actualIntersectPos);

        QCOMPARE(actualResult, expectedResult);
        if (expectedResult && actualResult) {
            QCOMPARE(actualIntersectPos, expectedIntersectPos);
        }
    }
};

QTEST_MAIN(TestIntersectsLine)
#include "tst_intersects_line.moc"
