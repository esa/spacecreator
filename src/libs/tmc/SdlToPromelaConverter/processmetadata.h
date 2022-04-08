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

#include <QFileInfo>
#include <QString>

namespace tmc::converter {
/**
 * @brief Process Metadata contains information about SDL process.
 */
class ProcessMetadata
{
public:
    /**
     * @brief Constructor.
     *
     * @param name SDL process name.
     * @param systemStructure system_structure.pr file location
     * @param process location of process implementation file
     * @param datamodel location of process datamodel ASN.1 file
     */
    ProcessMetadata(QString name, QFileInfo systemStructure, QFileInfo process, QFileInfo datamodel);

    /**
     * @brief Getter for process name.
     *
     * @return Name of SDL process.
     */
    const QString &getName() const noexcept;
    /**
     * @brief Getter for system structure location.
     *
     * @return Location of system_structure.pr file.
     */
    const QFileInfo &getSystemStructure() const noexcept;
    /**
     * @brief Getter for process implementation location.
     *
     * @return Location of process implementation file.
     */
    const QFileInfo &getProcess() const noexcept;
    /**
     * @brief Getter for process datamodel.
     *
     * @return Location fo process datamodel ASN.1 file.
     */
    const QFileInfo &getDatamodel() const noexcept;

private:
    const QString m_name;
    const QFileInfo m_systemStructure;
    const QFileInfo m_process;
    const QFileInfo m_datamodel;
};
}
