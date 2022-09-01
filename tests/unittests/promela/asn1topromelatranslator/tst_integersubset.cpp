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

#include "tst_integersubset.h"

#include <QObject>
#include <QtTest>
#include <promela/Asn1ToPromelaTranslator/integersubset.h>

using promela::translator::IntegerSubset;

namespace tmc::test {

void tst_IntegerSubset::testSimpleSumTwoNumbersA()
{
    IntegerSubset a(1);
    IntegerSubset b(2);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getMin(), 1);
    QCOMPARE(result.getMax(), 2);
}

void tst_IntegerSubset::testSimpleSumTwoNumbersB()
{
    IntegerSubset a(1);
    IntegerSubset b(5);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 2);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 1l)});
    QCOMPARE(result.getRanges().back(), {std::make_pair(5l, 5l)});
}

void tst_IntegerSubset::testSimpleSumSameNumber()
{
    IntegerSubset a(2);
    IntegerSubset b(2);

    IntegerSubset result = a | b;

    QVERIFY(result.isSingleValue());
    QCOMPARE(result.getMin(), 2);
    QCOMPARE(result.getMax(), 2);
    QCOMPARE(result.getRanges().size(), 1);
}

void tst_IntegerSubset::testSumRangeAndNumberSeparatedLeft()
{
    IntegerSubset a(3, 10);
    IntegerSubset b(1);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());

    QCOMPARE(result.getRanges().size(), 2);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 1l)});
    QCOMPARE(result.getRanges().back(), {std::make_pair(3l, 10l)});
}

void tst_IntegerSubset::testSumRangeAndNumberSeparatedRight()
{
    IntegerSubset a(3, 5);
    IntegerSubset b(7);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());

    QCOMPARE(result.getRanges().size(), 2);
    QCOMPARE(result.getRanges().front(), {std::make_pair(3l, 5l)});
    QCOMPARE(result.getRanges().back(), {std::make_pair(7l, 7l)});
}

void tst_IntegerSubset::testSumRangeAndNumberIncludedMiddle()
{
    IntegerSubset a(1, 10);
    IntegerSubset b(5);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 10l)});
}

void tst_IntegerSubset::testSumRangeAndNumberIncludedLeft()
{
    IntegerSubset a(1, 10);
    IntegerSubset b(1);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 10l)});
}

void tst_IntegerSubset::testSumRangeAndNumberIncludedRight()
{
    IntegerSubset a(1, 10);
    IntegerSubset b(10);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 10l)});
}

void tst_IntegerSubset::testSumRangeAndNumberNextLeft()
{
    IntegerSubset a(2, 10);
    IntegerSubset b(1);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 10l)});
}

void tst_IntegerSubset::testSumRangeAndNumberNextRight()
{
    IntegerSubset a(1, 9);
    IntegerSubset b(10);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 10l)});
}

void tst_IntegerSubset::testSumTwoRangesSeparated()
{
    IntegerSubset a(1, 3);
    IntegerSubset b(7, 10);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 2);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 3l)});
    QCOMPARE(result.getRanges().back(), {std::make_pair(7l, 10l)});
}

void tst_IntegerSubset::testSumTwoRangesSeparatedNext()
{
    IntegerSubset a(1, 3);
    IntegerSubset b(4, 10);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 10l)});
}

void tst_IntegerSubset::testSumTwoRangesIncludedMiddle()
{
    IntegerSubset a(1, 10);
    IntegerSubset b(3, 6);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 10l)});
}

void tst_IntegerSubset::testSumTwoRangesIncludedLeft()
{
    IntegerSubset a(1, 5);
    IntegerSubset b(2, 10);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 10l)});
}

void tst_IntegerSubset::testSumTwoRangesIncludedRight()
{
    IntegerSubset a(2, 10);
    IntegerSubset b(1, 5);

    IntegerSubset result = a | b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 10l)});
}

void tst_IntegerSubset::testSumThreeRanges()
{
    IntegerSubset a(1, 3);
    IntegerSubset b(7, 10);

    IntegerSubset c = a | b;

    IntegerSubset d(2, 8);

    IntegerSubset result = c | d;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 10l)});
}

void tst_IntegerSubset::testIntersectTwoNumbers()
{
    IntegerSubset a(2, 2);
    IntegerSubset b(3, 3);

    IntegerSubset result = a & b;

    QVERIFY(result.isEmpty());
}

void tst_IntegerSubset::testIntersectSameNumber()
{
    IntegerSubset a(2, 2);
    IntegerSubset b(2, 2);

    IntegerSubset result = a & b;

    QVERIFY(result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(2l, 2l)});
}

void tst_IntegerSubset::testIntersectRangeNumberMiddle()
{
    IntegerSubset a(1, 10);
    IntegerSubset b(2, 2);

    IntegerSubset result = a & b;

    QVERIFY(result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(2l, 2l)});
}

void tst_IntegerSubset::testIntersectRangeNumberLeft()
{
    IntegerSubset a(1, 10);
    IntegerSubset b(1, 1);

    IntegerSubset result = a & b;

    QVERIFY(result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(1l, 1l)});
}

void tst_IntegerSubset::testIntersectRangeNumberRight()
{
    IntegerSubset a(1, 10);
    IntegerSubset b(10, 10);

    IntegerSubset result = a & b;

    QVERIFY(result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(10l, 10l)});
}

void tst_IntegerSubset::testIntersectRangesSeparated()
{
    IntegerSubset a(1, 5);
    IntegerSubset b(6, 10);

    IntegerSubset result = a & b;

    QVERIFY(result.isEmpty());
}

void tst_IntegerSubset::testIntersectRangesEdge()
{
    IntegerSubset a(1, 5);
    IntegerSubset b(5, 10);

    IntegerSubset result = a & b;

    QVERIFY(result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(5l, 5l)});
}

void tst_IntegerSubset::testIntersectRangesIncludedMiddle()
{
    IntegerSubset a(1, 10);
    IntegerSubset b(2, 5);

    IntegerSubset result = a & b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(2l, 5l)});
}

void tst_IntegerSubset::testIntersectRangesIncludedLeft()
{
    IntegerSubset a(1, 5);
    IntegerSubset b(2, 10);

    IntegerSubset result = a & b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(2l, 5l)});
}

void tst_IntegerSubset::testIntersectRangesIncludedRight()
{
    IntegerSubset a(2, 10);
    IntegerSubset b(1, 5);

    IntegerSubset result = a & b;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 1);
    QCOMPARE(result.getRanges().front(), {std::make_pair(2l, 5l)});
}

void tst_IntegerSubset::testIntersectThreeRanges()
{
    IntegerSubset a(1, 3);
    IntegerSubset b(7, 10);

    IntegerSubset c = a | b;

    IntegerSubset d(2, 8);

    IntegerSubset result = c & d;

    QVERIFY(!result.isSingleValue());
    QCOMPARE(result.getRanges().size(), 2);
    QCOMPARE(result.getRanges().front(), {std::make_pair(2l, 3l)});
    QCOMPARE(result.getRanges().back(), {std::make_pair(7l, 8l)});
}

}
