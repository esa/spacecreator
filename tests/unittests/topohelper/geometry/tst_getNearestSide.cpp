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
#include <QtTest>

using namespace topohelp;
using namespace topohelp::geom;

class TestGetNearestSide : public QObject
{
    Q_OBJECT

private slots:
    void testGetNearestSide_data()
    {
        QTest::addColumn<QRectF>("boundingArea");
        QTest::addColumn<QPointF>("pos");
        QTest::addColumn<Qt::Alignment>("expectedResult");

        auto asFlags = [](Qt::Alignment flag) { return QFlags<Qt::AlignmentFlag>(flag); };

        constexpr QRectF testRect(0, 0, 100, 100);
        constexpr QPointF center(testRect.center());
        constexpr int padding(10);

        QTest::newRow("InsideLeft") << testRect << QPointF(testRect.left() + padding, center.y())
                                    << asFlags(Qt::AlignLeft);
        QTest::newRow("InsideTop") << testRect << QPointF(center.x(), testRect.top() + padding)
                                   << asFlags(Qt::AlignTop);
        QTest::newRow("InsideRight") << testRect << QPointF(testRect.right() - padding, center.y())
                                     << asFlags(Qt::AlignRight);
        QTest::newRow("InsideBottom") << testRect << QPointF(center.x(), testRect.bottom() - padding)
                                      << asFlags(Qt::AlignBottom);

        QTest::newRow("OutsideLeft") << testRect << QPointF(testRect.left() - padding, center.y())
                                     << asFlags(Qt::AlignLeft);
        QTest::newRow("OutsideTop") << testRect << QPointF(center.x(), testRect.top() - padding)
                                    << asFlags(Qt::AlignTop);
        QTest::newRow("OutsideRight") << testRect << QPointF(testRect.right() + padding, center.y())
                                      << asFlags(Qt::AlignRight);
        QTest::newRow("OutsideBottom") << testRect << QPointF(center.x(), testRect.bottom() + padding)
                                       << asFlags(Qt::AlignBottom);

        QTest::newRow("InvalidBoundingArea") << QRectF() << QPointF(50, 50) << asFlags(Qt::AlignCenter);
    }

    void testGetNearestSide()
    {
        QFETCH(QRectF, boundingArea);
        QFETCH(QPointF, pos);
        QFETCH(Qt::Alignment, expectedResult);

        const Qt::Alignment &actualResult = getNearestSide(boundingArea, pos);

        qDebug() << actualResult << expectedResult;

        QCOMPARE(actualResult, expectedResult);
    }
};

QTEST_MAIN(TestGetNearestSide)
#include "tst_getNearestSide.moc"
