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

#include <QProcess>
#include <memory>

namespace testgenerator {

/**
 * @brief  RAII implementing wrapper for QProcess. It is used to simplify code in GdbConnector class, as the required
 *         behavior is present in the destructor, called automatically when the object is destroyed
 */
class Process
{
public:
    /**
     * @brief Construct a Process and start a specified program as a process
     *
     * @param programPath path to program to be run
     * @param args        arguments for the program to be run
     * @param workingDir  working directory in which the program shall be ran
     */
    Process(const QString &programPath, const QStringList &args, const QString &workingDir);

    /**
     * @brief Destroy the Process
     *
     */
    ~Process();

    /**
     * @brief get a pointer to the process
     *
     * @return a pointer to the process
     */
    auto get() -> QProcess *;

private:
    std::unique_ptr<QProcess> m_process;
};

} // namespace testgenerator
