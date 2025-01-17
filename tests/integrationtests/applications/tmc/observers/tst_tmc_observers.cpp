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

class tst_Tmc_Observers : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_inputMonitoring();
    void test_inputMonitoringFailure();
    void test_mscNeverSequence();
    void test_mscNeverSequenceFailure();
    void test_mscNeverSequenceBacktracking();
    void test_mscNeverSequenceBacktrackingFailure();
    void test_mscNeverSequenceParametersFailure();
    void test_mscWhenThenSequence();
    void test_mscWhenThenSequenceFailure();
    void test_mscWhenThenSequenceParameters();
    void test_mscWhenThenSequenceParametersAny();
    void test_mscWhenThenNotSequence();
    void test_mscWhenThenNotSequenceFailure();
    void test_mscWhenThenNotSequenceParametersFailure();
    void test_differentInterfaceNames();
    void test_mscEscaping();
};

void tst_Tmc_Observers::test_inputMonitoring()
{
    const int result = system("./test_input_monitoring.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_inputMonitoringFailure()
{
    const int result = system("./test_input_monitoring_failure.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_mscNeverSequence()
{
    const int result = system("./test_msc_never_sequence.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_mscNeverSequenceFailure()
{
    const int result = system("./test_msc_never_sequence_failure.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_mscNeverSequenceBacktracking()
{
    const int result = system("./test_msc_never_sequence_backtracking.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_mscNeverSequenceBacktrackingFailure()
{
    const int result = system("./test_msc_never_sequence_backtracking_failure.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_mscNeverSequenceParametersFailure()
{
    const int result = system("./test_msc_never_sequence_parameters_failure.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_mscWhenThenSequence()
{
    const int result = system("./test_msc_when_then_sequence.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_mscWhenThenSequenceFailure()
{
    const int result = system("./test_msc_when_then_sequence_failure.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_mscWhenThenSequenceParameters()
{
    const int result = system("./test_msc_when_then_sequence_parameters.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_mscWhenThenSequenceParametersAny()
{
    const int result = system("./test_msc_when_then_sequence_parameters_any.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_mscWhenThenNotSequence()
{
    const int result = system("./test_msc_when_then_not_sequence.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_mscWhenThenNotSequenceFailure()
{
    const int result = system("./test_msc_when_then_not_sequence_failure.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_mscWhenThenNotSequenceParametersFailure()
{
    const int result = system("./test_msc_when_then_not_sequence_parameters_failure.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_differentInterfaceNames()
{
    const int result = system("./test_different_interface_names.sh");
    QCOMPARE(result, 0);
}

void tst_Tmc_Observers::test_mscEscaping()
{
    const int result = system("./test_msc_escaping.sh");
    QCOMPARE(result, 0);
}

} // namespace sedsconverter

QTEST_MAIN(tmc::tst_Tmc_Observers)

#include "tst_tmc_observers.moc"
