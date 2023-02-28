/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2023 N7 Space Sp. z o.o.
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

#include <QObject>
#include <QtTest>
#include <stdlib.h>

namespace tmc::test {

class tst_Tmc_Multicast : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_sporadic_many_to_one();
    void test_sporadic_one_to_many();

    void test_protected_many_to_one();
    void test_protected_one_to_many();
};

void tst_Tmc_Multicast::test_sporadic_many_to_one()
{
    const int result = system("./test_sporadic_many_to_one.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Multicast::test_sporadic_one_to_many()
{
    const int result = system("./test_sporadic_one_to_many.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Multicast::test_protected_many_to_one()
{
    const int result = system("./test_protected_many_to_one.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Multicast::test_protected_one_to_many()
{
    const int result = system("./test_protected_one_to_many.sh");
    QCOMPARE(result, 0);
}
}

QTEST_MAIN(tmc::test::tst_Tmc_Multicast)

#include "tst_tmc_multicast.moc"
