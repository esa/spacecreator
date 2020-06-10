/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "geometry.h"

#include <QRect>
#include <QtTest>

class tst_Geometry : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testRectInRectKeepOriginal();
    void testRectInRectShrinkToParent();
    void testRectInRectMoveFullyInParent();
    void testRectInRectShringAndMove();
};

void tst_Geometry::testRectInRectKeepOriginal()
{
    const auto parentRect = QRect(100, 100, 500, 300);
    const auto sourceRect = QRect(200, 200, 200, 100);
    const QRect result = shared::rectInRect(sourceRect, parentRect);
    QCOMPARE(result, sourceRect);
}

void tst_Geometry::testRectInRectShrinkToParent()
{
    const auto parentRect = QRect(100, 100, 500, 300);
    const auto sourceRect = QRect(200, 200, 900, 900);
    const QRect result = shared::rectInRect(sourceRect, parentRect);
    QCOMPARE(result.width(), parentRect.width());
    QCOMPARE(result.height(), parentRect.height());
}

void tst_Geometry::testRectInRectMoveFullyInParent()
{
    // top left
    const auto parentRect = QRect(100, 100, 500, 300);
    auto sourceRect = QRect(20, 20, 200, 100);
    QRect result = shared::rectInRect(sourceRect, parentRect);
    QCOMPARE(result, QRect(100, 100, 200, 100));

    // bottom right
    sourceRect = QRect(450, 350, 200, 100);
    result = shared::rectInRect(sourceRect, parentRect);
    const QRect expected(
            parentRect.right() - sourceRect.width() + 1, parentRect.bottom() - sourceRect.height() + 1, 200, 100);
    QCOMPARE(result, expected);
}

void tst_Geometry::testRectInRectShringAndMove()
{
    const auto parentRect = QRect(100, 100, 500, 300);
    const auto sourceRect = QRect(20, 20, 900, 900);
    const QRect result = shared::rectInRect(sourceRect, parentRect);
    QCOMPARE(result, parentRect);
}

QTEST_APPLESS_MAIN(tst_Geometry)

#include "tst_geometry.moc"
