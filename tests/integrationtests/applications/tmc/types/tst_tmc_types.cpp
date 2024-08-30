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
#include <QtTest>
#include <stdlib.h>

namespace tmc::test {

class tst_Tmc_Types : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_bool();
    void test_integer();
    void test_real();
    void test_real_constraints();
    void test_real_generation();
    void test_octet();
    void test_ia5string();
    void test_enum();
    void test_choice();
    void test_sequence();
    void test_sequenceof();
    void test_choice_name_mangling();
    void test_nested_sequenceof();
};

void tst_Tmc_Types::test_bool()
{
    const int result = system("./test_bool.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Types::test_integer()
{
    const int result = system("./test_integer.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Types::test_real()
{
    const int result = system("./test_real.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Types::test_real_constraints()
{
    const int result = system("./test_real_constraints.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Types::test_real_generation()
{
    const int result = system("./test_real_generation.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Types::test_octet()
{
    const int result = system("./test_octet.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Types::test_ia5string()
{
    const int result = system("./test_ia5string.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Types::test_enum()
{
    const int result = system("./test_enum.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Types::test_choice()
{
    const int result = system("./test_choice.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Types::test_sequence()
{
    const int result = system("./test_sequence.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Types::test_sequenceof()
{
    const int result = system("./test_sequenceof.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Types::test_choice_name_mangling()
{
    const int result = system("./test_choice_name_mangling.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Types::test_nested_sequenceof()
{
    const int result = system("./test_choice_name_mangling.sh");
    QCOMPARE(result, 0);
}
} // namespace tmc::test

QTEST_MAIN(tmc::test::tst_Tmc_Types)

#include "tst_tmc_types.moc"
