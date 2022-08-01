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

constexpr int BOARD_CONFIG_LENGTH = 9;
const QString defaultBinaryName = "hostpartition";

LaunchConfiguration::LaunchConfiguration(const QString &name, const QString &launchScriptPath, const QString &client,
        QString clientParams, const QString &server, QString serverParams,
        const DataReconstructor::TypeLayoutInfos &typeLayout, const QDataStream::ByteOrder byteOrder,
        const int stackSizeBytes)
    : boardName(name)
    , scriptPath(launchScriptPath)
    , clientName(client)
    , clientArgs(clientParams)
    , serverName(server)
    , serverArgs(serverParams)
    , typeLayoutInfos(typeLayout)
    , endianess(byteOrder)
    , stackSize(stackSizeBytes)
{
    clientArgsParsed = clientParams.replace("$SCRIPT_PATH", scriptPath).split(" ");
    serverArgsParsed = serverParams.replace("$BIN_PATH", defaultBinaryName).split(" ");
}

LaunchConfigLoader::LaunchConfigLoader(const QString &launchConfigPath)
    : configPath(launchConfigPath)
{
}

auto typeLayoutToString(const DataReconstructor::TypeLayoutInfos &layoutInfos) -> QString
{
    QString layoutInfosStr;
    for (const auto &typeName : layoutInfos.keys()) {
        layoutInfosStr += typeName + "," + QString::number(layoutInfos[typeName].first) + ","
                + QString::number(layoutInfos[typeName].second) + "|";
    }
    return layoutInfosStr;
}

auto typeLayoutFromString(const QString &layoutInfosStr) -> DataReconstructor::TypeLayoutInfos
{
    constexpr int TYPE_NAME_IDX = 0;
    constexpr int DATA_LENGTH_IDX = 1;
    constexpr int PADDING_LENGTH_IDX = 2;
    DataReconstructor::TypeLayoutInfos layoutInfos {};
    QStringList layoutInfosList = layoutInfosStr.split("|");
    layoutInfosList.removeLast();
    for (const auto &layoutInfoStr : layoutInfosList) {
        QStringList layoutInfoList = layoutInfoStr.split(",");
        layoutInfos.insert(layoutInfoList[TYPE_NAME_IDX],
                { layoutInfoList[DATA_LENGTH_IDX].toInt(), layoutInfoList[PADDING_LENGTH_IDX].toInt() });
    }
    return layoutInfos;
}

auto LaunchConfigLoader::loadConfig() -> std::optional<QMap<QString, LaunchConfiguration>>
{
    constexpr int BOARD_NAME_IDX = 0;
    constexpr int SCRIPT_PATH_IDX = 1;
    constexpr int CLIENT_NAME_IDX = 2;
    constexpr int CLIENT_PARAMS_IDX = 3;
    constexpr int SERVER_NAME_IDX = 4;
    constexpr int SERVER_PARAMS_IDX = 5;
    constexpr int TYPE_LAYOUT_IDX = 6;
    constexpr int BYTE_ORDER_IDX = 7;
    constexpr int STACK_SIZE_IDX = 8;

    QMap<QString, LaunchConfiguration> configMap;
    QFile file(configPath);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList conf = line.split(';');
            if (conf.size() >= BOARD_CONFIG_LENGTH) {
                configMap.insert(conf[BOARD_NAME_IDX],
                        LaunchConfiguration(conf[BOARD_NAME_IDX], conf[SCRIPT_PATH_IDX], conf[CLIENT_NAME_IDX],
                                conf[CLIENT_PARAMS_IDX], conf[SERVER_NAME_IDX], conf[SERVER_PARAMS_IDX],
                                typeLayoutFromString(conf[TYPE_LAYOUT_IDX]),
                                static_cast<QDataStream::ByteOrder>(conf[BYTE_ORDER_IDX].toInt()),
                                conf[STACK_SIZE_IDX].toInt()));
            } else {
                qWarning() << "Not enough information in boards_config.txt file for board " << conf[BOARD_NAME_IDX];
            }
        }
        file.close();
    } else {
        qWarning() << "Could not find file with default boards configuration at path: " << configPath;
        return std::nullopt;
    }
    return configMap;
}

auto LaunchConfigLoader::saveConfig(const QMap<QString, LaunchConfiguration> &configMap) -> bool
{
    QFile file(configPath);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        for (const auto &key : configMap.keys()) {
            stream << key << ';' << configMap[key].scriptPath << ";" << configMap[key].clientName << ";"
                   << configMap[key].clientArgs << ";" << configMap[key].serverName << ";" << configMap[key].serverArgs
                   << ";" << typeLayoutToString(configMap[key].typeLayoutInfos) << ";" << configMap[key].endianess
                   << ";" << QString::number(configMap[key].stackSize) + ';' + '\n';
        }
    }
    return true;
}

}
