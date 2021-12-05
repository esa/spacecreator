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

class tst_SedsConverter_Asn1ToSeds : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSimpleDataTypes();
    void testSequences();
    void testSequencesOf();
    void testChoices();
};

/// \SRS  ETB-FUN-3010
/// \SRS  ETB-FUN-3030
/// \SRS  ETB-FUN-3040
/// \SRS  ETB-FUN-3050
/// \SRS  ETB-FUN-3060
/// \SRS  ETB-FUN-3070
/// \SRS  ETB-FUN-3120
/// \SRS  ETB-FUN-3160
/// \SRS  ETB-FUN-3170
/// \SRS  ETB-IF-160
/// \SRS  ETB-IF-170
/// \SRS  ETB-IF-180
void tst_SedsConverter_Asn1ToSeds::testSimpleDataTypes()
{
    const int result = system("./test_simple_data_types.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-3010
/// \SRS  ETB-FUN-3090
/// \SRS  ETB-FUN-3092
/// \SRS  ETB-FUN-3200
/// \SRS  ETB-IF-160
/// \SRS  ETB-IF-170
/// \SRS  ETB-IF-180
void tst_SedsConverter_Asn1ToSeds::testSequences()
{
    const int result = system("./test_sequences.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-3100
/// \SRS  ETB-FUN-3110
/// \SRS  ETB-FUN-3150
/// \SRS  ETB-IF-160
/// \SRS  ETB-IF-170
/// \SRS  ETB-IF-180
void tst_SedsConverter_Asn1ToSeds::testSequencesOf()
{
    const int result = system("./test_sequences_of.sh");
    QCOMPARE(result, 0);
}

/// \SRS  ETB-FUN-3080
/// \SRS  ETB-IF-160
/// \SRS  ETB-IF-170
/// \SRS  ETB-IF-180
void tst_SedsConverter_Asn1ToSeds::testChoices()
{
    const int result = system("./test_choices.sh");
    QCOMPARE(result, 0);
}

} // namespace sedsconverter

QTEST_MAIN(sedsconverter::tst_SedsConverter_Asn1ToSeds)

#include "tst_sedsconverter_asn1toseds.moc"
