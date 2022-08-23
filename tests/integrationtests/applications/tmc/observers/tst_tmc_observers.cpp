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

} // namespace sedsconverter

QTEST_MAIN(tmc::tst_Tmc_Observers)

#include "tst_tmc_observers.moc"
