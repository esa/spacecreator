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

#pragma once

#include <QDir>
#include <QProcess>
#include <memory>

class GdbConnector final
{
public:
    static auto getTestResults(const QString &programToRun, const QString &binaryLocalization,
            const QString &serverNamePort = "host:1234", const QString &server = "gdbserver") -> QByteArray;

private:
    static auto makeAndStartGdbServer(const QString &server, const QStringList &serverArgs,
            const QString &binaryUnderTestDir) -> std::unique_ptr<QProcess>;
    static auto makeAndStartProgramWithArgs(const QString &programPath, const QStringList &args,
            const QString &workingDir = QDir::currentPath()) -> std::unique_ptr<QProcess>;

    static auto getGdbBatchScriptOutput(const QString &debuggerPath, const QString &scriptPath) -> QString;
    static auto getOneBeforeLastLine(const QString &src, const QString &newlineCharacter = "\n") -> QString;
    static auto getProgramOutput(const QString &programPath, const QStringList &args) -> QString;

    static auto splitAndExtractSrecData(const QString &strings, const QString &delimeter = "\r\n") -> QString;
    static auto string2byteArray(QString str) -> QByteArray;
};
