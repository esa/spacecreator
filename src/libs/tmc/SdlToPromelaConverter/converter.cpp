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

namespace tmc::converter {

const QString SdlToPromelaConverter::m_sdl2PromelaCommand = QStringLiteral("sdl2promela");

SdlToPromelaConverter::SdlToPromelaConverter(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_process(new QProcess(this))
{
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(processStderrReady()));
    connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStdoutReady()));
    connect(m_process, SIGNAL(started()), this, SLOT(processStarted()));
    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(processFinished(int, QProcess::ExitStatus)));
}

SdlToPromelaConverter::~SdlToPromelaConverter()
{
    m_timer->stop();
    if (m_process->state() != QProcess::ProcessState::NotRunning) {
        m_process->kill();
        m_process->waitForFinished();
    }
}

bool SdlToPromelaConverter::convertSdl(const ProcessMetadata &processMetadata, const QFileInfo &outputFile)
{
    if (processMetadata.getSystemStructure().has_value() && !processMetadata.getSystemStructure().value().exists()) {
        QString text = QString("File %1 does not exist.\n")
                               .arg(processMetadata.getSystemStructure().value().absoluteFilePath());
        Q_EMIT message(text);
        return false;
    }

    if (!processMetadata.getProcess().exists()) {
        QString text = QString("File %1 does not exist.\n").arg(processMetadata.getProcess().absoluteFilePath());
        Q_EMIT message(text);
        return false;
    }

    Q_EMIT message(QString("Converting SDL files:\n"));

    if (processMetadata.getSystemStructure().has_value()) {
        QString text = QString("    %1\n").arg(processMetadata.getSystemStructure().value().absoluteFilePath());
        Q_EMIT message(text);
    }

    Q_EMIT message(QString("    %1\n").arg(processMetadata.getProcess().absoluteFilePath()));

    Q_EMIT message(QString("  to:\n"));
    Q_EMIT message(QString("    %1\n").arg(outputFile.absoluteFilePath()));

    QStringList arguments = QStringList();
    arguments.append("--sdl");

    if (processMetadata.getSystemStructure().has_value()) {
        arguments.append(processMetadata.getSystemStructure().value().absoluteFilePath());
    }
    arguments.append(processMetadata.getProcess().absoluteFilePath());

    arguments.append("-o");
    arguments.append(outputFile.absoluteFilePath());

    arguments.append(m_sdl2PromelaArgs);

    startSdl2PromelaProcess(*m_process, arguments);

    return true;
}

bool SdlToPromelaConverter::convertObserverSdl(
        const ProcessMetadata &processMetadata, const QFileInfo &outputFile, const QFileInfo &outputInfoFile)
{
    if (processMetadata.getSystemStructure().has_value() && !processMetadata.getSystemStructure().value().exists()) {
        QString text = QString("File %1 does not exist.\n")
                               .arg(processMetadata.getSystemStructure().value().absoluteFilePath());
        Q_EMIT message(text);
        return false;
    }

    if (!processMetadata.getProcess().exists()) {
        QString text = QString("File %1 does not exist.\n").arg(processMetadata.getProcess().absoluteFilePath());
        Q_EMIT message(text);
        return false;
    }

    Q_EMIT message(QString("Converting SDL files:\n"));
    if (processMetadata.getSystemStructure().has_value()) {
        QString text = QString("    %1\n").arg(processMetadata.getSystemStructure().value().absoluteFilePath());
        Q_EMIT message(text);
    }

    Q_EMIT message(QString("    %1\n").arg(processMetadata.getProcess().absoluteFilePath()));

    Q_EMIT message(QString("  to:\n"));
    Q_EMIT message(QString("    %1\n").arg(outputFile.absoluteFilePath()));

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

    startSdl2PromelaProcess(*m_process, arguments);

    return true;
}

bool SdlToPromelaConverter::convertStopConditions(const QList<QFileInfo> &inputFiles, const QFileInfo &outputFile,
        const std::map<QString, ProcessMetadata> &inputSdlFiles, bool includeObservers)
{
    QStringList arguments;
    for (const auto &inputFile : inputFiles) {
        QString text = QString("Converting Stop Conditions file: %1 to %2\n")
                               .arg(inputFile.absoluteFilePath())
                               .arg(outputFile.absoluteFilePath());
        Q_EMIT message(text);
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

    startSdl2PromelaProcess(*m_process, arguments);

    return true;
}

void SdlToPromelaConverter::startSdl2PromelaProcess(QProcess &process, const QStringList &arguments)
{
    Q_EMIT message(QString("Executing: %1 with args:\n").arg(m_sdl2PromelaCommand));
    for (const QString &arg : arguments) {
        Q_EMIT message(QString("    %1\n").arg(arg));
    }

    m_startPoint = std::chrono::high_resolution_clock::now();

    m_timer->setSingleShot(true);
    m_timer->start(m_externalCommandStartTimeout);
    process.start(m_sdl2PromelaCommand, arguments);
}

void SdlToPromelaConverter::processStderrReady()
{
    if (m_process != nullptr) {
        QByteArray buffer = m_process->readAllStandardError();
        QString text = QString(buffer);
        Q_EMIT message(text);
    }
}

void SdlToPromelaConverter::processStdoutReady()
{
    if (m_process != nullptr) {
        QByteArray buffer = m_process->readAllStandardOutput();
        QString text = QString(buffer);
        Q_EMIT message(text);
    }
}

void SdlToPromelaConverter::processStarted()
{
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_startPoint).count();
    Q_EMIT message(QString("Elapsed time: %1ms\n").arg(elapsed));

    m_timer->stop();
    m_timer->setSingleShot(true);
    m_timer->start(m_externalCommandFinishTimeout);
}

void SdlToPromelaConverter::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_startPoint).count();
    Q_EMIT message(QString("Process was working for: %1ms\n").arg(elapsed));
    Q_UNUSED(exitStatus);
    m_timer->stop();
    if (exitCode != EXIT_SUCCESS) {
        Q_EMIT message(QString("%1 finished with code %2\n").arg(m_sdl2PromelaCommand).arg(exitCode));
        Q_EMIT conversionFinished(false);
        return;
    }
    Q_EMIT conversionFinished(true);
}

void SdlToPromelaConverter::timeout()
{
    m_process->terminate();
    Q_EMIT message(QString("Timeout.\n"));
    Q_EMIT conversionFinished(false);
}
}
