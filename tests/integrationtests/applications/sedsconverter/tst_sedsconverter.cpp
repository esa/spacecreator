/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

namespace sedsconverter {

class tst_SedsConverter : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    void testStateMachineWithBareInput();
    void testSimpleDataTypes();
    void testArrays();
    void testContainers();
    void testLedDemo();
    void testCalculator();
};

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2490
void tst_SedsConverter::testStateMachineWithBareInput()
{
    const int result = system("./test_state_machine_with_bare_inputs.sh");
    QCOMPARE(result, 0);
}

void tst_SedsConverter::testSimpleDataTypes()
{
    const int result = system("./test_simple_data_types.sh");
    QCOMPARE(result, 0);
}

void tst_SedsConverter::testArrays()
{
    const int result = system("./test_arrays.sh");
    QCOMPARE(result, 0);
}

void tst_SedsConverter::testContainers()
{
    const int result = system("./test_containers.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2020
/// \SRS  ETB-FUN-2060
/// \SRS  ETB-FUN-2140
/// \SRS  ETB-FUN-2150
/// \SRS  ETB-FUN-2170
/// \SRS  ETB-FUN-2400
/// \SRS  ETB-FUN-2490
/// \SRS  ETB-FUN-2500
/// \SRS  ETB-FUN-2520
/// \SRS  ETB-FUN-2550
/// \SRS  ETB-FUN-2560
/// \SRS  ETB-FUN-2660
/// \SRS  ETB-FUN-2670
void tst_SedsConverter::testLedDemo()
{
    const int result = system("./test_calculator.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2020
/// \SRS  ETB-FUN-2060
/// \SRS  ETB-FUN-2140
/// \SRS  ETB-FUN-2150
/// \SRS  ETB-FUN-2170
/// \SRS  ETB-FUN-2210
/// \SRS  ETB-FUN-2220
/// \SRS  ETB-FUN-2490
/// \SRS  ETB-FUN-2500
/// \SRS  ETB-FUN-2520
/// \SRS  ETB-FUN-2550
/// \SRS  ETB-FUN-2560
/// \SRS  ETB-FUN-2570
/// \SRS  ETB-FUN-2660
/// \SRS  ETB-FUN-2670
void tst_SedsConverter::testCalculator()
{
    const int result = system("./test_led_demo.sh");
    QCOMPARE(result, 0);
}

} // namespace sedsconverter

QTEST_MAIN(sedsconverter::tst_SedsConverter)

#include "tst_sedsconverter.moc"
