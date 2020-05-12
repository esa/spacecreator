/*
  Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "settingsmanager.h"

#include <QDebug>
#include <QStandardPaths>

static QString path()
{
    return QString("%1/settings.conf").arg(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
}

namespace shared {

SettingsManager::SettingsManager()
    : m_settings(new QSettings(path(), QSettings::IniFormat, this))
{
    qDebug() << "Config:" << m_settings->fileName();
}

SettingsManager *SettingsManager::instance()
{
    static shared::SettingsManager *instance = nullptr;

    if (instance == nullptr) {
        instance = new shared::SettingsManager;
    }

    return instance;
}

QSettings *SettingsManager::storage()
{
    return m_settings;
}

}
