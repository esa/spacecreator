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

#include "tst_integergenerator.h"

#include <promela/Asn1ToPromelaTranslator/integergenerator.h>
#include <promela/Asn1ToPromelaTranslator/integersubset.h>

using promela::translator::IntegerGenerator;
using promela::translator::IntegerSubset;

namespace tmc::test {

void tst_IntegerGenerator::testEmptySequence()
{
    IntegerSubset a(1);
    IntegerSubset b(3);
    IntegerSubset empty = a & b;

    IntegerGenerator generator(empty);
    QVERIFY(!generator.has_next());
}

void tst_IntegerGenerator::testContinuousSequence()
{
    IntegerSubset subset(1, 4);

    IntegerGenerator generator(subset);
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

void tst_IntegerGenerator::testTwoRanges()
{
    IntegerSubset a(1, 2);
    IntegerSubset b(11, 12);
    IntegerSubset subset = a | b;
    IntegerGenerator generator(subset);
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

void tst_IntegerGenerator::testSeparatedNumbers()
{
    IntegerSubset a(1);
    IntegerSubset b(5);
    IntegerSubset c(8);
    IntegerSubset d(11);
    IntegerSubset subset = a | b | c | d;
    IntegerGenerator generator(subset);
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
