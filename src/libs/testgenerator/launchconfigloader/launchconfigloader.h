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

#include <QDataStream>
#include <QTextStream>

namespace testgenerator {

/**
 * @brief Struct containing tested binary launching params.
 *
 */
struct LaunchConfiguration {
    /** name of the board */
    QString boardName;
    /** path to the GDB script for managing the data acquisition */
    QString scriptPath;
    /** debugger client to use for testing binary */
    QString clientName;
    /** arguments for debugger client */
    QString clientArgs;
    /** debugger server to use for testing binary */
    QString serverName;
    /** arguments for debugger server */
    QString serverArgs;
    /** arguments for debugger client converted to list with parsed variables */
    QStringList clientArgsParsed;
    /** arguments for debugger server converted to list with parsed variables */
    QStringList serverArgsParsed;
    /** endianess of the board architecture */
    QDataStream::ByteOrder endianess;

    /**
     * @brief Default constructor of the class
     *
     */
    LaunchConfiguration() = default;

    /**
     * @brief Constructor of the class
     *
     * @param name name of the board
     * @param launchScriptPath path to the GDB script for launching the tested binary
     * @param client name of the debugger client to use for testing binary
     * @param clientParams arguments for debugger client
     * @param server name of the debugger server to use for testing binary
     * @param serverParams arguments for debugger server
     * @param byteOrder byte order used by the board's architecture
     *
     * @return true if there was no error during creation of the test harness files and false otherwise
     */
    LaunchConfiguration(const QString &name, const QString &launchScriptPath, const QString &client,
            QString clientParams, const QString &server, QString serverParams, const QDataStream::ByteOrder byteOrder);
};

/**
 * @brief Class for storing launch parameters of every board
 *
 */
class LaunchConfigLoader
{
public:
    /**
     * @brief Constructor of the class
     *
     * @param launchConfigPath path to the file with launch configuration of each board
     */
    LaunchConfigLoader(const QString &launchConfigPath);

    /**
     * @brief Load launch parameters of every board
     *
     * @return an optional containing a mapping of boards to their configurations
     */
    auto loadConfig() -> std::optional<QMap<QString, LaunchConfiguration>>;

    /**
     * @brief Store launch parameters of every board in a CSV file specified in constructor
     *
     * @param configMap a configuration mapping to save
     *
     * @return true if saving configuration is successful, false otherwise
     */
    auto saveConfig(const QMap<QString, LaunchConfiguration> &configMap) -> bool;

private:
    QString configPath;
};

}
