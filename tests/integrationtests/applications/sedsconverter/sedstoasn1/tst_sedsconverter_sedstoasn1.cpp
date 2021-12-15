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

class tst_SedsConverter_SedsToAsn1 : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSimpleDataTypes();
    void testArrays();
    void testIndexedArray();
    void testContainers();
};

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-110
/// \SRS  ETB-FUN-130
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-120
void tst_SedsConverter_SedsToAsn1::testSimpleDataTypes()
{
    const int result = system("./test_simple_data_types.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-110
/// \SRS  ETB-FUN-130
/// \SRS  ETB-FUN-210
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-120
void tst_SedsConverter_SedsToAsn1::testArrays()
{
    const int result = system("./test_arrays.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-110
/// \SRS  ETB-FUN-130
/// \SRS  ETB-FUN-210
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-120
void tst_SedsConverter_SedsToAsn1::testIndexedArray()
{
    const int result = system("./test_indexed_array.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-10
/// \SRS  ETB-FUN-110
/// \SRS  ETB-IF-10
/// \SRS  ETB-IF-120
void tst_SedsConverter_SedsToAsn1::testContainers()
{
    const int result = system("./test_containers.sh");
    QCOMPARE(result, 0);
}

} // namespace sedsconverter

QTEST_MAIN(sedsconverter::tst_SedsConverter_SedsToAsn1)

#include "tst_sedsconverter_sedstoasn1.moc"
