/*
  Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "standardpaths.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

namespace shared {

bool StandardPaths::m_testMode = false;

/**
 * Returns the directory where files of type should be written to, or an empty string if the location cannot be
 * determined.
 * @note The storage location returned may not exist; that is, it may need to be created by the system or the user.
 * Check shared::StandardPaths::writableLocation for more details
 */
QString StandardPaths::writableLocation(QStandardPaths::StandardLocation type)
{
    const QString dir = spaceCreatorDir(type);
    if (!dir.isEmpty()) {
        QFileInfo fi(dir);
        if (fi.isWritable()) {
            return dir;
        }
    }

    // Fallback
    QString result = QStandardPaths::writableLocation(type);
    if (m_testMode) {
        if (!result.endsWith(qApp->applicationName())) {
            result = QString("%1%2%3").arg(result, QDir::separator(), qApp->applicationName());
        }
    }
    return result;
}

/**
 * Returns all the directories where files of type belong.
 * Check QStandardPaths::standardLocations for more details
 */
QStringList StandardPaths::standardLocations(QStandardPaths::StandardLocation type)
{
    QStringList dirList = QStandardPaths::standardLocations(type);
    const QString dir = spaceCreatorDir(type);
    if (!dir.isEmpty()) {
        dirList.prepend(dir);
    }

    return dirList;
}

/**
 * If testMode is true, this enables a special "test mode" in QStandardPaths, which changes writable locations to point
 * to test directories. This prevents auto tests from reading or writing to the current user's configuration.
 * Check QStandardPaths::setTestModeEnabled for more details
 */
void StandardPaths::setTestModeEnabled(bool testMode)
{
    m_testMode = testMode;
    QStandardPaths::setTestModeEnabled(testMode);
}

QString StandardPaths::spaceCreatorDir(QStandardPaths::StandardLocation type)
{
    if (m_testMode) {
        return {};
    }

    const QString home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    static const QString appDir = "QtProject/QtCreator";
    switch (type) {
    case QStandardPaths::AppLocalDataLocation:
    case QStandardPaths::AppDataLocation:
        return QString("%1/.local/share/%2").arg(home, appDir);
        break;
    case QStandardPaths::CacheLocation:
        return QString("%1/.cache/%2").arg(home, appDir);
        break;
    case QStandardPaths::AppConfigLocation:
        return QString("%1/.config/%2").arg(home, appDir);
        break;
    default:
        break;
    }
    return {};
}

} // namespace shared
