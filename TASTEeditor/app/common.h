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

#pragma once

#include <QMetaType>
#include <QUuid>

class QAction;
namespace taste3 {
namespace common {

typedef QUuid Id;
Id createId();

/*!
 * Copies the \a source file from resources to the \a target file.
 * \Returns true if the \a source file copied succesfully and
 * the QFile::WriteUser permission explicitly set for the \a target
 * (otherwise it would be read-only as any file in qrc).
 * If the \target file already exists, this function will not owervirite
 * and return false;
 */
bool copyResourceFile(const QString &source, const QString &target);

void setWidgetFontColor(QWidget *widget, const QColor &color);

bool ensureDirExists(const QString &path);

void registerAction(const QString &caller, QAction *action, const QString &title = QString(),
                    const QString &description = QString());

} // ns common
} // ns taste3

Q_DECLARE_METATYPE(taste3::common::Id)
