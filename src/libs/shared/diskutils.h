/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public License
along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include <QString>

namespace shared {

/*!
   \brief The FileCopyingMode enum
 */
enum FileCopyingMode
{
    Keep,
    Overwrite
};

bool moveDir(const QString &source, const QString &dest, FileCopyingMode replaceMode = FileCopyingMode::Keep);

bool copyDir(const QString &source, const QString &dest, FileCopyingMode replaceMode = FileCopyingMode::Keep);

bool copyFile(const QString &source, const QString &target, FileCopyingMode replaceMode = FileCopyingMode::Keep);

bool ensureDirExists(const QString &path);

bool ensureFileExists(const QString &filePath, const QString &defaultFilePath);

bool isSame(const QString &filePath1, const QString &filePath2);

/*!
 * Returns single string connecting all arguments using the filesystem separator
 */
QString joinedPath(const QString &arg1, const QString &arg2);
QString joinedPath(const QString &arg1, const QString &arg2, const QString &arg3);

/*!
 * Turns the directory, file basename and the filename suffix into a single file path string
 */
QString filePath(const QString &directory, const QString &filebasename, const QString &suffix);

} // namespace shared
