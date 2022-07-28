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

#include <QMap>
#include <QStandardPaths>
#include <QThread>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <memory>
#include <qdebug.h>
#include <stdexcept>
#include <system_error>
#include <utility>

namespace testgenerator {

QByteArray GdbConnector::getRawTestResults(const QString &binaryUnderTestDir, const QStringList &clientArgs,
        const QStringList &serverArgs, const QString &client, const QString &server)
{
    if (server.isEmpty()) {
        throw std::invalid_argument("Debugging server program name shall not be empty");
    }
    if (client.isEmpty()) {
        throw std::invalid_argument("Debugging client program name shall not be empty");
    }

    const QString errorLogFilename = "gdbclient-errors.log";
    const QString outputFilePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + "gdboutput.txt";
    qDebug() << "Output file path: " << outputFilePath;

    QString cmdServer = "cd " + binaryUnderTestDir + "; " + server + " " + serverArgs.join(" ") + " &";
    QString cmdClient = "cd " + binaryUnderTestDir + "; " + client + " " + clientArgs.join(" ");
    cmdClient += " 2> " + errorLogFilename + " > " + outputFilePath;

    qDebug() << "Server command: " << cmdServer;
    qDebug() << "Client command: " << cmdClient;
    qDebug() << "Binary under test dir: " << binaryUnderTestDir;

    std::system(cmdServer.toStdString().c_str());
    std::system(cmdClient.toStdString().c_str());
    std::system(("killall " + server).toStdString().c_str());

    QString outStr;
    QFile file(outputFilePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        outStr = QString(file.readAll());
        file.remove();
    }

    if (outStr.isEmpty()) {
        throw std::runtime_error(QString("Errors while debugging. Dumped to: %1").arg(errorLogFilename).toStdString());
    }

    const QString srecData = splitAndExtractSrecData(outStr, "\n");
    qDebug() << "srecData: " << srecData;

    return stringToByteArray(srecData);
}

QString GdbConnector::splitAndExtractSrecData(const QString &packetizedData, const QString &delimeter)
{
    const QMap<QString, int> startBytesToLength = {
        { "1", 2 },
        { "2", 3 },
        { "3", 4 },
        { "5", 2 },
    }; // S4 and S6 are reserved; S7, S8, S9 have no data fields

    const QChar startCharacter = 'S';
    const int recordStartByteLength = 1; // first byte contains start character and record type
    const int byteCount = 1;
    const int checksumLength = 1;

    QString rawData;
    rawData.reserve(packetizedData.size());
    const QStringList dataRows = packetizedData.split(delimeter);
    for (auto dataRow : dataRows) {
        if (dataRow.isEmpty() || dataRow.at(0) != startCharacter) {
            continue;
        }

        const int addressFieldLength = startBytesToLength.value(dataRow.at(1));
        if (addressFieldLength == 0) {
            continue;
        }

        const int headerCharacters = 2 * (recordStartByteLength + byteCount + addressFieldLength);

        dataRow = dataRow.left(dataRow.size() - 2 * checksumLength); // remove checksum byte
        dataRow = dataRow.right(dataRow.size() - headerCharacters); // remove header bytes
        rawData.append(dataRow);
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
