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

#include <QObject>
#include <QString>
#include <QStringList>

namespace shared {

/*!
 * \brief The QMakeFile class handles a single .pro file of the QMake project management
 */
class QMakeFile : public QObject
{
    Q_OBJECT
public:
    /*!
     * Replaces the \p oldFileName by \p newFileName in the given \p proFilePath .pro file
     */
    static bool renameFileName(const QString &proFilePath, const QString &oldFileName, const QString &newFileName);

    /*!
     * Rename directory the \p oldDirectory by \p newDirectory in the given \p proFilePath .pro file
     */
    static bool renameDirectory(const QString &proFilePath, const QString &oldDirectory, const QString &newDirectory);

    /*!
     * Creates a simple .pro file ind the directory set by \p path.
     * All files in that path are added to the project. As well and files set by \p externalFiles
     */
    static bool createProFileForDirectory(const QString &path, const QStringList &externalFiles);

    /*!
       Read from pro-file located at given \p path and returns list of files with
       given \p extinsions relative to \p path unless it has absolute filesystem path.
     */
    static QStringList readFilesList(const QString &path, const QStringList &fileExtentions);

private:
    static bool replace(const QString &proFilePath, const QString &oldString, const QString &newString,
            const QByteArray &allowedCharsBefore, const QByteArray &allowedCharsAfter);
};

} // namespace shared
