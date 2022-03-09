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

class tst_SedsConverter_SedsToIv : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testDatatypeInsideComponent();
    void testRequiredInterfaces();
    void testProvidedInterfaces();
    void testInterfaceInheritance();
    void testHwas();
    void testCommandArguments();
    void testParameters();
    void testGenericTypeMapping();
    void testAlternateSet();
    void testCrossReferenceArguments();
    void testCrossReferenceInterfaces();
    void testDescriptions();
};

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-1070
/// \SRS  ETB-FUN-1080
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-120
void tst_SedsConverter_SedsToIv::testDatatypeInsideComponent()
{
    const int result = system("./test_datatype_inside_component.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-1040
/// \SRS  ETB-FUN-1060
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-130
void tst_SedsConverter_SedsToIv::testRequiredInterfaces()
{
    const int result = system("./test_required_interfaces.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-1040
/// \SRS  ETB-FUN-1050
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-130
void tst_SedsConverter_SedsToIv::testProvidedInterfaces()
{
    const int result = system("./test_provided_interfaces.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-1040
/// \SRS  ETB-FUN-1050
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-130
void tst_SedsConverter_SedsToIv::testInterfaceInheritance()
{
    const int result = system("./test_interface_inheritance.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-1040
/// \SRS  ETB-FUN-1050
/// \SRS  ETB-FUN-1060
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-130
void tst_SedsConverter_SedsToIv::testHwas()
{
    const int result = system("./test_hwas.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-4030
/// \SRS  ETB-FUN-4040
/// \SRS  ETB-IF-10
void tst_SedsConverter_SedsToIv::testCommandArguments()
{
    const int result = system("./test_command_arguments.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-4050
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-130
void tst_SedsConverter_SedsToIv::testParameters()
{
    const int result = system("./test_parameters.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-1090
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-120
void tst_SedsConverter_SedsToIv::testGenericTypeMapping()
{
    const int result = system("./test_generic_type_mapping.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-1100
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-120
void tst_SedsConverter_SedsToIv::testAlternateSet()
{
    const int result = system("./test_alternate_set.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-1040
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-120
void tst_SedsConverter_SedsToIv::testCrossReferenceArguments()
{
    const int result = system("./test_cross_reference_arguments.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-1040
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-120
void tst_SedsConverter_SedsToIv::testCrossReferenceInterfaces()
{
    const int result = system("./test_cross_reference_interfaces.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-1030
void tst_SedsConverter_SedsToIv::testDescriptions()
{
    const int result = system("./test_descriptions.sh");
    QCOMPARE(result, 0);
}

} // namespace sedsconverter

QTEST_MAIN(sedsconverter::tst_SedsConverter_SedsToIv)

#include "tst_sedsconverter_sedstoiv.moc"
