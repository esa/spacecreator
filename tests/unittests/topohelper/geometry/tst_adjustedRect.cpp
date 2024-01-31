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

class TestAdjustedRect : public QObject
{
    Q_OBJECT

private slots:
    void testAdjustedRect_data()
    {
        QTest::addColumn<QRectF>("itemRect");
        QTest::addColumn<QRectF>("intersectedItemRect");
        QTest::addColumn<Qt::Alignment>("side");
        QTest::addColumn<LookupDirection>("direction");
        QTest::addColumn<QRectF>("expectedResult");

        auto asFlags = [](Qt::Alignment flag) { return QFlags<Qt::AlignmentFlag>(flag); };

        constexpr QSizeF testItemSize(100, 100);
        constexpr QRectF testItemRect({ 0., 0. }, testItemSize);
        constexpr QRectF intersectedItemRect(100, 0, 200, 100);
        const qreal paddedSideLength = testItemRect.height() + kInterfaceLayoutOffset;

        QTest::newRow("LeftCW") << testItemRect << intersectedItemRect << asFlags(Qt::AlignLeft)
                                << LookupDirection::Clockwise
                                << QRectF({ testItemRect.left(), -paddedSideLength }, testItemSize);

        QTest::newRow("LeftCCW") << testItemRect << intersectedItemRect << asFlags(Qt::AlignLeft)
                                 << LookupDirection::CounterClockwise
                                 << QRectF({ testItemRect.left(), paddedSideLength }, testItemSize);

        QTest::newRow("RightCW") << testItemRect << intersectedItemRect << asFlags(Qt::AlignRight)
                                 << LookupDirection::Clockwise
                                 << QRectF({ testItemRect.left(), paddedSideLength }, testItemSize);

        QTest::newRow("RightCCW") << testItemRect << intersectedItemRect << asFlags(Qt::AlignRight)
                                  << LookupDirection::CounterClockwise
                                  << QRectF({ testItemRect.left(), -paddedSideLength }, testItemSize);

        QTest::newRow("TopCW") << testItemRect << intersectedItemRect << asFlags(Qt::AlignTop)
                               << LookupDirection::Clockwise
                               << QRectF({ intersectedItemRect.width() + paddedSideLength, testItemRect.top() },
                                          testItemSize);
        QTest::newRow("TopCCW") << testItemRect << intersectedItemRect << asFlags(Qt::AlignTop)
                                << LookupDirection::CounterClockwise
                                << QRectF({ testItemRect.left() - kInterfaceLayoutOffset, testItemRect.top() },
                                           testItemSize);

        QTest::newRow("BottomCW") << testItemRect << intersectedItemRect << asFlags(Qt::AlignBottom)
                                  << LookupDirection::Clockwise
                                  << QRectF({ testItemRect.left() - kInterfaceLayoutOffset, testItemRect.top() },
                                             testItemSize);
        QTest::newRow("BottomCCW") << testItemRect << intersectedItemRect << asFlags(Qt::AlignBottom)
                                   << LookupDirection::CounterClockwise
                                   << QRectF({ intersectedItemRect.width() + paddedSideLength, testItemRect.top() },
                                              testItemSize);

        QTest::newRow("UnhandledCW") << testItemRect << testItemRect << asFlags(Qt::AlignCenter)
                                     << LookupDirection::Clockwise << QRectF();

        QTest::newRow("UnhandledCCW") << testItemRect << testItemRect << asFlags(Qt::AlignCenter)
                                      << LookupDirection::CounterClockwise << QRectF();
    }

    void testAdjustedRect()
    {
        QFETCH(QRectF, itemRect);
        QFETCH(QRectF, intersectedItemRect);
        QFETCH(Qt::Alignment, side);
        QFETCH(LookupDirection, direction);
        QFETCH(QRectF, expectedResult);

        const QRectF &actualResult = adjustedRect(itemRect, intersectedItemRect, side, direction);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestAdjustedRect)
#include "tst_adjustedRect.moc"
