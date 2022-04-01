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

#include "gdbconnector.h"

#include "process.h"

#include <QDebug>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <system_error>
#include <utility>

namespace testgenerator {

QByteArray GdbConnector::getRawTestResults(const QString &binaryUnderTestDir, const QStringList &clientArgs,
        const QStringList &serverArgs, const QString &client, const QString &server)
{
    std::unique_ptr<Process> gdbserver = nullptr;
    if (!server.isEmpty()) {
        gdbserver = std::make_unique<Process>(server, serverArgs, binaryUnderTestDir);
    }
    if (!client.isEmpty()) {
        Process gdbclient(client, clientArgs, QDir::currentPath());

        QProcess *const clientProcess = gdbclient.get();
        if (clientProcess->state() != QProcess::Running) {
            throw std::runtime_error("client is not running");
        }
        clientProcess->waitForFinished();

        const QString errorLogFilename = "gdbclient-errors.log";
        dumpProcessErrorsToFile(*clientProcess, errorLogFilename);

        const QString outStr = clientProcess->readAllStandardOutput();
        if (outStr.isEmpty()) {
            throw std::runtime_error(
                    QString("gdb client returned errors. Errors dumped to %1").arg(errorLogFilename).toStdString());
        }
        const QString srecData = splitAndExtractSrecData(outStr);

        return stringToByteArray(srecData);
    } else {
        throw std::invalid_argument("Debugging client program name shall not be empty");
    }
}

QString GdbConnector::splitAndExtractSrecData(const QString &packetizedData, const QString &delimeter)
{
    const QMap<QString, int> startBytesToLength = {
        { "S1", 2 }, // TODO: remove Ss
        { "S2", 3 },
        { "S3", 4 },
        { "S5", 2 },
    }; // S4 and S6 are reserved; S7, S8, S9 have no data fields

    const int recordStartByteLength = 1; // 1 character ('S')
    // const int recordTypeByteLength = 1;
    const int remainingCharactersLength = 1;
    const int checksumLength = 1;

    const int checksumCharacters = 2 * checksumLength;

    QString rawData;
    rawData.reserve(packetizedData.size());
    for (auto dataLine : packetizedData.split(delimeter)) {
        if (dataLine.isEmpty()) {
            continue;
        }

        const int addressFieldLength = startBytesToLength.value(dataLine.at(0));
        const int headerCharacters = 2 * (recordStartByteLength + remainingCharactersLength + addressFieldLength);

        if (addressFieldLength != 0) {
            dataLine = dataLine.left(dataLine.size() - checksumCharacters);
            dataLine = dataLine.right(dataLine.size() - headerCharacters);
            rawData.append(dataLine);
        }
    }

    return rawData;
}

QString GdbConnector::getOneBeforeLastLine(const QString &src, const QString &newlineCharacter)
{
    QString results;
    const QStringList srcList = src.split(newlineCharacter);
    if (srcList.length() >= 2) {
        const int oneBeforeLast = srcList.length() - 2;
        results = srcList.at(oneBeforeLast);
    } else {
        throw std::runtime_error("Cannot get one line before last because the size is less than 2");
    }

    return results;
}

QByteArray GdbConnector::stringToByteArray(const QString &str)
{
    QByteArray array;
    const int charsInByte = 2;

    for (int i = 0; i < str.size(); i += charsInByte) {
        const QString hexStr = str.mid(i, charsInByte);

        bool conversionSuccessfull = false;
        const char byteChar = hexStr.toUShort(&conversionSuccessfull, 16);
        if (!conversionSuccessfull) {
            throw std::runtime_error(
                    QString("Conversion of string: %1 to raw byte value failed").arg(hexStr).toStdString());
        }
        array.append(byteChar);
    }

    return array;
}

auto GdbConnector::dumpProcessErrorsToFile(QProcess &process, const QString &filename) -> void
{
    const QString errors = process.readAllStandardError();
    if (!errors.isEmpty()) {
        QFile errorLog(filename);
        if (!errorLog.open(QIODevice::WriteOnly | QIODevice::Text)) {
            throw std::filesystem::filesystem_error("could not create requested log text file", std::error_code());
        }
        QTextStream out(&errorLog);
        out << errors;
        errorLog.close();
    }
}

} // namespace testgenerator
