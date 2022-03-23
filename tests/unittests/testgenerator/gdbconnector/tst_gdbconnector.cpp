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

#include "../common.h"
#include "gdbconnectorstub.h"
#include "testdriver.h"
#include "testgenerator/gdbconnector/dataview-uniq.h"

#include <QDebug>
#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <algorithm>
#include <cstdarg>
#include <cstdint>
#include <iostream>
#include <memory>
#include <qdebug.h>
#include <qglobal.h>
#include <qobjectdefs.h>

using testgenerator::GdbConnector;

namespace tests::testgenerator {

typedef struct {
    asn1SccMyBool active;
    asn1SccMyReal temperature;
    asn1SccMyInteger posX;
    asn1SccMyInteger posY;
    asn1SccMyBool result;
} TestVector;

class tst_gdbconnector final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testNominal();

private:
    const unsigned int kTestDataSize = 5;
    TestVector *testData = nullptr;
};
static auto compareTestVectors(const TestVector &actual, const TestVector &expected) -> void;
static auto copyRawBytesIntoTestVector(const QByteArray &source, TestVector *const testData, unsigned int testDataSize)
        -> void;

void tst_gdbconnector::initTestCase()
{
    const unsigned int testDataBytesNum = kTestDataSize * sizeof(TestVector);
    testData = static_cast<TestVector *>(malloc(testDataBytesNum));
    memset(testData, 0, testDataBytesNum);
}

QString byteToHexStr(char byte)
{
    const auto number = static_cast<uint_least8_t>(byte);
    if (number < 16) {
        return QString("0%1").arg(number, 1, 16);
    } else {
        return QString("%1").arg(number, 2, 16);
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
    std::cerr << arrayInHex.toStdString();
}

void tst_gdbconnector::testNominal()
{
    const TestVector expectedTestData[] = {
        { true, 2.3000, -2, 3, true },
        { true, 5.3444, -2, 3, true },
        { false, 2.3000, -8, 3, true },
        { false, 2.3000, -1, -1, true },
        { true, 2.3000, 4, 1, true },
    };

    const QString binLocalization = qEnvironmentVariable("$HOME/testBinaries/");
    const QString script = "x86-linux-cpp.gdb";
    const QString binToRun = "hostpartition";
    const QByteArray rawTestResults =
            GdbConnector::getRawTestResults(binLocalization, { "-batch", "-x", script }, { "host:1234", binToRun });

    copyRawBytesIntoTestVector(rawTestResults, testData, kTestDataSize);
    for (unsigned int i = 0; i < kTestDataSize; i++) {
        compareTestVectors(testData[i], expectedTestData[i]);
    }
}

static void compareTestVectors(const TestVector &actual, const TestVector &expected)
{
    QCOMPARE(actual.active, expected.active);
    QCOMPARE(actual.temperature, expected.temperature);
    QCOMPARE(actual.posX, expected.posX);
    QCOMPARE(actual.posY, expected.posY);
    QCOMPARE(actual.result, expected.result);
}

static auto copyRawBytesIntoTestVector(const QByteArray &source, TestVector *const testData, unsigned int testDataSize)
        -> void
{
    if (static_cast<unsigned int>(source.size()) != testDataSize * sizeof(TestVector)) {
        return;
    }
    const TestVector *data = reinterpret_cast<const TestVector *>(source.data());
    for (int i = 0; i < static_cast<int>(testDataSize); i++) {
        testData[i] = *data;
        data++;
    }
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_gdbconnector)

#include "tst_gdbconnector.moc"
