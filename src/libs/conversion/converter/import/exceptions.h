/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include <QString>
#include <stdexcept>

namespace converter::import {

/**
 * @brief   Base class for all exceptions in importer module
 */
class ImportException : public std::exception
{
public:
    /**
     * @brief   Constructor
     *
     * @param   message     Error message
     */
    ImportException(QString message);

public:
    /**
     * @brief   Returns error message as const char*
     *
     * @returns Error message
     */
    virtual auto what() const noexcept -> const char * override;
    /**
     * @brief   Returns error message as QString
     *
     * @returns Error message
     */
    auto errorMessage() const noexcept -> const QString &;

protected:
    /**
     * @brief   Default constructor, only for inheriting classes
     */
    ImportException() = default;

protected:
    /** @brief  Error message */
    QString m_message;
};

/**
 * @brief   Exception thrown when required file doesn't exist
 */
class FileNotFound final : public ImportException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   filename    File that is missing
     * @param   message     Additional error message
     */
    FileNotFound(const QString &filename, const QString &message);
};

} // namespace converter::import
