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

#pragma once

#include <QObject>
#include <QtTest>

namespace tmc::test {

class tst_IntegerSubset : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSimpleSumTwoNumbersA();
    void testSimpleSumTwoNumbersB();
    void testSimpleSumSameNumber();

    void testSumRangeAndNumberSeparatedLeft();
    void testSumRangeAndNumberSeparatedRight();

    void testSumRangeAndNumberIncludedMiddle();
    void testSumRangeAndNumberIncludedLeft();
    void testSumRangeAndNumberIncludedRight();

    void testSumRangeAndNumberNextLeft();
    void testSumRangeAndNumberNextRight();

    void testSumTwoRangesSeparated();
    void testSumTwoRangesSeparatedNext();

    void testSumTwoRangesIncludedMiddle();
    void testSumTwoRangesIncludedLeft();
    void testSumTwoRangesIncludedRight();

    void testSumThreeRanges();

    void testIntersectTwoNumbers();
    void testIntersectSameNumber();

    void testIntersectRangeNumberMiddle();
    void testIntersectRangeNumberLeft();
    void testIntersectRangeNumberRight();

    void testIntersectRangesSeparated();
    void testIntersectRangesEdge();

    void testIntersectRangesIncludedMiddle();
    void testIntersectRangesIncludedLeft();
    void testIntersectRangesIncludedRight();

    void testIntersectThreeRanges();
};

}
