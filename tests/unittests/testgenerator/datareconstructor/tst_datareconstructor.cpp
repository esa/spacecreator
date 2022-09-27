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

#include "gdbconnectorstub.h"

#include <QObject>
#include <QTest>
#include <QVariant>
#include <QtTest/qtestcase.h>
#include <ivtools.h>
#include <modelloader.h>
#include <qobjectdefs.h>
#include <testgenerator/datareconstructor/datareconstructor.h>

using plugincommon::IvTools;
using plugincommon::ModelLoader;
using testgenerator::DataReconstructor;
using testgenerator::GdbConnector;

namespace tests::testgenerator {

class tst_datareconstructor final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testNominal() const;
    void testNominalBigEndian() const;
    void testBooleansInARow() const;
};
auto printQByteArrayInHex(const QByteArray &array) -> void;
static auto byteToHexStr(char byte) -> QString;

void tst_datareconstructor::testNominal() const
{
    const QVector<QVariant> expectedTestData = {
        true, 2.3000, -2, 3, true, //
        true, 5.3444, -2, 3, true, //
        false, 2.3000, -8, 3, true, //
        false, 2.3000, -1, -1, true, //
        true, 2.3000, 4, 1, true, //
    };

    // these are just dummy values
    const QString binLocalization = QString("%1%2%3%4")
                                            .arg(qEnvironmentVariable("HOME"))
                                            .arg(QDir::separator())
                                            .arg("testBinaries")
                                            .arg(QDir::separator());
    const QString script = "x86-linux-cpp.gdb";
    const QString binToRun = "hostpartition";
    const QByteArray rawTestData =
            GdbConnector::getRawTestResults(binLocalization, { "-batch", "-x", script }, { "host:1234", binToRun });

    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/interfaceview.xml");
    ivm::IVInterface *const ivIface = IvTools::getIfaceFromModel("InterfaceUnderTest", ivModel.get());

    const QString asn1Filepath = QString("%1%2%3").arg("resources").arg(QDir::separator()).arg("testharness.asn");
    const auto asn1Model = ModelLoader::loadAsn1Model(asn1Filepath);
    const DataReconstructor::TypeLayoutInfos typeLayoutInfos = {
        { "INTEGER", 8, 0 },
        { "BOOLEAN", 1, 0 },
        { "REAL", 8, 0 },
    };
    const QVector<QVariant> readTestData = DataReconstructor::getVariantVectorFromRawData(
            rawTestData, ivIface, asn1Model.get(), QDataStream::LittleEndian, typeLayoutInfos);

    QCOMPARE(readTestData.size(), expectedTestData.size());
    const int dataSize = readTestData.size();
    for (int i = 0; i < dataSize; i++) {
        if (readTestData.at(i) != expectedTestData.at(i)) {
            printQByteArrayInHex(rawTestData);

            const char readChar = readTestData.at(i).toChar().toLatin1();
            const char expectedChar = expectedTestData.at(i).toChar().toLatin1();
            qDebug().noquote() << i << byteToHexStr(readChar) << byteToHexStr(expectedChar);
        }

        QCOMPARE(readTestData.at(i), expectedTestData.at(i));
    }
}

void tst_datareconstructor::testNominalBigEndian() const
{
    const QVector<QVariant> expectedTestData = { 2, 2.5, true, 5.0, 4, 5.0, false, 0.8 };

    const char *const rawTestData = "\x00\x00\x00\x00\x00\x00\x00\x02"
                                    "\x40\x04\x00\x00\x00\x00\x00\x00"
                                    "\x01\x00\x00\x00\x00\x00\x00\x00"
                                    "\x40\x14\x00\x00\x00\x00\x00\x00"
                                    "\x00\x00\x00\x00\x00\x00\x00\x04"
                                    "\x40\x14\x00\x00\x00\x00\x00\x00"
                                    "\x00\x00\x00\x00\x00\x00\x00\x00"
                                    "\x3F\xE9\x99\x99\x99\x99\x99\x9A";

    const int resultBytesNum = 64;
    QByteArray rawTestByteArray = QByteArray::fromRawData(rawTestData, resultBytesNum);

    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/interfaceview_big_endian.xml");
    ivm::IVInterface *const ivIface = IvTools::getIfaceFromModel("Calculate", ivModel.get());

    const QString asn1Filepath =
            QString("%1%2%3").arg("resources").arg(QDir::separator()).arg("testharness_big_endian.asn");
    const auto asn1Model = ModelLoader::loadAsn1Model(asn1Filepath);

    const DataReconstructor::TypeLayoutInfos typeLayoutInfos = {
        { "INTEGER", 8, 0 },
        { "BOOLEAN", 1, 0 },
        { "REAL", 8, 0 },
    };
    const QVector<QVariant> readTestData = DataReconstructor::getVariantVectorFromRawData(
            rawTestByteArray, ivIface, asn1Model.get(), QDataStream::BigEndian, typeLayoutInfos);

    QCOMPARE(readTestData.size(), expectedTestData.size());
    const int dataSize = readTestData.size();
    for (int i = 0; i < dataSize; i++) {
        if (readTestData.at(i) != expectedTestData.at(i)) {
            printQByteArrayInHex(rawTestData);

            const char readChar = readTestData.at(i).toChar().toLatin1();
            const char expectedChar = expectedTestData.at(i).toChar().toLatin1();
            qDebug().noquote() << i << byteToHexStr(readChar) << byteToHexStr(expectedChar);
        }

        QCOMPARE(readTestData.at(i), expectedTestData.at(i));
    }
}

void tst_datareconstructor::testBooleansInARow() const
{
    const QVector<QVariant> expectedTestData = {
        true, true, true, true, //
        false, false, false, false //
    };

    const char *const rawTestData = "\x01\x01\x01\x01\x00\x00\x00\x00";

    const int resultBytesNum = 8;
    QByteArray rawTestByteArray = QByteArray::fromRawData(rawTestData, resultBytesNum);

    const auto ivModel = ModelLoader::loadIvModel("resources/config.xml", "resources/interfaceview_booleans.xml");
    ivm::IVInterface *const ivIface = IvTools::getIfaceFromModel("Calculate", ivModel.get());

    const QString asn1Filepath = QString("%1%2%3").arg("resources").arg(QDir::separator()).arg("testharness.asn");
    const auto asn1Model = ModelLoader::loadAsn1Model(asn1Filepath);

    const DataReconstructor::TypeLayoutInfos typeLayoutInfos = {
        { "INTEGER", 8, 0 },
        { "BOOLEAN", 1, 0 },
        { "REAL", 8, 0 },
    };
    const QVector<QVariant> readTestData = DataReconstructor::getVariantVectorFromRawData(
            rawTestByteArray, ivIface, asn1Model.get(), QDataStream::LittleEndian, typeLayoutInfos);

    QCOMPARE(readTestData.size(), expectedTestData.size());
    const int dataSize = readTestData.size();
    for (int i = 0; i < dataSize; i++) {
        if (readTestData.at(i) != expectedTestData.at(i)) {
            printQByteArrayInHex(rawTestData);

            const char readChar = readTestData.at(i).toChar().toLatin1();
            const char expectedChar = expectedTestData.at(i).toChar().toLatin1();
            qDebug().noquote() << i << byteToHexStr(readChar) << byteToHexStr(expectedChar);
        }

        QCOMPARE(readTestData.at(i), expectedTestData.at(i));
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

QTEST_MAIN(tests::testgenerator::tst_datareconstructor)

#include "tst_datareconstructor.moc"
