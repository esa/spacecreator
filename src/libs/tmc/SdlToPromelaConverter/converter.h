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
#include <QProcess>
#include <QString>
#include <QStringList>
#include <map>

namespace tmc::converter {
/**
 * @brief Wrapper class used to run sdl2promela.
 */
class SdlToPromelaConverter final
{
public:
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

private:
    bool startSdl2PromelaProcess(QProcess &process, const QStringList &arguments);
    bool waitForSdl2PromelaProcess(QProcess &process);

private:
    QStringList m_sdl2PromelaArgs;

    const static QString m_sdl2PromelaCommand;
    constexpr static int m_externalCommandTimeout = 30000;
};
}
