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

class tst_SedsConverter_SedsToSdl : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testStateMachineWithBareInput();
    void testLedDemo();
    void testCalculator();
    void testComparator();
    void testFactorial();
    void testGuard();
    void testComplexGuard();
    void testTimers();
    void testParameterMap();
    void testParameterActivityMapSync();
    void testParameterActivityMapAsync();
    void testSyncCommand();
    void testAsyncCommand();
    void testIndexing();
    void testInitialValues();
    void testNamedValues();
    void testDescriptions();
    void testCrossReference();
    void testSplineCalibrators();
    void testSedsSdlWithTasteOption();
    void testTransaction();
    void testFailureReportingSync();
    void testFailureReportingAsync();
    void testVariableWithArrayDimensions();
    void testTypeConditions();
};

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2490
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-140
void tst_SedsConverter_SedsToSdl::testStateMachineWithBareInput()
{
    const int result = system("./test_state_machine_with_bare_inputs.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
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
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-140
void tst_SedsConverter_SedsToSdl::testLedDemo()
{
    const int result = system("./test_led_demo.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2020
/// \SRS  ETB-FUN-2060
/// \SRS  ETB-FUN-2140
/// \SRS  ETB-FUN-2150
/// \SRS  ETB-FUN-2170
/// \SRS  ETB-FUN-2210
/// \SRS  ETB-FUN-2220
/// \SRS  ETB-FUN-2320
/// \SRS  ETB-FUN-2490
/// \SRS  ETB-FUN-2500
/// \SRS  ETB-FUN-2520
/// \SRS  ETB-FUN-2550
/// \SRS  ETB-FUN-2560
/// \SRS  ETB-FUN-2570
/// \SRS  ETB-FUN-2660
/// \SRS  ETB-FUN-2670
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-140
void tst_SedsConverter_SedsToSdl::testCalculator()
{
    const int result = system("./test_calculator.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2020
/// \SRS  ETB-FUN-2060
/// \SRS  ETB-FUN-2140
/// \SRS  ETB-FUN-2150
/// \SRS  ETB-FUN-2160
/// \SRS  ETB-FUN-2410
/// \SRS  ETB-FUN-2490
/// \SRS  ETB-FUN-2500
/// \SRS  ETB-FUN-2520
/// \SRS  ETB-FUN-2550
/// \SRS  ETB-FUN-2560
/// \SRS  ETB-FUN-2570
/// \SRS  ETB-FUN-2660
/// \SRS  ETB-FUN-2670
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-140
void tst_SedsConverter_SedsToSdl::testComparator()
{
    const int result = system("./test_comparator.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2020
/// \SRS  ETB-FUN-2060
/// \SRS  ETB-FUN-2140
/// \SRS  ETB-FUN-2150
/// \SRS  ETB-FUN-2160
/// \SRS  ETB-FUN-2210
/// \SRS  ETB-FUN-2240
/// \SRS  ETB-FUN-2400
/// \SRS  ETB-FUN-2420
/// \SRS  ETB-FUN-2490
/// \SRS  ETB-FUN-2500
/// \SRS  ETB-FUN-2520
/// \SRS  ETB-FUN-2550
/// \SRS  ETB-FUN-2560
/// \SRS  ETB-FUN-2570
/// \SRS  ETB-FUN-2660
/// \SRS  ETB-FUN-2670
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-140
void tst_SedsConverter_SedsToSdl::testFactorial()
{
    const int result = system("./test_factorial.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2020
/// \SRS  ETB-FUN-2060
/// \SRS  ETB-FUN-2490
/// \SRS  ETB-FUN-2650
/// \SRS  ETB-FUN-2660
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-140
void tst_SedsConverter_SedsToSdl::testGuard()
{
    const int result = system("./test_guard.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2020
/// \SRS  ETB-FUN-2060
/// \SRS  ETB-FUN-2490
/// \SRS  ETB-FUN-2650
/// \SRS  ETB-FUN-2660
/// \SRS  ETB-FUN-2680
/// \SRS  ETB-FUN-2690
/// \SRS  ETB-FUN-2500
/// \SRS  ETB-FUN-2510
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-140
void tst_SedsConverter_SedsToSdl::testComplexGuard()
{
    const int result = system("./test_complex_guard.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2490
/// \SRS  ETB-FUN-2640
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-140
void tst_SedsConverter_SedsToSdl::testTimers()
{
    const int result = system("./test_timers.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2040
/// \SRS  ETB-FUN-2080
/// \SRS  ETB-FUN-2090
/// \SRS  ETB-FUN-2100
/// \SRS  ETB-FUN-2110
/// \SRS  ETB-FUN-2550
/// \SRS  ETB-FUN-2600
/// \SRS  ETB-FUN-2610
void tst_SedsConverter_SedsToSdl::testParameterMap()
{
    const int result = system("./test_parameter_map.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2040
/// \SRS  ETB-FUN-2050
/// \SRS  ETB-FUN-2080
/// \SRS  ETB-FUN-2210
/// \SRS  ETB-FUN-2240
/// \SRS  ETB-FUN-2250
/// \SRS  ETB-FUN-2550
/// \SRS  ETB-FUN-2600
void tst_SedsConverter_SedsToSdl::testParameterActivityMapSync()
{
    const int result = system("./test_parameter_activity_map_sync.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2040
/// \SRS  ETB-FUN-2050
/// \SRS  ETB-FUN-2080
/// \SRS  ETB-FUN-2210
/// \SRS  ETB-FUN-2240
/// \SRS  ETB-FUN-2250
/// \SRS  ETB-FUN-2550
/// \SRS  ETB-FUN-2600
void tst_SedsConverter_SedsToSdl::testParameterActivityMapAsync()
{
    const int result = system("./test_parameter_activity_map_async.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2020
/// \SRS  ETB-FUN-2060
/// \SRS  ETB-FUN-2070
/// \SRS  ETB-FUN-2140
/// \SRS  ETB-FUN-2150
/// \SRS  ETB-FUN-2170
/// \SRS  ETB-FUN-2210
/// \SRS  ETB-FUN-2490
/// \SRS  ETB-FUN-2520
/// \SRS  ETB-FUN-2550
/// \SRS  ETB-FUN-2560
/// \SRS  ETB-FUN-2570
void tst_SedsConverter_SedsToSdl::testSyncCommand()
{
    const int result = system("./test_sync_command.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2020
/// \SRS  ETB-FUN-2060
/// \SRS  ETB-FUN-2140
/// \SRS  ETB-FUN-2150
/// \SRS  ETB-FUN-2170
/// \SRS  ETB-FUN-2490
/// \SRS  ETB-FUN-2550
/// \SRS  ETB-FUN-2560
void tst_SedsConverter_SedsToSdl::testAsyncCommand()
{
    const int result = system("./test_async_command.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-2020
/// \SRS  ETB-FUN-2060
/// \SRS  ETB-FUN-2400
void tst_SedsConverter_SedsToSdl::testIndexing()
{
    const int result = system("./test_indexing.sh");
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2020
void tst_SedsConverter_SedsToSdl::testInitialValues()
{
    const int result = system("./test_initial_values.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-2020
/// \SRS  ETB-FUN-2060
/// \SRS  ETB-FUN-2400
void tst_SedsConverter_SedsToSdl::testNamedValues()
{
    const int result = system("./test_named_values.sh");
}

/// \SRS  ETB-FUN-10
void tst_SedsConverter_SedsToSdl::testDescriptions()
{
    const int result = system("./test_descriptions.sh");
}

/// \SRS  ETB-FUN-10
void tst_SedsConverter_SedsToSdl::testCrossReference()
{
    const int result = system("./test_cross_reference.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2470
void tst_SedsConverter_SedsToSdl::testSplineCalibrators()
{
    const int result = system("./test_spline_calibration.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETD-18
void tst_SedsConverter_SedsToSdl::testSedsSdlWithTasteOption()
{
    const int result = system("./test_translation_taste_option.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2120
/// \SRS  ETB-FUN-2180
/// \SRS  ETB-FUN-2580
/// \SRS  ETB-FUN-2620
/// \SRS  ETB-FUN-2710
void tst_SedsConverter_SedsToSdl::testTransaction()
{
    const int result = system("./test_transaction.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2130
/// \SRS  ETB-FUN-2190
/// \SRS  ETB-FUN-2590
/// \SRS  ETB-FUN-2630
void tst_SedsConverter_SedsToSdl::testFailureReportingSync()
{
    const int result = system("./test_failure_reporting_sync.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2130
/// \SRS  ETB-FUN-2590
void tst_SedsConverter_SedsToSdl::testFailureReportingAsync()
{
    const int result = system("./test_failure_reporting_async.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2030
void tst_SedsConverter_SedsToSdl::testVariableWithArrayDimensions()
{
    const int result = system("./test_variable_with_array_dimensions.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-2700
void tst_SedsConverter_SedsToSdl::testTypeConditions()
{
    const int result = system("./test_type_conditions.sh");
    QCOMPARE(result, 0);
}

} // namespace sedsconverter

QTEST_MAIN(sedsconverter::tst_SedsConverter_SedsToSdl)

#include "tst_sedsconverter_sedstosdl.moc"
