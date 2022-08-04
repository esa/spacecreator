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

#include <QDebug>
#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <qobjectdefs.h>
#include <testgenerator/gdbconnector/gdbconnector.h>

using testgenerator::GdbConnector;

namespace tests::testgenerator {

class tst_gdbconnector final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void splitAndExtractSrecData() const;
    void stringToByteArray() const;
};
auto printQByteArrayInHex(const QByteArray &array) -> void;
static auto byteToHexStr(char byte) -> QString;

void tst_gdbconnector::splitAndExtractSrecData() const
{
    // clang-format off
    const QString srecPacketizedData = "S00E00002F6465762F7374646F7574B1\r\n"
                                   /* SREC header       data         checksum */
                                   "S31555568180" "0100000000000000"
                                                  "6666666666660240" "97\r\n"
                                   "S31555568190" "FEFFFFFFFFFFFFFF"
                                                  "0300000000000000" "34\r\n"
                                   "S315555681A0" "0100000000000000"

                                                  "0100000000000000" "1C\r\n"
                                   "S315555681B0" "38F8C264AA601540"
                                                  "FEFFFFFFFFFFFFFF" "62\r\n"
                                   "S315555681C0" "0300000000000000"
                                                  "0100000000000000" "FA\r\n"

                                   "S315555681D0" "0000000000000000"
                                                  "6666666666660240" "48\r\n"
                                   "S315555681E0" "F8FFFFFFFFFFFFFF"
                                                  "0300000000000000" "EA\r\n"
                                   "S315555681F0" "0100000000000000"

                                                  "0000000000000000" "CD\r\n"
                                   "S31555568200" "6666666666660240"
                                                  "FFFFFFFFFFFFFFFF" "1F\r\n"
                                   "S31555568210" "FFFFFFFFFFFFFFFF"
                                                  "0100000000000000" "B4\r\n"

                                   "S31555568220" "0100000000000000"
                                                  "6666666666660240" "F6\r\n"
                                   "S31555568230" "0400000000000000"
                                                  "0100000000000000" "88\r\n"
                                   "S30D55568240" "0100000000000000" "84\r\n"

                                   "S70500000000FA\r\n";
    // clang-format on

    const QString extractedStringData = GdbConnector::splitAndExtractSrecData(srecPacketizedData);

    const QString expectedStringData = "0100000000000000" //
                                       "6666666666660240"
                                       "FEFFFFFFFFFFFFFF"
                                       "0300000000000000"
                                       "0100000000000000"

                                       "0100000000000000"
                                       "38F8C264AA601540"
                                       "FEFFFFFFFFFFFFFF"
                                       "0300000000000000"
                                       "0100000000000000"

                                       "0000000000000000"
                                       "6666666666660240"
                                       "F8FFFFFFFFFFFFFF"
                                       "0300000000000000"
                                       "0100000000000000"

                                       "0000000000000000"
                                       "6666666666660240"
                                       "FFFFFFFFFFFFFFFF"
                                       "FFFFFFFFFFFFFFFF"
                                       "0100000000000000"

                                       "0100000000000000"
                                       "6666666666660240"
                                       "0400000000000000"
                                       "0100000000000000"
                                       "0100000000000000";

    const int comparisonResult = expectedStringData.compare(extractedStringData);
    if (comparisonResult != 0) {
        QCOMPARE(extractedStringData, expectedStringData);
    }
}

void tst_gdbconnector::stringToByteArray() const
{
    const QString stringData = "0100000000000000" //
                               "6666666666660240"
                               "FEFFFFFFFFFFFFFF"
                               "0300000000000000"
                               "0100000000000000"

                               "0100000000000000"
                               "38F8C264AA601540"
                               "FEFFFFFFFFFFFFFF"
                               "0300000000000000"
                               "0100000000000000"

                               "0000000000000000"
                               "6666666666660240"
                               "F8FFFFFFFFFFFFFF"
                               "0300000000000000"
                               "0100000000000000"

                               "0000000000000000"
                               "6666666666660240"
                               "FFFFFFFFFFFFFFFF"
                               "FFFFFFFFFFFFFFFF"
                               "0100000000000000"

                               "0100000000000000"
                               "6666666666660240"
                               "0400000000000000"
                               "0100000000000000"
                               "0100000000000000";

    const QByteArray convertedData = GdbConnector::stringToByteArray(stringData);

    const QByteArray expectedData = QByteArrayLiteral("\x01\x00\x00\x00\x00\x00\x00\x00" //
                                                      "\x66\x66\x66\x66\x66\x66\x02\x40"
                                                      "\xFE\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                                                      "\x03\x00\x00\x00\x00\x00\x00\x00"
                                                      "\x01\x00\x00\x00\x00\x00\x00\x00"

                                                      "\x01\x00\x00\x00\x00\x00\x00\x00"
                                                      "\x38\xF8\xC2\x64\xAA\x60\x15\x40"
                                                      "\xFE\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                                                      "\x03\x00\x00\x00\x00\x00\x00\x00"
                                                      "\x01\x00\x00\x00\x00\x00\x00\x00"

                                                      "\x00\x00\x00\x00\x00\x00\x00\x00"
                                                      "\x66\x66\x66\x66\x66\x66\x02\x40"
                                                      "\xF8\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                                                      "\x03\x00\x00\x00\x00\x00\x00\x00"
                                                      "\x01\x00\x00\x00\x00\x00\x00\x00"

                                                      "\x00\x00\x00\x00\x00\x00\x00\x00"
                                                      "\x66\x66\x66\x66\x66\x66\x02\x40"
                                                      "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                                                      "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                                                      "\x01\x00\x00\x00\x00\x00\x00\x00"

                                                      "\x01\x00\x00\x00\x00\x00\x00\x00"
                                                      "\x66\x66\x66\x66\x66\x66\x02\x40"
                                                      "\x04\x00\x00\x00\x00\x00\x00\x00"
                                                      "\x01\x00\x00\x00\x00\x00\x00\x00"
                                                      "\x01\x00\x00\x00\x00\x00\x00\x00");

    QCOMPARE(convertedData.size(), expectedData.size());

    const auto dataSize = convertedData.size();
    for (int i = 0; i < dataSize; i++) {
        if (convertedData.at(i) != expectedData.at(i)) {
            qDebug() << QString("Failed comparison (i = %1)").arg(i);
        }
        QCOMPARE(convertedData.at(i), expectedData.at(i));
    }
}

void printQByteArrayInHex(const QByteArray &array)
{
    QString arrayInHex = QString("QByteArray size: %1\n").arg(array.size());
    for (int i = 0; i < array.size(); i++) {
        arrayInHex += byteToHexStr(array.at(i));
        arrayInHex += " ";

        if ((i + 1) % 8 == 0) {
            arrayInHex += "| ";
        }

        if ((i + 1) % 16 == 0) {
            arrayInHex += "\n";
        }
    }

    qDebug().noquote() << arrayInHex;
}

QString byteToHexStr(const char byte)
{
    const int hexBase = 16;

    const auto number = static_cast<uint_least8_t>(byte);
    if (number < hexBase) {
        return QString("0%1").arg(number, 1, hexBase);
    } else {
        return QString("%1").arg(number, 2, hexBase);
    }
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_gdbconnector)

#include "tst_gdbconnector.moc"
