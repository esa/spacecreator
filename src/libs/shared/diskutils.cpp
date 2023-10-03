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

#include "diskutils.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>

namespace shared {

/*!
  \enum shared::FileCopyingMode

This enum specifies a file overwriting policy:
 \var Keep
     Do not overwrite existing file.
 \var Overwrite
     Do overwrite existing file.
*/

/*!
 * \brief Copies the \a source file from resources to the \a target file.

 * Returns \c true if the \a source file copied successfully and
 * the QFile::WriteUser permission explicitly set for the \a target
 * (otherwise it would be read-only as any file in qrc).
 * If the \a target file already exists, this function will not overwrite
 * and return \c false;
 */
bool copyFile(const QString &source, const QString &target, FileCopyingMode replaceMode)
{
    if (source.isEmpty() || target.isEmpty()) {
        return false;
    }
    if (!QFile::exists(source)) {
        qWarning() << "Source file " << source << " does not exist";
    }

    if (QFile::exists(target)) {
        if (FileCopyingMode::Overwrite == replaceMode) {
            if (!QFile::remove(target)) {
                qWarning() << "Unable to remove old file" << target;
                return false;
            }
        } else {
            return false;
        }
    }

    bool result(false);
#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup++;
#endif
    try {
        ensureDirExists(QFileInfo(target).path());
        if (QFile::copy(source, target)) {
            QFile storedFile(target);
            result = storedFile.setPermissions(QFile::WriteUser | QFile::ReadUser);
            if (!result) {
                qWarning() << "Unable to set permissions for " << target;
            }
        } else {
            qWarning() << "Can't copy resource file " << source << "-->" << target;
        }
    } catch (...) {
        qWarning() << "Failed to copy resource file " << source << "-->" << target;
    }
#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup--;
#endif
    return result;
}

bool ensureDirExists(const QString &path)
{
    QDir dir(path);
    if (!dir.exists(path)) {
        if (!dir.mkpath(path)) {
            qWarning() << "Failed to create path:" << path;
            return false;
        }
    }

    return true;
}

bool ensureFileExists(const QString &filePath, const QString &defaultFilePath)
{
    if (!QFileInfo::exists(filePath) && !copyFile(defaultFilePath, filePath)) {
        qWarning() << "Can't create default file path:" << filePath << "from:" << defaultFilePath;
        return false;
    }
    return true;
}

bool copyDir(const QString &source, const QString &dest, FileCopyingMode replaceMode)
{
    static const QDir::Filters filters = QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files;
    QDir sourceExportDir { source };
    if (sourceExportDir.isEmpty(filters)) {
        return true;
    }
    sourceExportDir.setFilter(filters);
    const QDir targetExportDir { dest };
    targetExportDir.mkpath(QLatin1String("."));
    QDirIterator it { sourceExportDir, QDirIterator::Subdirectories };
    bool success = true;
    while (it.hasNext()) {
        const QString filePath = it.next();
        const QFileInfo fileInfo = it.fileInfo();
        const QString relPath = sourceExportDir.relativeFilePath(fileInfo.absoluteFilePath());
        if (fileInfo.isSymLink()) {
            const QString linkTarget = fileInfo.dir().relativeFilePath(fileInfo.symLinkTarget());
            const bool result = QFile::link(linkTarget, targetExportDir.absoluteFilePath(relPath));
            if (!result) {
                qWarning() << "Error during symlink copying:" << filePath << fileInfo.symLinkTarget()
                           << targetExportDir.absoluteFilePath(relPath);
            }
            success = success && result;
        } else if (fileInfo.isDir()) {
            if (!QDir(targetExportDir.filePath(relPath)).exists()) {
                const bool result = targetExportDir.mkpath(relPath);
                success = success && result;
            }
        } else {
            const bool result =
                    copyFile(fileInfo.absoluteFilePath(), targetExportDir.absoluteFilePath(relPath), replaceMode);
            if (!result) {
                qWarning() << "Error during source file copying:" << filePath
                           << targetExportDir.absoluteFilePath(relPath);
            }
            success = success && result;
        }
    }
    return success;
}

QString joinedPath(const QString &arg1, const QString &arg2)
{
    QString plainDirectory = arg1;
    if (plainDirectory.endsWith(QDir::separator())) {
        plainDirectory.remove(plainDirectory.size() - 1, 1);
    }
    return QString("%1%3%2").arg(plainDirectory, arg2, QDir::separator());
}

QString joinedPath(const QString &arg1, const QString &arg2, const QString &arg3)
{
    return joinedPath(joinedPath(arg1, arg2), arg3);
}

QString filePath(const QString &directory, const QString &filebasename, const QString &suffix)
{
    QString plainSuffix = suffix;
    if (plainSuffix.startsWith(".")) {
        plainSuffix = plainSuffix.remove(0, 1);
    }
    QString plainDirectory = directory;
    if (plainDirectory.endsWith(QDir::separator())) {
        plainDirectory.remove(plainDirectory.size() - 1, 1);
    }
    return QString("%1%2%3.%4").arg(plainDirectory, QDir::separator(), filebasename, plainSuffix);
}

} // namespace shared
