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

namespace tmc {

class tst_Tmc_UseCases : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_dekker();
    void test_dekkerRunningWithStopCondition();
    void test_dekkerRunningWithInputObserver();
    void test_dekkerRunningWithOutputObserver();
    void test_uart();
    void test_uartRunningWithStopCondition();
    void test_uartRunningWithInputObserver();
    void test_uartRunningWithOutputObserver();
};

void tst_Tmc_UseCases::test_dekker()
{
    const int result = system("./test_dekker.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_UseCases::test_dekkerRunningWithStopCondition()
{
    const int result = system("./test_dekkerRunningWithStopCondition.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_UseCases::test_dekkerRunningWithInputObserver()
{
    const int result = system("./test_dekkerRunningWithInputObserver.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_UseCases::test_dekkerRunningWithOutputObserver()
{
    const int result = system("./test_dekkerRunningWithOutputObserver.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_UseCases::test_uart()
{
    const int result = system("./test_uart.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_UseCases::test_uartRunningWithStopCondition()
{
    const int result = system("./test_uartRunningWithStopCondition.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_UseCases::test_uartRunningWithInputObserver()
{
    const int result = system("./test_uartRunningWithInputObserver.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_UseCases::test_uartRunningWithOutputObserver()
{
    const int result = system("./test_uartRunningWithOutputObserver.sh");
    QCOMPARE(result, 0);
}

} // namespace sedsconverter

QTEST_MAIN(tmc::tst_Tmc_UseCases)

#include "tst_tmc_usecases.moc"
