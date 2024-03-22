/*
  Copyright (C) 2024 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QProcess>
#include <memory>

namespace shared {

/*!
 * The ExternalProcess class is used to start external programs with the environment outside of the SpaceCreator
 * AppImage.
 * In addition XDG_DATA_HOME and XDG_DATA_DIRS are set.
 */
class ExternalProcess
{
public:
    static std::unique_ptr<QProcess> create(QObject *parent = nullptr);
    static int executeBlocking(const QString &program, const QStringList &arguments = {});

protected:
    explicit ExternalProcess() { }
};

} // namespace shared
