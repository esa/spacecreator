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

class tst_SedsConverter_Validation : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testInvalidFails();
    void testMissingCoreSemanticsFails();
    void testMissingExtensionSemanticsFails();
    void testValidPasses();
};

/// \SRS  ETB-FUN-20
void tst_SedsConverter_Validation::testInvalidFails()
{
    const int result = system("./test_invalid_fails.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-20
/// \SRS  ETB-DAT-20
void tst_SedsConverter_Validation::testMissingCoreSemanticsFails()
{
    const int result = system("./test_missing_core_semantics_fails.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-20
/// \SRS  ETB-DAT-20
void tst_SedsConverter_Validation::testMissingExtensionSemanticsFails()
{
    const int result = system("./test_missing_extension_semantics_fails.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-20
/// \SRS  ETB-DAT-20
void tst_SedsConverter_Validation::testValidPasses()
{
    const int result = system("./test_valid_passes.sh");
    QCOMPARE(result, 0);
}

} // namespace sedsconverter

QTEST_MAIN(sedsconverter::tst_SedsConverter_Validation)

#include "tst_sedsconverter_validation.moc"
