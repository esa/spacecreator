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
#include "testdriver.h"
#include "testgenerator/gdbconnector/dataview-uniq.h"

#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <algorithm>
#include <cstdarg>
#include <cstdint>
#include <memory>
#include <qdebug.h>
#include <qobjectdefs.h>

// GdbConnector includes
#include <QProcess>

// TODO: remove this include
#include <QDebug>
#include <stdexcept>

namespace tests::testgenerator {

class tst_gdbconnector final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testNominal();

private:
    const unsigned int kTestDataSize = 5;

    typedef struct {
        asn1SccMyBool active;
        asn1SccMyReal temperature;
        asn1SccMyInteger posX;
        asn1SccMyInteger posY;
        asn1SccMyBool result;
    } TestVector;

    TestVector *testData = NULL;
};

void tst_gdbconnector::initTestCase()
{
    const unsigned int testDataBytesNum = kTestDataSize * sizeof(TestVector);
    testData = static_cast<TestVector *>(malloc(testDataBytesNum));
    memset(testData, 0, testDataBytesNum);
}

namespace gdbconnector {

// private:
std::unique_ptr<QProcess> makeAndStartProgramWithArgs(
        const QString &programPath, const QStringList &args, const QString &workingDir = QDir::currentPath())
{
    auto program = std::make_unique<QProcess>();
    program->setProgram(programPath);
    program->setArguments(args);
    program->setWorkingDirectory(workingDir);
    program->start();

    return program;
}

QString getProgramOutput(const QString &programPath, const QStringList &args)
{
    const auto client = makeAndStartProgramWithArgs(programPath, args);
    client->waitForFinished();
    const auto output = QString(client->readAllStandardOutput());
    client->close();

    return output;
}

QString getOneBeforeLastLine(const QString &src, const QString &newlineCharacter = "\n")
{
    QString results;
    const QStringList srcList = src.split(newlineCharacter);
    if (srcList.length() >= 2) {
        const int oneBeforeLast = srcList.length() - 2;
        results = srcList.at(oneBeforeLast);
    } else {
        throw std::runtime_error("Cannot get one line before last because the size is lower than 2");
    }

    return results;
}

QString splitAndExtractSrecData(const QString &strings, const QString &delimeter = "\r\n")
{
    QString srecData;

    const int checksumBytes = 2;
    const std::vector<QString> srecHeaders = { "S3150000", "S30D00000100" };
    const int srecHeaderLen = 12; // number of character (number of bytes / 2)

    for (auto srecDataString : strings.split(delimeter)) {
        const auto srecPrefixIt = std::find_if(
                srecHeaders.begin(), srecHeaders.end(), [&srecDataString](const QString &prefix) -> bool { //
                    return srecDataString.startsWith(prefix);
                });
        if (srecPrefixIt != srecHeaders.end()) {
            srecDataString = srecDataString.left(srecDataString.size() - checksumBytes);
            srecDataString = srecDataString.right(srecDataString.size() - srecHeaderLen);
            srecData += srecDataString;
        }
    }

    return srecData;
}

// public:
std::unique_ptr<QProcess> makeAndStartGdbServer(
        const QString &server, const QStringList &serverArgs, const QString &binaryUnderTestDir)
{
    auto gdbserver = makeAndStartProgramWithArgs(server, serverArgs, binaryUnderTestDir);
    gdbserver->waitForStarted();

    return gdbserver;
}

QString getGdbBatchScriptOutput(const QString &debuggerPath, const QString &scriptPath)
{
    return getProgramOutput(debuggerPath, { "-batch", "-x", scriptPath });
}

QByteArray string2byteArray(QString str)
{
    const int charsInByte = 2;
    QByteArray array;

    while (!str.isEmpty()) {
        const QString hexStr = str.left(charsInByte);
        str.remove(0, charsInByte);

        bool ok = false;
        const char byte = hexStr.toUShort(&ok, 16);
        array.append(byte);
    }

    return array;
}

QByteArray getTestResults()
{
    const QString server = "gdbserver";
    const QString serverNamePort = "host:1234";
    const QString programToRun = "hostpartition";
    const QString binaryLocalization = "/home/taste/example-projects/testharness/work/binaries";

    std::unique_ptr<QProcess> gdbserver;
    if (!server.isEmpty()) {
        gdbserver = makeAndStartGdbServer(server, { serverNamePort, programToRun }, binaryLocalization);
        gdbserver->waitForStarted();
    }

    const QString debugger = "gdb";
    const QString script = "x86-linux-cpp.gdb";
    const QString outStr = getGdbBatchScriptOutput(debugger, script);

    if (gdbserver != nullptr && gdbserver->isOpen()) {
        gdbserver->close();
    }

    const QString outQba = splitAndExtractSrecData(outStr);

    const QByteArray qba = string2byteArray(outQba);

    return qba;
}

} // namespace gdbconnector

void printQByteArrayInHex(const QByteArray &array);

void tst_gdbconnector::testNominal()
{
    QByteArray rawTestResults = gdbconnector::getTestResults();

    printQByteArrayInHex(rawTestResults);

    // reconstruct results:
    const TestVector *data = reinterpret_cast<const TestVector *>(rawTestResults.data());
    for (int i = 0; i < static_cast<int>(kTestDataSize); i++) {
        testData[i] = *data;
        data++;

        qDebug() << "i = " << i;
        qDebug() << "testData[i] active = " << testData[i].active;
        qDebug() << "testData[i] temperature = " << testData[i].temperature;
        qDebug() << "testData[i] posX = " << testData[i].posX;
        qDebug() << "testData[i] posY = " << testData[i].posY;
        qDebug() << "testData[i] result = " << testData[i].result;
        qDebug() << "";
    }

    QFAIL("this shall happen");
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
    qDebug().noquote() << arrayInHex;

    QString elements;
    for (int i = 0; i < array.size(); i++) {
        elements += byteToHexStr(array.at(i));
        if ((i + 1) % 8 == 0) {
            elements += " - ";
        } else {
            elements += " ";
        }
        if ((i + 1) % 40 == 0) {
            qInfo().noquote() << elements;
            qInfo().noquote() << "";
            elements.clear();
        }
    }
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_gdbconnector)

#include "tst_gdbconnector.moc"
