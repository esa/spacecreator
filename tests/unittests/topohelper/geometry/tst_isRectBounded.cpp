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

class TestIsRectBounded : public QObject
{
    Q_OBJECT

private slots:
    void testIsRectBounded_data()
    {
        QTest::addColumn<QRectF>("outerRect");
        QTest::addColumn<QRectF>("innerRect");
        QTest::addColumn<bool>("expectedResult");

        constexpr QRectF testRect(0, 0, 100, 100);
        QTest::newRow("InnerInside") << testRect << QRectF(10, 10, 90, 90) << true;
        QTest::newRow("InnerOutside") << testRect << QRectF(-10, -10, 120, 120) << false;
        QTest::newRow("OuterOutside") << QRectF(-10, -10, 120, 120) << testRect << true;
        QTest::newRow("InnerOutsideTopLeft") << testRect << QRectF(-10, -10, 100, 100) << false;
        QTest::newRow("InnerOutsideBottomRight") << testRect << QRectF(50, 50, 100, 100) << false;
        QTest::newRow("InvalidOuter") << QRectF() << QRectF(10, 10, 20, 20) << false;
        QTest::newRow("InvalidInner") << testRect << QRectF() << true;
        QTest::newRow("InvalidRects") << QRectF() << QRectF() << true;
    }

    void testIsRectBounded()
    {
        QFETCH(QRectF, outerRect);
        QFETCH(QRectF, innerRect);
        QFETCH(bool, expectedResult);

        const bool actualResult = isRectBounded(outerRect, innerRect);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestIsRectBounded)
#include "tst_isRectBounded.moc"
