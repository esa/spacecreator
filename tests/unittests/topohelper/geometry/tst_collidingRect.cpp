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

class TestCollidingRect : public QObject
{
    Q_OBJECT

private slots:
    void testCollidingRect_data()
    {
        QTest::addColumn<QRectF>("testRect");
        QTest::addColumn<RectsList>("existingRects");
        QTest::addColumn<QRectF>("expectedCollisionRect");

        constexpr qreal width(100);
        constexpr qreal height(100);
        constexpr QRectF testRect(0, 0, width, height);

        {
            constexpr QRectF first(testRect.translated(0, height + 1));
            constexpr QRectF second(testRect.translated(width + 1, 0));

            QTest::newRow("NoCollision") << testRect << RectsList { first, second } << QRectF();
        }

        {
            constexpr QRectF first(testRect.translated(0, height - 1));
            constexpr QRectF second(testRect.translated(width + 1, 0));

            QTest::newRow("CollisionWithFirst") << testRect << RectsList { first, second } << first;
        }

        {
            constexpr QRectF first(testRect.translated(0, height - 1));
            constexpr QRectF second(testRect.translated(width + 1, 0));

            QTest::newRow("CollisionWithLast") << testRect << RectsList { second, first } << first;
        }

        {
            constexpr QRectF first(testRect.adjusted(1, 1, -1, -1));
            QTest::newRow("CollisionWithChild") << testRect << RectsList { first } << first;
        }

        {
            constexpr QRectF first(testRect.adjusted(-1, -1, 1, 1));
            QTest::newRow("CollisionWithParent") << testRect << RectsList { first } << first;
        }

        QTest::newRow("CollisionWithEmpty") << testRect << RectsList { QRectF() } << QRectF();
        QTest::newRow("CollisionOfEmpty") << QRectF() << RectsList { testRect } << QRectF();
    }

    void testCollidingRect()
    {
        QFETCH(QRectF, testRect);
        QFETCH(RectsList, existingRects);
        QFETCH(QRectF, expectedCollisionRect);

        const auto &actualResult = collidingRect(testRect, existingRects);
        QCOMPARE(actualResult, expectedCollisionRect);
    }
};

QTEST_MAIN(TestCollidingRect)
#include "tst_collidingRect.moc"
