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

#include "launchconfigloader.h"

#include <QDebug>
#include <QFile>

namespace testgenerator {

constexpr int BOARD_CONFIG_LENGTH = 6;

LaunchConfiguration::LaunchConfiguration(const QString &launchScriptPath, const QString &client, QString clientParams,
        const QString &server, QString serverParams)
    : scriptPath(launchScriptPath)
    , clientName(client)
    , clientArgs(clientParams)
    , serverName(server)
    , serverArgs(serverParams)
{
    clientArgsParsed = clientParams.replace("$SCRIPT_PATH", scriptPath).split(" ");
    serverArgsParsed = serverParams.replace("$BIN_PATH", "hostpartition").split(" ");
}

LaunchConfigLoader::LaunchConfigLoader(const QString &launchConfigPath)
    : configPath(launchConfigPath)
{
    loadConfig();
}

auto LaunchConfigLoader::getConfig() -> QMap<QString, LaunchConfiguration> &
{
    return configMap;
}

auto LaunchConfigLoader::loadConfig() -> bool
{
    QFile file(configPath);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList conf = line.split(';');
            if (conf.size() >= BOARD_CONFIG_LENGTH) {
                configMap.insert(conf[0], LaunchConfiguration(conf[1], conf[2], conf[3], conf[4], conf[5]));
            } else {
                qDebug() << "Not enough information in boards_config.txt file for board " << conf[0];
            }
        }
        file.close();
    } else {
        qDebug() << "Could not find file with default boards configuration at path: " << configPath;
        return false;
    }
    return true;
}

auto LaunchConfigLoader::saveConfig(const QString &boardName, const LaunchConfiguration &launchConfig) -> bool
{
    configMap[boardName] = launchConfig;
    QFile file(configPath);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        for (const auto &key : configMap.keys()) {
            stream << key << ';' << configMap[key].scriptPath << ";" << configMap[key].clientName << ";"
                   << configMap[key].clientArgs << ";" << configMap[key].serverName << ";" << configMap[key].serverArgs
                   << '\n';
        }
    }
    return true;
}

}
