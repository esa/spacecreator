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

namespace testgenerator {

QByteArray GdbConnector::getRawTestResults(const QString &binaryUnderTestDir, const QStringList &debuggerArgs,
        const QStringList &serverArgs, const QString &debugger, const QString &server)
{
    std::unique_ptr<QProcess> gdbserver;
    if (!server.isEmpty()) {
        gdbserver = makeAndStartGdbServer(server, serverArgs, binaryUnderTestDir);
        gdbserver->waitForStarted();
    }

    const QString outStr = getProgramOutput(debugger, debuggerArgs);

    if (gdbserver != nullptr && gdbserver->isOpen()) {
        gdbserver->close();
    }

    const QString srecData = splitAndExtractSrecData(outStr);

    return string2byteArray(srecData);
}

std::unique_ptr<QProcess> GdbConnector::makeAndStartGdbServer(
        const QString &server, const QStringList &serverArgs, const QString &binaryUnderTestDir)
{
    auto gdbserver = makeAndStartProgramWithArgs(server, serverArgs, binaryUnderTestDir);
    gdbserver->waitForStarted();

    return gdbserver;
}

std::unique_ptr<QProcess> GdbConnector::makeAndStartProgramWithArgs(
        const QString &programPath, const QStringList &args, const QString &workingDir)
{
    auto program = std::make_unique<QProcess>();
    program->setProgram(programPath);
    program->setArguments(args);
    program->setWorkingDirectory(workingDir);
    program->start();

    return program;
}

QString GdbConnector::getOneBeforeLastLine(const QString &src, const QString &newlineCharacter)
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

QString GdbConnector::getProgramOutput(const QString &programPath, const QStringList &args)
{
    const auto client = makeAndStartProgramWithArgs(programPath, args);
    client->waitForFinished();
    auto output = QString(client->readAllStandardOutput());
    client->close();

    return output;
}

// TODO: this extraction function shall be supplied as lambda expression with constant default
QString GdbConnector::splitAndExtractSrecData(const QString &strings, const QString &delimeter)
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

QByteArray GdbConnector::string2byteArray(QString str)
{
    const int charsInByte = 2;
    QByteArray array;

    while (!str.isEmpty()) {
        const QString hexStr = str.left(charsInByte);
        str.remove(0, charsInByte);

        bool ok = false;
        const char byteChar = hexStr.toUShort(&ok, 16);
        array.append(byteChar);
    }

    return array;
}

} // namespace testgenerator
