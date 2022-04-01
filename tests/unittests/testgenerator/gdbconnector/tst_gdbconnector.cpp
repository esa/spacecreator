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
#include <QTest>
#include <QVariant>
#include <QtTest/qtestcase.h>
#include <ivtools.h>
#include <modelloader.h>
#include <qobjectdefs.h>
#include <testgenerator/datareconstructor/datareconstructor.h>
#include <testgenerator/gdbconnector/gdbconnector.h>

using testgenerator::GdbConnector;

namespace tests::testgenerator {

class tst_gdbconnector final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testNominal() const;
};
auto printQByteArrayInHex(const QByteArray &array) -> void;
static auto byteToHexStr(char byte) -> QString;

void tst_gdbconnector::testNominal() const
{
    const QString packetizedData = "";

    const QString extractedData = GdbConnector::splitAndExtractSrecData(packetizedData);

    const QString expectedData = "";

    const int comparisonResult = expectedData.compare(extractedData);
    if (comparisonResult != 0) {
        qDebug() << "expected: " << expectedData;
        qDebug() << "but was:  " << extractedData;

        QCOMPARE(extractedData, expectedData);
    }

    QFAIL("this should happen");
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
