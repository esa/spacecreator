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
    if (processMetadata.getSystemStructure().has_value() && !processMetadata.getSystemStructure().value().exists()) {
        qCritical() << "File " << processMetadata.getSystemStructure().value().absoluteFilePath() << " does not exist.";
        return false;
    }

    if (!processMetadata.getProcess().exists()) {
        qCritical() << "File " << processMetadata.getProcess().absoluteFilePath() << " does not exist.";
        return false;
    }

    qDebug() << "Converting SDL files:";
    if (processMetadata.getSystemStructure().has_value()) {
        qDebug() << "    " << processMetadata.getSystemStructure().value().absoluteFilePath();
    }

    qDebug() << "    " << processMetadata.getProcess().absoluteFilePath();

    qDebug() << "  to:";
    qDebug() << "    " << outputFile.absoluteFilePath();

    QStringList arguments = QStringList();
    arguments.append("--sdl");

    if (processMetadata.getSystemStructure().has_value()) {
        arguments.append(processMetadata.getSystemStructure().value().absoluteFilePath());
    }
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

bool SdlToPromelaConverter::convertObserverSdl(
        const ProcessMetadata &processMetadata, const QFileInfo &outputFile, const QFileInfo &outputInfoFile)
{
    if (processMetadata.getSystemStructure().has_value() && !processMetadata.getSystemStructure().value().exists()) {
        qCritical() << "File " << processMetadata.getSystemStructure().value().absoluteFilePath() << " does not exist.";
        return false;
    }

    if (!processMetadata.getProcess().exists()) {
        qCritical() << "File " << processMetadata.getProcess().absoluteFilePath() << " does not exist.";
        return false;
    }

    qDebug() << "Converting SDL files:";
    if (processMetadata.getSystemStructure().has_value()) {
        qDebug() << "    " << processMetadata.getSystemStructure().value().absoluteFilePath();
    }
    qDebug() << "    " << processMetadata.getProcess().absoluteFilePath();

    qDebug() << "  to:";
    qDebug() << "    " << outputFile.absoluteFilePath();

    QStringList arguments = QStringList();
    arguments.append("--sdl");

    if (processMetadata.getSystemStructure().has_value()) {
        arguments.append(processMetadata.getSystemStructure().value().absoluteFilePath());
    }
    arguments.append(processMetadata.getProcess().absoluteFilePath());

    arguments.append("-o");
    arguments.append(outputFile.absoluteFilePath());

    arguments.append("-oi");
    arguments.append(outputInfoFile.absoluteFilePath());

    arguments.append(m_sdl2PromelaArgs);

    QProcess process = QProcess();

    if (!startSdl2PromelaProcess(process, arguments)) {
        return false;
    }

    return waitForSdl2PromelaProcess(process);
}

bool SdlToPromelaConverter::convertStopConditions(const QList<QFileInfo> &inputFiles, const QFileInfo &outputFile,
        const std::map<QString, ProcessMetadata> &inputSdlFiles, bool includeObservers)
{
    QStringList arguments;
    for (const auto &inputFile : inputFiles) {
        qDebug() << "Converting Stop Conditions file: " << inputFile.absoluteFilePath() << " to "
                 << outputFile.absoluteFilePath();
        arguments << "--scl" << inputFile.absoluteFilePath();
    }

    arguments << "-o" << outputFile.absoluteFilePath();

    if (includeObservers) {
        arguments << "--os";
    }

    for (const auto &iter : inputSdlFiles) {
        arguments.append("--sdl");
        if (iter.second.getSystemStructure().has_value()) {
            arguments.append(iter.second.getSystemStructure().value().absoluteFilePath());
        }
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

    auto start = std::chrono::high_resolution_clock::now();

    process.start(m_sdl2PromelaCommand, arguments);

    if (!process.waitForStarted(m_externalCommandStartTimeout)) {
        qCritical("Cannot start process.");
        QByteArray standardError = process.readAllStandardError();
        QByteArray standardOutput = process.readAllStandardOutput();

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        qCritical() << "Elapsed time:" << elapsed << "ms";

        QString str = QString(standardError);
        qCritical() << "Stderr: " << str;
        str = QString(standardOutput);
        qCritical() << "Stdout: " << str;

        process.terminate();
        return false;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    qDebug() << "Starting a process took:" << elapsed << "ms";

    return true;
}

bool SdlToPromelaConverter::waitForSdl2PromelaProcess(QProcess &process)
{
    qDebug() << "Waiting for sdl2promela process now";

    auto start = std::chrono::high_resolution_clock::now();

    if (!process.waitForFinished(m_externalCommandFinishTimeout)) {
        qCritical() << "Timeout while waiting for external process.";

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        qCritical() << "Elapsed time:" << elapsed << "ms";

        process.terminate();
        return false;
    }

    if (process.exitCode() != EXIT_SUCCESS) {
        qCritical() << "External process finished with code: " << process.exitCode();

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        qCritical() << "Elapsed time:" << elapsed << "ms";

        return false;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    qDebug() << "Process was working for:" << elapsed << "ms";

    return true;
}
}
