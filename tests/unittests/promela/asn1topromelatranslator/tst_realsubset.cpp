/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "tst_realsubset.h"

#include <QObject>
#include <QtTest>
#include <promela/Asn1ToPromelaTranslator/realsubset.h>

using promela::translator::RealSubset;

namespace tmc::test {

void tst_RealSubset::testSimpleSumTwoNumbersA()
{
    RealSubset a(1);
    RealSubset b(2);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 2);
    QCOMPARE(result.getMin(), 1);
    QCOMPARE(result.getMax(), 2);
}

void tst_RealSubset::testSimpleSumTwoNumbersB()
{
    RealSubset a(1);
    RealSubset b(5);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 2);
    QCOMPARE(result.getRanges().front(), std::make_pair(1.f, 1.f));
    QCOMPARE(result.getRanges().back(), std::make_pair(5.f, 5.f));
}

void tst_RealSubset::testSimpleSumSameNumber()
{
    RealSubset a(2);
    RealSubset b(2);

    RealSubset result = a | b;

    QVERIFY(result.isSingleValue());
    QCOMPARE(result.getMin(), 2);
    QCOMPARE(result.getMax(), 2);
    QCOMPARE(result.getRanges().size(), 1);
}

void tst_RealSubset::testSumRangeAndNumberSeparatedLeft()
{
    RealSubset a(3, 10);
    RealSubset b(1);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());

    QCOMPARE(result.getRanges().size(), 2);
    QCOMPARE(result.getRanges().front(), std::make_pair(1.f, 1.f));
    QCOMPARE(result.getRanges().back(), std::make_pair(3.f, 10.f));
}

void tst_RealSubset::testSumRangeAndNumberSeparatedRight()
{
    RealSubset a(3, 5);
    RealSubset b(7);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());

    QCOMPARE(result.getRanges().size(), 2);
    QCOMPARE(result.getRanges().front(), std::make_pair(3.f, 5.f));
    QCOMPARE(result.getRanges().back(), std::make_pair(7.f, 7.f));
}

void tst_RealSubset::testSumRangeAndNumberIncludedMiddle()
{
    RealSubset a(1, 10);
    RealSubset b(5);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(1.f, 10.f));
}

void tst_RealSubset::testSumRangeAndNumberIncludedLeft()
{
    RealSubset a(1, 10);
    RealSubset b(1);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(1.f, 10.f));
}

void tst_RealSubset::testSumRangeAndNumberIncludedRight()
{
    RealSubset a(1, 10);
    RealSubset b(10);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(1.f, 10.f));
}

void tst_RealSubset::testSumRangeAndNumberNextLeft()
{
    RealSubset a(2, 10);
    RealSubset b(1);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 2);
    QCOMPARE(result.getRanges().front(), std::make_pair(1.f, 1.f));
    QCOMPARE(result.getRanges().back(), std::make_pair(2.f, 10.f));
}

void tst_RealSubset::testSumRangeAndNumberNextRight()
{
    RealSubset a(1, 9);
    RealSubset b(10);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 2);
    QCOMPARE(result.getRanges().front(), std::make_pair(1.f, 9.f));
    QCOMPARE(result.getRanges().back(), std::make_pair(10.f, 10.f));
}

void tst_RealSubset::testSumTwoRangesSeparated()
{
    RealSubset a(1, 3);
    RealSubset b(7, 10);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 2);
    QCOMPARE(result.getRanges().front(), std::make_pair(1.f, 3.f));
    QCOMPARE(result.getRanges().back(), std::make_pair(7.f, 10.f));
}

void tst_RealSubset::testSumTwoRangesSeparatedNext()
{
    RealSubset a(1, 3);
    RealSubset b(4, 10);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 2);
}

void tst_RealSubset::testSumTwoRangesIncludedMiddle()
{
    RealSubset a(1, 10);
    RealSubset b(3, 6);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(1.f, 10.f));
}

void tst_RealSubset::testSumTwoRangesIncludedLeft()
{
    RealSubset a(1, 5);
    RealSubset b(2, 10);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(1.f, 10.f));
}

void tst_RealSubset::testSumTwoRangesIncludedRight()
{
    RealSubset a(2, 10);
    RealSubset b(1, 5);

    RealSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(1.f, 10.f));
}

void tst_RealSubset::testSumThreeRanges()
{
    RealSubset a(1, 3);
    RealSubset b(7, 10);

    RealSubset c = a | b;

    RealSubset d(2, 8);

    RealSubset result = c | d;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(1.f, 10.f));
}

void tst_RealSubset::testIntersectTwoNumbers()
{
    RealSubset a(2, 2);
    RealSubset b(3, 3);

    RealSubset result = a & b;

    QVERIFY(result.isEmpty());
}

void tst_RealSubset::testIntersectSameNumber()
{
    RealSubset a(2, 2);
    RealSubset b(2, 2);

    RealSubset result = a & b;

    QVERIFY(result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(2.f, 2.f));
}

void tst_RealSubset::testIntersectRangeNumberMiddle()
{
    RealSubset a(1, 10);
    RealSubset b(2, 2);

    RealSubset result = a & b;

    QVERIFY(result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(2.f, 2.f));
}

void tst_RealSubset::testIntersectRangeNumberLeft()
{
    RealSubset a(1, 10);
    RealSubset b(1, 1);

    RealSubset result = a & b;

    QVERIFY(result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(1.f, 1.f));
}

void tst_RealSubset::testIntersectRangeNumberRight()
{
    RealSubset a(1, 10);
    RealSubset b(10, 10);

    RealSubset result = a & b;

    QVERIFY(result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(10.f, 10.f));
}

void tst_RealSubset::testIntersectRangesSeparated()
{
    RealSubset a(1, 5);
    RealSubset b(6, 10);

    RealSubset result = a & b;

    QVERIFY(result.isEmpty());
}

void tst_RealSubset::testIntersectRangesEdge()
{
    RealSubset a(1, 5);
    RealSubset b(5, 10);

    RealSubset result = a & b;

    QVERIFY(result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(5.f, 5.f));
}

void tst_RealSubset::testIntersectRangesIncludedMiddle()
{
    RealSubset a(1, 10);
    RealSubset b(2, 5);

    RealSubset result = a & b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(2.f, 5.f));
}

void tst_RealSubset::testIntersectRangesIncludedLeft()
{
    RealSubset a(1, 5);
    RealSubset b(2, 10);

    RealSubset result = a & b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(2.f, 5.f));
}

void tst_RealSubset::testIntersectRangesIncludedRight()
{
    RealSubset a(2, 10);
    RealSubset b(1, 5);

    RealSubset result = a & b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), std::make_pair(2.f, 5.f));
}

void tst_RealSubset::testIntersectThreeRanges()
{
    RealSubset a(1, 3);
    RealSubset b(7, 10);

    RealSubset c = a | b;

    RealSubset d(2, 8);

    RealSubset result = c & d;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 2);
    QCOMPARE(result.getRanges().front(), std::make_pair(2.f, 3.f));
    QCOMPARE(result.getRanges().back(), std::make_pair(7.f, 8.f));
}

}
