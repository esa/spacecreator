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

#include "appoptions.h"

#include "settingsmanager.h"

namespace taste3 {

AppOption::AppOption(const QString &name, const QVariant &defaultValue)
    : Name(name)
    , DefaultValue(defaultValue)
{
}

QVariant AppOption::read() const
{
    if (QSettings *settings = SettingsManager::instance()->storage())
        return settings->value(Name, DefaultValue);

    return QVariant();
}

void AppOption::write(const QVariant &val) const
{
    if (val == DefaultValue)
        return;

    if (QSettings *settings = SettingsManager::instance()->storage())
        settings->setValue(Name, val);
}

GroupMainWindow::GroupMainWindow()
    : Geometry(QString("%1/Geometry").arg(localName()))
    , State(QString("%1/State").arg(localName()))
    , LastFilePath(QString("%1/LastFilePath").arg(localName()))
    , LastTab(QString("%1/LastTab").arg(localName()))
{
}

const GroupMainWindow AppOptions::MainWindow = {};

GroupBugReport::GroupBugReport()
    : Host(QString("%1/Host").arg(localName()))
    , ProjectID(QString("%1/ProjectID").arg(localName()))
    , AccessToken(QString("%1/AccessToken").arg(localName()))
{
}

const GroupBugReport AppOptions::BugReport = {};

} // namespace taste3
