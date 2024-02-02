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

class TestPos : public QObject
{
    Q_OBJECT

private slots:
    void testPos_data()
    {
        QTest::addColumn<QVector<qint32>>("coordinates");
        QTest::addColumn<QPointF>("expectedResult");

        constexpr QPointF testPointF(10, 20);
        const QPoint &testPoint = testPointF.toPoint();
        constexpr QPointF noPoint;

        QTest::newRow("Empty") << QVector<qint32> {} << noPoint;

#ifdef QT_NO_DEBUG // avoid the assertion
        QTest::newRow("Invalid") << QVector<qint32> { testPoint.x() } << noPoint;
#endif
        QTest::newRow("Valid") << QVector<qint32> { testPoint.x(), testPoint.y() } << testPointF;
    }

    void testPos()
    {
        QFETCH(QVector<qint32>, coordinates);
        QFETCH(QPointF, expectedResult);

        const auto &actualResult = pos(coordinates);
        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestPos)
#include "tst_pos.moc"
