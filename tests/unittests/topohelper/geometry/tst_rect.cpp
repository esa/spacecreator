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

class TestRect : public QObject
{
    Q_OBJECT

private slots:
    void testRect_data()
    {
        QTest::addColumn<QVector<qint32>>("coordinates");
        QTest::addColumn<QRectF>("expectedResult");

        QTest::newRow("ValidCase") << QVector<qint32> { -10, 20, 30, 40 } << QRectF(QPointF(-10, 20), QPointF(30, 40));

        QTest::newRow("ZeroWidth") << QVector<qint32> { 0, 0, 0, 40 } << QRectF(0, 0, 0, 40);
        QTest::newRow("NegativeWidth") << QVector<qint32> { 0, 0, -30, 40 } << QRectF(QPointF(0, 0), QPointF(-30, 40));

        QTest::newRow("ZeroHeight") << QVector<qint32> { 0, 0, 40, 0 } << QRectF(QPointF(0, 0), QPointF(40, 0));
        QTest::newRow("NegativeHeight") << QVector<qint32> { 0, 0, 40, -40 } << QRectF(QPointF(0, 0), QPointF(40, -40));

        QTest::newRow("ZeroTopLeft") << QVector<qint32> { 0, 0, 30, 40 } << QRectF(QPointF(0, 0), QPointF(30, 40));
        QTest::newRow("ZeroBottomRight") << QVector<qint32> { -10, 20, 0, 0 }
                                         << QRectF(QPointF(-10, 20), QPointF(0, 0));
        QTest::newRow("AllZeroes") << QVector<qint32> { 0, 0, 0, 0 } << QRectF(QPointF(0, 0), QPointF(0, 0));

#ifdef QT_NO_DEBUG
        QTest::newRow("EmptyCoordinates") << QVector<qint32> {} << QRectF();
        QTest::newRow("InvalidCoordinates3") << QVector<qint32> { 10, 20, 30 } << QRectF();
        QTest::newRow("InvalidCoordinates5") << QVector<qint32> { 10, 20, 30, 40, 50 } << QRectF();
#endif
    }

    void testRect()
    {
        QFETCH(QVector<qint32>, coordinates);
        QFETCH(QRectF, expectedResult);

        const auto &actualResult = rect(coordinates);

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestRect)
#include "tst_rect.moc"
