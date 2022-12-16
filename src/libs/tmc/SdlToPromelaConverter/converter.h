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

#include "processmetadata.h"

#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <chrono>
#include <map>

namespace tmc::converter {
/**
 * @brief Wrapper class used to run sdl2promela.
 */
class SdlToPromelaConverter final : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor.
     *
     * @param parent parent of the object.
     */
    SdlToPromelaConverter(QObject *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~SdlToPromelaConverter();

    /**
     * @brief Convert SDL files into promela file.
     *
     * @param processMetadata process metadata, contains location of input SDL files.
     * @param outputFile output promela file.
     * @return true if execution was successful, otherwise false.
     */
    bool convertSdl(const ProcessMetadata &processMetadata, const QFileInfo &outputFile);

    /**
     * @brief Convert SDL Observer files into promela file.
     *
     * @param processMetadata process metadata, contains location of input SDL files.
     * @param outputFile output promela file.
     * @param outputInfoFile output info file.
     * @return true if execution was successful, otherwise false.
     */
    bool convertObserverSdl(
            const ProcessMetadata &processMetadata, const QFileInfo &outputFile, const QFileInfo &outputInfoFile);

    /**
     * @brief Convert stop condition file into promela file.
     *
     * @param inputFiles list of filepaths to input stop condition files
     * @param outputFile output promela file
     * @param inputSdlFiles a map of ProcessMetadata objects, which are context for stop condition file
     * @param includeObservers true if generated Stop Condition shall check observers
     * @return true if execution was successful, otherwise false
     */
    bool convertStopConditions(const QList<QFileInfo> &inputFiles, const QFileInfo &outputFile,
            const std::map<QString, ProcessMetadata> &inputSdlFiles, bool includeObservers);

    /**
     * @brief   Set timeout for external sdl2promela process.
     *
     * @param   timeout Timeout value.
     */
    void setSdl2PromelaTimeout(int timeout);

Q_SIGNALS:
    /**
     * @brief Conversion message.
     *
     * @param text message text.
     */
    void message(QString text);
    /**
     * @brief Conversion was finished.
     *
     * @param success true if conversion was successful, otherwise false.
     */
    void conversionFinished(bool success);

private:
    void startSdl2PromelaProcess(QProcess &process, const QStringList &arguments);

private Q_SLOTS:
    void processStderrReady();
    void processStdoutReady();
    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void timeout();

private:
    QStringList m_sdl2PromelaArgs;
    QTimer *m_timer;
    QProcess *m_process;
    std::optional<int> m_sdl2PromelaTimeout;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint;

    const static QString m_sdl2PromelaCommand;
    constexpr static int m_externalCommandStartTimeout = 45000;
    constexpr static int m_externalCommandFinishTimeout = 60000;
};
}
