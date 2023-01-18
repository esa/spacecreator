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

class tst_SedsConverter_IvToSeds : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSporadicInterface();
    void testProtectedInterface();
    void testUnprotectedInterface();
    void testCyclicInterface();
    void testRepeatedInterfaceName();
    void testWithDataTypes();
    void testFunctionToConvert();
    void testPinger();
};

/// \SRS  ETB-FUN-4010
/// \SRS  ETB-FUN-4020
/// \SRS  ETB-FUN-4050
/// \SRS  ETB-FUN-4060
/// \SRS  ETB-IF-150
/// \SRS  ETB-IF-190
void tst_SedsConverter_IvToSeds::testSporadicInterface()
{
    const int result = system("./test_sporadic_interface.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-4010
/// \SRS  ETB-FUN-4020
/// \SRS  ETB-FUN-4050
/// \SRS  ETB-FUN-4070
/// \SRS  ETB-IF-150
/// \SRS  ETB-IF-190
void tst_SedsConverter_IvToSeds::testProtectedInterface()
{
    const int result = system("./test_protected_interface.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-4010
/// \SRS  ETB-FUN-4020
/// \SRS  ETB-FUN-4050
/// \SRS  ETB-FUN-4080
/// \SRS  ETB-IF-150
/// \SRS  ETB-IF-190
void tst_SedsConverter_IvToSeds::testUnprotectedInterface()
{
    const int result = system("./test_unprotected_interface.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-4010
/// \SRS  ETB-FUN-4020
/// \SRS  ETB-FUN-4040
/// \SRS  ETB-FUN-4090
/// \SRS  ETB-IF-150
/// \SRS  ETB-IF-190
void tst_SedsConverter_IvToSeds::testCyclicInterface()
{
    const int result = system("./test_cyclic_interface.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-4010
/// \SRS  ETB-FUN-4020
/// \SRS  ETB-FUN-4030
/// \SRS  ETB-FUN-4050
/// \SRS  ETB-FUN-4060
/// \SRS  ETB-IF-150
/// \SRS  ETB-IF-190
void tst_SedsConverter_IvToSeds::testPinger()
{
    const int result = system("./test_pinger.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-4010
/// \SRS  ETB-FUN-4020
/// \SRS  ETB-FUN-4050
/// \SRS  ETB-FUN-4060
/// \SRS  ETB-IF-150
/// \SRS  ETB-IF-190
void tst_SedsConverter_IvToSeds::testRepeatedInterfaceName()
{
    const int result = system("./test_repeated_interface_name.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-4020
/// \SRS  ETB-FUN-4100
void tst_SedsConverter_IvToSeds::testWithDataTypes()
{
    const int result = system("./test_with_data_types.sh");
    QCOMPARE(result, 0);
}

void tst_SedsConverter_IvToSeds::testFunctionToConvert()
{
    const int result = system("./test_function_to_convert.sh");
    QCOMPARE(result, 0);
}

} // namespace sedsconverter

QTEST_MAIN(sedsconverter::tst_SedsConverter_IvToSeds)

#include "tst_sedsconverter_ivtoseds.moc"
