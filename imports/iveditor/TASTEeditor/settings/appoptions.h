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

#pragma once

#include "settingsappoption.h"

#include <QVector>

namespace aadlinterface {

class GroupMainWindow
{
public:
    GroupMainWindow();

    const shared::SettingsAppOption Geometry;
    const shared::SettingsAppOption State;
    const shared::SettingsAppOption LastFilePath;
    const shared::SettingsAppOption LastTab;

private:
    GroupMainWindow(const GroupMainWindow &) = delete;
    GroupMainWindow &operator=(const GroupMainWindow &) = delete;

    static const QString localName() { return "MainWindow"; }
};

class GroupBugReport
{
public:
    GroupBugReport();

    const shared::SettingsAppOption Host;
    const shared::SettingsAppOption ProjectID;
    const shared::SettingsAppOption AccessToken;

private:
    GroupBugReport(const GroupBugReport &) = delete;
    GroupBugReport &operator=(const GroupBugReport &) = delete;

    static const QString localName() { return "GroupBugReport"; }
};

class GroupAadl
{
public:
    GroupAadl();

    const shared::SettingsAppOption CustomPropertiesConfig;
    const shared::SettingsAppOption DataTypesConfig;
    const shared::SettingsAppOption ColorSchemeFile;

private:
    GroupAadl(const GroupBugReport &) = delete;
    GroupAadl &operator=(const GroupBugReport &) = delete;

    static const QString localName() { return "GroupAadl"; }
};

class AppOptions
{
public:
    static const GroupMainWindow MainWindow;
    static const GroupBugReport BugReport;
    static const GroupAadl Aadl;

private:
    AppOptions() = delete;
    AppOptions(const AppOptions &) = delete;
    AppOptions &operator=(const AppOptions &) = delete;
};

}
