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

#include "converter.h"

#include <QDebug>
#include <QProcess>

namespace tmc::converter {

const QString SdlToPromelaConverter::m_sdl2PromelaCommand = QStringLiteral("sdl2promela");

bool SdlToPromelaConverter::convertSdl(const ProcessMetadata &processMetadata, const QFileInfo &outputFile)
{
    if (!processMetadata.getSystemStructure().exists()) {
        qCritical() << "File " << processMetadata.getSystemStructure().absoluteFilePath() << " does not exist.";
        return false;
    }

    if (!processMetadata.getProcess().exists()) {
        qCritical() << "File " << processMetadata.getProcess().absoluteFilePath() << " does not exist.";
        return false;
    }

    qDebug() << "Converting SDL files:";
    qDebug() << "    " << processMetadata.getSystemStructure().absoluteFilePath();
    qDebug() << "    " << processMetadata.getProcess().absoluteFilePath();

    qDebug() << "  to:";
    qDebug() << "    " << outputFile.absoluteFilePath();

    QStringList arguments = QStringList();
    arguments.append("--sdl");

    arguments.append(processMetadata.getSystemStructure().absoluteFilePath());
    arguments.append(processMetadata.getProcess().absoluteFilePath());

    arguments.append("-o");
    arguments.append(outputFile.absoluteFilePath());

    arguments.append(m_sdl2PromelaArgs);

    QProcess process = QProcess();

    if (!startSdl2PromelaProcess(process, arguments)) {
        return false;
    }

    return waitForSdl2PromelaProcess(process);
}

bool SdlToPromelaConverter::convertStopCondition(const QFileInfo &inputFile, const QFileInfo &outputFile,
        const std::map<QString, ProcessMetadata> &inputSdlFiles)
{
    qDebug() << "Converting Stop Conditions file: " << inputFile.absoluteFilePath() << " to "
             << outputFile.absoluteFilePath();
    QStringList arguments = QStringList()
            << "--scl" << inputFile.absoluteFilePath() << "-o" << outputFile.absoluteFilePath();

    for (const auto &iter : inputSdlFiles) {
        arguments.append("--sdl");
        arguments.append(iter.second.getSystemStructure().absoluteFilePath());
        arguments.append(iter.second.getProcess().absoluteFilePath());
    }

    arguments.append(m_sdl2PromelaArgs);

    QProcess process = QProcess();

    if (!startSdl2PromelaProcess(process, arguments)) {
        return false;
    }

    return waitForSdl2PromelaProcess(process);
}

bool SdlToPromelaConverter::startSdl2PromelaProcess(QProcess &process, const QStringList &arguments)
{
    qDebug() << "Executing: " << m_sdl2PromelaCommand << " with args:";
    for (const QString &arg : arguments) {
        qDebug() << "    " << arg;
    }

    process.start(m_sdl2PromelaCommand, arguments);

    if (!process.waitForStarted()) {
        qCritical("Cannot start process.");
        QByteArray standardError = process.readAllStandardError();
        QByteArray standardOutput = process.readAllStandardOutput();

        QString str = QString(standardError);
        qCritical() << "Stderr: " << str;
        str = QString(standardOutput);
        qCritical() << "Stdout: " << str;

        process.terminate();
        return false;
    }

    return true;
}

bool SdlToPromelaConverter::waitForSdl2PromelaProcess(QProcess &process)
{
    if (!process.waitForFinished(m_externalCommandTimeout)) {
        qCritical() << "Timeot while waiting for external process.";
        process.terminate();
        return false;
    }

    if (process.exitCode() != EXIT_SUCCESS) {
        qCritical() << "External process finished wit code: " << process.exitCode();
        return false;
    }

    return true;
}
}
