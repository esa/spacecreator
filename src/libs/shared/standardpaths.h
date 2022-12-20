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

#pragma once

#include <QStandardPaths>
#include <QString>
#include <QStringList>

namespace shared {

/**
 * The StandardPaths class is a replacement for QStandardPaths. So SpaceCreator and it's tools use the same config and
 * cache diretories.
 */
class StandardPaths
{
public:
    static QString writableLocation(QStandardPaths::StandardLocation type);
    static QStringList standardLocations(QStandardPaths::StandardLocation type);

    static void setTestModeEnabled(bool testMode);

private:
    static QString spaceCreatorDir(QStandardPaths::StandardLocation type);

    static bool m_testMode;
};

} // namespace shared
