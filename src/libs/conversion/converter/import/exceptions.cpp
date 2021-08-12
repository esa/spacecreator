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

#include "import/exceptions.h"

namespace converter::import {

ImportException::ImportException(QString message)
    : m_message(std::move(message))
{
}

const char *ImportException::what() const noexcept
{
    return m_message.toUtf8().constData();
}

const QString &ImportException::errorMessage() const noexcept
{
    return m_message;
}

FileNotFound::FileNotFound(const QString &filename, const QString &message)
    : ImportException(QString("File '%1' not found: '%2'").arg(filename).arg(message))
{
}

} // namespace converter::import
