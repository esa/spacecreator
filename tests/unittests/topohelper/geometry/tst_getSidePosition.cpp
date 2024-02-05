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

class TestGetSidePosition : public QObject
{
    Q_OBJECT

private slots:
    void testGetSidePosition_data()
    {
        QTest::addColumn<QRectF>("boundingArea");
        QTest::addColumn<QPointF>("pos");
        QTest::addColumn<Qt::Alignment>("side");
        QTest::addColumn<QPointF>("expectedResult");

        auto asFlags = [](Qt::Alignment flag) { return QFlags<Qt::AlignmentFlag>(flag); };

        constexpr QRectF testRect(0, 0, 100, 100);
        static const QPointF &center = testRect.center();
        constexpr QRectF noRect = QRectF();
        constexpr QPointF noPoint = QPointF();

        QTest::newRow("Left") << testRect << center << asFlags(Qt::AlignLeft) << QPointF(testRect.left(), center.y());
        QTest::newRow("LeftNoRect") << noRect << center << asFlags(Qt::AlignLeft) << noPoint;
        QTest::newRow("LeftNoPoint") << testRect << noPoint << asFlags(Qt::AlignLeft) << noPoint;

        QTest::newRow("Right") << testRect << center << asFlags(Qt::AlignRight)
                               << QPointF(testRect.right(), center.y());
        QTest::newRow("RightNoRect") << noRect << center << asFlags(Qt::AlignRight) << noPoint;
        QTest::newRow("RightNoPoint") << testRect << noPoint << asFlags(Qt::AlignRight) << QPointF(testRect.right(), 0);

        QTest::newRow("Top") << testRect << center << asFlags(Qt::AlignTop) << QPointF(center.x(), testRect.top());
        QTest::newRow("TopNoRect") << noRect << center << asFlags(Qt::AlignTop) << noPoint;
        QTest::newRow("TopNoPoint") << testRect << noPoint << asFlags(Qt::AlignTop) << noPoint;

        QTest::newRow("Bottom") << testRect << center << asFlags(Qt::AlignBottom)
                                << QPointF(center.x(), testRect.bottom());
        QTest::newRow("BottomNoRect") << noRect << center << asFlags(Qt::AlignBottom) << noPoint;
        QTest::newRow("BottomNoPoint") << testRect << noPoint << asFlags(Qt::AlignBottom)
                                       << QPointF(0, testRect.bottom());

        QTest::newRow("Center") << testRect << center << asFlags(Qt::AlignCenter) << center;
    }

    void testGetSidePosition()
    {
        QFETCH(QRectF, boundingArea);
        QFETCH(QPointF, pos);
        QFETCH(Qt::Alignment, side);
        QFETCH(QPointF, expectedResult);

        const auto &actualResult = getSidePosition(boundingArea, pos, side);
        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestGetSidePosition)
#include "tst_getSidePosition.moc"
