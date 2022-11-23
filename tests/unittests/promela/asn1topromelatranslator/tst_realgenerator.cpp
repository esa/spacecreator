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

#include "tst_realgenerator.h"

#include <promela/Asn1ToPromelaTranslator/realgenerator.h>
#include <promela/Asn1ToPromelaTranslator/realsubset.h>

using promela::translator::RealGenerator;
using promela::translator::RealSubset;

namespace tmc::test {

void tst_RealGenerator::testEmptySequence()
{
    RealSubset a(1);
    RealSubset b(3);
    RealSubset empty = a & b;

    RealGenerator generator(empty, 1);
    QVERIFY(!generator.has_next());
}

void tst_RealGenerator::testContinuousSequence()
{
    RealSubset subset(1, 4);

    RealGenerator generator(subset, 1);
    QVERIFY(generator.has_next());
    QCOMPARE(generator.next(), 1);
    QVERIFY(generator.has_next());
    QCOMPARE(generator.next(), 2);
    QVERIFY(generator.has_next());
    QCOMPARE(generator.next(), 3);
    QVERIFY(generator.has_next());
    QCOMPARE(generator.next(), 4);
    QVERIFY(!generator.has_next());
}

void tst_RealGenerator::testTwoRanges()
{
    RealSubset a(1, 2);
    RealSubset b(11, 12);
    RealSubset subset = a | b;
    RealGenerator generator(subset, 1);
    QVERIFY(generator.has_next());
    QCOMPARE(generator.next(), 1);
    QVERIFY(generator.has_next());
    QCOMPARE(generator.next(), 2);
    QVERIFY(generator.has_next());
    QCOMPARE(generator.next(), 11);
    QVERIFY(generator.has_next());
    QCOMPARE(generator.next(), 12);
    QVERIFY(!generator.has_next());
}

void tst_RealGenerator::testSeparatedNumbers()
{
    RealSubset a(1);
    RealSubset b(5);
    RealSubset c(8);
    RealSubset d(11);
    RealSubset subset = a | b | c | d;
    RealGenerator generator(subset, 1);
    QVERIFY(generator.has_next());
    QCOMPARE(generator.next(), 1);
    QVERIFY(generator.has_next());
    QCOMPARE(generator.next(), 5);
    QVERIFY(generator.has_next());
    QCOMPARE(generator.next(), 8);
    QVERIFY(generator.has_next());
    QCOMPARE(generator.next(), 11);
    QVERIFY(!generator.has_next());
}

}
