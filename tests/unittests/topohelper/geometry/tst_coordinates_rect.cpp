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

class TestCoordinates_rect : public QObject
{
    Q_OBJECT

private slots:
    void testCoordinates_rect_data()
    {
        QTest::addColumn<QRectF>("rect");
        QTest::addColumn<QVector<qint32>>("expectedResult");

        constexpr QPointF point100x100(100, 100);
        QTest::newRow("CeilFloor_1") << QRectF(QPointF(10.5, 20.9), QPointF(30.4, 40.1))
                                     << QVector<qint32> { 11, 21, 30, 40 };
        QTest::newRow("CeilFloor_2") << QRectF(QPointF(30.4, 10.5), QPointF(40.1, 20.9))
                                     << QVector<qint32> { 30, 11, 40, 21 };

        QTest::newRow("EmptyInput") << QRectF {} << QVector<qint32> {};
        QTest::newRow("Zero") << QRectF(QPointF(0, 0), point100x100) << QVector<qint32> { 0, 0, 100, 100 };
        QTest::newRow("LargeOK") << QRectF(point100x100, QPointF(1000000, 1000000))
                                 << QVector<qint32> { 100, 100, 1000000, 1000000 };
        QTest::newRow("MaxMinOK_1") << QRectF(QPointF(-2147483648, -2147483648), QPointF(2147483647, 2147483647))
                                    << QVector<qint32> { -2147483648, -2147483648, 2147483647, 2147483647 };
        QTest::newRow("MaxMinOK_2") << QRectF(QPointF(100, -2147483648), QPointF(2147483647, -100))
                                    << QVector<qint32> { 100, -2147483648, 2147483647, -100 };

        // negative WxH:
        QTest::newRow("Large!OK") << QRectF(QPointF(1000000, 1000000), point100x100) << QVector<qint32> {};
        QTest::newRow("MaxMin!OK_1") << QRectF(QPointF(2147483647, -2147483648), point100x100) << QVector<qint32> {};
        QTest::newRow("MaxMin!OK_2") << QRectF(point100x100, QPointF(2147483647, -2147483648)) << QVector<qint32> {};
    }

    void testCoordinates_rect()
    {
        QFETCH(QRectF, rect);
        QFETCH(QVector<qint32>, expectedResult);

        const auto &actualResult = coordinates(rect);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestCoordinates_rect)
#include "tst_coordinates_rect.moc"
