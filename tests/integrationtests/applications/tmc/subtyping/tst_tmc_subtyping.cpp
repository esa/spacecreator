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

class tst_Tmc_Subtyping : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_subtyping();
    void test_sequenceof_fixed_size();
    void test_ia5string_fixed_size();
    void test_octetstring_fixed_size();
    void test_octetstring();
    void test_octetstring_sequenceof();
};

void tst_Tmc_Subtyping::test_subtyping()
{
    const int result = system("./test_subtyping.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Subtyping::test_sequenceof_fixed_size()
{
    const int result = system("./test_sequenceof_fixed_size.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Subtyping::test_ia5string_fixed_size()
{
    const int result = system("./test_ia5string_fixed_size.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Subtyping::test_octetstring_fixed_size()
{
    const int result = system("./test_octetstring_fixed_size.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Subtyping::test_octetstring()
{
    const int result = system("./test_octetstring.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Subtyping::test_octetstring_sequenceof()
{
    const int result = system("./test_octetstring_with_sequenceof.sh");
    QCOMPARE(result, 0);
}

} // namespace tmc::test

QTEST_MAIN(tmc::test::tst_Tmc_Subtyping)

#include "tst_tmc_subtyping.moc"
