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

#include "app/context/action/actionsmanager.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QFile>

#ifdef Q_OS_WIN
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

namespace taste3 {
namespace common {

Id createId()
{
    return QUuid::createUuid();
}

bool copyResourceFile(const QString &source, const QString &target, FileCopyingMode replaceMode)
{

    if (source.isEmpty() || target.isEmpty() || !QFile::exists(source))
        return false;

    if (QFile::exists(target)) {
        if (FileCopyingMode::Overwrite == replaceMode) {
            if (!QFile::remove(target))
                return false;
        } else
            return false;
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
    if (!dir.exists(path))
        if (!dir.mkpath(path)) {
            qWarning() << "Failed to create path:" << path;
            return false;
        }

    return true;
}

void registerAction(const QString &caller, QAction *action, const QString &title, const QString &description)
{
    if (!action) {
        qWarning() << "Null action can not be registered" << caller;
        return;
    }

    const QString &titleRef = title.isEmpty() ? action->text() : title;
    if (titleRef.isEmpty()) {
        qWarning() << "Action with no Title not be registered" << caller;
        return;
    }

    if (description.isEmpty()) {
        qWarning() << "Action with no Description not be registered" << caller;
        return;
    }

    if (!taste3::ctx::ActionsManager::registerScriptableAction(action, title, description)) {
        qWarning() << caller << "The registration of action failed; probably the duplicate key used:\n";
    }
}

QString validatedName(const QString &objectName)
{
    if (objectName.isEmpty())
        return {};

    QString result;
    std::transform(objectName.begin(), objectName.end(), std::back_inserter(result),
                   [](const QChar &ch) { return ch.isLetterOrNumber() ? ch : QLatin1Char('_'); });
    return result;
}

} // ns common
} // ns taste3
