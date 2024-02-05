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

class TestAdjustFromPoint : public QObject
{
    Q_OBJECT

private slots:
    void testAdjustFromPoint_data()
    {
        QTest::addColumn<QPointF>("pos");
        QTest::addColumn<qreal>("adjustment");
        QTest::addColumn<QRectF>("expectedResult");

        constexpr QPointF pnt0(0., 0.);
        constexpr QPointF testPoint(50., 50.);
        constexpr qreal shift(10.);

        auto expect = [](const QPointF &c, const qreal s) {
            QRectF r(QPointF(), QSizeF(s, s));
            r.moveCenter(c);
            return r;
        };

        QTest::newRow("PositiveAdjustment") << testPoint << shift << expect(testPoint, shift);
        QTest::newRow("NegativeAdjustment") << testPoint << -shift << expect(testPoint, -shift);
        QTest::newRow("NoAdjustment") << testPoint << 0. << expect(testPoint, 0.);

        QTest::newRow("PositiveAdjustmentZero") << pnt0 << shift << expect(pnt0, shift);
        QTest::newRow("NegativeAdjustmentZero") << pnt0 << -shift << expect(pnt0, -shift);
        QTest::newRow("NoPosNoAdjustment") << pnt0 << 0. << expect(pnt0, 0.);
    }

    void testAdjustFromPoint()
    {
        QFETCH(QPointF, pos);
        QFETCH(qreal, adjustment);
        QFETCH(QRectF, expectedResult);

        const QRectF &actualResult = adjustFromPoint(pos, adjustment);
        if ((!actualResult.isValid() && !actualResult.isNull())
                || (actualResult.isEmpty() && !actualResult.topLeft().isNull())) {
            qWarning() << "Invalid rect:" << actualResult << "TODO: validate input/clean-up result";
        }

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestAdjustFromPoint)
#include "tst_adjustFromPoint.moc"
