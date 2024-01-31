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

class TestIsCollided : public QObject
{
    Q_OBJECT

private slots:
    void testIsCollided_data()
    {
        QTest::addColumn<RectsList>("itemRects");
        QTest::addColumn<QRectF>("itemRect");
        QTest::addColumn<QRectF>("expectedCollidingRect");
        QTest::addColumn<bool>("expectedResult");

        constexpr QRectF emptyRect = QRectF();
        constexpr QRectF itemRect(0, 0, 10, 10);
        constexpr QRectF collidedRect10(5, 5, 10, 10);
        constexpr QRectF collidedRect15(5, 5, 15, 15);
        constexpr QRectF collidedRect25(15, 15, 25, 25);
        constexpr QRectF collidedRect30(20, 20, 30, 30);

        QTest::newRow("CollidedWithAnother")
                << RectsList { collidedRect15, collidedRect30 } << itemRect << collidedRect15 << true;

        QTest::newRow("NotCollidedWithAny")
                << RectsList { collidedRect25, collidedRect30 } << itemRect << emptyRect << false;

        QTest::newRow("CollidedWithSame")
                << RectsList { collidedRect10, collidedRect30 } << itemRect << collidedRect10 << true;

        QTest::newRow("EmptyRects") << RectsList {} << itemRect << emptyRect << false;
        QTest::newRow("EmptyRect") << RectsList { collidedRect10 } << emptyRect << emptyRect << false;
    }

    void testIsCollided()
    {
        QFETCH(RectsList, itemRects);
        QFETCH(QRectF, itemRect);
        QFETCH(QRectF, expectedCollidingRect);
        QFETCH(bool, expectedResult);

        QRectF actualCollidingRect;
        const bool actualResult = isCollided(itemRects, itemRect, &actualCollidingRect);

        QCOMPARE(actualResult, expectedResult);
        QCOMPARE(actualCollidingRect, expectedCollidingRect);
    }
};

QTEST_MAIN(TestIsCollided)
#include "tst_isCollided.moc"
