/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "common.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QPalette>
#include <QWidget>

#ifdef Q_OS_WIN
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

namespace shared {

/*!
  \namespace shared

  \brief The collection of utilites shared among all submodules.
*/

/*!
 * \typedef Id
 * \brief The common Id used as internal objects referring.
 */

/*!
 * \brief InvalidId
 * Global constant representing the empty/invalid ID.
 */

Id createId()
{
    return QUuid::createUuid();
}

/*!
  \enum shared::FileCopyingMode

  This enum specifies a file overwriting policy:
    \value Keep
        Do not overwrite existing file.
    \value Overwrite
        Do overwrite existing file.
 */

/*!
 * \brief Copies the \a source file from resources to the \a target file.

 * Returns \c true if the \a source file copied succesfully and
 * the QFile::WriteUser permission explicitly set for the \a target
 * (otherwise it would be read-only as any file in qrc).
 * If the \target file already exists, this function will not owervirite
 * and return \c false;
 */
bool copyResourceFile(const QString &source, const QString &target, FileCopyingMode replaceMode)
{
    if (source.isEmpty() || target.isEmpty() || !QFile::exists(source)) {
        return false;
    }

    if (QFile::exists(target)) {
        if (FileCopyingMode::Overwrite == replaceMode) {
            if (!QFile::remove(target)) {
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
            storedFile.setPermissions(QFile::WriteUser | QFile::ReadUser);
            result = true;
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

void setWidgetFontColor(QWidget *widget, const QColor &color)
{
    if (!widget || !color.isValid())
        return;

    QPalette p(widget->palette());
    p.setColor(QPalette::Text, color);
    widget->setPalette(p);
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

}
