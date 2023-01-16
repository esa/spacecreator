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

#include <QObject>
#include <QProcess>
#include <QtTest>
#include <stdlib.h>

namespace tmc::test {

class tst_Tmc_Subtyping_Constraints : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_basic_absent();
    void test_basic_present();
    void test_choice_absent();
    void test_choice_present();
    void test_constrained_absent();
};

void tst_Tmc_Subtyping_Constraints::test_basic_absent()
{
    const int result = system("timeout 30s ./test_basic_absent.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Subtyping_Constraints::test_basic_present()
{
    const int result = system("timeout 30s ./test_basic_present.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Subtyping_Constraints::test_choice_absent()
{
    const int result = system("timeout 30s ./test_choice_absent.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Subtyping_Constraints::test_choice_present()
{
    const int result = system("timeout 30s ./test_choice_present.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Subtyping_Constraints::test_constrained_absent()
{
    const int result = system("timeout 30s ./test_constrained_absent.sh");
    QCOMPARE(result, 0);
}

} // namespace tmc::test

QTEST_MAIN(tmc::test::tst_Tmc_Subtyping_Constraints)

#include "tst_tmc_subtyping_constraints.moc"
